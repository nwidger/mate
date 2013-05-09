/* Niels Widger
 * Time-stamp: <01 May 2013 at 21:15:32 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "class.h"
#include "class_table.h"
#include "constants.h"
#include "disassemble.h"
#include "frame.h"
#include "garbage_collector.h"
#include "globals.h"
#include "heap.h"
#include "instruction_table.h"
#include "instructions.h"
#include "integer.h"
#include "invoke_method.h"
#include "local_variable_array.h"
#include "mdb.h"
#include "method_area.h"
#include "native_method_array.h"
#include "native_methods.h"
#include "object.h"
#include "operand_stack.h"
#include "string.h"
#include "symbol_table.h"
#include "thread.h"
#include "vm_stack.h"

#define SETUP_INSTRUCTION()						\
	uint32_t pc;							\
	struct vm_stack *vm_stack;					\
	struct frame *frame;						\
	struct operand_stack *operand_stack;				\
	struct local_variable_array *local_variable_array;		\
	struct frame *calling_frame;					\
	struct operand_stack *calling_frame_operand_stack;		\
									\
        pc = thread_get_pc(t);						\
        vm_stack = thread_get_vm_stack(t);				\
									\
	frame = vm_stack_peek(vm_stack);                                \
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
int is_whitespace(char c);
uint32_t increment_pc(int n, struct thread *t);

int aconst_null_decode_size(uint32_t a) {
	return 1;
}

int aconst_null_decode(uint32_t a) {
	fprintf(stderr, "  %s\n", ACONST_NULL_NAME);
	return 1;
}

int aconst_null_instruction(uint32_t o, struct thread *t) {
	SETUP_INSTRUCTION();

	operand_stack_push(operand_stack, 0);

	thread_set_pc(t, increment_pc(1, t));
	return 0;
}

int aload_decode_size(uint32_t a) {
	return 2;
}

int aload_decode(uint32_t a) {
	uint32_t index;

	index = mvm_disassemble_argument(a, 0);

	fprintf(stderr, "  %s %" PRIu32 "\n", ALOAD_NAME, index);
	return 2;
}

int aload_execute(uint32_t o, struct thread *t, uint32_t index) {
	int ref;

	SETUP_INSTRUCTION();

	ref = local_variable_array_load(local_variable_array, index);
	operand_stack_push(operand_stack, ref);

	thread_set_pc(t, increment_pc(2, t));
	return 0;
}

int aload_instruction(uint32_t o, struct thread *t) {
	uint32_t pc, index;

	pc = thread_get_pc(t);
	index = mvm_disassemble_argument(pc, 0);

	return aload_execute(o, t, index);
}

int areturn_decode_size(uint32_t a) {
	return 1;
}

int areturn_decode(uint32_t a) {
	fprintf(stderr, "  %s\n", ARETURN_NAME);
	return 1;
}

int areturn_instruction(uint32_t o, struct thread *t) {
	int ref, n;
	struct object *object;
	struct integer *integer;

	SETUP_INSTRUCTION();
	n = 0;

	ref = operand_stack_peek_n(operand_stack, n++);

	if (vm_stack_size(vm_stack) > 1) {
		operand_stack_push(calling_frame_operand_stack, ref);
	} else if (vm_stack_size(vm_stack) == 1) {
		object = heap_fetch_object(heap, ref);
		integer = object_get_integer(object);
		main_block_return_value = integer_get_value(integer);
	}

	operand_stack_pop_n(operand_stack, n);
	thread_set_pc(t, vm_stack_pop(vm_stack));

	return 0;
}

int astore_decode_size(uint32_t a) {
	return 2;
}

int astore_decode(uint32_t a) {
	uint32_t index;

	index = mvm_disassemble_argument(a, 0);

	fprintf(stderr, "  %s %" PRIu32 "\n", ASTORE_NAME, index);
	return 2;
}

int astore_execute(uint32_t o, struct thread *t, uint32_t index) {
	int ref, n;

	SETUP_INSTRUCTION();
	n = 0;

	ref = operand_stack_peek_n(operand_stack, n++);
	local_variable_array_store(local_variable_array, index, ref);
	operand_stack_pop_n(operand_stack, n);

	thread_set_pc(t, increment_pc(2, t));
	return 0;
}

int astore_instruction(uint32_t o, struct thread *t) {
	uint32_t pc, index;

	pc = thread_get_pc(t);
	index = mvm_disassemble_argument(pc, 0);

	return astore_execute(o, t, index);
}

int checkcast_decode_size(uint32_t a) {
	return 2;
}

int checkcast_decode(uint32_t a) {
	uint32_t vmt;
	char *class_name;
	struct class *to_class;

	vmt = mvm_disassemble_argument(a, 0);

	to_class = class_table_find(class_table, vmt);
	class_name = class_get_name(to_class);

	fprintf(stderr, "  %s $%s\n", CHECKCAST_NAME, class_name);
	return 2;
}

int checkcast_execute(uint32_t o, struct thread *t, uint32_t vmt) {
	int ref;
	int32_t good, halt;
	struct class *from_class, *to_class;
	struct integer *integer;
	struct object *object;

	SETUP_INSTRUCTION();

	to_class = class_table_find(class_table, vmt);

	ref = operand_stack_peek(operand_stack);
	object = heap_fetch_object(heap, ref);
	integer = object_get_integer(object);
	halt = integer_get_value(integer);

	operand_stack_pop(operand_stack);

	ref = operand_stack_peek(operand_stack);

	good = 1;
	if (ref != 0) {
		object = heap_fetch_object(heap, ref);
		from_class = object_get_class(object);
		if (class_can_cast_to(from_class, to_class) == 0)
			good = 0;
	}

	if (halt != 0) {
		if (good == 0) {
			fprintf(stderr, "mvm: class cast exception!\n");
			mvm_halt();
		}
	} else {
		operand_stack_pop(operand_stack);

		/* lock */
		garbage_collector_pause(garbage_collector);

		ref = class_table_new_integer(class_table, good, NULL);
		operand_stack_push(operand_stack, ref);

		/* unlock */
		garbage_collector_unpause(garbage_collector);
	}

	thread_set_pc(t, increment_pc(2, t));
	return 0;
}

