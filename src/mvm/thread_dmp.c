/* Niels Widger
 * Time-stamp: <07 Mar 2011 at 17:32:10 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "constants.h"
#include "dmp.h"
#include "globals.h"
#include "heap.h"
#include "thread.h"
#include "thread_dmp.h"

/* struct definitions */
struct thread_dmp {
	struct thread *thread;
	enum thread_dmp_state state;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	struct thread_dmp_attr attr;
};

struct thread_dmp * thread_dmp_create(struct thread *t, struct thread_dmp_attr *a) {
	struct thread_dmp *td;

	if ((td = (struct thread_dmp *)heap_malloc(heap, sizeof(struct thread_dmp))) == NULL)
		mvm_halt();

	td->thread = t;
	memcpy(&td->attr, a, sizeof(struct thread_dmp_attr));

	if (pthread_mutex_init(&td->mutex, NULL) != 0) {
		perror("mvm: pthread_mutex_init");
		mvm_halt();
	}

	if (pthread_cond_init(&td->cond, NULL) != 0) {
		perror("mvm: pthread_cond_init");
		mvm_halt();
	}

	return td;
}

void thread_dmp_destroy(struct thread_dmp *td) {
	if (td != NULL) {
		pthread_mutex_destroy(&td->mutex);
		pthread_cond_destroy(&td->cond);
		heap_free(heap, td);
	}
}

void thread_dmp_clear(struct thread_dmp *td) {
	if (td != NULL) {
		memset(&td->attr, 0, sizeof(struct thread_dmp_attr));
	}
}

enum thread_dmp_state thread_dmp_get_state(struct thread_dmp *td) {
	int state;

	if (td == NULL) {
		fprintf(stderr, "mvm: thread_dmp not initialized!\n");
		mvm_halt();
	}

	pthread_mutex_lock(&td->mutex);
	state = td->state;
	pthread_mutex_unlock(&td->mutex);

	return state;
}

enum thread_dmp_state thread_dmp_get_state_nonblock(struct thread_dmp *td) {
	if (td == NULL) {
		fprintf(stderr, "mvm: thread_dmp not initialized!\n");
		mvm_halt();
	}

	return td->state;
}

int thread_dmp_set_state(struct thread_dmp *td, enum thread_dmp_state s) {
	int state;

	if (td == NULL) {
		fprintf(stderr, "mvm: thread_dmp not initialized!\n");
		mvm_halt();
	}

	pthread_mutex_lock(&td->mutex);
	state = td->state;
	td->state = s;
	pthread_mutex_unlock(&td->mutex);

	return state;
}

int thread_dmp_wait(struct thread_dmp *td) {
	if (td == NULL) {
		fprintf(stderr, "mvm: thread_dmp not initialized!\n");
		mvm_halt();
	}

	pthread_mutex_lock(&td->mutex);

	td->state = waiting_state;

	mvm_print("thread %" PRIu32 ": sleeping until awoken...\n", thread_get_ref());

	while (td->state == waiting_state)
		pthread_cond_wait(&td->cond, &td->mutex);

	mvm_print("thread %" PRIu32 ": has awoken...\n", thread_get_ref());

	pthread_mutex_unlock(&td->mutex);

	return 0;
}

int thread_dmp_signal(struct thread_dmp *td) {
	if (td == NULL) {
		fprintf(stderr, "mvm: thread_dmp not initialized!\n");
		mvm_halt();
	}

	pthread_mutex_lock(&td->mutex);

	if (td->state != waiting_state)
		return 0;

	mvm_print("thread %" PRIu32 ": waking thread %" PRIu32 "...\n", thread_get_ref(), _thread_get_ref(td->thread));

	td->state = running_state;
	pthread_cond_signal(&td->cond);

	pthread_mutex_unlock(&td->mutex);
	
	return 0;
}

struct thread * thread_dmp_get_thread(struct thread_dmp *td) {
	if (td == NULL) {
		fprintf(stderr, "mvm: thread_dmp not initialized!\n");
		mvm_halt();
	}

	return td->thread;
}

int thread_dmp_set_thread(struct thread_dmp *td, struct thread *t) {
	if (td == NULL) {
		fprintf(stderr, "mvm: thread_dmp not initialized!\n");
		mvm_halt();
	}

	td->thread = t;
	return 0;
}

int thread_dmp_get_quantum_size(struct thread_dmp *td) {
	if (td == NULL) {
		fprintf(stderr, "mvm: thread_dmp not initialized!\n");
		mvm_halt();
	}

	return td->attr.quantum_size;
}

int thread_dmp_set_quantum_size(struct thread_dmp *td, int q) {
	if (td == NULL) {
		fprintf(stderr, "mvm: thread_dmp not initialized!\n");
		mvm_halt();
	}

	td->attr.quantum_size = q;
	return 0;
}

