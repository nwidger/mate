/* Niels Widger
 * Time-stamp: <03 Dec 2010 at 21:05:29 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>

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
#include "string.h"
#include "table.h"
#include "thread.h"
#include "vm_stack.h"

#define SETUP_NATIVE_METHOD()						\
	struct vm_stack *vm_stack;					\
	struct frame *frame;						\
	struct operand_stack *operand_stack;				\
	struct local_variable_array *local_variable_array;		\
	struct frame *calling_frame;					\
	struct operand_stack *calling_frame_operand_stack;		\
									\
        vm_stack = thread_get_vm_stack();				\
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

/* globals */
struct frame *frame;
struct operand_stack *operand_stack;
struct local_variable_array *local_variable_array;
struct frame *calling_frame;
struct operand_stack *calling_frame_operand_stack;

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

	table = table_create(TABLE_DEFAULT_INITIAL_CAPACITY);
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

	table = table_create(value);
	object_set_table(this, table);

	return 0;
}

int native_table_get(uint32_t i) {
	int ref, n;
	struct object *this, *key;
	struct table *table;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);
	table = object_get_table(this);

	ref = local_variable_array_load(local_variable_array, n++);
	key = heap_fetch_object(heap, ref);

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
	struct object *this, *key, *value;
	struct table *table;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);
	table = object_get_table(this);

	ref = local_variable_array_load(local_variable_array, n++);
	key = heap_fetch_object(heap, ref);

	ref = local_variable_array_load(local_variable_array, n++);
	value = heap_fetch_object(heap, ref);

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
	struct object *this, *key;
	struct table *table;

	SETUP_NATIVE_METHOD();
	n = 0;

	ref = local_variable_array_load(local_variable_array, n++);
	this = heap_fetch_object(heap, ref);
	table = object_get_table(this);

	ref = local_variable_array_load(local_variable_array, n++);
	key = heap_fetch_object(heap, ref);

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

