/* Niels Widger
 * Time-stamp: <21 Nov 2010 at 23:38:51 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "class_table.h"
#include "globals.h"
#include "heap.h"
#include "invoke_method.h"
#include "object.h"
#include "operand_stack.h"
#include "thread.h"
#include "vm_stack.h"

/* struct definitions */
struct thread {
	int ref;
	pthread_t id;
	enum thread_state state;

	uint32_t pc;
	struct vm_stack *vm_stack;
};

/* globals */
pthread_key_t key;
pthread_once_t key_once = PTHREAD_ONCE_INIT;

/* forward declarations */
void thread_make_key();
void * thread_run0(void *p);
void * thread_run0_main(void *p);

struct thread * thread_create() {
	struct thread *t;

	pthread_once(&key_once, thread_make_key);

	if ((t = (struct thread *)heap_malloc(heap, sizeof(struct thread))) == NULL)
		mvm_halt();

	t->ref = 0;
	t->state = new_state;
	t->pc = 0;

	if ((t->vm_stack = vm_stack_create()) == NULL)
		mvm_halt();

	return t;
}

void thread_destroy(struct thread *t) {
	if (t != NULL) {
		thread_clear(t);
		vm_stack_destroy(t->vm_stack);
		heap_free(heap, t);
	}
}

void thread_clear(struct thread *t) {
	if (t != NULL) {

	}
}

void thread_make_key() {
	if (pthread_key_create(&key, NULL) != 0) {
		perror("mvm: pthread_key_create");
		mvm_halt();
	}
}

struct thread * thread_get_current() {
	struct thread *t;

	t = (struct thread *)pthread_getspecific(key);

	if (t == NULL) {
		fprintf(stderr, "mvm: thread not initialized!\n");
		mvm_halt();
	}

	return t;
}

int thread_set_current(struct thread *t) {
	if (pthread_setspecific(key, (void *)t) != 0) {
		perror("mvm: pthread_setspecific");
		mvm_halt();
	}

	return 0;
}

struct vm_stack * thread_get_vm_stack() {
	struct thread *t;

	t = thread_get_current();

	return t->vm_stack;
}

uint32_t thread_get_pc() {
	struct thread *t;

	t = thread_get_current();

	return t->pc;
}

uint32_t thread_set_pc(uint32_t p) {
	struct thread *t;

	t = thread_get_current();

	t->pc = p;
	return p;
}

int thread_start(struct object *o) {
	struct thread *t;
	struct frame *frame;
	struct vm_stack *vm_stack;
	struct operand_stack *operand_stack;

	t = object_get_thread(o);
	vm_stack = t->vm_stack;
	
	if (t == NULL) {
		fprintf(stderr, "mvm: thread not initialized!\n");
		mvm_halt();
	}

	t->ref = object_get_ref(o);

	vm_stack_push(vm_stack, "thread_start", 0, 0, 0, 0, 0);
	frame = vm_stack_peek(vm_stack);
	operand_stack = frame_get_operand_stack(frame);
	operand_stack_push(operand_stack, t->ref);

	if (pthread_create(&t->id, NULL, thread_run0, (void *)t) != 0) {
		perror("mvm: pthread_create");
		mvm_halt();
	}

	return 0;
}

int thread_start_main(struct object *o) {
	struct thread *t;

	t = object_get_thread(o);

	if (t == NULL) {
		fprintf(stderr, "mvm: thread not initialized!\n");
		mvm_halt();
	}

	t->ref = object_get_ref(o);

	if (pthread_create(&t->id, NULL, thread_run0_main, (void *)t) != 0) {
		perror("mvm: pthread_create");
		mvm_halt();
	}

	return 0;
}

int thread_join(struct object *o) {
	struct thread *t;

	t = object_get_thread(o);

	if (t == NULL) {
		fprintf(stderr, "mvm: thread not initialized!\n");
		mvm_halt();
	}

	if (pthread_join(t->id, NULL) != 0) {
		perror("mvm: pthread_join");
		mvm_halt();
	}

	return 0;
}

int thread_run(struct object *o) {
	struct thread *t;

	t = object_get_thread(o);

	if (t == NULL) {
		fprintf(stderr, "mvm: thread not initialized!\n");
		mvm_halt();
	}

	return 0;
}

void * thread_run0(void *p) {
	struct thread *t;

	t = (struct thread *)p;

	if (thread_set_current(t) != 0)
		mvm_halt();

	t->state = runnable_state;

	if (invoke_virtual_method_by_name(t->ref, t->pc, "run", 0) != 0)
		mvm_halt();

	t->state = terminated_state;
	/* run method has terminated, thread instance can now be collected */
	heap_include_ref(heap, t->ref);
	pthread_exit(0);
}

void * thread_run0_main(void *p) {
	struct thread *t;

	t = (struct thread *)p;

	if (thread_set_current(t) != 0)
		mvm_halt();

	t->state = runnable_state;

	main_block_return_value = 0;
	if (invoke_method("mainBlock", main_block_address, main_block_end,
			  0, main_block_max_locals, 0) != 0) {
		mvm_halt();
	}

	t->state = terminated_state;
	/* run method has terminated, thread instance can now be collected */
	heap_include_ref(heap, t->ref);
	pthread_exit(0);
}
