/* Niels Widger
 * Time-stamp: <27 Jan 2011 at 19:36:24 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "class.h"
#include "class_table.h"
#include "globals.h"
#include "frame.h"
#include "heap.h"
#include "integer.h"
#include "invoke_method.h"
#include "nlock.h"
#include "object.h"
#include "operand_stack.h"
#include "real.h"
#include "ref_set.h"
#include "string.h"
#include "table.h"
#include "thread.h"
#include "vm_stack.h"

/* struct definitions */
struct object {
	int ref;

	int predefined_type;
	union predefined {
		struct integer *integer;
		struct string *string;
		struct table *table;
		struct thread *thread;
		struct real *real;
	} predefined;

	struct class *class;
	uint32_t num_fields;
	uint32_t *fields;

	struct nlock *monitor;

	int owner;
};

/* forward declarations */
int object_destroy_predefined_type(struct object *o);

int object_create(struct class *c, uint32_t n, struct object **o) {
	int size, ref;
	struct object *object;

	size = sizeof(struct object)+(sizeof(uint32_t)*n);
	if ((object = (struct object *)heap_malloc_ref(heap, size, &ref)) == NULL)
		mvm_halt();

	object->ref = ref;
	object->predefined_type = object_type;

	object->class = c;
        object->num_fields = n;
	object->fields = (uint32_t *)(((uint8_t *)object)+sizeof(struct object));

	memset(object->fields, 0, sizeof(uint32_t)*object->num_fields);

	if ((object->monitor = nlock_create()) == NULL)
		mvm_halt();

	object->owner = 0;

	if (o != NULL)
		*o = object;

	return ref;
}

void object_destroy(struct object *o) {
	if (o != NULL) {
		object_clear(o);
		object_destroy_predefined_type(o);
		nlock_destroy(o->monitor);
		heap_free(heap, o);
	}
}

void object_clear(struct object *o) {
	if (o != NULL) {
		o->class = NULL;
		memset(o->fields, 0, sizeof(uint32_t)*o->num_fields);
	}
}

int object_acquire_monitor(struct object *o) {
	if (o == NULL) {
		fprintf(stderr, "mvm: object has not been initialized!\n");
		mvm_halt();
	}

	nlock_lock(o->monitor);
	return 0;
}

int object_release_monitor(struct object *o) {
	if (o == NULL) {
		fprintf(stderr, "mvm: object has not been initialized!\n");
		mvm_halt();
	}

	nlock_unlock(o->monitor);
	return 0;
}

int object_wait(struct object *o) {
	if (o == NULL) {
		fprintf(stderr, "mvm: object has not been initialized!\n");
		mvm_halt();
	}

	return nlock_wait(o->monitor);
}

int object_timedwait(struct object *o, struct object *p) {
	int32_t timeout;
	struct integer *i;

	if (o == NULL) {
		fprintf(stderr, "mvm: object has not been initialized!\n");
		mvm_halt();
	}

	i = object_get_integer(p);
	timeout = integer_get_value(i);

	return nlock_timedwait(o->monitor, (long)timeout);
}

int object_notify(struct object *o) {
	return nlock_notify(o->monitor);
}

int object_notify_all(struct object *o) {
	return nlock_notify_all(o->monitor);
}

int object_get_ref(struct object *o) {
	if (o == NULL) {
		fprintf(stderr, "mvm: object has not been initialized!\n");
		mvm_halt();
	}

	return o->ref;
}

struct class * object_get_class(struct object *o) {
	if (o == NULL) {
		fprintf(stderr, "mvm: object has not been initialized!\n");
		mvm_halt();
	}

	return o->class;
}

int object_load_field(struct object *o, int i) {
	if (o == NULL) {
		fprintf(stderr, "mvm: object has not been initialized!\n");
		mvm_halt();
	}

	if (i < 0 || i >= o->num_fields) {
		fprintf(stderr, "mvm: invalid field index!\n");
		mvm_halt();
	}

	return o->fields[i];
}

int object_store_field(struct object *o, int i, int r) {
	int tmp;

	if (o == NULL) {
		fprintf(stderr, "mvm: object has not been initialized!\n");
		mvm_halt();
	}

	if (i < 0 || i >= o->num_fields) {
		fprintf(stderr, "mvm: invalid field index!\n");
		mvm_halt();
	}

	tmp = o->fields[i];
	o->fields[i] = r;

	return tmp;
}

uint32_t object_get_num_fields(struct object *o) {
	if (o == NULL) {
		fprintf(stderr, "mvm: object has not been initialized!\n");
		mvm_halt();
	}

	return o->num_fields;
}

int object_get_predefined_type(struct object *o) {
	if (o == NULL) {
		fprintf(stderr, "mvm: object has not been initialized!\n");
		mvm_halt();
	}

	return o->predefined_type;
}

struct integer * object_get_integer(struct object *o) {
	if (o == NULL) {
		fprintf(stderr, "mvm: object has not been initialized!\n");
		mvm_halt();
	}

	if (o->predefined_type != integer_type) {
		fprintf(stderr, "mvm: object's predefined type is not integer!\n");
		mvm_halt();
	}

	return (o->predefined).integer;
}

struct string * object_get_string(struct object *o) {
	if (o == NULL) {
		fprintf(stderr, "mvm: object has not been initialized!\n");
		mvm_halt();
	}

	if (o->predefined_type != string_type) {
		fprintf(stderr, "mvm: object's predefined type is not string!\n");
		mvm_halt();
	}

	return (o->predefined).string;
}

struct table * object_get_table(struct object *o) {
	if (o == NULL) {
		fprintf(stderr, "mvm: object has not been initialized!\n");
		mvm_halt();
	}

	if (o->predefined_type != table_type) {
		fprintf(stderr, "mvm: object's predefined type is not table!\n");
		mvm_halt();
	}

