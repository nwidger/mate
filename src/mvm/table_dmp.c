/* Niels Widger
 * Time-stamp: <28 Jan 2012 at 15:08:20 by nwidger on macros.local>
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
#include "heap.h"
#include "table.h"
#include "table_dmp.h"
#include "thread.h"
#include "thread_dmp.h"

/* struct definitions */
struct table_dmp {
	struct table *table;
	struct table_dmp_attr attr;
};

struct table_dmp * table_dmp_create(struct table *t, struct table_dmp_attr *a) {
	struct thread *u;
	struct table_dmp *td;

	if ((td = (struct table_dmp *)heap_malloc(heap, sizeof(struct table_dmp))) == NULL)
		mvm_halt();

	td->table = t;
	memcpy(&td->attr, a, sizeof(struct table_dmp_attr));

	if (td->attr.owner == -1) {
		u = thread_get_current();
		td->attr.owner = (u == NULL) ? 0 : thread_get_ref(u);
	}

	return td;
}

void table_dmp_destroy(struct table_dmp *td) {
	if (td != NULL) {
		heap_free(heap, td);
	}
}

void table_dmp_clear(struct table_dmp *td) {
	if (td != NULL) {
		memset(&td->attr, 0, sizeof(struct table_dmp_attr));
	}
}

int table_dmp_get_owner(struct table_dmp *td) {
	if (td == NULL) {
		fprintf(stderr, "mvm: table_dmp not initialized!\n");
		mvm_halt();
	}

	return td->attr.owner;
}

int table_dmp_get(struct table_dmp *td, struct object *k) {
	if (td == NULL) {
		fprintf(stderr, "mvm: table_dmp not initialized!\n");
		mvm_halt();
	}

	if (td->attr.ops->get == NULL)
		return 0;

	return (*td->attr.ops->get)(td, k);
}

int table_dmp_put(struct table_dmp *td, struct object *k, struct object *v) {
	if (td == NULL) {
		fprintf(stderr, "mvm: table_dmp not initialized!\n");
		mvm_halt();
	}

	if (td->attr.ops->put == NULL)
		return 0;

	return (*td->attr.ops->put)(td, k, v);
}

int table_dmp_remove(struct table_dmp *td, struct object *k) {
	if (td == NULL) {
		fprintf(stderr, "mvm: table_dmp not initialized!\n");
		mvm_halt();
	}

	if (td->attr.ops->remove == NULL)
		return 0;

	return (*td->attr.ops->remove)(td, k);
}

int table_dmp_first_key(struct table_dmp *td) {
	if (td == NULL) {
		fprintf(stderr, "mvm: table_dmp not initialized!\n");
		mvm_halt();
	}

	if (td->attr.ops->first_key == NULL)
		return 0;

	return (*td->attr.ops->first_key)(td);
}

int table_dmp_next_key(struct table_dmp *td) {
	if (td == NULL) {
		fprintf(stderr, "mvm: table_dmp not initialized!\n");
		mvm_halt();
	}

	if (td->attr.ops->next_key == NULL)
		return 0;

	return (*td->attr.ops->next_key)(td);
}

/* default ops */
struct table_dmp_ops table_dmp_default_ops = {
	table_dmp_default_get,
	table_dmp_default_put,
	table_dmp_default_remove,
	table_dmp_default_first_key,
	table_dmp_default_next_key,
};

/* default attr */
struct table_dmp_attr table_dmp_default_attr = {
	TABLE_DMP_DEFAULT_OWNER,
	&table_dmp_default_ops
};

/* default functions */
int table_dmp_default_get(struct table_dmp *td, struct object *k) {
	int me, current;
	struct thread_dmp *ud;
	enum dmp_owner_action oaction;
	enum dmp_thread_action taction;

	mvm_print("thread %" PRIu32 ": in table_dmp_default_get\n", thread_get_ref(NULL));

	me = thread_get_ref(NULL);
	current = table_dmp_get_owner(td);

	dmp_shm_read(dmp, current, me, &taction, &oaction);

	switch (taction) {
	case block_action:
		/* block */
		ud = thread_get_dmp(NULL);
		dmp_thread_block(dmp, ud);
		break;
	case proceed_action:
		/* proceed */
		break;
	}

	switch (oaction) {
	case shared_action:
		/* set shared */
		td->attr.owner = 0;
		break;
	case private_action:
		/* set private */
		td->attr.owner = me;
		break;
	case no_action:
		break;
	}

	return 0;
}