int checkcast_instruction(uint32_t o, struct thread *t) {
	uint32_t pc, vmt;

	pc = thread_get_pc(t);
	vmt = mvm_disassemble_argument(pc, 0);

	return checkcast_execute(o, t, vmt);
}

int dup_decode_size(uint32_t a) {
	return 1;
}

int dup_decode(uint32_t a) {
	fprintf(stderr, "  %s\n", DUP_NAME);
	return 1;
}

int dup_instruction(uint32_t o, struct thread *t) {
	int ref;

	SETUP_INSTRUCTION();

	ref = operand_stack_peek(operand_stack);
	operand_stack_push(operand_stack, ref);

	thread_set_pc(t, increment_pc(1, t));
	return 0;
}

/* TODO: implement these correctly */

int dup_x1_decode_size(uint32_t a) {
	return 1;
}

int dup_x1_decode(uint32_t a) {
	fprintf(stderr, "  %s\n", DUP_X1_NAME);
	return 1;
}

int dup_x1_instruction(uint32_t o, struct thread *t) {
	int ref1, ref2;

	SETUP_INSTRUCTION();

	/* lock */
	garbage_collector_pause(garbage_collector);

	ref1 = operand_stack_pop(operand_stack);
	ref2 = operand_stack_pop(operand_stack);

	operand_stack_push(operand_stack, ref1);
	operand_stack_push(operand_stack, ref2);
	operand_stack_push(operand_stack, ref1);

	/* unlock */
	garbage_collector_unpause(garbage_collector);

	thread_set_pc(t, increment_pc(1, t));
	return 0;
}