	return (o->predefined).table;
}

struct thread * object_get_thread(struct object *o) {
	if (o == NULL) {
		fprintf(stderr, "mvm: object has not been initialized!\n");
		mvm_halt();
	}

	if (o->predefined_type != thread_type) {
		fprintf(stderr, "mvm: object's predefined type is not thread!\n");
		mvm_halt();
	}

	return (o->predefined).thread;
}

struct real * object_get_real(struct object *o) {
	if (o == NULL) {
		fprintf(stderr, "mvm: object has not been initialized!\n");
		mvm_halt();
	}

	if (o->predefined_type != real_type) {
		fprintf(stderr, "mvm: object's predefined type is not real!\n");
		mvm_halt();
	}

	return (o->predefined).real;
}

int object_set_integer(struct object *o, struct integer *i) {
	if (o == NULL) {
		fprintf(stderr, "mvm: object has not been initialized!\n");
		mvm_halt();
	}

	if (object_destroy_predefined_type(o) != 0)
		mvm_halt();

	o->predefined_type = integer_type;
	(o->predefined).integer = i;

	return 0;
}

int object_set_string(struct object *o, struct string *s) {
	if (o == NULL) {
		fprintf(stderr, "mvm: object has not been initialized!\n");
		mvm_halt();
	}

	if (object_destroy_predefined_type(o) != 0)
		mvm_halt();

	o->predefined_type = string_type;
	(o->predefined).string = s;

	return 0;
}

int object_set_table(struct object *o, struct table *t) {
	if (o == NULL) {
		fprintf(stderr, "mvm: object has not been initialized!\n");
		mvm_halt();
	}

	if (object_destroy_predefined_type(o) != 0)
		mvm_halt();

	o->predefined_type = table_type;
	(o->predefined).table = t;

	return 0;
}

int object_set_thread(struct object *o, struct thread *t) {
	if (o == NULL) {
		fprintf(stderr, "mvm: object has not been initialized!\n");
		mvm_halt();
	}

	if (object_destroy_predefined_type(o) != 0)
		mvm_halt();

	o->predefined_type = thread_type;
	(o->predefined).thread = t;

	return 0;
}

int object_set_real(struct object *o, struct real *r) {
	if (o == NULL) {
		fprintf(stderr, "mvm: object has not been initialized!\n");
		mvm_halt();
	}

	if (object_destroy_predefined_type(o) != 0)
		mvm_halt();

	o->predefined_type = real_type;
	(o->predefined).real = r;

	return 0;
}

int object_populate_ref_set(struct object *o, struct ref_set *r) {
	int i;

	if (o == NULL) {
		fprintf(stderr, "mvm: object has not been initialized!\n");
		mvm_halt();
	}

	for (i = 0; i < o->num_fields; i++) {
		if (o->fields[i] != 0)
			ref_set_add(r, o->fields[i]);
	}

	if (o->predefined_type == table_type)
		table_populate_ref_set((o->predefined).table, r);

	return 0;
}

int object_equals(struct object *o, struct object *p) {
	int32_t value;
	int ref;

	value = (o == p) ? 1 : 0;
	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int object_hash_code(struct object *o) {
	int ref;
	int32_t value;

	if (o == NULL) {
		fprintf(stderr, "mvm: object instance not initialized!\n");
		mvm_halt();
	}

	value = (long)o;
	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int object_to_string(struct object *o) {
	int ref;

	if (o == NULL) {
		fprintf(stderr, "mvm: object instance not initialized!\n");
		mvm_halt();
	}

	if ((ref = class_table_new_string(class_table, "Object", NULL)) == 0)
		mvm_halt();

	return ref;
}

int object_destroy_predefined_type(struct object *o) {
	if (o == NULL) {
		fprintf(stderr, "mvm: object instance not initialized!\n");
		mvm_halt();
	}

	switch(o->predefined_type) {
	case integer_type:
		if (o->predefined.integer != NULL) {
			integer_destroy(o->predefined.integer);
			o->predefined.integer = NULL;
		}
		break;
	case string_type:
		if (o->predefined.string != NULL) {
			string_destroy(o->predefined.string);
			o->predefined.string = NULL;
		}
		break;
	case table_type:
		if (o->predefined.table != NULL) {
			table_destroy(o->predefined.table);
			o->predefined.table = NULL;
		}
		break;
	case thread_type:
		if (o->predefined.thread != NULL) {
			thread_destroy(o->predefined.thread);
			o->predefined.thread = NULL;
		}
		break;
	case real_type:
		if (o->predefined.real != NULL) {
			real_destroy(o->predefined.real);
			o->predefined.real = NULL;
		}
		break;
	}

	return 0;
}

int object_get_owner(struct object *o) {
	if (o == NULL) {
		fprintf(stderr, "mvm: object instance not initialized!\n");
		mvm_halt();
	}

	return o->owner;
}

int object_set_owner(struct object *o, int r) {
	if (o == NULL) {
		fprintf(stderr, "mvm: object instance not initialized!\n");
		mvm_halt();
	}

	o->owner = r;
	return 0;
}


int object_dump(struct object *o, int f) {
	uint32_t i;
	char *class_name;
	struct class *class;

	if (o == NULL) {
		fprintf(stderr, "ref=0");
		return 0;
	}

	class = object_get_class(o);
	class_name = class_get_name(class);

	fprintf(stderr, "ref=%d class=%s", o->ref, class_name);

	if (f == 0)
		return 0;

	for (i = 0; i < o->num_fields; i++) {
		fprintf(stderr, "\n      fields[%" PRIu32 "] = ", i);
		object_dump(heap_fetch_object(heap, o->fields[i]), 0);
	}

	return 0;
}
