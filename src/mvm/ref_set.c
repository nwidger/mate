/* Niels Widger
 * Time-stamp: <21 Jan 2012 at 12:35:57 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "globals.h"
#include "nlock.h"
#include "ref_set.h"

/* struct definitions */
struct ref_set_record {
	int ref;
	struct ref_set_record *next;

	struct ref_set_record *list_next;
	struct ref_set_record *list_prev;
};

struct ref_set {
	struct nlock *nlock;

	int size;
	float load_factor;
	int current_capacity;

	struct ref_set_record *iterator_next;

	struct ref_set_record **buckets;

	struct ref_set_record *list_head;
	struct ref_set_record *list_tail;
};

/* forward declarations */
struct ref_set * ref_set_create_n(int n);
int ref_set_resize(struct ref_set *h, int n);
struct ref_set_record * ref_set_record_create(int r);
void ref_set_record_destroy(struct ref_set_record *r);

struct ref_set * ref_set_create() {
	return ref_set_create_n(REF_SET_DEFAULT_INITIAL_CAPACITY);
}

struct ref_set * ref_set_create_n(int n) {
	struct ref_set *h;

	if ((h = (struct ref_set *)malloc(sizeof(struct ref_set))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	if ((h->nlock = nlock_create()) == NULL)
		mvm_halt();

	h->current_capacity = n;
	h->load_factor = REF_SET_DEFAULT_LOAD_FACTOR;
	h->size = 0;

	if ((h->buckets = (struct ref_set_record **)
	     calloc(h->current_capacity, sizeof(struct ref_set_record *))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	h->iterator_next = NULL;

	h->list_head = NULL;
	h->list_tail = NULL;

	return h;
}

void ref_set_destroy(struct ref_set *h) {
	if (h != NULL) {
		/* lock */
		ref_set_lock(h);

		ref_set_clear(h);

		/* unlock */
		ref_set_unlock(h);

		nlock_destroy(h->nlock);
		free(h->buckets);
		free(h);
	}
}

void ref_set_clear(struct ref_set *h) {
	struct ref_set_record *p, *q;

	if (h != NULL) {
		/* lock */
		ref_set_lock(h);

		p = h->list_head;
		while (p != NULL) {
			q = p;
			p = p->list_next;
			ref_set_record_destroy(q);
		}

		memset(h->buckets, 0, sizeof(struct ref_set_record *)*h->current_capacity);

	        h->size = 0;

		h->iterator_next = NULL;

		h->list_head = NULL;
		h->list_tail = NULL;

		/* unlock */
		ref_set_unlock(h);
	}
}

int ref_set_add(struct ref_set *h, int r) {
	int n;
	struct ref_set_record *p, *q, *s;

	if (h == NULL) {
		fprintf(stderr, "mvm: ref set not initialized!\n");
		mvm_halt();
	}

	/* lock */
	ref_set_lock(h);

	s = ref_set_record_create(r);
	n = r % h->current_capacity;

	if (h->buckets[n] == NULL) {
		h->buckets[n] = s;
	} else if (r < h->buckets[n]->ref) {
		s->next = h->buckets[n];
		h->buckets[n] = s;
	} else {
		for (p = h->buckets[n], q = NULL; p != NULL && r > p->ref; q = p, p = p->next);

		if (p != NULL && p->ref == r) {
			ref_set_record_destroy(s);
			/* unlock */
			ref_set_unlock(h);
			return 1;
		}

		s->next = q->next;
		q->next = s;
	}

	/* add to list */
	s->list_next = NULL;
	s->list_prev = h->list_tail;
	
	if (h->size == 0)
		h->list_head = s;
	else
		h->list_tail->list_next = s;

	h->list_tail = s;	

	/* clean up iterator */
	if (h->iterator_next == NULL)
		h->iterator_next = s;

	h->size++;
	if ((h->size * h->load_factor) > h->current_capacity)
		ref_set_resize(h, h->current_capacity*2);

	/* unlock */
	ref_set_unlock(h);

	return 0;
}

int ref_set_remove(struct ref_set *h, int r) {
	int n;
	struct ref_set_record *p, *q;

	if (h == NULL) {
		fprintf(stderr, "mvm: ref set not initialized!\n");
		mvm_halt();
	}

	/* lock */
	ref_set_lock(h);

	n = r % h->current_capacity;
	for (p = h->buckets[n], q = NULL; p != NULL && p->ref < r; q = p, p = p->next);

	if (p == NULL || p->ref != r) {
		/* unlock */
		ref_set_unlock(h);
		return 1;
	}

	if (p == h->buckets[n])
		h->buckets[n] = p->next;
	else
		q->next = p->next;

	/* remove from list */
	if (h->size == 1) {
		h->list_head = NULL;
		h->list_tail = NULL;
	} else if (h->list_head == p) {
		h->list_head = p->list_next;
		h->list_head->list_prev = NULL;
	} else if (h->list_tail == p) {
		h->list_tail = p->list_prev;
		h->list_tail->list_next = NULL;
	} else {
		p->list_prev->list_next = p->list_next;
		p->list_next->list_prev = p->list_prev;
	}

	/* clean up iterator */
	if (h->iterator_next == p)
		h->iterator_next = p->list_next;

	ref_set_record_destroy(p);
	h->size--;

	/* unlock */
	ref_set_unlock(h);

	return 0;
}

int ref_set_size(struct ref_set *h) {
	int size;

	if (h == NULL) {
		fprintf(stderr, "mvm: ref set not initialized!\n");
		mvm_halt();
	}

	/* lock */
	ref_set_lock(h);

	size = h->size;

	/* unlock */
	ref_set_unlock(h);

	return size;
}

int ref_set_contains(struct ref_set *h, int r) {
	int n;
	struct ref_set_record *p;

	if (h == NULL) {
		fprintf(stderr, "mvm: ref set not initialized!\n");
		mvm_halt();
	}

	/* lock */
	ref_set_lock(h);

	n = r % h->current_capacity;
	for (p = h->buckets[n]; p != NULL && p->ref < r; p = p->next);

	if (p == NULL || p->ref != r) {
		/* unlock */
		ref_set_unlock(h);
		return 0;
	}

	/* unlock */
	ref_set_unlock(h);

	return 1;
}

int ref_set_iterator_init(struct ref_set *h) {
	if (h == NULL) {
		fprintf(stderr, "mvm: ref set not initialized!\n");
		mvm_halt();
	}

	/* lock */
	ref_set_lock(h);

	if (h->list_head == NULL) {
		h->iterator_next = NULL;
		/* unlock */
		ref_set_unlock(h);
		return 1;
	}

	h->iterator_next = h->list_head;

	/* unlock */
	ref_set_unlock(h);

	return 0;
}

int ref_set_iterator_next(struct ref_set *h) {
	int ref;

	if (h == NULL) {
		fprintf(stderr, "mvm: ref set not initialized!\n");
		mvm_halt();
	}

	/* lock */
	ref_set_lock(h);

	if (h->iterator_next == NULL) {
		/* unlock */
		ref_set_unlock(h);
		return 0;
	}

	ref = h->iterator_next->ref;
	h->iterator_next = h->iterator_next->list_next;

	/* unlock */
	ref_set_unlock(h);

	return ref;
}

int ref_set_resize(struct ref_set *h, int n) {
	struct ref_set *new_set;
	struct ref_set_record *p;

	if (h == NULL) {
		fprintf(stderr, "mvm: ref set not initialized!\n");
		mvm_halt();
	}

	/* lock */
	ref_set_lock(h);

	if ((new_set = ref_set_create_n(n)) == NULL) {
		/* unlock */
		ref_set_unlock(h);
		mvm_halt();
	}

	for (p = h->list_head; p != NULL; p = p->list_next) {
		ref_set_add(new_set, p->ref);

		if (p == h->iterator_next)
			new_set->iterator_next = new_set->list_tail;
	}

	ref_set_clear(h);
	free(h->buckets);

	h->size = new_set->size;
	h->load_factor = new_set->load_factor;
	h->current_capacity = new_set->current_capacity;
	h->iterator_next = new_set->iterator_next;
	h->buckets = new_set->buckets;
	h->list_head = new_set->list_head;
	h->list_tail = new_set->list_tail;

	nlock_destroy(new_set->nlock);
	free(new_set);

	/* unlock */
	ref_set_unlock(h);

	return 0;
}

int ref_set_dump(struct ref_set *h) {
	int n;
	struct ref_set_record *p;

	if (h == NULL) {
		fprintf(stderr, "mvm: ref set not initialized!\n");
		mvm_halt();
	}

	/* lock */
	ref_set_lock(h);

	fprintf(stderr, "dumping ref_set\n");
	fprintf(stderr, "  size = %d\n", h->size);
	fprintf(stderr, "  load_factor = %f\n", h->load_factor);
	fprintf(stderr, "  current_capacity = %d\n", h->current_capacity);
	fprintf(stderr, "\n");
	fprintf(stderr, "  iterator_next = %p\n", (void *)h->iterator_next);
	fprintf(stderr, "\n");

	for (n = 0; n < h->current_capacity; n++) {
		fprintf(stderr, "  bucket[%d]: ", n);
		for (p = h->buckets[n]; p != NULL; p = p->next)
			fprintf(stderr, "%d ", p->ref);
		fprintf(stderr, "\n");
	}

	/* unlock */
	ref_set_unlock(h);

	return 0;
}

int ref_set_dump_iterator(struct ref_set *h) {
	int len;
	char *buf;
	struct ref_set_record *p;

	if (h == NULL) {
		fprintf(stderr, "mvm: ref set not initialized!\n");
		mvm_halt();
	}

	/* lock */
	ref_set_lock(h);

	len = strlen("dumping ref_set iterator\n") +
		strlen("  size = ") + strlen("2147483648") + strlen("\n") +
		strlen("  refs = ") + (h->size*(strlen("*")+strlen("2147483648"))) +
		strlen("\n");

	if ((buf = (char *)malloc(len+1)) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	sprintf(buf, "dumping ref_set iterator\n");
	sprintf(buf+strlen(buf), "  size = %d\n", h->size);
	sprintf(buf+strlen(buf), "  refs = ");
	for (p = h->list_head; p != NULL; p = p->list_next)
		sprintf(buf+strlen(buf), "%s%d ", p == h->iterator_next ? "*" : "", p->ref);

	sprintf(buf+strlen(buf), "\n");

        free(buf);

	/* unlock */
	ref_set_unlock(h);

	fprintf(stderr, "%s", buf);

	return 0;
}

int ref_set_lock(struct ref_set *h) {
	if (h == NULL) {
		fprintf(stderr, "mvm: ref set not initialized!\n");
		mvm_halt();
	}

	/* lock */
	nlock_lock(h->nlock);

	return 0;
}

int ref_set_unlock(struct ref_set *h) {
	if (h == NULL) {
		fprintf(stderr, "mvm: ref set not initialized!\n");
		mvm_halt();
	}

	/* unlock */
	nlock_unlock(h->nlock);

	return 0;
}

struct ref_set_record * ref_set_record_create(int r) {
	struct ref_set_record *p;

	if ((p = (struct ref_set_record *)malloc(sizeof(struct ref_set_record))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	p->ref = r;

	p->next = NULL;
	p->list_next = NULL;
	p->list_prev = NULL;

	return p;
}

void ref_set_record_destroy(struct ref_set_record *r) {
	if (r != NULL) {
		free(r);
	}
}
