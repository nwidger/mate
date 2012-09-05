/* Niels Widger
 * Time-stamp: <03 Sep 2012 at 21:49:41 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "class.h"
#include "class_table.h"
#include "constants.h"
#include "frame.h"
#include "garbage_collector.h"
#include "globals.h"
#include "heap.h"
#include "integer.h"
#include "local_variable_array.h"
#include "native_method_array.h"
#include "native_methods.h"
#include "object.h"
#include "operand_stack.h"
#include "parse.h"
#include "real.h"
#include "string.h"
#include "table.h"
#include "thread.h"
#include "vm_stack.h"

#ifdef DMP
#include "dmp.h"
#include "object_dmp.h"
#endif

/* extern declarations */
extern int line;
extern FILE *yyin;
extern int yylex();
extern char *yytext;
extern int yyparse();
#ifdef YYDEBUG
extern int yydebug;
#endif

/* globals */
int max_native_index = 0;
struct native_class *native_classes;

#define SETUP_NATIVE_METHOD()						\
	struct vm_stack *vm_stack;					\
	struct frame *frame;						\
	struct operand_stack *operand_stack;				\
	struct local_variable_array *local_variable_array;		\
	struct frame *calling_frame;					\
	struct operand_stack *calling_frame_operand_stack;		\
									\
        vm_stack = thread_get_vm_stack(NULL);				\
									\
	frame = vm_stack_peek(vm_stack);				\
	if (frame == NULL) {						\
		operand_stack = NULL;					\
		local_variable_array = NULL;				\
		calling_frame = NULL;					\
		calling_frame_operand_stack = NULL;			\
	} else {							\
		operand_stack = frame_get_operand_stack(frame);		\
		local_variable_array =					\
			frame_get_local_variable_array(frame);		\
		calling_frame = frame_get_calling_frame(frame);		\
		if (calling_frame == NULL) {				\
			calling_frame_operand_stack = NULL;		\
		} else {						\
			calling_frame_operand_stack =			\
				frame_get_operand_stack(calling_frame);	\
		}							\
	}

/* forward declarations */
uint32_t mvm_disassemble_argument(int n);
uint32_t increment_pc(int n);

int native_object_constructor(uint32_t i) {
	SETUP_NATIVE_METHOD();

	return 0;
}

int native_object_equals(uint32_t i) {
	int ref, n;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);
	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = object_equals(this, object);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_object_hash_code(uint32_t i) {
	int ref, n;
	struct object *this;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = object_hash_code(this);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_object_to_string(uint32_t i) {
	int ref, n;
	struct object *this;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = object_to_string(this);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_object_notify(uint32_t i) {
	int ref, n;
	struct object *this;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = object_notify(this);
	operand_stack_push(calling_frame_operand_stack, ref);

	return 0;
}

int native_object_notify_all(uint32_t i) {
	int ref, n;
	struct object *this;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = object_notify_all(this);
	operand_stack_push(calling_frame_operand_stack, ref);

	return 0;
}

int native_object_wait(uint32_t i) {
	int ref, n;
	struct object *this;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = object_wait(this);
	operand_stack_push(calling_frame_operand_stack, ref);

	return 0;
}

int native_object_wait_integer(uint32_t i) {
	int ref, n;
	struct object *this, *timeout;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	timeout = heap_fetch_object(heap, ref);

	ref = object_timedwait(this, timeout);
	operand_stack_push(calling_frame_operand_stack, ref);

	return 0;
}

int native_integer_constructor(uint32_t i) {
	int ref, n;
	struct integer *integer;
	struct object *this;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	integer = integer_create(0);
	object_set_integer(this, integer);

	return 0;
}

int native_integer_constructor_integer(uint32_t i) {
	int ref, n;
	int32_t value;
	struct integer *integer;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);
	integer = object_get_integer(object);
	value = integer_get_value(integer);

	integer = integer_create(value);
	object_set_integer(this, integer);

	return 0;
}

int native_integer_constructor_real(uint32_t i) {
	int ref, n;
	float value;
	struct real *real;
	struct integer *integer;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);
	real = object_get_real(object);
	value = real_get_value(real);

	integer = integer_create((int32_t)value);
	object_set_integer(this, integer);

	return 0;
}

