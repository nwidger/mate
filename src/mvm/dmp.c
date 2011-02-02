/* Niels Widger
 * Time-stamp: <02 Feb 2011 at 17:18:16 by nwidger on macros.local>
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
#include "object_dmp.h"
#include "ref_set.h"
#include "thread_dmp.h"

/* struct definitions */
struct dmp {
	enum dmp_mode mode;

	struct ref_set *thread_set;

	struct barrier *pbarrier;
	struct barrier *sbarrier;

	/* object_dmp defaults */
	struct object_dmp_attr *od_attr;

	/* thread_dmp defaults */
	struct thread_dmp_attr *td_attr;
};

struct dmp * dmp_create(struct object *o, struct object_dmp_attr *a, struct thread_dmp_attr *t) {
	struct dmp *dmp;

	if ((dmp = (struct dmp *)malloc(sizeof(struct dmp))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	dmp->mode = round_start_mode;

	dmp->thread_set = ref_set_create();

	dmp->pbarrier = barrier_create(1);
	dmp->sbarrier = barrier_create(1);

	dmp->od_attr = a;
	dmp->td_attr = t;

	return dmp;
}

void dmp_destroy(struct dmp *d) {
	if (d != NULL) {
		ref_set_destroy(d->thread_set);
		barrier_destroy(d->pbarrier);
		barrier_destroy(d->sbarrier);
		free(d);
	}
}

void dmp_clear(struct dmp *d) {
	if (d != NULL) {
		barrier_clear(d->pbarrier);
		barrier_clear(d->sbarrier);
		d->od_attr = NULL;
	}
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

int dmp_start_watchdog(struct dmp *d) {
	return 0;
}

int dmp_stop_watchdog(struct dmp *d) {
	return 0;
}

void dmp_watchdog(void *arg) {
	while (1) {		/* while nthreads > 0 */
		/* set mode to round_start */

		/* initialize pbarrier to nthreads */

		/* set mode to parallel_start_mode */

		/* wakeup all nthreads, go to sleep */

		/* last thread to hit pbarrier wakes us up */

		/* set mode to parallel_end_mode */

		/* set mode to serial_start_mode */

		/* for (thread t : thread_list)
		 *     wakeup thread, go to sleep
		 *     wakeup when thread hits sbarrier
		 */

		/* set mode to serial_end_mode */

		/* run garbage collector */

		/* set mode to round_end_mode */
	}
}