int table_dmp_default_put(struct table_dmp *td, struct object *k, struct object *v) {
	int me, current;
	struct thread_dmp *ud;
	enum dmp_owner_action oaction;
	enum dmp_thread_action taction;

	mvm_print("thread %" PRIu32 ": in table_dmp_default_put\n", thread_get_ref(NULL));

	me = thread_get_ref(NULL);
	current = table_dmp_get_owner(td);

	dmp_shm_write(dmp, current, me, &taction, &oaction);

	switch (taction) {
	case block_action:
		/* block */
		ud = thread_get_dmp(NULL);
		dmp_thread_block(dmp, ud);
		break;
	case proceed_action:
		/* proceed */
		break;
	}

	switch (oaction) {
	case shared_action:
		/* set shared */
		td->attr.owner = 0;
		break;
	case private_action:
		/* set private */
		td->attr.owner = me;
		break;
	case no_action:
		break;
	}

	return 0;
}

int table_dmp_default_remove(struct table_dmp *td, struct object *k) {
	int me, current;
	struct thread_dmp *ud;
	enum dmp_owner_action oaction;
	enum dmp_thread_action taction;

	mvm_print("thread %" PRIu32 ": in table_dmp_default_remove\n", thread_get_ref(NULL));

	me = thread_get_ref(NULL);
	current = table_dmp_get_owner(td);

	dmp_shm_write(dmp, current, me, &taction, &oaction);

	switch (taction) {
	case block_action:
		/* block */
		ud = thread_get_dmp(NULL);
		dmp_thread_block(dmp, ud);
		break;
	case proceed_action:
		/* proceed */
		break;
	}

	switch (oaction) {
	case shared_action:
		/* set shared */
		td->attr.owner = 0;
		break;
	case private_action:
		/* set private */
		td->attr.owner = me;
		break;
	case no_action:
		break;
	}

	return 0;
}

int table_dmp_default_first_key(struct table_dmp *td) {
	int me, current;
	struct thread_dmp *ud;
	enum dmp_owner_action oaction;
	enum dmp_thread_action taction;

	mvm_print("thread %" PRIu32 ": in table_dmp_default_first_key\n", thread_get_ref(NULL));

	me = thread_get_ref(NULL);
	current = table_dmp_get_owner(td);

	dmp_shm_write(dmp, current, me, &taction, &oaction);

	switch (taction) {
	case block_action:
		/* block */
		ud = thread_get_dmp(NULL);
		dmp_thread_block(dmp, ud);
		break;
	case proceed_action:
		/* proceed */
		break;
	}

	switch (oaction) {
	case shared_action:
		/* set shared */
		td->attr.owner = 0;
		break;
	case private_action:
		/* set private */
		td->attr.owner = me;
		break;
	case no_action:
		break;
	}

	return 0;
}

int table_dmp_default_next_key(struct table_dmp *td) {
	int me, current;
	struct thread_dmp *ud;
	enum dmp_owner_action oaction;
	enum dmp_thread_action taction;

	mvm_print("thread %" PRIu32 ": in table_dmp_default_next_key\n", thread_get_ref(NULL));

	me = thread_get_ref(NULL);
	current = table_dmp_get_owner(td);

	dmp_shm_write(dmp, current, me, &taction, &oaction);

	switch (taction) {
	case block_action:
		/* block */
		ud = thread_get_dmp(NULL);
		dmp_thread_block(dmp, ud);
		break;
	case proceed_action:
		/* proceed */
		break;
	}

	switch (oaction) {
	case shared_action:
		/* set shared */
		td->attr.owner = 0;
		break;
	case private_action:
		/* set private */
		td->attr.owner = me;
		break;
	case no_action:
		break;
	}

	return 0;
}

