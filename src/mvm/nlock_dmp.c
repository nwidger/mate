/* Niels Widger
 * Time-stamp: <07 Sep 2013 at 15:20:42 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "constants.h"
#include "dmp.h"
#include "globals.h"
#include "nlock.h"
#include "nlock_dmp.h"
#include "thread.h"
#include "thread_dmp.h"

/* struct definitions */
struct nlock_dmp {
	struct nlock *nlock;
	struct nlock_dmp_attr attr;
};

struct nlock_dmp * nlock_dmp_create(struct nlock *n, struct nlock_dmp_attr *a) {
	struct nlock_dmp *nd;

	if ((nd = (struct nlock_dmp *)malloc(sizeof(struct nlock_dmp))) == NULL)
		mvm_halt();

	nd->nlock = n;
	memcpy(&nd->attr, a, sizeof(struct nlock_dmp_attr));
	return nd;
}

void nlock_dmp_destroy(struct nlock_dmp *nd) {
	if (nd != NULL) {
		free(nd);
	}
}

void nlock_dmp_clear(struct nlock_dmp *nd) {
	if (nd != NULL) {
		memset(&nd->attr, 0, sizeof(struct nlock_dmp_attr));
	}
}

struct nlock * nlock_dmp_get_nlock(struct nlock_dmp *nd) {
	if (nd == NULL) {
		fprintf(stderr, "mvm: nlock_dmp not initialized!\n");
		mvm_halt();
	}

	return nd->nlock;
}

int nlock_dmp_set_nlock(struct nlock_dmp *nd, struct nlock *n) {
	if (nd == NULL) {
		fprintf(stderr, "mvm: nlock_dmp not initialized!\n");
		mvm_halt();
	}

	nd->nlock = n;
	return 0;
}

int nlock_dmp_lock(struct nlock_dmp *nd) {
	if (nd == NULL) {
		fprintf(stderr, "mvm: nlock_dmp not initialized!\n");
		mvm_halt();
	}

	if (nd->attr.ops->lock == NULL)
		return 0;

	return (*nd->attr.ops->lock)(nd);
}

int nlock_dmp_unlock(struct nlock_dmp *nd) {
	if (nd == NULL) {
		fprintf(stderr, "mvm: nlock_dmp not initialized!\n");
		mvm_halt();
	}

	if (nd->attr.ops->unlock == NULL)
		return 0;

	return (*nd->attr.ops->unlock)(nd);
}

int nlock_dmp_timedwait(struct nlock_dmp *nd) {
	if (nd == NULL) {
		fprintf(stderr, "mvm: nlock_dmp not initialized!\n");
		mvm_halt();
	}

	if (nd->attr.ops->timedwait == NULL)
		return 0;

	return (*nd->attr.ops->timedwait)(nd);
}

/* default ops */
struct nlock_dmp_ops nlock_dmp_default_ops = {
	nlock_dmp_default_lock,
	nlock_dmp_default_unlock,
	nlock_dmp_default_timedwait
};

/* default attr */
struct nlock_dmp_attr nlock_dmp_default_attr = {
	&nlock_dmp_default_ops
};

/* default functions */
int nlock_dmp_default_lock(struct nlock_dmp *nd) {
	int tries, left;
	struct thread_dmp *td;

	tries = 5;
	left = dmp_get_mode(dmp) == parallel_mode ? tries : 0;

	td = thread_get_dmp(NULL);

	for (;;) {
		if (nlock_trylock(nd->nlock) == 0) {
			thread_dmp_incr_lock_count(td);
			break;
		}

		if (dmp_get_mode(dmp) == serial_mode || dmp_thread_block_toggles_mode(dmp)) {
			mvm_print("thread %" PRIu32 ": cannot get monitor in serial mode or last thread to block, blocking\n", thread_get_ref(NULL));
			dmp_thread_block(dmp, td);
			/* now in parallel mode, refill tries */
			if (dmp_get_mode(dmp) == parallel_mode) left = tries;
		} else if (dmp_get_mode(dmp) == parallel_mode && --left == 0) {
			mvm_print("thread %" PRIu32 ": cannot get monitor after %d tries in parallel mode, blocking\n", thread_get_ref(NULL), tries);
			dmp_thread_block(dmp, td);
			left = 0; /* now in serial mode, empty tries */
		}
	}

	return 0;
}

int nlock_dmp_default_unlock(struct nlock_dmp *nd) {
	int lock_count;
	struct thread_dmp *td;
	enum thread_dmp_serial_mode mode;

	td = thread_get_dmp(NULL);
	lock_count = thread_dmp_decr_lock_count(td);
	mode = thread_dmp_get_serial_mode(td);

	if (mode == reduced_mode && lock_count == 0 &&
	    dmp_get_mode(dmp) == serial_mode) {
			mvm_print("thread %" PRIu32 ": unlocked last monitor in reduced serial mode, blocking\n", thread_get_ref(NULL));
		dmp_thread_block(dmp, td);
	}

	return 0;
}

int nlock_dmp_default_timedwait(struct nlock_dmp *nd) {
	enum dmp_mode mode;
	struct thread_dmp *td;

	td = thread_get_dmp(NULL);

	mode = dmp_get_mode(dmp);

	if (mode == serial_mode || dmp_thread_block_toggles_mode(dmp)) {
		mvm_print("thread %" PRIu32 ": in timedwait and serial mode or last thread to block, blocking\n", thread_get_ref(NULL));
		dmp_thread_block(dmp, td);
	}

	return 0;
}
