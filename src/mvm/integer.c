/* Niels Widger
 * Time-stamp: <10 May 2011 at 20:51:58 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "class.h"
#include "class_table.h"
#include "constants.h"
#include "globals.h"
#include "heap.h"
#include "integer.h"
#include "object.h"

/* struct definitions */
struct integer {
	int32_t value;
};

struct integer * integer_create(int32_t v) {
	struct integer *i;

	if ((i = (struct integer *)heap_malloc(heap, sizeof(struct integer))) == NULL)
		mvm_halt();

	i->value = v;

	return i;
}

void integer_destroy(struct integer *i) {
	if (i != NULL) {
		integer_clear(i);
		heap_free(heap, i);
	}
}

void integer_clear(struct integer *i) {
	if (i != NULL) {
	}
}

int32_t integer_get_value(struct integer *i) {
	if (i == NULL) {
		fprintf(stderr, "mvm: integer not initialized!\n");
		mvm_halt();
	}

	return i->value;
}

int integer_add(struct object *o, struct object *p) {
	int ref;
	int32_t value;
	struct integer *i, *j;

	i = object_get_integer(o);
	j = object_get_integer(p);

	if (i == NULL || j == NULL) {
		fprintf(stderr, "mvm: integer not initialized!\n");
		mvm_halt();
	}

	value = i->value + j->value;
	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int integer_subtract(struct object *o, struct object *p) {
	int ref;
	int32_t value;
	struct integer *i, *j;

	i = object_get_integer(o);
	j = object_get_integer(p);

	if (i == NULL || j == NULL) {
		fprintf(stderr, "mvm: integer not initialized!\n");
		mvm_halt();
	}

	value = i->value - j->value;
	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int integer_multiply(struct object *o, struct object *p) {
	int ref;
	int32_t value;
	struct integer *i, *j;

	i = object_get_integer(o);
	j = object_get_integer(p);

	if (i == NULL || j == NULL) {
		fprintf(stderr, "mvm: integer not initialized!\n");
		mvm_halt();
	}

	value = i->value * j->value;
	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int integer_divide(struct object *o, struct object *p) {
	int ref;
	int32_t value;
	struct integer *i, *j;

	i = object_get_integer(o);
	j = object_get_integer(p);

	if (i == NULL || j == NULL) {
		fprintf(stderr, "mvm: integer not initialized!\n");
		mvm_halt();
	}

	if (j->value == 0) {
		fprintf(stderr, "mvm: integer division by zero!\n");
		mvm_halt();
	}

	value = i->value / j->value;
	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int integer_greater_than(struct object *o, struct object *p) {
	int ref;
	int32_t value;
	struct integer *i, *j;
	
	i = object_get_integer(o);
	j = object_get_integer(p);

	if (i == NULL || j == NULL) {
		fprintf(stderr, "mvm: integer not initialized!\n");
		mvm_halt();
	}

	value = (i->value > j->value) ? 1 : 0;
	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int integer_less_than(struct object *o, struct object *p) {
	int ref;
	int32_t value;
	struct integer *i, *j;

	i = object_get_integer(o);
	j = object_get_integer(p);

	if (i == NULL || j == NULL) {
		fprintf(stderr, "mvm: integer not initialized!\n");
		mvm_halt();
	}

	value = (i->value < j->value) ? 1 : 0;
	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int integer_greater_than_equal(struct object *o, struct object *p) {
	int ref;
	int32_t value;
	struct integer *i, *j;
	
	i = object_get_integer(o);
	j = object_get_integer(p);

	if (i == NULL || j == NULL) {
		fprintf(stderr, "mvm: integer not initialized!\n");
		mvm_halt();
	}

	value = (i->value >= j->value) ? 1 : 0;
	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int integer_less_than_equal(struct object *o, struct object *p) {
	int ref;
	int32_t value;
	struct integer *i, *j;

	i = object_get_integer(o);
	j = object_get_integer(p);

	if (i == NULL || j == NULL) {
		fprintf(stderr, "mvm: integer not initialized!\n");
		mvm_halt();
	}

	value = (i->value <= j->value) ? 1 : 0;
	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int integer_not(struct object *o) {
	int ref;
	int32_t value;
	struct integer *i;

	i = object_get_integer(o);

	if (i == NULL) {
		fprintf(stderr, "mvm: integer not initialized!\n");
		mvm_halt();
	}

	value = (i->value == 0) ? 1 : 0;
	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int integer_minus(struct object *o) {
	int ref;
	int32_t value;
	struct integer *i;

	i = object_get_integer(o);

	if (i == NULL) {
		fprintf(stderr, "mvm: integer not initialized!\n");
		mvm_halt();
	}

	value = -(i->value);
	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int integer_equals(struct object *o, struct object *p) {
	int ref;
	int32_t value;
	struct class *integer_class;
	struct integer *i, *j;

	integer_class = class_table_find_predefined(class_table, integer_type);

	if (class_can_cast_to(object_get_class(p), integer_class) == 0) {
		value = 0;
	} else {
		i = object_get_integer(o);
		j = object_get_integer(p);

		if (i == NULL || j == NULL) {
			fprintf(stderr, "mvm: integer not initialized!\n");
			mvm_halt();
		}
	
		value = (i->value == j->value) ? 1 : 0;
	}
	
	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)	
		mvm_halt();

	return ref;
}

int integer_hash_code(struct object *o) {
	int ref;
	int32_t value;
	struct integer *i;

	i = object_get_integer(o);

	if (i == NULL) {
		fprintf(stderr, "mvm: integer not initialized!\n");
		mvm_halt();
	}

	value = i->value;
	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)	
		mvm_halt();

	return ref;
}

int integer_to_string(struct object *o) {
	int ref;
	char buf[INTEGER_TO_STRING_BUFFER_SIZE];
	struct integer *i;

	i = object_get_integer(o);
	
	if (i == NULL) {
		fprintf(stderr, "mvm: integer not initialized!\n");
		mvm_halt();
	}

	snprintf(buf, INTEGER_TO_STRING_BUFFER_SIZE, "%" PRId32, i->value);
	if ((ref = class_table_new_string(class_table, buf, NULL)) == 0)		
		mvm_halt();

	return ref;
}