int add_native_methods(struct native_method_array *n) {
	native_method_array_set(n, OBJECT_CONSTRUCTOR_NATIVE_INDEX,
				OBJECT_CONSTRUCTOR_NATIVE_NAME,
				native_object_constructor);
	native_method_array_set(n, OBJECT_EQUALS_NATIVE_INDEX,
				OBJECT_EQUALS_NATIVE_NAME,
				native_object_equals);
	native_method_array_set(n, OBJECT_HASH_CODE_NATIVE_INDEX,
				OBJECT_HASH_CODE_NATIVE_NAME,
				native_object_hash_code);
	native_method_array_set(n, OBJECT_TO_STRING_NATIVE_INDEX,
				OBJECT_TO_STRING_NATIVE_NAME,
				native_object_to_string);
	native_method_array_set(n, OBJECT_NOTIFY_NATIVE_INDEX,
				OBJECT_NOTIFY_NATIVE_NAME,
				native_object_notify);
	native_method_array_set(n, OBJECT_NOTIFY_ALL_NATIVE_INDEX,
				OBJECT_NOTIFY_ALL_NATIVE_NAME,
				native_object_notify_all);
	native_method_array_set(n, OBJECT_WAIT_NATIVE_INDEX,
				OBJECT_WAIT_NATIVE_NAME,
				native_object_wait);
	native_method_array_set(n, OBJECT_WAIT_INTEGER_NATIVE_INDEX,
				OBJECT_WAIT_INTEGER_NATIVE_NAME,
				native_object_wait_integer);

	native_method_array_set(n, INTEGER_CONSTRUCTOR_NATIVE_INDEX,
				INTEGER_CONSTRUCTOR_NATIVE_NAME,
				native_integer_constructor);
	native_method_array_set(n, INTEGER_CONSTRUCTOR_INTEGER_NATIVE_INDEX,
				INTEGER_CONSTRUCTOR_INTEGER_NATIVE_NAME,
				native_integer_constructor_integer);
	native_method_array_set(n, INTEGER_ADD_NATIVE_INDEX,
				INTEGER_ADD_NATIVE_NAME,
				native_integer_add);
	native_method_array_set(n, INTEGER_SUBTRACT_NATIVE_INDEX,
				INTEGER_SUBTRACT_NATIVE_NAME,
				native_integer_subtract);
	native_method_array_set(n, INTEGER_MULTIPLY_NATIVE_INDEX,
				INTEGER_MULTIPLY_NATIVE_NAME,
				native_integer_multiply);
	native_method_array_set(n, INTEGER_DIVIDE_NATIVE_INDEX,
				INTEGER_DIVIDE_NATIVE_NAME,
				native_integer_divide);
	native_method_array_set(n, INTEGER_GREATER_THAN_NATIVE_INDEX,
				INTEGER_GREATER_THAN_NATIVE_NAME,
				native_integer_greater_than);
	native_method_array_set(n, INTEGER_LESS_THAN_NATIVE_INDEX,
				INTEGER_LESS_THAN_NATIVE_NAME,
				native_integer_less_than);
	native_method_array_set(n, INTEGER_NOT_NATIVE_INDEX,
				INTEGER_NOT_NATIVE_NAME,
				native_integer_not);
	native_method_array_set(n, INTEGER_MINUS_NATIVE_INDEX,
				INTEGER_MINUS_NATIVE_NAME,
				native_integer_minus);
	native_method_array_set(n, INTEGER_ADD_OP_NATIVE_INDEX,
				INTEGER_ADD_OP_NATIVE_NAME,
				native_integer_add);
	native_method_array_set(n, INTEGER_SUBTRACT_OP_NATIVE_INDEX,
				INTEGER_SUBTRACT_OP_NATIVE_NAME,
				native_integer_subtract);
	native_method_array_set(n, INTEGER_MULTIPLY_OP_NATIVE_INDEX,
				INTEGER_MULTIPLY_OP_NATIVE_NAME,
				native_integer_multiply);
	native_method_array_set(n, INTEGER_DIVIDE_OP_NATIVE_INDEX,
				INTEGER_DIVIDE_OP_NATIVE_NAME,
				native_integer_divide);
	native_method_array_set(n, INTEGER_GREATER_THAN_OP_NATIVE_INDEX,
				INTEGER_GREATER_THAN_OP_NATIVE_NAME,
				native_integer_greater_than);
	native_method_array_set(n, INTEGER_LESS_THAN_OP_NATIVE_INDEX,
				INTEGER_LESS_THAN_OP_NATIVE_NAME,
				native_integer_less_than);
	native_method_array_set(n, INTEGER_NOT_OP_NATIVE_INDEX,
				INTEGER_NOT_OP_NATIVE_NAME,
				native_integer_not);
	native_method_array_set(n, INTEGER_MINUS_OP_NATIVE_INDEX,
				INTEGER_MINUS_OP_NATIVE_NAME,
				native_integer_minus);
	native_method_array_set(n, INTEGER_EQUALS_NATIVE_INDEX,
				INTEGER_EQUALS_NATIVE_NAME,
				native_integer_equals);
	native_method_array_set(n, INTEGER_HASH_CODE_NATIVE_INDEX,
				INTEGER_HASH_CODE_NATIVE_NAME,
				native_integer_hash_code);
	native_method_array_set(n, INTEGER_TO_STRING_NATIVE_INDEX,
				INTEGER_TO_STRING_NATIVE_NAME,
				native_integer_to_string);

	native_method_array_set(n, STRING_CONSTRUCTOR_STRING_NATIVE_INDEX,
				STRING_CONSTRUCTOR_STRING_NATIVE_NAME,
				native_string_constructor_string);
	native_method_array_set(n, STRING_LENGTH_NATIVE_INDEX,
				STRING_LENGTH_NATIVE_NAME,
				native_string_length);
	native_method_array_set(n, STRING_SUBSTR_NATIVE_INDEX,
				STRING_SUBSTR_NATIVE_NAME,
				native_string_substr);
	native_method_array_set(n, STRING_TO_INTEGER_NATIVE_INDEX,
				STRING_TO_INTEGER_NATIVE_NAME,
				native_string_to_integer);
	native_method_array_set(n, STRING_CONCAT_NATIVE_INDEX,
				STRING_CONCAT_NATIVE_NAME,
				native_string_concat);
	native_method_array_set(n, STRING_GREATER_THAN_NATIVE_INDEX,
				STRING_GREATER_THAN_NATIVE_NAME,
				native_string_greater_than);
	native_method_array_set(n, STRING_LESS_THAN_NATIVE_INDEX,
				STRING_LESS_THAN_NATIVE_NAME,
				native_string_less_than);
	native_method_array_set(n, STRING_HASH_CODE_NATIVE_INDEX,
				STRING_HASH_CODE_NATIVE_NAME,
				native_string_hash_code);
	native_method_array_set(n, STRING_EQUALS_NATIVE_INDEX,
				STRING_EQUALS_NATIVE_NAME,
				native_string_equals);
	native_method_array_set(n, STRING_TO_STRING_NATIVE_INDEX,
				STRING_TO_STRING_NATIVE_NAME,
				native_string_to_string);
	native_method_array_set(n, STRING_CONCAT_OP_NATIVE_INDEX,
				STRING_CONCAT_OP_NATIVE_NAME,
				native_string_concat);

	native_method_array_set(n, TABLE_CONSTRUCTOR_NATIVE_INDEX,
				TABLE_CONSTRUCTOR_NATIVE_NAME,
				native_table_constructor);
	native_method_array_set(n, TABLE_CONSTRUCTOR_INTEGER_NATIVE_INDEX,
				TABLE_CONSTRUCTOR_INTEGER_NATIVE_NAME,
				native_table_constructor_integer);
	native_method_array_set(n, TABLE_GET_NATIVE_INDEX,
				TABLE_GET_NATIVE_NAME,
				native_table_get);
	native_method_array_set(n, TABLE_PUT_NATIVE_INDEX,
				TABLE_PUT_NATIVE_NAME,
				native_table_put);
	native_method_array_set(n, TABLE_REMOVE_NATIVE_INDEX,
				TABLE_REMOVE_NATIVE_NAME,
				native_table_remove);
	native_method_array_set(n, TABLE_FIRST_KEY_NATIVE_INDEX,
				TABLE_FIRST_KEY_NATIVE_NAME,
				native_table_first_key);
	native_method_array_set(n,
				TABLE_NEXT_KEY_NATIVE_INDEX,
				TABLE_NEXT_KEY_NATIVE_NAME,
				native_table_next_key);

	native_method_array_set(n, THREAD_CONSTRUCTOR_NATIVE_INDEX,
				THREAD_CONSTRUCTOR_NATIVE_NAME,
				native_thread_constructor);
	native_method_array_set(n, THREAD_START_NATIVE_INDEX,
				THREAD_START_NATIVE_NAME,
				native_thread_start);
	native_method_array_set(n, THREAD_RUN_NATIVE_INDEX,
				THREAD_RUN_NATIVE_NAME,
				native_thread_run);
	native_method_array_set(n, THREAD_JOIN_NATIVE_INDEX,
				THREAD_JOIN_NATIVE_NAME,
				native_thread_join);
	native_method_array_set(n, THREAD_SLEEP_NATIVE_INDEX,
				THREAD_SLEEP_NATIVE_NAME,
				native_thread_sleep);


	return 0;
}