int getfield_decode_size(uint32_t a) {
	return 2;
}

int getfield_decode(uint32_t a) {
	uint32_t index;

	index = mvm_disassemble_argument(a, 0);

	fprintf(stderr, "  %s %" PRIu32 "\n", GETFIELD_NAME, index);
	return 2;
}

int getfield_execute(uint32_t o, struct thread *t, uint32_t index) {
	int ref, fieldref, n;
	struct object *object;

	SETUP_INSTRUCTION();
	n = 0;

	ref = operand_stack_peek_n(operand_stack, n++);
	object = heap_fetch_object(heap, ref);
	fieldref = object_load_field(object, index);

	operand_stack_pop_n(operand_stack, n);
	operand_stack_push(operand_stack, fieldref);

	thread_set_pc(t, increment_pc(2, t));
	return 0;
}

int getfield_instruction(uint32_t o, struct thread *t) {
	uint32_t pc, index;

	pc = thread_get_pc(t);
	index = mvm_disassemble_argument(pc, 0);

	return getfield_execute(o, t, index);
}

int goto_decode_size(uint32_t a) {
	return 2;
}

int goto_decode(uint32_t a) {
	char *label;
	uint32_t address;

	address = mvm_disassemble_argument(a, 0);

	if ((label = symbol_table_find_label(symbol_table, address)) != NULL)
		fprintf(stderr, "  %s $%s\n", GOTO_NAME, label);
	else
		fprintf(stderr, "  %s %" PRIu32 "\n", GOTO_NAME, address);

	return 2;
}

int goto_execute(uint32_t o, struct thread *t, uint32_t address) {
	SETUP_INSTRUCTION();

	thread_set_pc(t, address);

	return 0;
}

int goto_instruction(uint32_t o, struct thread *t) {
	uint32_t pc, address;

	pc = thread_get_pc(t);
	address = mvm_disassemble_argument(pc, 0);

	return goto_execute(o, t, address);
}

int ifeq_decode_size(uint32_t a) {
	return 2;
}

int ifeq_decode(uint32_t a) {
	char *label;
	uint32_t address;

	address = mvm_disassemble_argument(a, 0);

	if ((label = symbol_table_find_label(symbol_table, address)) != NULL)
		fprintf(stderr, "  %s $%s\n", IFEQ_NAME, label);
	else
		fprintf(stderr, "  %s %" PRIu32 "\n", IFEQ_NAME, address);

	return 2;
}

int ifeq_execute(uint32_t o, struct thread *t, uint32_t address) {
	int ref, n;
	int32_t value;
	struct object *object;
	struct integer *integer;

	SETUP_INSTRUCTION();
	n = 0;

	ref = operand_stack_peek_n(operand_stack, n++);
	object = heap_fetch_object(heap, ref);
	integer = object_get_integer(object);
	value = integer_get_value(integer);

	operand_stack_pop_n(operand_stack, n);

	if (value == 0)
		thread_set_pc(t, address);
	else
		thread_set_pc(t, increment_pc(2, t));

	return 0;
}

int ifeq_instruction(uint32_t o, struct thread *t) {
	uint32_t pc, address;

	pc = thread_get_pc(t);
	address = mvm_disassemble_argument(pc, 0);

	return ifeq_execute(o, t, address);
}

int in_decode_size(uint32_t a) {
	return 1;
}

int in_decode(uint32_t a) {
	fprintf(stderr, "  %s\n", IN_NAME);
	return 1;
}