int native_integer_add(uint32_t i) {
	int ref, n;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = integer_add(this, object);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_integer_subtract(uint32_t i) {
	int ref, n;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = integer_subtract(this, object);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_integer_multiply(uint32_t i) {
	int ref, n;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = integer_multiply(this, object);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_integer_divide(uint32_t i) {
	int ref, n;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = integer_divide(this, object);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_integer_greater_than(uint32_t i) {
	int ref, n;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = integer_greater_than(this, object);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_integer_less_than(uint32_t i) {
	int ref, n;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = integer_less_than(this, object);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_integer_greater_than_equal(uint32_t i) {
	int ref, n;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = integer_greater_than_equal(this, object);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_integer_less_than_equal(uint32_t i) {
	int ref, n;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = integer_less_than_equal(this, object);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_integer_not(uint32_t i) {
	int ref, n;
	struct object *this;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = integer_not(this);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_integer_minus(uint32_t i) {
	int ref, n;
	struct object *this;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = integer_minus(this);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_integer_equals(uint32_t i) {
	int ref, n;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = integer_equals(this, object);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_integer_hash_code(uint32_t i) {
	int ref, n;
	struct object *this;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = integer_hash_code(this);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_integer_to_string(uint32_t i) {
	int ref, n;
	struct object *this;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = integer_to_string(this);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_integer_mod(uint32_t i) {
	int ref, n;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = integer_mod(this, object);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_string_constructor_string(uint32_t i) {
	int ref, n;
	char *chars;
	struct string *string;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);
	string = object_get_string(object);
	chars = string_get_chars(string);

	string = string_create(chars);
	object_set_string(this, string);

	return 0;
}

int native_string_length(uint32_t i) {
	int ref, n;
	struct object *this;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = string_length(this);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_string_substr(uint32_t i) {
	int ref, n;
	struct object *this, *beg, *end;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	beg = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	end = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = string_substr(this, beg, end);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_string_to_integer(uint32_t i) {
	int ref, n;
	struct object *this;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = string_to_integer(this);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_string_to_real(uint32_t i) {
	int ref, n;
	struct object *this;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = string_to_real(this);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_string_concat(uint32_t i) {
	int ref, n;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = string_concat(this, object);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_string_greater_than(uint32_t i) {
	int ref, n;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = string_greater_than(this, object);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_string_less_than(uint32_t i) {
	int ref, n;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = string_less_than(this, object);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_string_hash_code(uint32_t i) {
	int ref, n;
	struct object *this;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = string_hash_code(this);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_string_equals(uint32_t i) {
	int ref, n;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = string_equals(this, object);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_string_to_string(uint32_t i) {
	int ref, n;
	struct object *this;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = string_to_string(this);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_table_constructor(uint32_t i) {
	int ref, n;
	struct object *this;
	struct table *table;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	table = table_create(TABLE_DEFAULT_INITIAL_CAPACITY, this);
	object_set_table(this, table);

	return 0;
}

int native_table_constructor_integer(uint32_t i) {
	int ref, n;
	int32_t value;
	struct object *this, *object;
	struct integer *integer;
	struct table *table;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);
	integer = object_get_integer(object);
	value = integer_get_value(integer);

	table = table_create(value, this);
	object_set_table(this, table);

	return 0;
}

int native_table_get(uint32_t i) {
	int ref, n;
	struct table *table;
	struct object *this, *key;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);
	table = object_get_table(this);

	ref = local_variable_array_load(local_variable_array, n++);
	key = heap_fetch_object(heap, ref);

#ifdef DMP
	if (dmp != NULL) {
		struct object_dmp *od;
			
		od = object_get_dmp(this);
		object_dmp_load(od, 0);
	}
#endif

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = table_get(table, key);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_table_put(uint32_t i) {
	int ref, n;
	struct table *table;
	struct object *this, *key, *value;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);
	table = object_get_table(this);

	ref = local_variable_array_load(local_variable_array, n++);
	key = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	value = heap_fetch_object(heap, ref);

#ifdef DMP	
	if (dmp != NULL) {
		struct object_dmp *od;

		od = object_get_dmp(this);
		object_dmp_store(od, 0, 0);
	}
#endif
	
	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = table_put(table, key, value);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_table_remove(uint32_t i) {
	int ref, n;
	struct table *table;
	struct object *this, *key;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);
	table = object_get_table(this);

	ref = local_variable_array_load(local_variable_array, n++);
	key = heap_fetch_object(heap, ref);

#ifdef DMP	
	if (dmp != NULL) {
		struct object_dmp *od;

		od = object_get_dmp(this);
		object_dmp_store(od, 0, 0);
	}
#endif
	
	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = table_remove(table, key);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_table_first_key(uint32_t i) {
	int ref, n;
	struct object *this;
	struct table *table;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);
	table = object_get_table(this);

#ifdef DMP	
	if (dmp != NULL) {
		struct object_dmp *od;
		
		od = object_get_dmp(this);
		object_dmp_store(od, 0, 0);
	}
#endif

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = table_first_key(table);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_table_next_key(uint32_t i) {
	int ref, n;
	struct object *this;
	struct table *table;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);
	table = object_get_table(this);

#ifdef DMP	
	if (dmp != NULL) {
		struct object_dmp *od;

		od = object_get_dmp(this);
		object_dmp_store(od, 0, 0);
	}
#endif
	
	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = table_next_key(table);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_thread_constructor(uint32_t i) {
	int ref, n;
	struct object *this;
	struct thread *thread;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	thread = thread_create();
	object_set_thread(this, thread);
	heap_add_thread_ref(heap, ref);	

	return 0;
}

int native_thread_start(uint32_t i) {
	int ref, n;
	struct object *this;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = thread_start(this);
	operand_stack_push(calling_frame_operand_stack, ref);

	return 0;
}

int native_thread_run(uint32_t i) {
	int ref, n;
	struct object *this;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = thread_run(this);
	operand_stack_push(calling_frame_operand_stack, ref);

	return 0;
}

int native_thread_join(uint32_t i) {
	int ref, n;
	struct object *this;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = thread_join(this);
	operand_stack_push(calling_frame_operand_stack, ref);

	return 0;
}

int native_thread_sleep(uint32_t i) {
	int ref, n;
	struct object *this, *millisec;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	millisec = heap_fetch_object(heap, ref);

	ref = thread_sleep(this, millisec);
	operand_stack_push(calling_frame_operand_stack, ref);

	return 0;
}

int native_real_constructor(uint32_t i) {
	int ref, n;
	struct real *real;
	struct object *this;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	real = real_create(0.0);
	object_set_real(this, real);

	return 0;
}

int native_real_constructor_real(uint32_t i) {
	int ref, n;
	float value;
	struct real *real;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);
	real = object_get_real(object);
	value = real_get_value(real);

	real = real_create(value);
	object_set_real(this, real);

	return 0;
}

int native_real_constructor_integer(uint32_t i) {
	int ref, n;
	int32_t value;
	struct real *real;
	struct integer *integer;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);
	integer = object_get_integer(object);
	value = integer_get_value(integer);

	real = real_create((float)value);
	object_set_real(this, real);

	return 0;
}

int native_real_add(uint32_t i) {
	int ref, n;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = real_add(this, object);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_real_subtract(uint32_t i) {
	int ref, n;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = real_subtract(this, object);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_real_multiply(uint32_t i) {
	int ref, n;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = real_multiply(this, object);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_real_divide(uint32_t i) {
	int ref, n;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = real_divide(this, object);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_real_greater_than(uint32_t i) {
	int ref, n;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = real_greater_than(this, object);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_real_less_than(uint32_t i) {
	int ref, n;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = real_less_than(this, object);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_real_greater_than_equal(uint32_t i) {
	int ref, n;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = real_greater_than_equal(this, object);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_real_less_than_equal(uint32_t i) {
	int ref, n;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = real_less_than_equal(this, object);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_real_not(uint32_t i) {
	int ref, n;
	struct object *this;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = real_not(this);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_real_minus(uint32_t i) {
	int ref, n;
	struct object *this;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = real_minus(this);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_real_equals(uint32_t i) {
	int ref, n;
	struct object *this, *object;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	object = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = real_equals(this, object);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_real_hash_code(uint32_t i) {
	int ref, n;
	struct object *this;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = real_hash_code(this);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_real_to_string(uint32_t i) {
	int ref, n;
	struct object *this;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = real_to_string(this);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

int native_real_square_root(uint32_t i) {
	int ref, n;
	struct object *this;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);

	/* lock */
	garbage_collector_lock(garbage_collector);

	ref = real_square_root(this);
	operand_stack_push(calling_frame_operand_stack, ref);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	return 0;
}

/* struct definitions */
struct native_class {
        char *name;
	struct native_method *methods;
	struct native_class *next;
};

struct native_method {
        unsigned long index;
	char *name;
	struct native_method *next;
};

struct native_class * native_class_create(char *n, struct native_method *m, struct native_class *x) {
        struct native_class *c;

	if ((c = (struct native_class *)malloc(sizeof(struct native_class))) == NULL) {
	        perror("mvm: malloc");
		return NULL;
	}

	c->name = n;
	c->methods = m;
	c->next = x;

	return c;
}

int native_class_set_next(struct native_class *c, struct native_class *x) {
	if (c == NULL)
		return 1;
	
	c->next = x;
	return 0;
}

struct native_method * native_method_create(unsigned long i, char *n, struct native_method *x) {
        struct native_method *m;

	if ((m = (struct native_method *)malloc(sizeof(struct native_method))) == NULL) {
	        perror("mvm: malloc");
		return NULL;
	}

	m->index = i;
	m->name = n;
	m->next = x;

	return m;
}

int native_method_set_next(struct native_method *c, struct native_method *x) {
	if (c == NULL)
		return 1;
	
	c->next = x;
	return 0;
}

/* return largest native index found in predefined classes file,
 * or -1 on error */
int native_methods_parse_predefined_classes_file() {
	FILE *f;
	char *predefined_file;

	predefined_file = DATA_PATH "/predefined_classes.m";

	if ((f = fopen(predefined_file, "r")) == NULL) {
		fprintf(stderr, "mvm: %s: %s\n", predefined_file, strerror(errno));
		return -1;
	}

	yyin = f;
	
	if (yyparse() != 0)
		return -1;

	fclose(f);

	return max_native_index;
}

int native_methods_load_predefined_classes_file(struct native_method_array *n) {
	int len;
	char *name;
	struct native_class *c, *pc;
	struct native_method *m, *pm;

	c = native_classes;
	
	while (c != NULL) {
		
		m = c->methods;
		
		while (m != NULL) {
			if (strncmp(m->name+strlen(c->name), "_constructor",
				    strlen("_constructor")) != 0) {
				len = strlen(c->name)+strlen("$")+strlen(m->name);

				name = malloc(sizeof(char)*(len+2));
				name[0] = '\0';

				strcat(name, c->name);
				strcat(name, "$");
				strcat(name, m->name);

				free(m->name);
				m->name = name;
			}
			
			native_method_array_set(n, m->index, m->name);

			pm = m;
			m = m->next;
			free(pm->name);
			free(pm);
		}

		pc = c;
		c = c->next;
		free(pc->name);
		free(pc);
	}

	return 0;
}

int add_native_methods(struct native_method_array *n) {
	native_method_array_set_method(n, "Object_constructor", native_object_constructor);
	native_method_array_set_method(n, "Object$equals$Object", native_object_equals);
	native_method_array_set_method(n, "Object$hashCode", native_object_hash_code);
	native_method_array_set_method(n, "Object$toString", native_object_to_string);
	native_method_array_set_method(n, "Object$notify", native_object_notify);
	native_method_array_set_method(n, "Object$notifyAll", native_object_notify_all);
	native_method_array_set_method(n, "Object$wait", native_object_wait);
	native_method_array_set_method(n, "Object$wait$Integer", native_object_wait_integer);

	native_method_array_set_method(n, "Integer_constructor", native_integer_constructor);
	native_method_array_set_method(n, "Integer_constructor$Integer", native_integer_constructor_integer);
	native_method_array_set_method(n, "Integer_constructor$Real", native_integer_constructor_real);
	native_method_array_set_method(n, "Integer$add$Integer", native_integer_add);
	native_method_array_set_method(n, "Integer$subtract$Integer", native_integer_subtract);
	native_method_array_set_method(n, "Integer$multiply$Integer", native_integer_multiply);
	native_method_array_set_method(n, "Integer$divide$Integer", native_integer_divide);
	native_method_array_set_method(n, "Integer$greaterThan$Integer", native_integer_greater_than);
	native_method_array_set_method(n, "Integer$lessThan$Integer", native_integer_less_than);
	native_method_array_set_method(n, "Integer$greaterThanEqual$Integer", native_integer_greater_than_equal);
	native_method_array_set_method(n, "Integer$lessThanEqual$Integer", native_integer_less_than_equal);
	native_method_array_set_method(n, "Integer$not", native_integer_not);
	native_method_array_set_method(n, "Integer$minus", native_integer_minus);
	native_method_array_set_method(n, "Integer$operator+$Integer", native_integer_add);
	native_method_array_set_method(n, "Integer$operator-$Integer", native_integer_subtract);
	native_method_array_set_method(n, "Integer$operator*$Integer", native_integer_multiply);
	native_method_array_set_method(n, "Integer$operator/$Integer", native_integer_divide);
	native_method_array_set_method(n, "Integer$operator>$Integer", native_integer_greater_than);
	native_method_array_set_method(n, "Integer$operator<$Integer", native_integer_less_than);
	native_method_array_set_method(n, "Integer$operator>=$Integer", native_integer_greater_than_equal);
	native_method_array_set_method(n, "Integer$operator<=$Integer", native_integer_less_than_equal);
	native_method_array_set_method(n, "Integer$operator!", native_integer_not);
	native_method_array_set_method(n, "Integer$operator-", native_integer_minus);
	native_method_array_set_method(n, "Integer$equals$Object", native_integer_equals);
	native_method_array_set_method(n, "Integer$hashCode", native_integer_hash_code);
	native_method_array_set_method(n, "Integer$toString", native_integer_to_string);

	native_method_array_set_method(n, "String_constructor$String", native_string_constructor_string);
	native_method_array_set_method(n, "String$length", native_string_length);
	native_method_array_set_method(n, "String$substr$Integer$Integer", native_string_substr);
	native_method_array_set_method(n, "String$toInteger", native_string_to_integer);
	native_method_array_set_method(n, "String$toReal", native_string_to_real);
	native_method_array_set_method(n, "String$concat$String", native_string_concat);
	native_method_array_set_method(n, "String$operator+$String", native_string_concat);
	native_method_array_set_method(n, "String$operator>$String", native_string_greater_than);
	native_method_array_set_method(n, "String$operator<$String", native_string_less_than);
	native_method_array_set_method(n, "String$hashCode", native_string_hash_code);
	native_method_array_set_method(n, "String$equals$Object", native_string_equals);
	native_method_array_set_method(n, "String$toString", native_string_to_string);

	native_method_array_set_method(n, "Table_constructor", native_table_constructor);
	native_method_array_set_method(n, "Table_constructor$Integer", native_table_constructor_integer);
	native_method_array_set_method(n, "Table$get$Object", native_table_get);
	native_method_array_set_method(n, "Table$put$Object$Object", native_table_put);
	native_method_array_set_method(n, "Table$remove$Object", native_table_remove);
	native_method_array_set_method(n, "Table$firstKey", native_table_first_key);
	native_method_array_set_method(n, "Table$nextKey", native_table_next_key);

	native_method_array_set_method(n, "Thread_constructor", native_thread_constructor);
	native_method_array_set_method(n, "Thread$start", native_thread_start);
	native_method_array_set_method(n, "Thread$run", native_thread_run);
	native_method_array_set_method(n, "Thread$join", native_thread_join);
	native_method_array_set_method(n, "Thread$sleep$Integer", native_thread_sleep);

	native_method_array_set_method(n, "Real_constructor", native_real_constructor);
	native_method_array_set_method(n, "Real_constructor$Real", native_real_constructor_real);
	native_method_array_set_method(n, "Real_constructor$Integer", native_real_constructor_integer);
	native_method_array_set_method(n, "Real$add$Real", native_real_add);
	native_method_array_set_method(n, "Real$subtract$Real", native_real_subtract);
	native_method_array_set_method(n, "Real$multiply$Real", native_real_multiply);
	native_method_array_set_method(n, "Real$divide$Real", native_real_divide);
	native_method_array_set_method(n, "Real$greaterThan$Real", native_real_greater_than);
	native_method_array_set_method(n, "Real$lessThan$Real", native_real_less_than);
	native_method_array_set_method(n, "Real$greaterThanEqual$Real", native_real_greater_than_equal);
	native_method_array_set_method(n, "Real$lessThanEqual$Real", native_real_less_than_equal);
	native_method_array_set_method(n, "Real$not", native_real_not);
	native_method_array_set_method(n, "Real$minus", native_real_minus);
	native_method_array_set_method(n, "Real$operator+$Real", native_real_add);
	native_method_array_set_method(n, "Real$operator-$Real", native_real_subtract);
	native_method_array_set_method(n, "Real$operator*$Real", native_real_multiply);
	native_method_array_set_method(n, "Real$operator/$Real", native_real_divide);
	native_method_array_set_method(n, "Real$operator>$Real", native_real_greater_than);
	native_method_array_set_method(n, "Real$operator<$Real", native_real_less_than);
	native_method_array_set_method(n, "Real$operator>=$Real", native_real_greater_than_equal);
	native_method_array_set_method(n, "Real$operator<=$Real", native_real_less_than_equal);	
	native_method_array_set_method(n, "Real$operator!", native_real_not);
	native_method_array_set_method(n, "Real$operator-", native_real_minus);
	native_method_array_set_method(n, "Real$equals$Object", native_real_equals);
	native_method_array_set_method(n, "Real$hashCode", native_real_hash_code);
	native_method_array_set_method(n, "Real$toString", native_real_to_string);
	native_method_array_set_method(n, "Real$squareRoot", native_real_square_root);
	
	return 0;
}
