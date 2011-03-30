/* Niels Widger
 * Time-stamp: <13 Mar 2011 at 14:35:23 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dmp.h"
#include "globals.h"
#include "heap.h"
#include "object.h"
#include "object_dmp.h"
#include "ref_set.h"
#include "thread.h"

/* struct definitions */
struct object_dmp {
	struct object *object;
	struct object_dmp_attr attr;
};

/* forward declarations */
int object_dmp_default_chown_aux(struct object_dmp *od, int n, int d);

struct object_dmp * object_dmp_create(struct object *o, struct object_dmp_attr *a) {
	struct thread *t;
	struct object_dmp *od;

	if ((od = (struct object_dmp *)heap_malloc(heap, sizeof(struct object_dmp))) == NULL)
		mvm_halt();

	od->object = o;
	memcpy(&od->attr, a, sizeof(struct object_dmp_attr));

	if (od->attr.owner == -1) {
		t = thread_get_current();
		od->attr.owner = thread_get_ref(t);
	}

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

int object_dmp_load(struct object_dmp *od, int i) {
	if (od == NULL) {
		fprintf(stderr, "mvm: object_dmp not initialized!\n");
		mvm_halt();
	}

	if (od->attr.ops->load == NULL)
		return 0;

	return (*od->attr.ops->load)(od, i);
}

int object_dmp_store(struct object_dmp *od, int i, int r) {
	if (od == NULL) {
		fprintf(stderr, "mvm: object_dmp not initialized!\n");
		mvm_halt();
	}

	if (od->attr.ops->store == NULL)
		return 0;

	return (*od->attr.ops->store)(od, i, r);
}

int object_dmp_chown(struct object_dmp *od, int i) {
	if (od == NULL) {
		fprintf(stderr, "mvm: object_dmp not initialized!\n");
		mvm_halt();
	}

	if (od->attr.ops->chown == NULL)
		return 0;

	return (*od->attr.ops->chown)(od, i);
}

/* default ops */
struct object_dmp_ops object_dmp_default_ops = {
	object_dmp_default_load,
	object_dmp_default_store,
	object_dmp_default_chown
};

/* default attr */
struct object_dmp_attr object_dmp_default_attr = {
	OBJECT_DMP_DEFAULT_OWNER,
	OBJECT_DMP_DEFAULT_DEPTH,
	&object_dmp_default_ops
};

/* default functions */
int object_dmp_default_load(struct object_dmp *od, int i) {
	int me, current;
	struct thread_dmp *td;

	mvm_print("thread %" PRIu32 ": in object_dmp_default_load\n", thread_get_ref());

	current = object_dmp_get_owner(od);
	me = thread_get_ref();
	td = thread_get_dmp();

	if (current == 0) {
		mvm_print("thread %" PRIu32 ":     shared: proceed!\n", thread_get_ref());
	} else if (current != me) {
		mvm_print("thread %" PRIu32 ":     private not owned by me: block, set shared, proceed!\n", thread_get_ref());

		if (dmp_get_mode(dmp) == parallel_mode) {
			/* block */
			dmp_thread_block(dmp, td);
		}

		/* set shared */
		object_dmp_chown(od, 0);
	} else {
		mvm_print("thread %" PRIu32 ":     private owned by me: proceed!\n", thread_get_ref());
	}

	/* proceed */

	return 0;
}

int object_dmp_default_store(struct object_dmp *od, int i, int r) {
	int me, current;
	struct thread_dmp *td;

	mvm_print("thread %" PRIu32 ": in object_dmp_default_store\n", thread_get_ref());

	current = object_dmp_get_owner(od);
	me = thread_get_ref();
	td = thread_get_dmp();

	if (current == 0) {
		mvm_print("thread %" PRIu32 ":     shared: block, set private owned by me, proceed!\n", thread_get_ref());

		if (dmp_get_mode(dmp) == parallel_mode) {
			/* block */
			dmp_thread_block(dmp, td);
		}

		/* set private owned by me */
		object_dmp_chown(od, me);
	} else if (current != me) {
		mvm_print("thread %" PRIu32 ":     private not owned by me: block, set shared, proceed!\n", thread_get_ref());

		if (dmp_get_mode(dmp) == parallel_mode) {
			/* block */
			dmp_thread_block(dmp, td);
		}

		/* set shared */
		object_dmp_chown(od, me);
	} else {
		mvm_print("thread %" PRIu32 ":     private owned by me: proceed!\n", thread_get_ref());
	}

	/* proceed */

	return 0;
}

int object_dmp_default_chown(struct object_dmp *od, int n) {
	mvm_print("thread %" PRIu32 ": in object_dmp_default_chown\n", thread_get_ref());
	return object_dmp_default_chown_aux(od, n, od->attr.depth);
}

int object_dmp_default_chown_aux(struct object_dmp *od, int n, int d) {
	int ref;
	struct ref_set *fields;
	struct object_dmp *field_od;
	struct object *object, *field;

	if (d == 0)
		return 0;

	od->attr.owner = n;

	object = od->object;
	fields = ref_set_create();
	object_populate_ref_set(object, fields);

	ref_set_iterator_init(fields);

	while ((ref = ref_set_iterator_next(fields)) != 0) {
		field = heap_fetch_object(heap, ref);
		field_od = object_get_dmp(field);

		object_dmp_default_chown_aux(field_od, n, d-1);
	}

	return 0;
}