int thread_dmp_thread_creation(struct thread_dmp *td) {
	if (td == NULL) {
		fprintf(stderr, "mvm: thread_dmp not initialized!\n");
		mvm_halt();
	}

	if (td->attr.ops->thread_creation == NULL)
		return 0;

	return (*td->attr.ops->thread_creation)(td);
}

int thread_dmp_thread_start(struct thread_dmp *td) {
	if (td == NULL) {
		fprintf(stderr, "mvm: thread_dmp not initialized!\n");
		mvm_halt();
	}

	if (td->attr.ops->thread_start == NULL)
		return 0;

	return (*td->attr.ops->thread_start)(td);
}

int thread_dmp_thread_destruction(struct thread_dmp *td) {
	if (td == NULL) {
		fprintf(stderr, "mvm: thread_dmp not initialized!\n");
		mvm_halt();
	}

	if (td->attr.ops->thread_destruction == NULL)
		return 0;

	return (*td->attr.ops->thread_destruction)(td);
}

int thread_dmp_thread_join(struct thread_dmp *td) {
	if (td == NULL) {
		fprintf(stderr, "mvm: thread_dmp not initialized!\n");
		mvm_halt();
	}

	if (td->attr.ops->thread_join == NULL)
		return 0;

	return (*td->attr.ops->thread_join)(td);
}

int thread_dmp_execute_instruction(struct thread_dmp *td, uint32_t o) {
	if (td == NULL) {
		fprintf(stderr, "mvm: thread_dmp not initialized!\n");
		mvm_halt();
	}

	if (td->attr.ops->execute_instruction == NULL)
		return 0;

	return (*td->attr.ops->execute_instruction)(td, o);
}

/* default ops */
struct thread_dmp_ops thread_dmp_default_ops = {
	.thread_creation = thread_dmp_default_thread_creation,
	.thread_start = thread_dmp_default_thread_start,
	.thread_destruction = thread_dmp_default_thread_destruction,
	.thread_join = thread_dmp_default_thread_join,
	.execute_instruction = thread_dmp_default_execute_instruction
};

/* default attr */
struct thread_dmp_attr thread_dmp_default_attr = {
	.quantum_size = THREAD_DMP_DEFAULT_QUANTUM_SIZE,
	.instruction_counter = THREAD_DMP_DEFAULT_INSTRUCTION_COUNTER,
	.ops = &thread_dmp_default_ops
};

int thread_dmp_default_thread_creation(struct thread_dmp *td) {
	int ref;
	struct thread *thread;

	mvm_print("thread %" PRIu32 ": in thread_dmp_default_thread_creation\n", thread_get_ref());

	thread = td->thread;
	ref = _thread_get_ref(thread);

	if (dmp_get_mode(dmp) == parallel_mode) {
		/* block until serial mode */
		dmp_thread_block(dmp, td);
	}

	/* add to thread set */
	dmp_add_thread(dmp, ref);

	return 0;
}

int thread_dmp_default_thread_start(struct thread_dmp *td) {
	int ref;
	struct thread *thread;

	mvm_print("thread %" PRIu32 ": in thread_dmp_default_thread_start\n", thread_get_ref());

	thread = td->thread;
	ref = thread_get_ref(thread);

	/* block until parallel mode */
	dmp_thread_block(dmp, td);

	return 0;
}

int thread_dmp_default_thread_destruction(struct thread_dmp *td) {
	int ref;
	struct thread *thread;

	mvm_print("thread %" PRIu32 ": in thread_dmp_default_thread_destruction\n", thread_get_ref());

	thread = td->thread;
	ref = thread_get_ref(thread);

	if (dmp_get_mode(dmp) == parallel_mode) {
		/* block until parallel mode */
		dmp_thread_block(dmp, td);
	}

	td->state = destroyed_state;

	/* remove from thread set */
	dmp_remove_thread(dmp, ref);

	return 0;
}

int thread_dmp_default_thread_join(struct thread_dmp *td) {
	mvm_print("thread %" PRIu32 ": in thread_dmp_default_thread_join\n", thread_get_ref());

	while (td->state != destroyed_state)
		dmp_thread_block(dmp, td);

	return 0;
}

int thread_dmp_default_execute_instruction(struct thread_dmp *td, uint32_t o) {
	mvm_print("thread %" PRIu32 ": in thread_dmp_default_execute_instruction\n", thread_get_ref());

	td->attr.instruction_counter++;

	if (td->attr.instruction_counter > td->attr.quantum_size) {
		mvm_print("thread %" PRIu32 ":     quantum reached, blocking!\n", thread_get_ref());

		dmp_thread_block(dmp, td);
		if (dmp_get_mode(dmp) == serial_mode) {
			mvm_print("thread %" PRIu32 ":     quantum finished, blocking!\n", thread_get_ref());
			dmp_thread_block(dmp, td);
		}

		td->attr.instruction_counter = 0;
	}

	return 0;
}
