/* Niels Widger
 * Time-stamp: <25 Jun 2012 at 18:47:24 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#define _POSIX_C_SOURCE 200112L

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "class.h"
#include "class_table.h"
#include "constants.h"
#include "globals.h"
#include "heap.h"
#include "real.h"
#include "object.h"

/* struct definitions */
struct real {
	float value;
};

struct real * real_create(float v) {
	struct real *r;

	if ((r = (struct real *)heap_malloc(heap, sizeof(struct real))) == NULL)
		mvm_halt();

	r->value = v;

	return r;
}

void real_destroy(struct real *r) {
	if (r != NULL) {
		real_clear(r);
		heap_free(heap, r);
	}
}

void real_clear(struct real *r) {
	if (r != NULL) {
	}
}

float real_get_value(struct real *r) {
	if (r == NULL) {
		fprintf(stderr, "mvm: real not initialized!\n");
		mvm_halt();
	}

	return r->value;
}

int real_add(struct object *o, struct object *p) {
	int ref;
	float value;
	struct real *r, *j;

	r = object_get_real(o);
	j = object_get_real(p);

	if (r == NULL || j == NULL) {
		fprintf(stderr, "mvm: real not initialized!\n");
		mvm_halt();
	}

	value = r->value + j->value;
	if ((ref = class_table_new_real(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int real_subtract(struct object *o, struct object *p) {
	int ref;
	float value;
	struct real *r, *j;

	r = object_get_real(o);
	j = object_get_real(p);

	if (r == NULL || j == NULL) {
		fprintf(stderr, "mvm: real not initialized!\n");
		mvm_halt();
	}

	value = r->value - j->value;
	if ((ref = class_table_new_real(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int real_multiply(struct object *o, struct object *p) {
	int ref;
	float value;
	struct real *r, *j;

	r = object_get_real(o);
	j = object_get_real(p);

	if (r == NULL || j == NULL) {
		fprintf(stderr, "mvm: real not initialized!\n");
		mvm_halt();
	}

	value = r->value * j->value;
	if ((ref = class_table_new_real(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int real_divide(struct object *o, struct object *p) {
	int ref;
	float value;
	struct real *r, *j;

	r = object_get_real(o);
	j = object_get_real(p);

	if (r == NULL || j == NULL) {
		fprintf(stderr, "mvm: real not initialized!\n");
		mvm_halt();
	}

	if (j->value == 0.0) {
		fprintf(stderr, "mvm: real division by zero!\n");
		mvm_halt();
	}

	value = r->value / j->value;
	if ((ref = class_table_new_real(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int real_greater_than(struct object *o, struct object *p) {
	int ref;
	int32_t value;
	struct real *r, *j;
	
	r = object_get_real(o);
	j = object_get_real(p);

	if (r == NULL || j == NULL) {
		fprintf(stderr, "mvm: real not initialized!\n");
		mvm_halt();
	}

	value = (r->value > j->value) ? 1 : 0;
	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int real_less_than(struct object *o, struct object *p) {
	int ref;
	int32_t value;
	struct real *r, *j;

	r = object_get_real(o);
	j = object_get_real(p);

	if (r == NULL || j == NULL) {
		fprintf(stderr, "mvm: real not initialized!\n");
		mvm_halt();
	}

	value = (r->value < j->value) ? 1 : 0;
	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int real_greater_than_equal(struct object *o, struct object *p) {
	int ref;
	int32_t value;
	struct real *r, *j;
	
	r = object_get_real(o);
	j = object_get_real(p);

	if (r == NULL || j == NULL) {
		fprintf(stderr, "mvm: real not initialized!\n");
		mvm_halt();
	}

	value = (r->value >= j->value) ? 1 : 0;
	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int real_less_than_equal(struct object *o, struct object *p) {
	int ref;
	int32_t value;
	struct real *r, *j;

	r = object_get_real(o);
	j = object_get_real(p);

	if (r == NULL || j == NULL) {
		fprintf(stderr, "mvm: real not initialized!\n");
		mvm_halt();
	}

	value = (r->value <= j->value) ? 1 : 0;
	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int real_not(struct object *o) {
	int ref;
	int32_t value;
	struct real *r;

	r = object_get_real(o);

	if (r == NULL) {
		fprintf(stderr, "mvm: real not initialized!\n");
		mvm_halt();
	}

	value = (r->value == 0.0) ? 1 : 0;
	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int real_minus(struct object *o) {
	int ref;
	float value;
	struct real *r;

	r = object_get_real(o);

	if (r == NULL) {
		fprintf(stderr, "mvm: real not initialized!\n");
		mvm_halt();
	}

	value = -(r->value);
	if ((ref = class_table_new_real(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int real_equals(struct object *o, struct object *p) {
	int ref;
	int32_t value;
	struct class *real_class;
	struct real *r, *j;

	real_class = class_table_find_predefined(class_table, real_type);

	if (class_can_cast_to(object_get_class(p), real_class) == 0) {
		value = 0;
	} else {
		r = object_get_real(o);
		j = object_get_real(p);

		if (r == NULL || j == NULL) {
			fprintf(stderr, "mvm: real not initialized!\n");
			mvm_halt();
		}
	
		value = (r->value == j->value) ? 1 : 0;
	}
	
	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)	
		mvm_halt();

	return ref;
}

int real_hash_code(struct object *o) {
	int ref;
	int32_t value;
	struct real *r;

	r = object_get_real(o);

	if (r == NULL) {
		fprintf(stderr, "mvm: real not initialized!\n");
		mvm_halt();
	}

	value = (int)r->value;
	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)	
		mvm_halt();

	return ref;
}

int real_to_string(struct object *o) {
	int ref;
	char buf[REAL_TO_STRING_BUFFER_SIZE];
	struct real *r;

	r = object_get_real(o);
	
	if (r == NULL) {
		fprintf(stderr, "mvm: real not initialized!\n");
		mvm_halt();
	}

	snprintf(buf, REAL_TO_STRING_BUFFER_SIZE, "%f", r->value);
	if ((ref = class_table_new_string(class_table, buf, NULL)) == 0)		
		mvm_halt();

	return ref;
}

int real_square_root(struct object *o) {
	int ref;
	float value;
	struct real *r;

	r = object_get_real(o);
	
	if (r == NULL) {
		fprintf(stderr, "mvm: real not initialized!\n");
		mvm_halt();
	}

	value = sqrtf(r->value);

	if ((ref = class_table_new_real(class_table, value, NULL)) == 0)		
		mvm_halt();

	return ref;
}
