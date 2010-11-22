/* Niels Widger
 * Time-stamp: <19 Nov 2010 at 21:08:16 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "constants.h"
#include "frame.h"
#include "garbage_collector.h"
#include "globals.h"
#include "heap.h"
#include "local_variable_array.h"
#include "nlock.h"
#include "object.h"
#include "operand_stack.h"
#include "ref_set.h"
#include "vm_stack.h"

/* struct definitions */
struct garbage_collector {
	int type;
	int interval;
	pthread_t threadid;

	struct nlock *nlock;

	int is_running;
	pthread_mutex_t running_mutex;
	pthread_cond_t running_cond;

	int is_collecting;
	pthread_mutex_t collecting_mutex;
	pthread_cond_t collecting_cond;

	struct ref_set *white;
	struct ref_set *grey;
	struct ref_set *black;
};

/* forward declarations */
void * garbage_collector_run(void *a);
int tricolor_garbage_collector(struct garbage_collector *g);
int move_ref_to(struct garbage_collector *g, struct ref_set *s, int r);

struct garbage_collector * garbage_collector_create() {
	struct garbage_collector *g;

	if ((g = (struct garbage_collector *)malloc(sizeof(struct garbage_collector))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	g->type = GARBAGE_COLLECTOR_DEFAULT_TYPE;
	g->interval = GARBAGE_COLLECTOR_DEFAULT_INTERVAL;
	g->threadid = 0;

	if ((g->nlock = nlock_create()) == NULL)
		mvm_halt();
	
	g->is_running = 0;

	if (pthread_cond_init(&g->running_cond, NULL) != 0) {
		perror("mvm: pthread_cond_init");
		mvm_halt();
	}

	if (pthread_mutex_init(&g->running_mutex, NULL) != 0) {
		perror("mvm: pthread_mutex_init");
		mvm_halt();
	}

	g->is_collecting = 0;

	if (pthread_cond_init(&g->collecting_cond, NULL) != 0) {
		perror("mvm: pthread_cond_init");
		mvm_halt();
	}

	if (pthread_mutex_init(&g->collecting_mutex, NULL) != 0) {
		perror("mvm: pthread_mutex_init");
		mvm_halt();
	}

	g->white = NULL;
	g->grey = NULL;
	g->black = NULL;

	return g;
}

void garbage_collector_destroy(struct garbage_collector *g) {
	if (g != NULL) {
		garbage_collector_clear(g);

		nlock_destroy(g->nlock);
		
		if (pthread_cond_destroy(&g->running_cond) != 0) {
			perror("mvm: pthread_cond_destroy");
			mvm_halt();
		}
		if (pthread_mutex_destroy(&g->running_mutex) != 0) {
			perror("mvm: pthread_mutex_destroy");
			mvm_halt();
		}
		if (pthread_cond_destroy(&g->collecting_cond) != 0) {
			perror("mvm: pthread_cond_destroy");
			mvm_halt();
		}
		if (pthread_mutex_destroy(&g->collecting_mutex) != 0) {
			perror("mvm: pthread_mutex_destroy");
			mvm_halt();
		}

		free(g);
	}
}

void garbage_collector_clear(struct garbage_collector *g) {
	if (g != NULL) {
		g->type = 0;
		g->interval = 0;
		g->threadid = 0;

		g->is_running = 0;
		g->is_collecting = 0;
	}
}

int garbage_collector_is_running(struct garbage_collector *g) {
	if (g == NULL) {
		fprintf(stderr, "mvm: garbage collector has not been initialized!\n");
		mvm_halt();
	}

	return g->is_running;
}

int garbage_collector_start(struct garbage_collector *g,
			    enum garbage_collector_type t, int i) {
	if (g == NULL) {
		fprintf(stderr, "mvm: garbage collector has not been initialized!\n");
		mvm_halt();
	}

	if (g->is_running == 1)
		return 0;

	g->type = t;
	g->interval = (g->type == concurrent_type) ? i : 0;

	g->is_running = 1;

	if (g->type == concurrent_type) {
		if (pthread_create(&g->threadid, NULL,
				   garbage_collector_run, (void *)g) != 0) {
			perror("mvm: pthread_create");
			mvm_halt();
		}
	}

	return 0;
}

int garbage_collector_stop(struct garbage_collector *g) {
	long retval;

	if (g == NULL) {
		fprintf(stderr, "mvm: garbage collector has not been initialized!\n");
		mvm_halt();
	}

	if (g->is_running == 0)
		return 0;

	g->is_running = 0;

	retval = 0;
	if (g->type == concurrent_type) {
		if (pthread_cond_broadcast(&g->running_cond) != 0) {
			perror("mvm: pthread_cond_broadcast");
			mvm_halt();
		}

		if (pthread_join(g->threadid, (void **)&retval) != 0) {
			perror("mvm: pthread_join");
			mvm_halt();
		}
	}

	return retval;
}

void * garbage_collector_run(void *a) {
	long retval;
	struct timespec ts;
	struct garbage_collector *g;

	g = (struct garbage_collector *)a;
	if (g == NULL) {
		fprintf(stderr, "mvm: garbage collector has not been initialized!\n");
		mvm_halt();
	}

	retval = 0;
	while (g->is_running == 1 && retval == 0) {
		pthread_mutex_lock(&g->running_mutex);
		ts.tv_sec = time(NULL) + g->interval;
		while (g->is_running == 1 && g->is_collecting == 0 &&
		       time(NULL) <= ts.tv_sec) {
			pthread_cond_timedwait(&g->running_cond,
					       &g->running_mutex, &ts);
		}
		pthread_mutex_unlock(&g->running_mutex);

		if (g->is_running == 0)
			break;

		pthread_mutex_lock(&g->collecting_mutex);
		g->is_collecting = 1;
		pthread_mutex_unlock(&g->collecting_mutex);

		retval = tricolor_garbage_collector(g);

		pthread_mutex_lock(&g->collecting_mutex);
		g->is_collecting = 0;
		pthread_mutex_unlock(&g->collecting_mutex);

		if (pthread_cond_broadcast(&g->collecting_cond) != 0) {
			perror("mvm: pthread_cond_broadcast");
			mvm_halt();
		}
	}

	pthread_exit(&retval);
	return (void *)retval;
}

int garbage_collector_collect_now(struct garbage_collector *g) {
	if (g == NULL) {
		fprintf(stderr, "mvm: garbage collector has not been initialized!\n");
		mvm_halt();
	}

	if (g->is_running == 0) {
		fprintf(stderr, "mvm: garbage collector is not running!\n");
		mvm_halt();
	}

	pthread_mutex_lock(&g->collecting_mutex);
	g->is_collecting = 1;
	pthread_mutex_unlock(&g->collecting_mutex);

	if (g->type == serial_type) {
		tricolor_garbage_collector(g);
	} else {
		if (pthread_cond_broadcast(&g->running_cond) != 0) {
			perror("mvm: pthread_cond_broadcast");
			mvm_halt();
		}

		pthread_mutex_lock(&g->collecting_mutex);
		while (g->is_running == 1 && g->is_collecting == 1) {
			pthread_cond_wait(&g->collecting_cond, &g->collecting_mutex);
		}
		pthread_mutex_unlock(&g->collecting_mutex);
	}

	return 0;
}

int tricolor_garbage_collector(struct garbage_collector *g) {
	int i, size, object_ref, ref;
	struct vm_stack *vm_stack;
	struct frame *frame;
	struct operand_stack *operand_stack;
	struct object *object;
	struct local_variable_array *local_variable_array;
	struct ref_set *tmp;

	vm_stack = NULL;

	mvm_print("garbage_collector: collection started\n");

	g->white = ref_set_create();
	g->grey = ref_set_create();
	g->black = ref_set_create();

	/* lock */
	garbage_collector_lock(g);

	/* add all references in heap to white set */
	heap_populate_ref_set(heap, g->white);

	vm_stack_lock(vm_stack);

	/* move root set to grey set */
	for (frame = vm_stack_peek(vm_stack); frame != NULL; frame = frame_get_calling_frame(frame)) {
		frame_lock(frame);

		operand_stack = frame_get_operand_stack(frame);
		operand_stack_lock(operand_stack);

		size = operand_stack_size(operand_stack);
		for (i = 0; i < size; i++) {
			ref = operand_stack_peek_n(operand_stack, i);
			if (ref != 0)
				move_ref_to(g, g->grey, ref);
		}

		operand_stack_unlock(operand_stack);

		local_variable_array = frame_get_local_variable_array(frame);
		local_variable_array_lock(local_variable_array);

		size = local_variable_array_size(local_variable_array);
		for (i = 0; i < size; i++) {
			ref = local_variable_array_load(local_variable_array, i);
			if (ref != 0)
				move_ref_to(g, g->grey, ref);
		}

		local_variable_array_unlock(local_variable_array);

		frame_unlock(frame);
	}

	vm_stack_unlock(vm_stack);

	/* unlock */
	garbage_collector_unlock(g);

	/* blacken grey references until grey set is empty */
	while (ref_set_size(g->grey) > 0) {
		ref_set_iterator_init(g->grey);
		ref = ref_set_iterator_next(g->grey);
		object = heap_fetch_object(heap, ref);

		/* grey all references inside object */
		tmp = ref_set_create();
		object_populate_ref_set(object, tmp);

		ref_set_iterator_init(tmp);
		while ((object_ref = ref_set_iterator_next(tmp)) != 0) {
			if (object_ref != 0 && ref_set_contains(g->white, object_ref))
				move_ref_to(g, g->grey, object_ref);
		}

		ref_set_destroy(tmp);
		move_ref_to(g, g->black, ref);
	}

	ref_set_iterator_init(g->white);
	while ((ref = ref_set_iterator_next(g->white)) != 0) {
		object = heap_fetch_object(heap, ref);
		object_destroy(object);
	}

	ref_set_destroy(g->white);
	ref_set_destroy(g->grey);
	ref_set_destroy(g->black);

	mvm_print("garbage_collector: collection ended\n");

	return 0;
}

int move_ref_to(struct garbage_collector *g, struct ref_set *s, int r) {
	if (s != g->white)
		ref_set_remove(g->white, r);
	if (s != g->grey)
		ref_set_remove(g->grey, r);
	if (s != g->black)
		ref_set_remove(g->black, r);

	ref_set_add(s, r);
	return 0;
}

int garbage_collector_dump(struct garbage_collector *g) {
	if (g == NULL) {
		fprintf(stderr, "mvm: garbage collector not initialized!\n");
		mvm_halt();
	}

	if (g->type == serial_type) {
		fprintf(stderr, "%-16s%s\n", "type", "serial");
	} else {
		fprintf(stderr, "%-16s%s\n", "type", "concurrent");
		fprintf(stderr, "%-16s%d\n", "interval", g->interval);
	}

	fprintf(stderr, "%-16s%s\n", "running",
		(g->is_running == 0) ? "no" : "yes");
	fprintf(stderr, "%-16s%s\n", "collecting",
		(g->is_collecting == 0) ? "no" : "yes");

	return 0;
}

int garbage_collector_lock(struct garbage_collector *g) {
	if (g == NULL) {
		fprintf(stderr, "mvm: garbage collector not initialized!\n");
		mvm_halt();
	}

	/* lock */
	nlock_lock(g->nlock);

	return 0;
}

int garbage_collector_unlock(struct garbage_collector *g) {
	if (g == NULL) {
		fprintf(stderr, "mvm: garbage collector not initialized!\n");
		mvm_halt();
	}

	/* unlock */
	nlock_unlock(g->nlock);

	return 0;
}

int garbage_collector_release(struct garbage_collector *g) {
	if (g == NULL) {
		fprintf(stderr, "mvm: garbage collector not initialized!\n");
		mvm_halt();
	}

	return nlock_release(g->nlock);
}

int garbage_collector_reacquire(struct garbage_collector *g, int l) {
	if (g == NULL) {
		fprintf(stderr, "mvm: garbage collector not initialized!\n");
		mvm_halt();
	}

	return nlock_reacquire(g->nlock, l);
}
