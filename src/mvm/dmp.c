/* Niels Widger
 * Time-stamp: <12 Feb 2011 at 21:13:36 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "barrier.h"
#include "dmp.h"
#include "globals.h"
#include "heap.h"
#include "nlock_dmp.h"
#include "object_dmp.h"
#include "ref_set.h"
#include "thread_dmp.h"

/* struct definitions */
struct dmp {
	enum dmp_mode mode;

	struct ref_set *thread_set;

	struct barrier *barrier;

	/* object_dmp defaults */
	struct object_dmp_attr *od_attr;

	/* thread_dmp defaults */
	struct thread_dmp_attr *td_attr;

	/* nlock_dmp defaults */
	struct nlock_dmp_attr *nd_attr;
};

/* forward declarations */
void * dmp_watchdog(void *arg);

struct dmp * dmp_create(struct object_dmp_attr *a,
			struct thread_dmp_attr *t,
			struct nlock_dmp_attr *n) {
	struct dmp *dmp;

	if ((dmp = (struct dmp *)malloc(sizeof(struct dmp))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	dmp->mode = parallel_mode;

	dmp->thread_set = ref_set_create();

	dmp->barrier = barrier_create(1);

	dmp->od_attr = a;
	dmp->td_attr = t;
	dmp->nd_attr = n;

	return dmp;
}

void dmp_destroy(struct dmp *d) {
	if (d != NULL) {
		ref_set_destroy(d->thread_set);
		barrier_destroy(d->barrier);
		free(d);
	}
}

void dmp_clear(struct dmp *d) {
	if (d != NULL) {
		barrier_clear(d->barrier);
		d->od_attr = NULL;
	}
}

int dmp_get_mode(struct dmp *d) {
	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	return d->mode;
}

int dmp_add_thread(struct dmp *d, int r) {
	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	ref_set_add(d->thread_set, r);
	return 0;
}

int dmp_remove_thread(struct dmp *d, int r) {
	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	ref_set_remove(d->thread_set, r);
	return 0;
}

struct object_dmp * dmp_create_object_dmp(struct dmp *d, struct object *o) {
	struct object_dmp *od;

	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	od = object_dmp_create(o, d->od_attr);
	return od;
}

struct thread_dmp * dmp_create_thread_dmp(struct dmp *d, struct thread *t) {
	struct thread_dmp *td;

	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	td = thread_dmp_create(t, d->td_attr);
	return td;
}

struct nlock_dmp * dmp_create_nlock_dmp(struct dmp *d, struct nlock *n) {
	struct nlock_dmp *nd;

	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	nd = nlock_dmp_create(n, d->nd_attr);
	return nd;
}

int dmp_thread_block(struct dmp *d, struct thread_dmp *td) {
	struct object *o;
	struct thread *t;
	int me, ref, retval, blocking, nthreads;
	struct thread_dmp *ud;

	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	me = thread_get_ref();

	if (d->mode == parallel_mode) {
		/* block until all threads finished */
		retval = barrier_await(d->barrier);

		if (retval != 0) { /* 0 == last to block */
			/* sleep until turn */
			thread_dmp_wait(td);
		} else {
			d->mode = serial_mode;

			/* ensure all threads are sleeping */
			nthreads = ref_set_size(d->thread_set) - 1;

			if (nthreads != 0) {
				blocking = 0;
				ref_set_iterator_init(d->thread_set);
				
				while (blocking != nthreads) {
					if (ref == me) {
						continue;
					} else if ((ref = ref_set_iterator_next(d->thread_set)) == 0) {
						ref_set_iterator_init(d->thread_set);
					} else {
						o = heap_fetch_object(heap, ref);
						t = object_get_thread(o);
						ud = _thread_get_dmp(t);

						if (thread_dmp_get_state_nonblock(ud) == blocking_state)
							blocking++;
					}
				}
			}

			ref_set_iterator_init(d->thread_set);
			ref = ref_set_iterator_next(d->thread_set);

			if (ref == me) {
				/* execute turn */
			} else {
				/* wake first thread */
				o = heap_fetch_object(heap, ref);
				t = object_get_thread(o);
				ud = _thread_get_dmp(t);
				thread_dmp_signal(ud);

				/* sleep until turn */
				thread_dmp_wait(td);
			}
		}
	} else {		/* serial_mode */
		ref = ref_set_iterator_next(d->thread_set);

		if (ref != 0) {
			/* wake next thread */
			o = heap_fetch_object(heap, ref);
			t = object_get_thread(o);
			ud = _thread_get_dmp(t);			
			thread_dmp_signal(ud);

			/* block until all threads finished */
			barrier_await(d->barrier);
		} else {
			d->mode = parallel_mode;
			
			/* wake all threads */
			ref_set_iterator_init(d->thread_set);
			
			while ((ref = ref_set_iterator_next(d->thread_set)) != 0) {
				if (ref == me) {
					continue;
				} else {
					o = heap_fetch_object(heap, ref);
					t = object_get_thread(o);
					ud = _thread_get_dmp(t);			
					thread_dmp_signal(ud);
				}
			}
		}
	}

	return 0;
}
