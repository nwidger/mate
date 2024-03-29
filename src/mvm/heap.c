/* Niels Widger
 * Time-stamp: <18 Mar 2013 at 20:42:10 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "class_table.h"
#include "constants.h"
#include "garbage_collector.h"
#include "globals.h"
#include "heap.h"
#include "nlock.h"
#include "object.h"
#include "ref_set.h"
#include "thread.h"

/* struct definitions */
struct free_ref {
	int ref;
	struct free_ref *next;
};

struct heap {
	uint64_t mem_size;
	uint64_t mem_free;

	int num_buckets;
	struct heap_ref **ref_buckets;
	struct heap_ref **ptr_buckets;

	int next_ref;
	struct free_ref *free_list;

	struct ref_set *excluded_set;
	struct ref_set *thread_set;
	pthread_rwlock_t rwlock;

	int force_malloc;
};

/* forward declarations */
inline static int heap_hash_ptr(void *p);
inline static int heap_hash_ref(uintptr_t r);
struct heap_ref * heap_malloc_aux(struct heap *h, int b);
int heap_garbage_collect(struct heap *h);
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

struct heap * heap_create(uint64_t m) {
	struct heap *h;

	if (m != -1 && m <= 0) {
		fprintf(stderr, "mvm: heap size must be greater than zero!\n");
		mvm_halt();
	}

