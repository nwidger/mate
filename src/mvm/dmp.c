/* Niels Widger
 * Time-stamp: <22 Feb 2011 at 22:18:52 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "barrier.h"
#include "dmp.h"
#include "garbage_collector.h"
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
int dmp_acquiesce(struct dmp *d, int r, enum thread_dmp_state s);
int dmp_acquiesce_all(struct dmp *d, enum thread_dmp_state s);
int dmp_wake_all(struct dmp *d);
int dmp_toggle_mode(struct dmp *d);
int dmp_thread_block_parallel(struct dmp *d, struct thread_dmp *td);
int dmp_thread_block_serial(struct dmp *d, struct thread_dmp *td);

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

int dmp_toggle_mode(struct dmp *d) {
	int mode;

	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	mode = d->mode;

	if (mode == parallel_mode) {
		mvm_print("thread %" PRIu32 ": entering serial mode\n", thread_get_ref());
		d->mode = serial_mode;
	} else {
		mvm_print("thread %" PRIu32 ": entering parallel mode\n", thread_get_ref());
		d->mode = parallel_mode;
	}

	return 0;
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

int dmp_acquiesce(struct dmp *d, int r, enum thread_dmp_state s) {
	int ref;
	struct object *o;
	struct thread *t;
	struct thread_dmp *ud;

	mvm_print("thread %" PRIu32 ": blocking until thread %" PRIu32 " is in state %d\n", thread_get_ref(), r, s);

	o = heap_fetch_object(heap, ref);
	t = object_get_thread(o);
	ud = _thread_get_dmp(t);

	while (thread_dmp_get_state_nonblock(ud) != s);

	return 0;
}

int dmp_acquiesce_all(struct dmp *d, enum thread_dmp_state s) {
	int me, nthreads, blocking, ref;

	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	mvm_print("thread %" PRIu32 ": blocking until all threads are in state %d\n", thread_get_ref(), s);

	me = thread_get_ref();

	/* ensure all threads are sleeping */
	nthreads = ref_set_size(d->thread_set) - 1;

	if (nthreads > 0) {
		blocking = 0;
		ref_set_iterator_init(d->thread_set);

		while (blocking != nthreads) {
			if (ref == me) {
				continue;
			} else if ((ref = ref_set_iterator_next(d->thread_set)) == 0) {
				ref_set_iterator_init(d->thread_set);
			} else {
				if (dmp_acquiesce(dmp, ref, s) == 0)
					blocking++;
			}
		}
	}

	return 0;
}

int dmp_wake_all(struct dmp *d) {
	int me, ref;
	struct object *o;
	struct thread *t;
	struct thread_dmp *ud;

	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	mvm_print("thread %" PRIu32 ": waking all threads\n", thread_get_ref());

	me = thread_get_ref();

	/* ensure all threads are sleeping */
	dmp_acquiesce_all(dmp, blocking_state);

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

	return 0;
}

int dmp_thread_block(struct dmp *d, struct thread_dmp *td) {
	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	mvm_print("thread %" PRIu32 ": in dmp_thread_block\n", thread_get_ref());

	return (d->mode == parallel_mode) ?
		dmp_thread_block_parallel(d, td) :
		dmp_thread_block_serial(d, td);
}

int dmp_thread_block_parallel(struct dmp *d, struct thread_dmp *td) {
	struct object *o;
	struct thread *t;
	int me, ref, retval;
	struct thread_dmp *ud;

	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	me = thread_get_ref();

	/* block until all threads finished */
	retval = barrier_await(d->barrier);

	if (retval != 0) { /* 0 == last to block */
		/* sleep until turn */
		thread_dmp_wait(td);
	} else {
		dmp_toggle_mode(d);

		mvm_print("thread %" PRIu32 ": ensuring all threads are sleeping...\n", thread_get_ref());

		/* ensure all threads are sleeping */
		dmp_acquiesce_all(dmp, blocking_state);

		ref_set_iterator_init(d->thread_set);
		ref = ref_set_iterator_next(d->thread_set);

		mvm_print("thread %" PRIu32 ": waking first thread %" PRIu32 "\n", thread_get_ref(), ref);

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

	return 0;
}

int dmp_thread_block_serial(struct dmp *d, struct thread_dmp *td) {
	struct object *o;
	struct thread *t;
	struct thread_dmp *ud;
	int me, ref, nthreads;

	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	/* ensure all threads are sleeping */
	dmp_acquiesce_all(dmp, blocking_state);

	me = thread_get_ref();

	ref = ref_set_iterator_next(d->thread_set);

	if (ref != 0) {
		mvm_print("thread %" PRIu32 ": waking next thread %" PRIu32"\n", thread_get_ref(), ref);

		/* wake next thread */
		o = heap_fetch_object(heap, ref);
		t = object_get_thread(o);
		ud = _thread_get_dmp(t);
		thread_dmp_signal(ud);

		/* block until all threads finished */
		barrier_await(d->barrier);
	} else {
		mvm_print("thread %" PRIu32 ": calling garbage collector\n", thread_get_ref());

		if (garbage_collector_collect_now(garbage_collector) != 0)
			mvm_halt();

		mvm_print("thread %" PRIu32 ": returned from garbage collector\n", thread_get_ref());

		nthreads = ref_set_size(d->thread_set);
		mvm_print("thread %" PRIu32 ": resetting barrier to %d threads\n", thread_get_ref(), nthreads);
		barrier_reset_parties(dmp->barrier, nthreads);
		dmp_toggle_mode(d);

		/* wake all threads */
		dmp_wake_all(dmp);
	}

	return 0;
}
