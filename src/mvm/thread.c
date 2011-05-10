/* Niels Widger
 * Time-stamp: <09 May 2011 at 16:22:39 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "class_table.h"
#include "dmp.h"
#include "globals.h"
#include "heap.h"
#include "integer.h"
#include "invoke_method.h"
#include "mdb.h"
#include "object.h"
#include "operand_stack.h"
#include "thread.h"
#include "thread_dmp.h"
#include "vm_stack.h"

/* struct definitions */
struct thread {
	int ref;
	pthread_t id;
	enum thread_state state;

	uint32_t pc;
	struct vm_stack *vm_stack;

	struct thread_dmp *dmp;
};

/* globals */
pthread_key_t key;
pthread_once_t key_once = PTHREAD_ONCE_INIT;

/* forward declarations */
void thread_make_key();
int thread_join0(struct object *o, int d);
int thread_pthread_create(struct thread *t, void * (*s)(void *));
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

	if (dmp == NULL)
		t->dmp = NULL;
	else
		t->dmp = dmp_create_thread_dmp(dmp, t);

	return t;
}

void thread_destroy(struct thread *t) {
	if (t != NULL) {
		thread_set_current(NULL);
		thread_clear(t);
		vm_stack_destroy(t->vm_stack);
		heap_free(heap, t);
	}
}

void thread_clear(struct thread *t) {
	if (t == NULL)
		return;

	if (t->state == new_state ||
	    t->state == terminated_state) {
		return;
	}

	if (pthread_kill(t->id, 0) != 0) {
		perror("mvm: pthread_kill");
		mvm_halt();
	}

	t->ref = 0;
	t->state = new_state;
	t->pc = 0;

	vm_stack_clear(t->vm_stack);
}

struct thread_dmp * thread_get_dmp() {
	struct thread *t;

	t = (struct thread *)pthread_getspecific(key);
	
	if (t == NULL) {
		fprintf(stderr, "mvm: thread not initialized!\n");
		mvm_halt();
	}

	return t->dmp;
}

struct thread_dmp * _thread_get_dmp(struct thread *t) {
	if (t == NULL) {
		fprintf(stderr, "mvm: thread not initialized!\n");
		mvm_halt();
	}

	return t->dmp;
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

	return t;
}

int thread_set_current(struct thread *t) {
	if (pthread_setspecific(key, (void *)t) != 0) {
		perror("mvm: pthread_setspecific");
		mvm_halt();
	}

	return 0;
}

int thread_get_ref() {
	struct thread *t;

	t = thread_get_current();

	return t->ref;
}

int _thread_get_ref(struct thread *t) {
	if (t == NULL) {
		fprintf(stderr, "mvm: thread not initialized!\n");
		mvm_halt();
	}

	return t->ref;
}

struct vm_stack * thread_get_vm_stack() {
	struct thread *t;

	t = thread_get_current();

	return t->vm_stack;
}

struct vm_stack * _thread_get_vm_stack(struct thread *t) {
	if (t == NULL) {
		fprintf(stderr, "mvm: thread not initialized!\n");
		mvm_halt();
	}

	return t->vm_stack;
}

int thread_get_state() {
	struct thread *t;

	t = thread_get_current();

	return t->state;
}

int _thread_get_state(struct thread *t) {
	if (t == NULL) {
		fprintf(stderr, "mvm: thread not initialized!\n");
		mvm_halt();
	}

	return t->state;
}

uint32_t thread_get_pc() {
	struct thread *t;

	t = thread_get_current();

	return t->pc;
}

uint32_t _thread_get_pc(struct thread *t) {
	if (t == NULL) {
		fprintf(stderr, "mvm: thread not initialized!\n");
		mvm_halt();
	}

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

	if (t->dmp != NULL)
		thread_dmp_thread_creation(t->dmp);

	vm_stack_push(vm_stack, "thread_start", 0, 0, 0, 0, 0);
	frame = vm_stack_peek(vm_stack);
	operand_stack = frame_get_operand_stack(frame);
	operand_stack_push(operand_stack, t->ref);

	if (thread_pthread_create(t, thread_run0) != 0)
		mvm_halt();

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

	if (t->dmp != NULL)
		dmp_add_thread(dmp, t->ref);

	if (thread_pthread_create(t, thread_run0_main) != 0)
		mvm_halt();

	return 0;
}

int thread_pthread_create(struct thread *t, void * (*s)(void *)) {
	int err;
	pthread_attr_t attr;

	if (pthread_attr_init(&attr) != 0) {
		perror("mvm: pthread_attr_init");
		mvm_halt();
	}

	if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE) != 0) {
		perror("mvm: pthread_attr_setdetachstate");
		mvm_halt();
	}

	for (;;) {
		err = pthread_create(&t->id, &attr, s, (void *)t);

		if (err == 0) {
			break;
		} else if (err == EAGAIN) {
			usleep(50000);
			continue;
		} else {
			perror("mvm: pthread_create");
			mvm_halt();
		}
	}

	return 0;
}

int thread_join(struct object *o) {
	return thread_join0(o, 1);
}

int thread_join_main(struct object *o) {
	return thread_join0(o, 0);
}

int thread_join0(struct object *o, int d) {
	void *value;
	struct thread *t;

	t = object_get_thread(o);

	if (t == NULL) {
		fprintf(stderr, "mvm: thread not initialized!\n");
		mvm_halt();
	}

	if (d != 0 && t->dmp != NULL)
		thread_dmp_thread_join(t->dmp);

	if (pthread_join(t->id, &value) != 0) {
		perror("mvm: pthread_join");
		mvm_halt();
	}

	return 0;
}

int thread_sleep(struct object *o, struct object *p) {
	int32_t millisec;
	struct thread *t;
	struct integer *i;

	t = object_get_thread(o);

	if (t == NULL) {
		fprintf(stderr, "mvm: thread not initialized!\n");
		mvm_halt();
	}

	i = object_get_integer(p);	
	millisec = integer_get_value(i);

	if (usleep(millisec * 1000) != 0) {
		perror("mvm: usleep");
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

	if (t->dmp != NULL)
		thread_dmp_thread_start(t->dmp);

	t->state = runnable_state;

	if (invoke_virtual_method_by_name(t->ref, t->pc, "run", 0) != 0)
		mvm_halt();

	if (t->dmp != NULL)
		thread_dmp_thread_destruction(t->dmp);

	t->state = terminated_state;
	/* run method has terminated, thread instance can now be collected */
	heap_include_ref(heap, t->ref);
	heap_remove_thread_ref(heap, t->ref);
	pthread_exit(0);
}

void * thread_run0_main(void *p) {
	struct thread *t;

	t = (struct thread *)p;

	if (thread_set_current(t) != 0)
		mvm_halt();

	t->state = runnable_state;

	if (debug != 0)
		mdb_hook(startup_hook);

	main_block_return_value = 0;
	if (invoke_method("mainBlock", main_block_address, main_block_end,
			  0, main_block_max_locals, 0) != 0) {
		mvm_halt();
	}

	if (t->dmp != NULL)
		thread_dmp_thread_destruction(t->dmp);	
	
	t->state = terminated_state;
	/* run method has terminated, thread instance can now be collected */
	heap_include_ref(heap, t->ref);
	heap_remove_thread_ref(heap, t->ref);
	pthread_exit(0);
}
