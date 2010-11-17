/* Niels Widger
 * Time-stamp: <05 Mar 2010 at 15:11:42 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "garbage_collector.h"
#include "globals.h"
#include "heap.h"
#include "nlock.h"
#include "object.h"
#include "ref_set.h"

/* struct definitions */
struct free_ref {
	int ref;
	struct free_ref *next;
};

struct heap_ref {
	int ref;
	int size;
	void *ptr;
	struct heap_ref *ref_next;
	struct heap_ref *ptr_next;
};

struct heap {
	int mem_size;
	int mem_free;

	int num_buckets;
	struct heap_ref **ref_buckets;
	struct heap_ref **ptr_buckets;

	struct heap_ref *last_malloc;

	int next_ref;
	struct free_ref *free_list;

	struct ref_set *excluded_set;
	struct nlock *nlock;
};

/* forward declarations */
int heap_add_to_ref(struct heap *h, struct heap_ref *r);
int heap_add_to_ptr(struct heap *h, struct heap_ref *r);
struct heap_ref * heap_remove_from_ptr(struct heap *h, void *p);
struct heap_ref * heap_remove_from_ref(struct heap *h, int e);
int heap_generate_ref(struct heap *h);
struct heap_ref * heap_ref_create(int r, void *p, int s);
void heap_ref_destroy(struct heap_ref *r);
struct free_ref * free_ref_create(int r, struct free_ref *n);
void free_ref_destroy(struct free_ref *f);
int heap_release(struct heap *h);
int heap_reacquire(struct heap *h, int l);

struct heap * heap_create(int m) {
	struct heap *h;

	if (m <= 0) {
		fprintf(stderr, "mvm: heap size must be greater than zero!\n");
		mvm_halt();
	}

