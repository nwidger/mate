/* Niels Widger
 * Time-stamp: <14 Mar 2011 at 18:25:40 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

/* default ops */
struct nlock_dmp_ops nlock_dmp_default_ops = {
	nlock_dmp_default_lock,
	NULL
};

/* default attr */
struct nlock_dmp_attr nlock_dmp_default_attr = {
	&nlock_dmp_default_ops
};

/* default functions */
int nlock_dmp_default_lock(struct nlock_dmp *nd) {
	int err;
	struct thread_dmp *td;

	td = thread_get_dmp();

	while (1) {
		if ((err = nlock_trylock(nd->nlock)) == 0)
			break;

		thread_dmp_execute_instruction(td, MONITORENTER_OPCODE);

		if (dmp_get_mode(dmp) == serial_mode) {
			mvm_print("thread %" PRIu32 ": cannot get monitor in serial mode, blocking\n", thread_get_ref());
			dmp_thread_block(dmp, td);
			break;
		}
	}

	return 0;
}

int nlock_dmp_default_unlock(struct nlock_dmp *nd) {
	nlock_unlock(nd->nlock);
	return 0;
}
