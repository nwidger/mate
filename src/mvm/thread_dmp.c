/* Niels Widger
 * Time-stamp: <02 Feb 2011 at 17:15:41 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "constants.h"
#include "globals.h"
#include "heap.h"
#include "thread.h"
#include "thread_dmp.h"

/* struct definitions */
struct thread_dmp {
	struct thread *thread;
	struct thread_dmp_attr attr;
};

struct thread_dmp * thread_dmp_create(struct thread *t, struct thread_dmp_attr *a) {
	struct thread_dmp *td;

	if ((td = (struct thread_dmp *)heap_malloc(heap, sizeof(struct thread_dmp))) == NULL)
		mvm_halt();

	td->thread = t;
	memcpy(&td->attr, a, sizeof(struct thread_dmp_attr));
	return td;
}

void thread_dmp_destroy(struct thread_dmp *td) {
	if (td != NULL) {
		heap_free(heap, td);
	}
}

void thread_dmp_clear(struct thread_dmp *td) {
	if (td != NULL) {
		memset(&td->attr, 0, sizeof(struct thread_dmp_attr));
	}
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

int thread_dmp_round_start(struct thread_dmp *td) {
	if (td == NULL) {
		fprintf(stderr, "mvm: thread_dmp not initialized!\n");
		mvm_halt();
	}

	if (td->attr.ops->round_start == NULL)
		return 0;

	return (*td->attr.ops->round_start)(td);
}

int thread_dmp_pmode_start(struct thread_dmp *td) {
	if (td == NULL) {
		fprintf(stderr, "mvm: thread_dmp not initialized!\n");
		mvm_halt();
	}

	if (td->attr.ops->pmode_start == NULL)
		return 0;

	return (*td->attr.ops->pmode_start)(td);
}

int thread_dmp_pmode_end(struct thread_dmp *td) {
	if (td == NULL) {
		fprintf(stderr, "mvm: thread_dmp not initialized!\n");
		mvm_halt();
	}

	if (td->attr.ops->pmode_end == NULL)
		return 0;

	return (*td->attr.ops->pmode_end)(td);
}

int thread_dmp_smode_start(struct thread_dmp *td) {
	if (td == NULL) {
		fprintf(stderr, "mvm: thread_dmp not initialized!\n");
		mvm_halt();
	}

	if (td->attr.ops->smode_start == NULL)
		return 0;

	return (*td->attr.ops->smode_start)(td);
}

int thread_dmp_smode_end(struct thread_dmp *td) {
	if (td == NULL) {
		fprintf(stderr, "mvm: thread_dmp not initialized!\n");
		mvm_halt();
	}

	if (td->attr.ops->smode_end == NULL)
		return 0;

	return (*td->attr.ops->smode_end)(td);
}

int thread_dmp_round_end(struct thread_dmp *td) {
	if (td == NULL) {
		fprintf(stderr, "mvm: thread_dmp not initialized!\n");
		mvm_halt();
	}

	if (td->attr.ops->round_end == NULL)
		return 0;

	return (*td->attr.ops->round_end)(td);
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

int thread_dmp_thread_destruction(struct thread_dmp *td) {
	if (td == NULL) {
		fprintf(stderr, "mvm: thread_dmp not initialized!\n");
		mvm_halt();
	}

	if (td->attr.ops->thread_destruction == NULL)
		return 0;

	return (*td->attr.ops->thread_destruction)(td);
}

int thread_dmp_execute_instruction(struct thread_dmp *td, uint32_t p, uint32_t o) {
	if (td == NULL) {
		fprintf(stderr, "mvm: thread_dmp not initialized!\n");
		mvm_halt();
	}

	if (td->attr.ops->execute_instruction == NULL)
		return 0;

	return (*td->attr.ops->execute_instruction)(td, p, o);
}

/* default ops */
struct thread_dmp_ops thread_dmp_default_ops = {
	.round_start = thread_dmp_default_round_start,
	.pmode_start = thread_dmp_default_pmode_start,
	.pmode_end = thread_dmp_default_pmode_end,
	.smode_start = thread_dmp_default_smode_start,
	.smode_end = thread_dmp_default_smode_end,
	.round_end = thread_dmp_default_round_end,
	.thread_creation = thread_dmp_default_thread_creation,
	.thread_destruction = thread_dmp_default_thread_destruction,
	.execute_instruction = thread_dmp_default_execute_instruction
};

int thread_dmp_default_round_start(struct thread_dmp *td) {
	fprintf(stderr, "in thread_dmp_default_round_start\n");
	return 0;
}

int thread_dmp_default_pmode_start(struct thread_dmp *td) {
	fprintf(stderr, "in thread_dmp_default_pmode_start\n");
	return 0;
}

int thread_dmp_default_pmode_end(struct thread_dmp *td) {
	fprintf(stderr, "in thread_dmp_default_pmode_end\n");
	return 0;
}

int thread_dmp_default_smode_start(struct thread_dmp *td) {
	fprintf(stderr, "in thread_dmp_default_smode_start\n");
	return 0;
}

int thread_dmp_default_smode_end(struct thread_dmp *td) {
	fprintf(stderr, "in thread_dmp_default_smode_end\n");
	return 0;
}

int thread_dmp_default_round_end(struct thread_dmp *td) {
	fprintf(stderr, "in thread_dmp_default_round_end\n");
	return 0;
}

int thread_dmp_default_thread_creation(struct thread_dmp *td) {
	fprintf(stderr, "in thread_dmp_default_thread_creation\n");
	return 0;
}

int thread_dmp_default_thread_destruction(struct thread_dmp *td) {
	fprintf(stderr, "in thread_dmp_default_thread_destruction\n");
	return 0;
}

int thread_dmp_default_execute_instruction(struct thread_dmp *td, uint32_t p, uint32_t o) {
	fprintf(stderr, "in thread_dmp_default_execute instruction\n");	
	return 0;
}
