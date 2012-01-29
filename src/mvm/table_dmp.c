/* Niels Widger
 * Time-stamp: <29 Jan 2012 at 14:20:13 by nwidger on macros.local>
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
#include "object_dmp.h"
#include "table.h"
#include "table_dmp.h"
#include "thread.h"
#include "thread_dmp.h"

/* struct definitions */
struct table_dmp {
	struct table *table;
	struct object_dmp *od;
	struct table_dmp_attr attr;
};

struct table_dmp * table_dmp_create(struct table *t, struct table_dmp_attr *a, struct object_dmp *od) {
	struct table_dmp *td;

	if ((td = (struct table_dmp *)heap_malloc(heap, sizeof(struct table_dmp))) == NULL)
		mvm_halt();

	td->table = t;
	td->od = od;
	memcpy(&td->attr, a, sizeof(struct table_dmp_attr));

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

int table_dmp_load(struct table_dmp *td) {
	if (td == NULL) {
		fprintf(stderr, "mvm: table_dmp not initialized!\n");
		mvm_halt();
	}

	if (td->attr.ops->load == NULL)
		return 0;

	return (*td->attr.ops->load)(td);
}

int table_dmp_store(struct table_dmp *td) {
	if (td == NULL) {
		fprintf(stderr, "mvm: table_dmp not initialized!\n");
		mvm_halt();
	}

	if (td->attr.ops->store == NULL)
		return 0;

	return (*td->attr.ops->store)(td);
}

/* default ops */
struct table_dmp_ops table_dmp_default_ops = {
	table_dmp_default_load,
	table_dmp_default_store,
};

/* default attr */
struct table_dmp_attr table_dmp_default_attr = {
	&table_dmp_default_ops
};

/* default functions */
int table_dmp_default_load(struct table_dmp *td) {
	mvm_print("thread %" PRIu32 ": in table_dmp_default_load\n", thread_get_ref(NULL));

	return object_dmp_load(td->od, 0);
}

int table_dmp_default_store(struct table_dmp *td) {
	mvm_print("thread %" PRIu32 ": in table_dmp_default_store\n", thread_get_ref(NULL));

	return object_dmp_store(td->od, 0, 0);
}