	if ((h = (struct heap *)malloc(sizeof(struct heap))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	h->mem_size = m;
	h->mem_free = m;

	h->num_buckets = HEAP_NUM_BUCKETS;

	if ((h->ref_buckets = (struct heap_ref **)
	     calloc(h->num_buckets, sizeof(struct heap_ref *))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	if ((h->ptr_buckets = (struct heap_ref **)
	     calloc(h->num_buckets, sizeof(struct heap_ref *))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	h->last_malloc = NULL;

	h->next_ref = 1;
	h->free_list = NULL;

	if ((h->excluded_set = ref_set_create()) == NULL)
		mvm_halt();

	if ((h->nlock = nlock_create()) == NULL)
		mvm_halt();

	return h;
}

void heap_destroy(struct heap *h) {
	if (h != NULL) {
		/* lock */
		heap_lock(h);

		heap_clear(h);
		ref_set_destroy(h->excluded_set);

		/* unlock */
		heap_unlock(h);

		nlock_destroy(h->nlock);
		free(h->ref_buckets);
		free(h->ptr_buckets);
		free(h);
	}
}

void heap_clear(struct heap *h) {
	int i;
	struct heap_ref *r, *s;
	struct free_ref *f, *g;

	if (h != NULL) {
		/* lock */
		heap_lock(h);

		for (i = 0; i < h->num_buckets; i++) {
			r = h->ptr_buckets[i];
			while (r != NULL) {
				s = r;
				r = r->ptr_next;
				heap_ref_destroy(s);
			}
		}

		memset(h->ref_buckets, 0,
		       sizeof(struct heap_ref *)*h->num_buckets);
		memset(h->ptr_buckets, 0,
		       sizeof(struct heap_ref *)*h->num_buckets);

		h->last_malloc = NULL;

		f = h->free_list;
		while (f != NULL) {
			g = f;
			f = f->next;
			free_ref_destroy(g);
		}

		h->free_list = NULL;
		h->next_ref = 1;

		ref_set_clear(h->excluded_set);
		h->mem_free = h->mem_size;

		/* unlock */
		heap_unlock(h);
	}
}

int heap_resize(struct heap *h, int m) {
	/* lock */
	heap_lock(h);

	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	if (m < (h->mem_size - h->mem_free)) {
		fprintf(stderr, "mvm: new heap size too small!\n");
		/* unlock */
		heap_unlock(h);
		mvm_halt();
	}

	h->mem_size = m;

	/* unlock */
	heap_unlock(h);

	return 0;
}

void * heap_malloc(struct heap *h, int b) {
	int l, m;
	void *ptr;
	struct heap_ref *r;

	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	if (b <= 0) {
		fprintf(stderr, "mvm: allocation size must be > 0!\n");
		mvm_halt();
	}

	/* lock */
	heap_lock(h);

	if (b > h->mem_free) {
		/* unlock */
		l = heap_release(h);
		/* unlock garbage_collector */
		m = garbage_collector_release(garbage_collector);

		mvm_print("heap: calling garbage_collector\n");

		if (garbage_collector_collect_now(garbage_collector) != 0)
			mvm_halt();

		mvm_print("heap: returned from garbage_collector\n");

		/* lock garbage_collector */
		garbage_collector_reacquire(garbage_collector, m);
		/* lock */
		heap_reacquire(h, l);

		if (b > h->mem_free) {
			fprintf(stderr, "mvm: out of memory!\n");
			/* unlock */
			heap_unlock(h);
			mvm_halt();
		}
	}

	if ((ptr = (void *)malloc(b)) == NULL) {
		perror("mvm: malloc");
		/* unlock */
		heap_unlock(h);
		mvm_halt();
	}

	if ((r = heap_ref_create(0, ptr, b)) == NULL) {
		/* unlock */
		heap_unlock(h);
		mvm_halt();
	}

	/* add to ptr_buckets */
	heap_add_to_ptr(h, r);

	h->last_malloc = r;
	h->mem_free -= b;

	/* unlock */
	heap_unlock(h);

	return ptr;
}

void * heap_malloc_ref(struct heap *h, int b, int *r) {
	void *p;
	int ref;

	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	/* lock */
	heap_lock(h);

	if ((p = heap_malloc(h, b)) == NULL) {
		/* unlock */
		heap_unlock(h);
		mvm_halt();
	}

	ref = heap_generate_ref(h);
	h->last_malloc->ref = ref;

	/* add to ref_buckets */
	heap_add_to_ref(h, h->last_malloc);
	h->last_malloc = NULL;

	if (r != NULL)
		*r = ref;

	/* unlock */
	heap_unlock(h);

	return p;
}

int heap_free(struct heap *h, void *p) {
	int ref;
	struct heap_ref *q;

	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	/* lock */
	heap_lock(h);

	/* remove from ptr_buckets */
	q = heap_remove_from_ptr(h, p);

	if (q == NULL) {
		/* unlock */
		heap_unlock(h);
		return 1;
	}

	ref = q->ref;

	if (ref != 0) {
		/* add to free list */
		h->free_list = free_ref_create(ref, h->free_list);
		/* remove from ref_buckets */
		heap_remove_from_ref(h, ref);
	}

	h->mem_free += q->size;
	heap_ref_destroy(q);

	/* unlock */
	heap_unlock(h);

	return 0;
}

void * heap_fetch(struct heap *h, int r) {
	int n;
	struct heap_ref *p;

	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	if (r == 0)
		return NULL;

	/* lock */
	heap_lock(h);

	n = r % h->num_buckets;

	for (p = h->ref_buckets[n]; p != NULL && p->ref <= r; p = p->ref_next) {
		if (p->ref == r) {
			/* unlock */
			heap_unlock(h);
			return p->ptr;
		}
	}

	/* unlock */
	heap_unlock(h);

	return NULL;
}

struct object * heap_fetch_object(struct heap *h, int r) {
	struct object *object;

	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	/* lock */
	heap_lock(h);

	object = (struct object *)heap_fetch(h, r);

	/* unlock */
	heap_unlock(h);

	return object;
}

int heap_compact(struct heap *h) {
	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	/* lock */
	heap_lock(h);

	/* no compaction needed for this implementation */

	/* unlock */
	heap_unlock(h);

	return 0;
}

int heap_populate_ref_set(struct heap *h, struct ref_set *r) {
	int i;
	struct heap_ref *p;

	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	/* lock */
	heap_lock(h);

	for (i = 0; i < h->num_buckets; i++) {
		for (p = h->ref_buckets[i]; p != NULL; p = p->ref_next) {
			if (p->ref != 0 &&
			    ref_set_contains(h->excluded_set, p->ref) == 0) {
				ref_set_add(r, p->ref);
			}
		}
	}

	/* unlock */
	heap_unlock(h);

	return 0;
}

int heap_exclude_ref(struct heap *h, int r) {
	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	/* lock */
	heap_lock(h);

	if (r != 0)
		ref_set_add(h->excluded_set, r);

	/* unlock */
	heap_unlock(h);

	return 0;
}

int heap_include_ref(struct heap *h, int r) {
	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	/* lock */
	heap_lock(h);

	if (r != 0)
		ref_set_remove(h->excluded_set, r);

	/* unlock */
	heap_unlock(h);

	return 0;
}

int heap_add_to_ref(struct heap *h, struct heap_ref *r) {
	int n, ref;
	struct heap_ref *s, *t;

	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	/* lock */
	heap_lock(h);

	ref = r->ref;
	n = ref % h->num_buckets;

	if (h->ref_buckets[n] == NULL) {
		h->ref_buckets[n] = r;
	} else if (ref < h->ref_buckets[n]->ref) {
		r->ref_next = h->ref_buckets[n];
		h->ref_buckets[n] = r;
	} else {
		for (s = h->ref_buckets[n], t = NULL;
		     s != NULL && ref > s->ref;
		     t = s, s = s->ref_next);

		r->ref_next = t->ref_next;
		t->ref_next = r;
	}

	/* unlock */
	heap_unlock(h);

	return 0;
}

int heap_add_to_ptr(struct heap *h, struct heap_ref *r) {
	int n;
	void *ptr;
	struct heap_ref *p, *q;

	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	/* lock */
	heap_lock(h);

	ptr = r->ptr;
	n = (uintptr_t)ptr % h->num_buckets;

	if (h->ptr_buckets[n] == NULL) {
		h->ptr_buckets[n] = r;
	} else if (ptr < h->ptr_buckets[n]->ptr) {
		r->ptr_next = h->ptr_buckets[n];
		h->ptr_buckets[n] = r;
	} else {
		for (p = h->ptr_buckets[n], q = NULL;
		     p != NULL && ptr > p->ptr;
		     q = p, p = p->ptr_next);

		r->ptr_next = q->ptr_next;
		q->ptr_next = r;
	}

	/* unlock */
	heap_unlock(h);

	return 0;
}

struct heap_ref * heap_remove_from_ptr(struct heap *h, void *p) {
	int n;
	struct heap_ref *q, *r;

	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	/* lock */
	heap_lock(h);

	/* remove from ptr_buckets */
	n = (uintptr_t)p % h->num_buckets;
	for (q = h->ptr_buckets[n], r = NULL;
	     q != NULL && q->ptr < p;
	     r = q, q = q->ptr_next);

	if (q == NULL || q->ptr != p)
		q = NULL;
	else if (q == h->ptr_buckets[n])
		h->ptr_buckets[n] = q->ptr_next;
	else
		r->ptr_next = q->ptr_next;

	/* unlock */
	heap_unlock(h);

	return q;
}

struct heap_ref * heap_remove_from_ref(struct heap *h, int e) {
	int ref, n;
	struct heap_ref *q, *r;

	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	/* lock */
	heap_lock(h);

	ref = e;

	/* remove from ref_buckets */
	n = ref % h->num_buckets;
	for (q = h->ref_buckets[n], r = NULL;
	     q != NULL && q->ref < ref;
	     r = q, q = q->ref_next);

	if (q == NULL || q->ref != ref)
		q = NULL;
	else if (q == h->ref_buckets[n])
		h->ref_buckets[n] = q->ref_next;
	else
		r->ref_next = q->ref_next;

	/* unlock */
	heap_unlock(h);

	return q;
}

int heap_generate_ref(struct heap *h) {
	int ref;
	struct free_ref *f;

	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	/* lock */
	heap_lock(h);

	if (h->free_list == NULL) {
		ref = h->next_ref++;
	} else {
		ref = h->free_list->ref;
		f = h->free_list;
		h->free_list = h->free_list->next;
		free_ref_destroy(f);
	}

	/* unlock */
	heap_unlock(h);

	return ref;
}

int heap_dump(struct heap *h) {
	int i;
	struct heap_ref *r;
	struct object *object;

	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	/* lock */
	heap_lock(h);

	for (i = 0; i < h->num_buckets; i++) {
		fprintf(stderr, "bucket %d =\n", i);
		
		for (r = h->ptr_buckets[i]; r != NULL; r = r->ptr_next) {
			fprintf(stderr, "               ");
			
			if (r->ref == 0) {
				fprintf(stderr, "size=%d ref=%d ptr=%p",
					r->size, r->ref, r->ptr);
			} else {
				object = (struct object *)r->ptr;
				fprintf(stderr, "size=%d ", r->size);
				object_dump(object, 0);
			}
			
			fprintf(stderr, "\n");
		}
	}

	fprintf(stderr, "mem_size = %d\n", h->mem_size);
	fprintf(stderr, "mem_free = %d\n", h->mem_free);	

/* 	fprintf(stderr, "dumping heap excluded set...\n"); */
/* 	ref_set_dump(h->excluded_set); */

	/* unlock */
	heap_unlock(h);

	return 0;
}

struct heap_ref * heap_ref_create(int r, void *p, int s) {
	struct heap_ref *q;

	if ((q = (struct heap_ref *)malloc(sizeof(struct heap_ref))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

        q->ref = r;
	q->ptr = p;
	q->size = s;
	q->ref_next = NULL;
	q->ptr_next = NULL;

	return q;
}

void heap_ref_destroy(struct heap_ref *r) {
	if (r != NULL) {
		if (r->ptr != NULL)
			free(r->ptr);
		free(r);
	}
}

struct free_ref * free_ref_create(int r, struct free_ref *n) {
	struct free_ref *f;

	if ((f = (struct free_ref *)malloc(sizeof(struct free_ref))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	f->ref = r;
	f->next = n;

	return f;
}

void free_ref_destroy(struct free_ref *f) {
	if (f != NULL)
		free(f);
}

int heap_lock(struct heap *h) {
	if (h == NULL) {
		fprintf(stderr, "mvm: heap not initialized!\n");
		mvm_halt();
	}

	/* lock */
	nlock_lock(h->nlock);

	return 0;
}

int heap_unlock(struct heap *h) {
	if (h == NULL) {
		fprintf(stderr, "mvm: heap not initialized!\n");
		mvm_halt();
	}

	/* unlock */
	nlock_unlock(h->nlock);

	return 0;
}

int heap_release(struct heap *h) {
	if (h == NULL) {
		fprintf(stderr, "mvm: heap not initialized!\n");
		mvm_halt();
	}
	
	return nlock_release(h->nlock);
}

int heap_reacquire(struct heap *h, int l) {
	if (h == NULL) {
		fprintf(stderr, "mvm: heap not initialized!\n");
		mvm_halt();
	}

	return nlock_reacquire(h->nlock, l);
}
