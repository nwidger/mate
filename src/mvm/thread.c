/* Niels Widger
 * Time-stamp: <23 Dec 2012 at 20:41:29 by nwidger on macros.local>
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
#include "globals.h"
#include "heap.h"
#include "integer.h"
#include "invoke_method.h"
#include "mdb.h"
#include "object.h"
#include "operand_stack.h"
#include "thread.h"
#include "vm_stack.h"

#ifdef DMP
#include "dmp.h"
#include "thread_dmp.h"
#endif

/* struct definitions */
struct thread {
	int ref;
	pthread_t id;
	enum thread_state state;

	uint32_t pc;
	struct vm_stack *vm_stack;

#ifdef DMP
	struct thread_dmp *dmp;
#endif
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

#ifdef DMP	
	if (dmp == NULL)
		t->dmp = NULL;
	else
		t->dmp = dmp_create_thread_dmp(dmp, t);
#endif

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

#ifdef DMP
struct thread_dmp * thread_get_dmp(struct thread *t) {
	if (t == NULL)
		t = thread_get_current();

	if (t == NULL) {
		fprintf(stderr, "mvm: thread not initialized!\n");
		mvm_halt();
	}

	return t->dmp;
}
#endif

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

int thread_get_ref(struct thread *t) {
	if (t == NULL)
		t = thread_get_current();

	if (t == NULL) {
		fprintf(stderr, "mvm: thread not initialized!\n");
		mvm_halt();
	}

	return t->ref;
}

struct vm_stack * thread_get_vm_stack(struct thread *t) {
	if (t == NULL)
		t = thread_get_current();

	if (t == NULL) {
		fprintf(stderr, "mvm: thread not initialized!\n");
		mvm_halt();
	}

	return t->vm_stack;
}

int thread_get_state(struct thread *t) {
	if (t == NULL)
		t = thread_get_current();

	if (t == NULL) {
		fprintf(stderr, "mvm: thread not initialized!\n");
		mvm_halt();
	}

	return t->state;
}

uint32_t thread_get_pc(struct thread *t) {
	if (t == NULL)
		t = thread_get_current();

	if (t == NULL) {
		fprintf(stderr, "mvm: thread not initialized!\n");
		mvm_halt();
	}

	return t->pc;
}

uint32_t thread_set_pc(struct thread *t, uint32_t p) {
	if (t == NULL)
		t = thread_get_current();

	if (t == NULL) {
		fprintf(stderr, "mvm: thread not initialized!\n");
		mvm_halt();
	}

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

	if (t->state != new_state) {
		fprintf(stderr, "mvm: thread cannot be started more than once!\n");
		mvm_halt();
	}

	t->ref = object_get_ref(o);

#ifdef DMP
	if (t->dmp != NULL)
		thread_dmp_thread_creation(t->dmp);
#endif	

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

#ifdef DMP	
	if (t->dmp != NULL)
		dmp_add_thread(dmp, t->ref);
#endif

	if (thread_pthread_create(t, thread_run0_main) != 0)
		mvm_halt();

	return 0;
}

int thread_pthread_create(struct thread *t, void * (*s)(void *)) {
	int i, err;
	pthread_attr_t attr;

	if (pthread_attr_init(&attr) != 0) {
		perror("mvm: pthread_attr_init");
		mvm_halt();
	}

	if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE) != 0) {
		perror("mvm: pthread_attr_setdetachstate");
		mvm_halt();
	}

	for (i = 0; i < 10; i++) {
		err = pthread_create(&t->id, &attr, s, (void *)t);

		if (err == 0) {
			break;
		} else if (err == EAGAIN) {
			usleep(50000 * i);
			continue;
		} else {
			break;
		}
	}

	if (err != 0) {
		perror("mvm: pthread_create");
		mvm_halt();
	}

#if defined(__linux)
	cpu_set_t cpuset, retset;
	pthread_t thread;
	int retval;
	/* first thread is assigned to cpu 0 */
	static int next = 0;

	thread = pthread_self();

	for (;;) {
		/* try to assign to next cpu */
		CPU_ZERO(&cpuset);
		CPU_SET(next++, &cpuset);

		if ((retval = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset)) != 0) {
			/* if cpu does not exist, start over at 0 */
			if (retval == EINVAL) {
				next = 0;
				continue;
			}

			errno = retval;
			perror("mvm: pthread_setaffinity_np");
			mvm_halt();
		}

		/* retrieve mask */
		if ((retval = pthread_getaffinity_np(thread, sizeof(cpu_set_t), &retset)) != 0) {
			errno = retval;
			perror("mvm: pthread_setaffinity_np");
			mvm_halt();
		}

		/* ensure mask is set correctly, if not start over at 0 */
		if (!CPU_EQUAL(&cpuset, &retset)) {
			next = 0;
			continue;
		}

		/* mask is set properly, break out */
		break;
	}

	errno = 0;
	mvm_print("thread %" PRIu32 ": printing bound CPUs:\n", t->ref);

	for(i = 0; i < CPU_SETSIZE; i++) {
		if (CPU_ISSET(i, &cpuset))
			mvm_print("    CPU %d\n", i);
	}
#endif

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

#ifdef DMP
	if (d != 0 && t->dmp != NULL)
		thread_dmp_thread_join(t->dmp);
#endif

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

	while (usleep(millisec * 1000) != 0) {
		if (errno != EINTR) {
			perror("mvm: usleep");
			mvm_halt();
		}
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

#ifdef DMP
	if (t->dmp != NULL)
		thread_dmp_thread_start(t->dmp);
#endif

	t->state = runnable_state;

	if (invoke_virtual_method_by_name(t, t->ref, t->pc, "run", 0) != 0)
		mvm_halt();

#ifdef DMP
	if (t->dmp != NULL)
		thread_dmp_thread_destruction(t->dmp);
#endif

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
	if (invoke_method(t, "mainBlock", main_block_address, main_block_end,
			  0, main_block_max_locals, 0) != 0) {
		mvm_halt();
	}

#ifdef DMP
	if (t->dmp != NULL)
		thread_dmp_thread_destruction(t->dmp);
#endif

	t->state = terminated_state;
	/* run method has terminated, thread instance can now be collected */
	heap_include_ref(heap, t->ref);
	heap_remove_thread_ref(heap, t->ref);
	pthread_exit(0);
}