	if ((h = (struct heap *)malloc(sizeof(struct heap))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	h->mem_size = m == -1 ? UINT64_MAX : m;
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

	h->next_ref = 1;
	h->free_list = NULL;

	if ((h->excluded_set = ref_set_create(1)) == NULL)
		mvm_halt();

	if ((h->thread_set = ref_set_create(1)) == NULL)
		mvm_halt();
	
	if (pthread_rwlock_init(&h->rwlock, NULL) != 0) {
		perror("mvm: pthread_rwlock_init");
		mvm_halt();
	}

	h->force_malloc = 0;

	return h;
}

void heap_destroy(struct heap *h) {
	if (h != NULL) {
		ref_set_destroy(h->excluded_set);
		ref_set_destroy(h->thread_set);		

		pthread_rwlock_destroy(&h->rwlock);
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
		for (i = 0; i < h->num_buckets; i++) {
			r = h->ptr_buckets[i];
			while (r != NULL) {
				s = r;
				r = r->ptr_next;
				free(s);
			}
		}

		memset(h->ref_buckets, 0,
		       sizeof(struct heap_ref *)*h->num_buckets);
		memset(h->ptr_buckets, 0,
		       sizeof(struct heap_ref *)*h->num_buckets);

		f = h->free_list;
		while (f != NULL) {
			g = f;
			f = f->next;
			free_ref_destroy(g);
		}

		h->free_list = NULL;
		h->next_ref = 1;

		ref_set_clear(h->excluded_set);
		ref_set_clear(h->thread_set);		
		h->mem_free = h->mem_size;
	}
}

uint64_t heap_get_size(struct heap *h) {
	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	return h->mem_size;
}

uint64_t heap_get_free(struct heap *h) {
	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	return h->mem_free;
}

int heap_resize(struct heap *h, uint64_t m) {
	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	if (m < (h->mem_size - h->mem_free)) {
		fprintf(stderr, "mvm: new heap size too small!\n");
		mvm_halt();
	}

	/* lock */
	heap_wrlock(h);

	h->mem_size = m;

	/* unlock */
	heap_unlock(h);

	return 0;
}

int heap_garbage_collect(struct heap *h) {
	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	mvm_print("heap: calling garbage_collector\n");

	if (garbage_collector_collect_now(garbage_collector) != 0)
		mvm_halt();

	mvm_print("heap: returned from garbage_collector\n");

	return 0;
}

void * heap_malloc(struct heap *h, int b) {
	struct heap_ref *r;

	r = heap_malloc_aux(h, b);

	return r->ptr;
}

struct heap_ref * heap_malloc_aux(struct heap *h, int b) {
	void *ptr;
	struct thread *t;
	struct heap_ref *r;

	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	if (b <= 0) {
		fprintf(stderr, "mvm: allocation size must be > 0!\n");
		mvm_halt();
	}

	if (b > h->mem_free) {
		heap_garbage_collect(h);

		if (b > h->mem_free) {
			fprintf(stderr, "mvm: out of memory!\n");
			mvm_halt();
		}
	}

	if (h->force_malloc != 0 ||
	    (t = thread_get_current()) == NULL ||
	    (r = thread_remove_from_free(t, b)) == NULL) {
		if ((ptr = (void *)malloc(b)) == NULL) {
			perror("mvm: malloc");
			mvm_halt();
		}

		if ((r = heap_ref_create(0, ptr, b)) == NULL) {
			mvm_halt();
		}
	}

	/* lock */
	heap_wrlock(h);

	/* add to ptr_buckets */
	heap_add_to_ptr(h, r);

	h->mem_free -= b;

	/* unlock */
	heap_unlock(h);

	return r;
}

void * heap_malloc_ref(struct heap *h, int b, int *r) {
	void *p;
	int ref;
	struct heap_ref *hr;

	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	if ((hr = heap_malloc_aux(h, b)) == NULL) {
		mvm_halt();
	}

	/* lock */
	heap_wrlock(h);

	p = hr->ptr;
	ref = hr->ref = heap_generate_ref(h);

	/* add to ref_buckets */
	heap_add_to_ref(h, hr);

	if (r != NULL)
		*r = ref;

	/* unlock */
	heap_unlock(h);

	return p;
}

int heap_free(struct heap *h, void *p) {
	int ref;
	struct thread *t;
	struct heap_ref *q;

	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	/* lock */
	heap_wrlock(h);

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

	/* unlock */
	heap_unlock(h);

	/* add to free list */
	if ((t = thread_get_current()) == NULL ||
	    thread_add_to_free(t, q) != 0) {
		/* no thread or free bucket is full, really free */
		heap_ref_destroy(q);
	}

	return 0;
}

inline static int heap_hash_ptr(void *p) {
	uintptr_t r;

	r = (uintptr_t)p;
	
	switch(sizeof(r)) {
	case 8: /* 64-bit int */
		r = (~r) + (r << 21); // r = (r << 21) - r - 1;
		r = r ^ (r >> 24);
		r = (r + (r << 3)) + (r << 8); // r * 265
		r = r ^ (r >> 14);
		r = (r + (r << 2)) + (r << 4); // r * 21
		r = r ^ (r >> 28);
		r = r + (r << 31);
		break;
	case 4: /* 32-bit int */
		r = (r ^ 61) ^ (r >> 16);
		r = r + (r << 3);
		r = r ^ (r >> 4);
		r = r * 0x27d4eb2d;
		r = r ^ (r >> 15);
		break;
	}
	
	return abs(r);
}

inline static int heap_hash_ref(uintptr_t r) {
	return heap_hash_ptr((void *)r);
}

void * heap_fetch(struct heap *h, int r) {
	int n;
	void *ptr;
	struct thread *t;
	struct heap_ref *p;

	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	if (r == 0)
		return NULL;

	ptr = NULL;
	t = thread_get_current();

	/* check thread ref cache */
	if (t != NULL && (ptr = thread_fetch_ref(t, r)) != NULL) {
		return ptr;
	}

	n = heap_hash_ref(r) % h->num_buckets;
	
	/* lock */
	heap_rdlock(h);

	for (p = h->ref_buckets[n]; p != NULL && p->ref <= r; p = p->ref_next) {
		if (p->ref == r) {
			ptr = p->ptr;
			if (t != NULL) { thread_add_to_ref(t, p); }
			break;
		}
	}

	/* unlock */
	heap_unlock(h);

	return ptr;
}

struct object * heap_fetch_object(struct heap *h, int r) {
	struct object *object;

	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	object = (struct object *)heap_fetch(h, r);

	return object;
}

int heap_compact(struct heap *h) {
	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	/* no compaction needed for this implementation */

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
	heap_rdlock(h);

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

int heap_populate_thread_set(struct heap *h, struct ref_set *r) {
	int ref;

	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	/* lock */
	heap_rdlock(h);

	ref_set_iterator_init(h->thread_set);

	while ((ref = ref_set_iterator_next(h->thread_set)) != 0)
		ref_set_add(r, ref);

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
	/* heap_lock(h); */

	if (r != 0)
		ref_set_add(h->excluded_set, r);

	/* unlock */
	/* heap_unlock(h); */

	return 0;
}

int heap_include_ref(struct heap *h, int r) {
	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	/* lock */
	/* heap_lock(h); */

	if (r != 0)
		ref_set_remove(h->excluded_set, r);

	/* unlock */
	/* heap_unlock(h); */

	return 0;
}

int heap_add_thread_ref(struct heap *h, int r) {
	struct object *o;
	
	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	o = heap_fetch_object(h, r);

	if (object_get_predefined_type(o) != thread_type) {
		fprintf(stderr, "mvm: object is not of type thread!\n");
		mvm_halt();
	}

	/* lock */
	/* heap_lock(h); */

	if (r != 0)
		ref_set_add(h->thread_set, r);

	/* unlock */
	/* heap_unlock(h); */

	return 0;
}

int heap_remove_thread_ref(struct heap *h, int r) {
	struct object *o;
	
	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	o = heap_fetch_object(h, r);

	if (object_get_predefined_type(o) != thread_type) {
		fprintf(stderr, "mvm: object is not of type thread!\n");
		mvm_halt();
	}
	
	/* lock */
	/* heap_lock(h); */

	if (r != 0)
		ref_set_remove(h->thread_set, r);

	/* unlock */
	/* heap_unlock(h); */

	return 0;
}

/* caller MUST acquire wrlock */
int heap_add_to_ref(struct heap *h, struct heap_ref *r) {
	int n, ref;
	struct heap_ref *s, *t;

	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	ref = r->ref;
	n = heap_hash_ref(ref) % h->num_buckets;

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

	return 0;
}

/* caller MUST acquire wrlock */
int heap_add_to_ptr(struct heap *h, struct heap_ref *r) {
	int n;
	void *ptr;
	struct heap_ref *p, *q;

	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	ptr = r->ptr;
	n = heap_hash_ptr(ptr) % h->num_buckets;

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

	return 0;
}

/* caller MUST acquire wrlock */
struct heap_ref * heap_remove_from_ptr(struct heap *h, void *p) {
	int n;
	struct heap_ref *q, *r;

	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	/* remove from ptr_buckets */
	n = heap_hash_ptr(p) % h->num_buckets;
	for (q = h->ptr_buckets[n], r = NULL;
	     q != NULL && q->ptr < p;
	     r = q, q = q->ptr_next);

	if (q == NULL || q->ptr != p)
		q = NULL;
	else if (q == h->ptr_buckets[n])
		h->ptr_buckets[n] = q->ptr_next;
	else
		r->ptr_next = q->ptr_next;

	return q;
}

/* caller MUST acquire wrlock */
struct heap_ref * heap_remove_from_ref(struct heap *h, int e) {
	int ref, n;
	struct heap_ref *q, *r;

	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	ref = e;

	/* remove from ref_buckets */
	n = heap_hash_ref(ref) % h->num_buckets;
	for (q = h->ref_buckets[n], r = NULL;
	     q != NULL && q->ref < ref;
	     r = q, q = q->ref_next);

	if (q == NULL || q->ref != ref)
		q = NULL;
	else if (q == h->ref_buckets[n])
		h->ref_buckets[n] = q->ref_next;
	else
		r->ref_next = q->ref_next;

	return q;
}

/* caller MUST acquire wrlock */
int heap_generate_ref(struct heap *h) {
	int ref;
	struct free_ref *f;

	if (h == NULL) {
		fprintf(stderr, "mvm: heap has not been initialized!\n");
		mvm_halt();
	}

	if (h->free_list == NULL) {
		ref = h->next_ref++;
	} else {
		ref = h->free_list->ref;
		f = h->free_list;
		h->free_list = h->free_list->next;
		free_ref_destroy(f);
	}

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
	heap_rdlock(h);

	for (i = 0; i < h->num_buckets; i++) {
		fprintf(stderr, "ref bucket %d =\n", i);
		
		for (r = h->ref_buckets[i]; r != NULL; r = r->ref_next) {
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

	for (i = 0; i < h->num_buckets; i++) {
		fprintf(stderr, "ptr bucket %d =\n", i);
		
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

	fprintf(stderr, "mem_size = %" PRIu64 "\n", h->mem_size);
	fprintf(stderr, "mem_free = %" PRIu64 "\n", h->mem_free);	

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

int heap_wrlock(struct heap *h) {
	int err;
	
	if (h == NULL) {
		fprintf(stderr, "mvm: heap not initialized!\n");
		mvm_halt();
	}

	/* lock */
	err = pthread_rwlock_trywrlock(&h->rwlock);

	switch (err) {
	case 0:			/* acquired lock */
	case EDEADLK:		/* we already own the lock */
		break;
	case EBUSY:		/* someone else owns the lock, block */
		if ((err = pthread_rwlock_wrlock(&h->rwlock)) != 0) {
			fprintf(stderr, "mvm: pthread_rwlock_wrlock: %s\n", strerror(err));
			mvm_halt();
		}
		break;
	default:
		fprintf(stderr, "mvm: pthread_rwlock_trywrlock: %s\n", strerror(err));
		mvm_halt();
	}

	return 0;
}

int heap_rdlock(struct heap *h) {
	int err;
	
	if (h == NULL) {
		fprintf(stderr, "mvm: heap not initialized!\n");
		mvm_halt();
	}

	/* lock */
	err = pthread_rwlock_tryrdlock(&h->rwlock);

	switch (err) {
	case 0:			/* acquired lock */
	case EDEADLK:		/* we already own the lock */
		break;
	case EBUSY:		/* someone else owns the lock, block */
		if ((err = pthread_rwlock_rdlock(&h->rwlock)) != 0) {
			fprintf(stderr, "mvm: pthread_rwlock_rdlock: %s\n", strerror(err));
			mvm_halt();
		}
		break;
	default:
		fprintf(stderr, "mvm: pthread_rwlock_tryrdlock: %s\n", strerror(err));
		mvm_halt();
	}

	return 0;
}

int heap_unlock(struct heap *h) {
	if (h == NULL) {
		fprintf(stderr, "mvm: heap not initialized!\n");
		mvm_halt();
	}

	/* lock */
	pthread_rwlock_unlock(&h->rwlock);

	return 0;
}

int heap_release(struct heap *h) {
	if (h == NULL) {
		fprintf(stderr, "mvm: heap not initialized!\n");
		mvm_halt();
	}

	return 0;
}

int heap_reacquire(struct heap *h, int l) {
	if (h == NULL) {
		fprintf(stderr, "mvm: heap not initialized!\n");
		mvm_halt();
	}

	return 0;
}

int heap_enable_force_malloc(struct heap *h) {
	if (h == NULL) {
		fprintf(stderr, "mvm: heap not initialized!\n");
		mvm_halt();
	}

	h->force_malloc = 1;
	return 0;
}

int heap_disable_force_malloc(struct heap *h) {
	if (h == NULL) {
		fprintf(stderr, "mvm: heap not initialized!\n");
		mvm_halt();
	}

	h->force_malloc = 0;
	return 0;
}
