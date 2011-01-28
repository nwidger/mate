/* Niels Widger
 * Time-stamp: <27 Jan 2011 at 19:45:00 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "barrier.h"
#include "class.h"
#include "class_table.h"
#include "constants.h"
#include "disassemble.h"
#include "garbage_collector.h"
#include "globals.h"
#include "heap.h"
#include "instruction_table.h"
#include "invoke_method.h"
#include "local_variable_array.h"
#include "mdb.h"
#include "method_area.h"
#include "native_method_array.h"
#include "object.h"
#include "operand_stack.h"
#include "symbol_table.h"
#include "thread.h"
#include "vm_stack.h"

/* forward declarations */
int execute_method(char *e, uint32_t a, uint32_t b, uint32_t n, uint32_t m, uint32_t r);

int invoke_virtual_method_by_name(int r, uint32_t a, char *m, int n, ...) {
	int i, len;
	va_list ap;
	struct class *class;
	struct object *object;
	char *arg, *buf, *name;
	uint32_t num_methods;

	len = strlen(m);

	va_start(ap, n);
	for (i = 0; i < n; i++) {
		arg = va_arg(ap, char *);
		len += 1 + strlen(arg); /* need "$" separator */
	}
	va_end(ap);

	if ((buf = (char *)calloc(len+1, sizeof(char))) == NULL) {
		perror("mvm: calloc");
		mvm_halt();
	}

	strcat(buf, m);

	va_start(ap, n);
	for (i = 0; i < n; i++) {
		arg = va_arg(ap, char *);
		strcat(buf, "$");
		strcat(buf, arg);
	}
	va_end(ap);

	object = heap_fetch_object(heap, r);
	class = object_get_class(object);

	num_methods = class_get_num_methods(class);
	for (i = 0; i < num_methods; i++) {
		/* strip off leading class name and "$" separator */
		name = strchr(class_get_method_name(class, i), '$') + 1;
		if (strcmp(name, buf) == 0)
			break;
	}

	if (i >= num_methods) {
		fprintf(stderr, "mvm: No such method %s!\n", buf);
		free(buf);
		mvm_halt();
	}

	invoke_virtual_method(r, i, n+1, a);

	free(buf);
	return 0;
}

int invoke_virtual_method(int r, int i, int n, uint32_t a) {
	struct class *class;
	struct object *object;
	char *method_name;
	uint32_t method_address, method_end, max_locals, native_index;

	object = heap_fetch_object(heap, r);
	class = object_get_class(object);

	if (class_method_is_native(class, i) == 1) {
		native_index = class_get_method_field(class, i, native_index_field);
		invoke_native_method(native_index, a);
	} else {
		method_name = class_get_method_name(class, i);
		method_address = class_get_method_field(class, i, address_field);
		method_end = class_get_method_field(class, i, end_field);
		max_locals = class_get_method_field(class, i, max_locals_field);
		execute_method(method_name, method_address,
			       method_end, n, max_locals, a);
	}

	return 0;
}

int invoke_native_method(int i, uint32_t r) {
	uint32_t pc;
	int num_args;
	char *name;
	struct vm_stack *vm_stack;

	pc = thread_get_pc();
	vm_stack = thread_get_vm_stack();

	name = native_method_array_get_name(native_method_array, i);
	num_args = native_method_array_get_num_args(native_method_array, i);

	/* lock */
	garbage_collector_lock(garbage_collector);

	vm_stack_push(vm_stack, name, num_args, num_args, 0, 0, r);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	if (debug != 0) {
		mdb_hook(enter_method_hook);
		if (restart != 0)
			return 0;
	}

	if (native_method_array_execute(native_method_array, i) != 0)
		mvm_halt();

	thread_set_pc(vm_stack_pop(vm_stack));

	if (debug != 0) {
		mdb_hook(leave_method_hook);
		if (restart != 0)
			return 0;
	}

	return 0;
}

int invoke_method(char *e, uint32_t a, uint32_t b, int n, int m, uint32_t r) {
	return execute_method(e, a, b, n, m, r);
}

int execute_method(char *e, uint32_t a, uint32_t b, uint32_t n, uint32_t m, uint32_t r) {
	int old_size;
	uint32_t pc, opcode;
	struct frame *frame;
	struct vm_stack *vm_stack;

	vm_stack = thread_get_vm_stack();	

	/* lock */
	garbage_collector_lock(garbage_collector);

	old_size = vm_stack_size(vm_stack);
	frame = vm_stack_push(vm_stack, e, n, m, a, b, r);

	/* unlock */
	garbage_collector_unlock(garbage_collector);

	thread_set_pc(a);

	while (vm_stack_empty(vm_stack) == 0 &&
	       vm_stack_size(vm_stack) > old_size) {
		pc = thread_get_pc();
		
		frame_set_current_address(frame, pc);
		opcode = method_area_fetch(method_area, pc);

		if (debug == 0 && verbose != 0)
			mvm_disassemble_instruction(0, pc);

		if (debug != 0) {
			opcode = mdb_hook(execute_instruction_hook);
			if (restart != 0)
				return 0;
		}

		if (instruction_table_execute(instruction_table, opcode) != 0)
			mvm_halt();

		thread_increment_instruction_counter();

		if ((thread_get_instruction_counter() % 40) == 0)
			barrier_await(pbarrier);

		if (debug != 0 && restart != 0)
			return 0;
	}

	if (debug != 0) {
		if (vm_stack_size(vm_stack) != 0)
			mdb_hook(leave_method_hook);
		else
			mdb_hook(terminate_hook);
	}

	return 0;
}
