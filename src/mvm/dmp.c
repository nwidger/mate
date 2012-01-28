/* Niels Widger
 * Time-stamp: <27 Jan 2012 at 20:22:53 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "barrier.h"
#include "constants.h"
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

	int first;
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
int dmp_toggle_mode(struct dmp *d);
void * dmp_barrier_parallel_hook(int i, void *a);
void * dmp_barrier_serial_hook(int i, void *a);

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

	dmp->first = 0;
	dmp->barrier = barrier_create(0);
	barrier_set_hook(dmp->barrier, dmp_barrier_parallel_hook, dmp);

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
		ref_set_clear(d->thread_set);
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
	void * (*hook)(int, void *);

	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	if (d->mode == parallel_mode) {
		mvm_print("thread %" PRIu32 ": entering serial mode\n", thread_get_ref(NULL));
		d->mode = serial_mode;
		hook = dmp_barrier_serial_hook;
	} else {
		mvm_print("thread %" PRIu32 ": entering parallel mode\n", thread_get_ref(NULL));
		d->mode = parallel_mode;
		hook = dmp_barrier_parallel_hook;

	}

	barrier_set_hook(d->barrier, hook, (void *)d);
	barrier_reset(d->barrier);

	if (debug == 0 && verbose != 0)
		ref_set_dump_iterator(d->thread_set);

	return 0;
}

int dmp_add_thread(struct dmp *d, int r) {
	struct thread *t;
	
	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	if (r != 1 && dmp_get_mode(dmp) == parallel_mode) {
		/* assertion fail */
		mvm_print("adding thread %d to thread_set in parallel mode!\n", r);
		mvm_halt();
	}

	if ((t = thread_get_current(NULL)) == NULL)
		mvm_print("adding thread %d to thread_set!\n", r);
	else
		mvm_print("thread %" PRIu32 ": adding thread %d to thread_set!\n", thread_get_ref(t), r);

	ref_set_add(d->thread_set, r);
	barrier_inc_parties(d->barrier);

	if (debug == 0 && verbose != 0)
		ref_set_dump_iterator(d->thread_set);
	
	return 0;
}

int dmp_remove_thread(struct dmp *d, int r) {
	struct thread *t;
	
	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	if (dmp_get_mode(dmp) == serial_mode) {
		mvm_print("removing thread %d from thread_set in serial mode!\n", r);
		mvm_halt();
	}

	if ((t = thread_get_current(NULL)) == NULL)
		mvm_print("removing thread %d from thread_set!\n", r);
	else
		mvm_print("thread %" PRIu32 ": removing thread %d from thread_set!\n", thread_get_ref(t), r);

	ref_set_remove(d->thread_set, r);
	barrier_dec_parties(d->barrier);
	
	if (debug == 0 && verbose != 0)
		ref_set_dump_iterator(d->thread_set);

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
	struct object *o;
	struct thread *t;
	struct thread_dmp *ud;

	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	if (r == thread_get_ref(NULL)) {
		mvm_print("thread %" PRIu32 ": tried to acquiesce on ourselves!\n", thread_get_ref(NULL));
		mvm_halt();
	}

	mvm_print("thread %" PRIu32 ": blocking until thread %" PRIu32 " is in state %s\n", thread_get_ref(NULL), r,
		  thread_dmp_state_to_string(s));

	o = heap_fetch_object(heap, r);
	t = object_get_thread(o);
	ud = thread_get_dmp(t);

	while (thread_dmp_get_state_nonblock(ud) != s);

	return 0;
}

int dmp_thread_block(struct dmp *d, struct thread_dmp *td) {
	int me, ref;
	struct object *o;
	struct thread *t;
	struct thread_dmp *ud;

	ref = me = -1;

	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	mvm_print("thread %" PRIu32 ": in dmp_thread_block\n", thread_get_ref(NULL));

	thread_dmp_set_state(td, blocking_state);

	if (d->mode == serial_mode) {
		ref = ref_set_iterator_next(d->thread_set);

		if (ref != 0) {
			dmp_acquiesce(d, ref, waiting_state);
			o = heap_fetch_object(heap, ref);
			t = object_get_thread(o);
			ud = thread_get_dmp(t);
			thread_dmp_signal(ud);
		}
	}

	barrier_await(d->barrier);

	if (d->mode == serial_mode) {
		me = thread_get_ref(NULL);
		if (me != d->first) thread_dmp_wait(td);
	}

	thread_dmp_set_state(td, running_state);

	return 0;
}

void * dmp_barrier_parallel_hook(int i, void *a) {
	struct dmp *d;

	d = (struct dmp *)a;

	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	if (i == 0) {
		/* determine first thread */
		ref_set_iterator_init(d->thread_set);
		d->first = ref_set_iterator_next(d->thread_set);
                /* end of parallel mode */
		dmp_toggle_mode(d);
	}

	return NULL;
}

void * dmp_barrier_serial_hook(int i, void *a) {
	double load;	
	int nthreads;
	struct dmp *d;
	uint64_t size, free;

	d = (struct dmp *)a;

	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	if (i == 0) {
		size = heap_get_size(heap);
		free = heap_get_free(heap);

		load = (size - free) / size;

		if (load > DMP_GARBAGE_COLLECTOR_LOAD_FACTOR) {
			/* run garbage collector */
			mvm_print("thread %" PRIu32 ": calling garbage collector\n", thread_get_ref(NULL));

			if (garbage_collector_collect_now(garbage_collector) != 0)
				mvm_halt();

			mvm_print("thread %" PRIu32 ": returned from garbage collector\n", thread_get_ref(NULL));
		}

		ref_set_iterator_init(d->thread_set);
		d->first = 0;
		/* update barrier */
		nthreads = ref_set_size(d->thread_set);
		barrier_reset_parties(d->barrier, nthreads);
                /* end of serial mode */
		dmp_toggle_mode(d);
	}

	return NULL;
}
