/* Niels Widger
 * Time-stamp: <02 Feb 2011 at 17:15:03 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "globals.h"
#include "heap.h"
#include "object.h"
#include "object_dmp.h"

/* struct definitions */
struct object_dmp {
	struct object *object;
	struct object_dmp_attr attr;
};

struct object_dmp * object_dmp_create(struct object *o, struct object_dmp_attr *a) {
	struct object_dmp *od;

	if ((od = (struct object_dmp *)heap_malloc(heap, sizeof(struct object_dmp))) == NULL)
		mvm_halt();

	od->object = o;
	memcpy(&od->attr, a, sizeof(struct object_dmp_attr));
	return od;
}

void object_dmp_destroy(struct object_dmp *od) {
	if (od != NULL) {
		heap_free(heap, od);
	}
}

void object_dmp_clear(struct object_dmp *od) {
	if (od != NULL) {
		memset(&od->attr, 0, sizeof(struct object_dmp_attr));
	}
}

struct object * object_dmp_get_object(struct object_dmp *od) {
	if (od == NULL) {
		fprintf(stderr, "mvm: object_dmp not initialized!\n");
		mvm_halt();
	}

	return od->object;
}

int object_dmp_set_object(struct object_dmp *od, struct object *o) {
	if (od == NULL) {
		fprintf(stderr, "mvm: object_dmp not initialized!\n");
		mvm_halt();
	}

	od->object = o;
	return 0;
}

int object_dmp_get_owner(struct object_dmp *od) {
	if (od == NULL) {
		fprintf(stderr, "mvm: object_dmp not initialized!\n");
		mvm_halt();
	}

	return od->attr.owner;
}

int object_dmp_set_owner(struct object_dmp *od, int n) {
	if (od == NULL) {
		fprintf(stderr, "mvm: object_dmp not initialized!\n");
		mvm_halt();
	}

	od->attr.owner = n;
	return 0;
}

int object_dmp_is_shared(struct object_dmp *od) {
	if (od == NULL) {
		fprintf(stderr, "mvm: object_dmp not initialized!\n");
		mvm_halt();
	}

	return SHARED(od->attr.owner);
}

int object_dmp_is_private(struct object_dmp *od) {
	if (od == NULL) {
		fprintf(stderr, "mvm: object_dmp not initialized!\n");
		mvm_halt();
	}

	return PRIVATE(od->attr.owner);
}

int object_dmp_get_depth(struct object_dmp *od) {
	if (od == NULL) {
		fprintf(stderr, "mvm: object_dmp not initialized!\n");
		mvm_halt();
	}

	return od->attr.depth;
}

int object_dmp_set_depth(struct object_dmp *od, int d) {
	if (od == NULL) {
		fprintf(stderr, "mvm: object_dmp not initialized!\n");
		mvm_halt();
	}

	od->attr.depth = d;
	return 0;
}

struct object_dmp_ops * object_dmp_get_ops(struct object_dmp *od) {
	if (od == NULL) {
		fprintf(stderr, "mvm: object_dmp not initialized!\n");
		mvm_halt();
	}

	return od->attr.ops;
}

int object_dmp_set_ops(struct object_dmp *od, struct object_dmp_ops *p) {
	if (od == NULL) {
		fprintf(stderr, "mvm: object_dmp not initialized!\n");
		mvm_halt();
	}

	od->attr.ops = p;
	return 0;
}

int object_dmp_load(struct object_dmp *od, struct thread *t, int i) {
	if (od == NULL) {
		fprintf(stderr, "mvm: object_dmp not initialized!\n");
		mvm_halt();
	}

	if (od->attr.ops->load == NULL)
		return 0;

	return (*od->attr.ops->load)(od, t, i);
}

int object_dmp_store(struct object_dmp *od, struct thread *t, int i) {
	if (od == NULL) {
		fprintf(stderr, "mvm: object_dmp not initialized!\n");
		mvm_halt();
	}

	if (od->attr.ops->store == NULL)
		return 0;

	return (*od->attr.ops->store)(od, t, i);
}

int object_dmp_chown(struct object_dmp *od, struct thread *t, int i) {
	if (od == NULL) {
		fprintf(stderr, "mvm: object_dmp not initialized!\n");
		mvm_halt();
	}

	if (od->attr.ops->chown == NULL)
		return 0;

	return (*od->attr.ops->chown)(od, t, i);
}

/* default ops */
struct object_dmp_ops object_dmp_default_ops = {
	.load  = object_dmp_default_load,
	.store = object_dmp_default_store,
	.chown = object_dmp_default_chown
};

int object_dmp_default_load(struct object_dmp *od, struct thread *t, int i) {
	fprintf(stderr, "in object_dmp_default_load\n");
	return 0;
}

int object_dmp_default_store(struct object_dmp *od, struct thread *t, int i) {
	fprintf(stderr, "in object_dmp_default_store\n");

	return 0;
}

int object_dmp_default_chown(struct object_dmp *od, struct thread *t, int n) {
	fprintf(stderr, "in object_dmp_default_chown\n");
	return 0;
}
