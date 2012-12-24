/* Niels Widger
 * Time-stamp: <23 Dec 2012 at 20:51:04 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "globals.h"
#include "heap.h"
#include "local_variable_array.h"
#include "nlock.h"
#include "object.h"

/* struct definitions */
struct local_variable_array {
	int *locals;
	int size;
	struct nlock *nlock;
};

struct local_variable_array * local_variable_array_create(int m) {
	struct local_variable_array *l;

	if (m < 0) {
		fprintf(stderr, "mvm: local variable array size must be >= 0!\n");
		mvm_halt();
	}

	if ((l = (struct local_variable_array *)malloc(sizeof(struct local_variable_array))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	l->size = m;

	if ((l->locals = (int *)calloc(m, sizeof(int))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	if ((l->nlock = nlock_create()) == NULL)
		mvm_halt();
	
	return l;
}

void local_variable_array_destroy(struct local_variable_array *l) {
	if (l != NULL) {
		/* lock */
		/* local_variable_array_lock(l); */

		local_variable_array_clear(l);
		free(l->locals);

		/* unlock */
		/* local_variable_array_unlock(l); */

		nlock_destroy(l->nlock);
		free(l);
	}
}

void local_variable_array_clear(struct local_variable_array *l) {
	if (l != NULL) {
		/* lock */
		/* local_variable_array_lock(l); */

		memset(l->locals, 0, sizeof(int)*l->size);

		/* unlock */
		/* local_variable_array_unlock(l); */
	}
}

int local_variable_array_size(struct local_variable_array *l) {
	int size;

	if (l == NULL) {
		fprintf(stderr, "mvm: local variable array not initialized!\n");
		mvm_halt();
	}

	/* lock */
	/* local_variable_array_lock(l); */

	size = l->size;

	/* unlock */
	/* local_variable_array_unlock(l); */

	return size;
}

int local_variable_array_load(struct local_variable_array *l, int i) {
	int ref;

	if (l == NULL) {
		fprintf(stderr, "mvm: local variable array not initialized!\n");
		mvm_halt();
	}

	/* lock */
	/* local_variable_array_lock(l); */

	if (i < 0 || i >= l->size) {
		fprintf(stderr, "mvm: locals index out of bounds!\n");
		mvm_halt();
	}

	ref = l->locals[i];

	/* unlock */
	/* local_variable_array_unlock(l); */

	return ref;
}

int local_variable_array_store(struct local_variable_array *l, int i, int r) {
	int tmp;

	if (l == NULL) {
		fprintf(stderr, "mvm: local variable array not initialized!\n");
		mvm_halt();
	}

	/* lock */
	/* local_variable_array_lock(l); */

	if (i < 0 || i >= l->size) {
		fprintf(stderr, "mvm: locals index out of bounds!\n");
		mvm_halt();
	}

	tmp = l->locals[i];
	l->locals[i] = r;

	/* unlock */
	/* local_variable_array_unlock(l); */

	return tmp;
}

int local_variable_array_dump(struct local_variable_array *l) {
	int i;
	struct object *object;

	if (l == NULL) {
		fprintf(stderr, "mvm: local variable array not initialized!\n");
		mvm_halt();
	}

	if (l->size == 0) {
		fprintf(stderr, "empty!\n");
		return 0;
	}

	for (i = 0; i < l->size; i++) {
		fprintf(stderr, "#%d  ", i);
		object = heap_fetch_object(heap, l->locals[i]);
		object_dump(object, 0);
		fprintf(stderr, "\n");
	}

	return 0;
}

int local_variable_array_lock(struct local_variable_array *l) {
	if (l == NULL) {
		fprintf(stderr, "mvm: local variable array not initialized!\n");
		mvm_halt();
	}

	/* unlock */
	/* nlock_lock(l->nlock); */

	return 0;
}

int local_variable_array_unlock(struct local_variable_array *l) {
	if (l == NULL) {
		fprintf(stderr, "mvm: local variable array not initialized!\n");
		mvm_halt();
	}

	/* unlock */
	/* nlock_unlock(l->nlock); */

	return 0;
}