int in_instruction(uint32_t o, struct thread *t) {
	int ref, i, size;
	char *buf, c;

	SETUP_INSTRUCTION();

	size = INSTRUCTION_INITIAL_BUFFER_SIZE;
	if ((buf = (char *)malloc(sizeof(char)*size)) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	while ((c = fgetc(input)) != EOF) {
		if (is_whitespace(c) == 0)
			break;
	}

	if (c == EOF) {
		operand_stack_push(operand_stack, 0);
	} else {
		buf[0] = c;

		i = 1;
		while ((c = fgetc(input)) != EOF) {
			if (is_whitespace(c) == 1)
				break;

			buf[i++] = c;
			if (i == size) {
				size *= INSTRUCTION_BUFFER_RESIZE_FACTOR;
				if ((buf = (char *)realloc(buf, sizeof(char)*size)) == NULL) {
					perror("mvm: realloc");
					mvm_halt();
				}
			}
		}

		buf[i] = '\0';

		/* lock */
		garbage_collector_pause(garbage_collector);

		ref = class_table_new_string(class_table, buf, NULL, 0);
		operand_stack_push(operand_stack, ref);

		/* unlock */
		garbage_collector_unpause(garbage_collector);
	}

	thread_set_pc(t, increment_pc(1, t));
	free(buf);
	return 0;
}

int invokespecial_decode_size(uint32_t a) {
	return 4;
}

int invokespecial_decode(uint32_t a) {
	char *label;
	uint32_t address, num_args, max_locals;

	address = mvm_disassemble_argument(a, 0);
	num_args = mvm_disassemble_argument(a, 1);
	max_locals = mvm_disassemble_argument(a, 2);

	if ((label = symbol_table_find_label(symbol_table, address)) != NULL) {
		fprintf(stderr, "  %s $%s %" PRIu32 " %" PRIu32 "\n",
			INVOKESPECIAL_NAME, label, num_args, max_locals);
	} else {
		fprintf(stderr, "  %s %" PRIu32 " %" PRIu32 " %" PRIu32 "\n",
			INVOKESPECIAL_NAME, address, num_args, max_locals);
	}

	return 4;
}

int invokespecial_execute(uint32_t o, struct thread *t, uint32_t address, uint32_t num_args, uint32_t max_locals) {
	char *method_name;
	int ref, i, num_methods;
	struct object *object;
	struct class *class;
	uint32_t end, return_address;

	SETUP_INSTRUCTION();

	ref = operand_stack_peek_n(operand_stack, num_args-1);

	if (ref == 0) {
		fprintf(stderr, "mvm: cannot invoke method on null reference!\n");
		mvm_halt();
	}

	object = heap_fetch_object(heap, ref);
	class = object_get_class(object);

	if ((method_name = symbol_table_find_label(symbol_table, address)) == NULL)
		method_name = "???";

	end = 0;

	num_methods = class_get_num_methods(class);
	for (i = 0; i < num_methods; i++) {
		if (class_method_is_native(class, i) == 0 &&
		    address == class_get_method_field(class, i, address_field)) {
			method_name = class_get_method_name(class, i);
			end = class_get_method_field(class, i, end_field);
		}
	}

	if (end == 0) {
		if (debug != 0) mdb_uninsert_enabled_breakpoints();
		end = mvm_disassemble_method_end(address);
		if (debug != 0) mdb_insert_enabled_breakpoints();
	}

	return_address = increment_pc(4, t);
	invoke_method(t, method_name, address, end, num_args, max_locals, return_address);

	return 0;
}

int invokespecial_instruction(uint32_t o, struct thread *t) {
	uint32_t pc, address, num_args, max_locals;

	pc = thread_get_pc(t);

	address = mvm_disassemble_argument(pc, 0);
	num_args = mvm_disassemble_argument(pc, 1);
	max_locals = mvm_disassemble_argument(pc, 2);

	return invokespecial_execute(o, t, address, num_args, max_locals);
}

int invokenative_decode_size(uint32_t a) {
	return 2;
}

int invokenative_decode(uint32_t a) {
	uint32_t index;

	index = mvm_disassemble_argument(a, 0);

	fprintf(stderr, "  %s %" PRIu32 "\n", INVOKENATIVE_NAME, index);
	return 2;
}

int invokenative_execute(uint32_t o, struct thread *t, uint32_t index) {
	uint32_t return_address;

	SETUP_INSTRUCTION();

	return_address = increment_pc(2, t);
	invoke_native_method(t, index, return_address);

	return 0;
}

int invokenative_instruction(uint32_t o, struct thread *t) {
	uint32_t pc, index;

	pc = thread_get_pc(t);
	index = mvm_disassemble_argument(pc, 0);

	return invokenative_execute(o, t, index);
}

int invokevirtual_decode_size(uint32_t a) {
	return 3;
}

int invokevirtual_decode(uint32_t a) {
	uint32_t index, num_args;

	index = mvm_disassemble_argument(a, 0);
	num_args = mvm_disassemble_argument(a, 1);

	fprintf(stderr, "  %s %" PRIu32 " %" PRIu32 "\n",
		INVOKEVIRTUAL_NAME, index, num_args);
	return 3;
}

int invokevirtual_execute(uint32_t o, struct thread *t, uint32_t index, uint32_t num_args) {
	int ref;
	uint32_t return_address;

	SETUP_INSTRUCTION();

	ref = operand_stack_peek_n(operand_stack, num_args-1);

	if (ref == 0) {
		fprintf(stderr, "mvm: cannot invoke method on null reference!\n");
		mvm_halt();
	}

	return_address = increment_pc(3, t);
	invoke_virtual_method(t, ref, index, num_args, return_address);

	return 0;
}

int invokevirtual_instruction(uint32_t o, struct thread *t) {
	uint32_t pc, index, num_args;

	pc = thread_get_pc(t);
	index = mvm_disassemble_argument(pc, 0);
	num_args = mvm_disassemble_argument(pc, 1);

	return invokevirtual_execute(o, t, index, num_args);
}

int monitorenter_decode_size(uint32_t a) {
	return 1;
}

int monitorenter_decode(uint32_t a) {
	fprintf(stderr, "  %s\n", MONITORENTER_NAME);
	return 1;
}

int monitorenter_instruction(uint32_t o, struct thread *t) {
	int ref;
	struct object *object;

	SETUP_INSTRUCTION();

	ref = operand_stack_pop(operand_stack);

	if (ref == 0) {
		fprintf(stderr, "mvm: cannot acquire monitor on null reference!\n");
		mvm_halt();
	}

	object = heap_fetch_object(heap, ref);
	object_acquire_monitor(object);

	thread_set_pc(t, increment_pc(1, t));
	return 0;
}

int monitorexit_decode_size(uint32_t a) {
	return 1;
}

int monitorexit_decode(uint32_t a) {
	fprintf(stderr, "  %s\n", MONITOREXIT_NAME);
	return 1;
}

int monitorexit_instruction(uint32_t o, struct thread *t) {
	int ref;
	struct object *object;

	SETUP_INSTRUCTION();

	ref = operand_stack_pop(operand_stack);

	if (ref == 0) {
		fprintf(stderr, "mvm: cannot release monitor of null reference!\n");
		mvm_halt();
	}

	object = heap_fetch_object(heap, ref);
	object_release_monitor(object);

	thread_set_pc(t, increment_pc(1, t));
	return 0;
}

int new_decode_size(uint32_t a) {
	return 2;
}

int new_decode(uint32_t a) {
	uint32_t vmt;
	char *class_name;
	struct class *class;

	vmt = mvm_disassemble_argument(a, 0);

	class = class_table_find(class_table, vmt);
	class_name = class_get_name(class);

	fprintf(stderr, "  %s $%s\n", NEW_NAME, class_name);
	return 2;
}

int new_execute(uint32_t o, struct thread *t, uint32_t vmt) {
	int ref;

	SETUP_INSTRUCTION();

	/* lock */
	garbage_collector_pause(garbage_collector);

	ref = class_table_new(class_table, vmt, NULL);
	operand_stack_push(operand_stack, ref);

	/* unlock */
	garbage_collector_unpause(garbage_collector);

	thread_set_pc(t, increment_pc(2, t));
	return 0;
}

int new_instruction(uint32_t o, struct thread *t) {
	uint32_t pc, vmt;

	pc = thread_get_pc(t);
	vmt = mvm_disassemble_argument(pc, 0);

	return new_execute(o, t, vmt);
}

int newint_decode_size(uint32_t a) {
	return 2;
}

int newint_decode(uint32_t a) {
	uint32_t value;

	value = mvm_disassemble_argument(a, 0);

	fprintf(stderr, "  %s %" PRIu32 "\n", NEWINT_NAME, value);
	return 2;
}

int newint_execute(uint32_t o, struct thread *t, uint32_t value) {
	int ref;

	SETUP_INSTRUCTION();

	if (value < INTEGER_MIN_INTEGER) {
		fprintf(stderr, "mvm: integer literal is too small!");
		mvm_halt();
	}

	if (value > INTEGER_MAX_INTEGER) {
		fprintf(stderr, "mvm: integer literal is too large!");
		mvm_halt();
	}

	/* lock */
	garbage_collector_pause(garbage_collector);

	ref = class_table_new_integer(class_table, (int32_t)value, NULL);
	operand_stack_push(operand_stack, ref);

	/* unlock */
	garbage_collector_unpause(garbage_collector);

	thread_set_pc(t, increment_pc(2, t));
	return 0;
}

int newint_instruction(uint32_t o, struct thread *t) {
	uint32_t pc, value;

	pc = thread_get_pc(t);
	value = mvm_disassemble_argument(pc, 0);

	return newint_execute(o, t, value);
}

int newreal_decode_size(uint32_t a) {
	return newstr_decode_size(a);
}


int newreal_decode(uint32_t a) {
	int len;
	char *buf;

	buf = mvm_disassemble_string_argument(a);
	len = strlen(buf);

	fprintf(stderr, "  %s \"%s\"\n", NEWREAL_NAME, buf);
	free(buf);
	return len+2;
}

int newreal_instruction(uint32_t o, struct thread *t) {
	int ref;
	char *buf;
	float value;

	SETUP_INSTRUCTION();

	buf = mvm_disassemble_string_argument(pc);
	value = strtof(buf, NULL);

	/* lock */
	garbage_collector_pause(garbage_collector);

	ref = class_table_new_real(class_table, value, NULL);
	operand_stack_push(operand_stack, ref);

	/* unlock */
	garbage_collector_unpause(garbage_collector);

	thread_set_pc(t, increment_pc(strlen(buf)+2, t));
	free(buf);
	return 0;
}

int newstr_decode_size(uint32_t a) {
	int len;
	char *buf;

	buf = mvm_disassemble_string_argument(a);
	len = strlen(buf);

	free(buf);
	return len+2;
}

int newstr_decode(uint32_t a) {
	int len;
	char *buf;

	buf = mvm_disassemble_string_argument(a);
	len = strlen(buf);

	fprintf(stderr, "  %s \"%s\"\n", NEWSTR_NAME, buf);
	free(buf);
	return len+2;
}

int newstr_instruction(uint32_t o, struct thread *t) {
	int ref;
	char *buf;

	SETUP_INSTRUCTION();

	buf = mvm_disassemble_string_argument(pc);

	/* lock */
	garbage_collector_pause(garbage_collector);

	ref = class_table_new_string(class_table, buf, NULL, 1);
	operand_stack_push(operand_stack, ref);

	/* unlock */
	garbage_collector_unpause(garbage_collector);

	thread_set_pc(t, increment_pc(strlen(buf)+2, t));
	free(buf);
	return 0;
}

int out_decode_size(uint32_t a) {
	return 1;
}

int out_decode(uint32_t a) {
	fprintf(stderr, "  %s\n", OUT_NAME);
	return 1;
}

int out_instruction(uint32_t o, struct thread *t) {
	int ref, n;
	char *chars;
	struct object *object;
	struct string *string;

	SETUP_INSTRUCTION();
	n = 0;

	ref = operand_stack_peek_n(operand_stack, n++);
	object = heap_fetch_object(heap, ref);
	string = object_get_string(object);
	chars = string_get_chars(string);

	fprintf(output, "%s", chars);

	operand_stack_pop_n(operand_stack, n);

	thread_set_pc(t, increment_pc(1, t));
	return 0;
}

int pop_decode_size(uint32_t a) {
	return 1;
}

int pop_decode(uint32_t a) {
	fprintf(stderr, "  %s\n", POP_NAME);
	return 1;
}

int pop_instruction(uint32_t o, struct thread *t) {
	SETUP_INSTRUCTION();

	operand_stack_pop(operand_stack);

	thread_set_pc(t, increment_pc(1, t));
	return 0;
}

int putfield_decode_size(uint32_t a) {
	return 2;
}

int putfield_decode(uint32_t a) {
	uint32_t index;

	index = mvm_disassemble_argument(a, 0);

	fprintf(stderr, "  %s %" PRIu32 "\n", PUTFIELD_NAME, index);
	return 2;
}

int putfield_execute(uint32_t o, struct thread *t, uint32_t index) {
	int ref, objectref, n;
	struct object *object;

	SETUP_INSTRUCTION();
	n = 0;

	ref = operand_stack_peek_n(operand_stack, n++);

	objectref = operand_stack_peek_n(operand_stack, n++);
	object = heap_fetch_object(heap, objectref);

	object_store_field(object, index, ref);
	operand_stack_pop_n(operand_stack, n);

	thread_set_pc(t, increment_pc(2, t));
	return 0;
}

int putfield_instruction(uint32_t o, struct thread *t) {
	uint32_t pc, index;

	pc = thread_get_pc(t);
	index = mvm_disassemble_argument(pc, 0);

	return putfield_execute(o, t, index);
}

int refcmp_decode_size(uint32_t a) {
	return 1;
}

int refcmp_decode(uint32_t a) {
	fprintf(stderr, "  %s\n", REFCMP_NAME);
	return 1;
}

int refcmp_instruction(uint32_t o, struct thread *t) {
	int32_t value;
	int ref, ref1, ref2;

	SETUP_INSTRUCTION();

	ref1 = operand_stack_pop(operand_stack);
	ref2 = operand_stack_pop(operand_stack);

	value = (ref1 == ref2) ? 1 : 0;

	/* lock */
	garbage_collector_pause(garbage_collector);

	ref = class_table_new_integer(class_table, value, NULL);
	operand_stack_push(operand_stack, ref);

	/* unlock */
	garbage_collector_unpause(garbage_collector);

	thread_set_pc(t, increment_pc(1, t));
	return 0;
}

int return_decode_size(uint32_t a) {
	return 1;
}

int return_decode(uint32_t a) {
	fprintf(stderr, "  %s\n", RETURN_NAME);
	return 1;
}

int return_instruction(uint32_t o, struct thread *t) {
	SETUP_INSTRUCTION();

	thread_set_pc(t, vm_stack_pop(vm_stack));
	return 0;
}

int add_instructions(struct instruction_table *i) {
	instruction_table_add(i, ACONST_NULL_NAME, ACONST_NULL_OPCODE,
			      aconst_null_instruction, aconst_null_decode,
			      aconst_null_decode_size);
	instruction_table_add(i, ALOAD_NAME, ALOAD_OPCODE,
			      aload_instruction, aload_decode,
			      aload_decode_size);
	instruction_table_add(i, ARETURN_NAME, ARETURN_OPCODE,
			      areturn_instruction, areturn_decode,
			      areturn_decode_size);
	instruction_table_add(i, ASTORE_NAME, ASTORE_OPCODE,
			      astore_instruction, astore_decode,
			      astore_decode_size);
	instruction_table_add(i, CHECKCAST_NAME, CHECKCAST_OPCODE,
			      checkcast_instruction, checkcast_decode,
			      checkcast_decode_size);
	instruction_table_add(i, DUP_NAME, DUP_OPCODE,
			      dup_instruction, dup_decode,
			      dup_decode_size);
	instruction_table_add(i, DUP_X1_NAME, DUP_X1_OPCODE,
			      dup_x1_instruction, dup_x1_decode,
			      dup_x1_decode_size);
	instruction_table_add(i, GETFIELD_NAME, GETFIELD_OPCODE,
			      getfield_instruction, getfield_decode,
			      getfield_decode_size);
	instruction_table_add(i, GOTO_NAME, GOTO_OPCODE,
			      goto_instruction, goto_decode,
			      goto_decode_size);
	instruction_table_add(i, IFEQ_NAME, IFEQ_OPCODE,
			      ifeq_instruction, ifeq_decode,
			      ifeq_decode_size);
	instruction_table_add(i, IN_NAME, IN_OPCODE,
			      in_instruction, in_decode,
			      in_decode_size);
	instruction_table_add(i, INVOKESPECIAL_NAME, INVOKESPECIAL_OPCODE,
			      invokespecial_instruction, invokespecial_decode,
			      invokespecial_decode_size);
	instruction_table_add(i, INVOKENATIVE_NAME, INVOKENATIVE_OPCODE,
			      invokenative_instruction, invokenative_decode,
			      invokenative_decode_size);
	instruction_table_add(i, INVOKEVIRTUAL_NAME, INVOKEVIRTUAL_OPCODE,
			      invokevirtual_instruction, invokevirtual_decode,
			      invokevirtual_decode_size);
	instruction_table_add(i, MONITORENTER_NAME, MONITORENTER_OPCODE,
			      monitorenter_instruction, monitorenter_decode,
			      monitorenter_decode_size);
	instruction_table_add(i, MONITOREXIT_NAME, MONITOREXIT_OPCODE,
			      monitorexit_instruction, monitorexit_decode,
			      monitorexit_decode_size);
	instruction_table_add(i, NEW_NAME, NEW_OPCODE,
			      new_instruction, new_decode,
			      new_decode_size);
	instruction_table_add(i, NEWINT_NAME, NEWINT_OPCODE,
			      newint_instruction, newint_decode,
			      newint_decode_size);
	instruction_table_add(i, NEWREAL_NAME, NEWREAL_OPCODE,
			      newreal_instruction, newreal_decode,
			      newreal_decode_size);
	instruction_table_add(i, NEWSTR_NAME, NEWSTR_OPCODE,
			      newstr_instruction, newstr_decode,
			      newstr_decode_size);
	instruction_table_add(i, OUT_NAME, OUT_OPCODE,
			      out_instruction, out_decode,
			      out_decode_size);
	instruction_table_add(i, POP_NAME, POP_OPCODE,
			      pop_instruction, pop_decode,
			      pop_decode_size);
	instruction_table_add(i, PUTFIELD_NAME, PUTFIELD_OPCODE,
			      putfield_instruction, putfield_decode,
			      putfield_decode_size);
	instruction_table_add(i, REFCMP_NAME, REFCMP_OPCODE,
			      refcmp_instruction, refcmp_decode,
			      refcmp_decode_size);
	instruction_table_add(i, RETURN_NAME, RETURN_OPCODE,
			      return_instruction, return_decode,
			      return_decode_size);

	return 0;
}

int is_whitespace(char c) {
	int i, n;
	char *whitespace = IN_INSTRUCTION_WHITESPACE;

	n = strlen(whitespace);
	for (i = 0; i < n; i++) {
		if (c == whitespace[i])
			return 1;
	}

	return 0;
}

uint32_t increment_pc(int n, struct thread *t) {
	return thread_get_pc(t) + (sizeof(uint32_t)*n);
}
