/* Niels Widger
 * Time-stamp: <11 Feb 2010 at 12:23:21 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "globals.h"
#include "ref_set.h"

/* struct definitions */
struct ref_set_record {
	int ref;
	struct ref_set_record *next;
};

struct ref_set {
	int size;
	float load_factor;
	int current_capacity;

	int iterator_bucket;
	struct ref_set_record *iterator_record;
	
	struct ref_set_record **buckets;
};

/* forward declarations */
struct ref_set * ref_set_create_n(int n);
int ref_set_resize(struct ref_set *h, int n);
struct ref_set_record * ref_set_record_create(int r, struct ref_set_record *n);
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

	h->current_capacity = n;
	h->load_factor = REF_SET_DEFAULT_LOAD_FACTOR;
	h->size = 0;
	
	if ((h->buckets = (struct ref_set_record **)
	     calloc(h->current_capacity, sizeof(struct ref_set_record *))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	h->iterator_bucket = 0;
	h->iterator_record = NULL;
	
	return h;
}

void ref_set_destroy(struct ref_set *h) {
	if (h != NULL) {
		ref_set_clear(h);
		free(h->buckets);
		free(h);
	}
}

void ref_set_clear(struct ref_set *h) {
	int n;
	struct ref_set_record *p, *q;

	if (h != NULL) {
		for (n = 0; n < h->current_capacity; n++) {
			p = h->buckets[n];
			while (p != NULL) {
				q = p;
				p = p->next;
				if (q != NULL)
					free(q);
			}

			h->buckets[n] = NULL;
		}

	        h->size = 0;
	}
}

int ref_set_add(struct ref_set *h, int r) {
	int n;
	struct ref_set_record *p, *q, *s;

	if (h == NULL) {
		fprintf(stderr, "mvm: ref set not initialized!\n");
		mvm_halt();
	}

	s = ref_set_record_create(r, NULL);
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
			return 1;
		}

		s->next = q->next;
		q->next = s;
	}

	h->size++;
	if ((h->size * h->load_factor) > h->current_capacity)
		ref_set_resize(h, h->current_capacity*2);
	
	return 0;
}

int ref_set_remove(struct ref_set *h, int r) {
	int n;
	struct ref_set_record *p, *q;

	if (h == NULL) {
		fprintf(stderr, "mvm: ref set not initialized!\n");
		mvm_halt();
	}

	n = r % h->current_capacity;
	for (p = h->buckets[n], q = NULL; p != NULL && p->ref < r; q = p, p = p->next);

	if (p == NULL || p->ref != r)
		return 1;
	
	if (p == h->buckets[n])
		h->buckets[n] = p->next;
	else
		q->next = p->next;

	ref_set_record_destroy(p);

	h->size--;
	return 0;
}

int ref_set_size(struct ref_set *h) {
	if (h == NULL) {
		fprintf(stderr, "mvm: ref set not initialized!\n");
		mvm_halt();
	}

	return h->size;;
}

int ref_set_contains(struct ref_set *h, int r) {
	int n;
	struct ref_set_record *p;

	if (h == NULL) {
		fprintf(stderr, "mvm: ref set not initialized!\n");
		mvm_halt();
	}

	n = r % h->current_capacity;
	for (p = h->buckets[n]; p != NULL && p->ref < r; p = p->next);

	if (p == NULL || p->ref != r)
		return 0;
	
	return 1;
}

int ref_set_iterator_init(struct ref_set *h) {
	int n;
	
	if (h == NULL) {
		fprintf(stderr, "mvm: ref set not initialized!\n");
		mvm_halt();
	}

	for (n = 0; n < h->current_capacity; n++) {
		if (h->buckets[n] != NULL) {
			h->iterator_bucket = n;
			h->iterator_record = h->buckets[n];
			return 0;
		}
	}

	return 1;
}

int ref_set_iterator_next(struct ref_set *h) {
	int ref, n;
	
	if (h == NULL) {
		fprintf(stderr, "mvm: ref set not initialized!\n");
		mvm_halt();
	}

	if (h->iterator_record == NULL)
		return 0;

	ref = h->iterator_record->ref;

	h->iterator_record = h->iterator_record->next;
	if (h->iterator_record != NULL)
		return ref;

	for (n = h->iterator_bucket+1; n < h->current_capacity; n++) {
		if (h->buckets[n] != NULL) {
			h->iterator_bucket = n;
			h->iterator_record = h->buckets[n];
			break;
		}
	}

	if (n >= h->current_capacity) {
		h->iterator_bucket = 0;
		h->iterator_record = NULL;
	}

	return ref;
}

int ref_set_resize(struct ref_set *h, int n) {
	int i;
	struct ref_set_record *p;
	struct ref_set *new_set;
	
	if (h == NULL) {
		fprintf(stderr, "mvm: ref set not initialized!\n");
		mvm_halt();
	}

	if ((new_set = ref_set_create_n(n)) == NULL)
		mvm_halt();

	for (i = 0; i < h->current_capacity; i++) {
		for (p = h->buckets[i]; p != NULL; p = p->next) {
			ref_set_add(new_set, p->ref);
		}
	}

	ref_set_clear(h);
	free(h->buckets);
	memcpy(h, new_set, sizeof(struct ref_set));
	free(new_set);

	return 0;
}

int ref_set_dump(struct ref_set *h) {
	int n;
	struct ref_set_record *p;
	
	if (h == NULL) {
		fprintf(stderr, "mvm: ref set not initialized!\n");
		mvm_halt();
	}

	fprintf(stderr, "dumping ref_set\n");
	fprintf(stderr, "  size = %d\n", h->size);
	fprintf(stderr, "  load_factor = %f\n", h->load_factor);
	fprintf(stderr, "  current_capacity = %d\n", h->current_capacity);
	fprintf(stderr, "\n");	
	fprintf(stderr, "  iterator_bucket = %d\n", h->iterator_bucket);
	fprintf(stderr, "  iterator_record = %p\n", h->iterator_record);
	fprintf(stderr, "\n");

	for (n = 0; n < h->current_capacity; n++) {
		fprintf(stderr, "  bucket[%d]: ", n);
		for (p = h->buckets[n]; p != NULL; p = p->next)
			fprintf(stderr, "%d ", p->ref);
		fprintf(stderr, "\n");		
	}

	return 0;
}

struct ref_set_record * ref_set_record_create(int r, struct ref_set_record *n) {
	struct ref_set_record *p;

	if ((p = (struct ref_set_record *)malloc(sizeof(struct ref_set_record))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	p->ref = r;
	p->next = n;

	return p;
}

void ref_set_record_destroy(struct ref_set_record *r) {
	if (r != NULL) {
		free(r);
	}
}
