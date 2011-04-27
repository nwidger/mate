/* Niels Widger
 * Time-stamp: <06 Apr 2011 at 20:50:29 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "barrier.h"
#include "globals.h"

/* struct definitions */
struct barrier {
	uint64_t generation;
	int parties;
	int waiting;
	pthread_mutex_t mutex;
	pthread_cond_t cond;

	void * (*hook)(int, void *);
	void *hook_arg;
};

/* forward declarations */
int barrier_broadcast(struct barrier *b);

struct barrier * barrier_create(int p) {
	struct barrier *b;

	if ((b = (struct barrier *)malloc(sizeof(struct barrier))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	b->generation = 0;
	b->parties = p;
	b->waiting = 0;

	if (pthread_cond_init(&b->cond, NULL) != 0) {
		perror("mvm: pthread_cond_init");
		mvm_halt();
	}

	if (pthread_mutex_init(&b->mutex, NULL) != 0) {
		perror("mvm: pthread_mutex_init");
		mvm_halt();
	}

	b->hook = NULL;
	b->hook_arg = NULL;

	return b;
}

void barrier_destroy(struct barrier *b) {
	if (b != NULL) {
		if (pthread_cond_destroy(&b->cond) != 0) {
			perror("mvm: pthread_cond_destroy");
			mvm_halt();
		}

		pthread_mutex_destroy(&b->mutex);
	}
}

void barrier_clear(struct barrier *b) {
	if (b != NULL) {
		b->generation = 0;
		b->waiting = 0;
	}
}

int barrier_set_hook(struct barrier *b, void * (*h)(int, void *), void *a) {
	if (b == NULL) {
		fprintf(stderr, "mvm: barrier not initialized!\n");
		mvm_halt();
	}

	b->hook = h;
	b->hook_arg = a;

	return 0;
}

int barrier_clear_hook(struct barrier *b) {
	if (b == NULL) {
		fprintf(stderr, "mvm: barrier not initialized!\n");
		mvm_halt();
	}

	b->hook = NULL;
	b->hook_arg = NULL;

	return 0;
}

int barrier_await(struct barrier *b) {
	int i;
	uint64_t g;

	if (b == NULL) {
		fprintf(stderr, "mvm: barrier not initialized!\n");
		mvm_halt();
	}

	pthread_mutex_lock(&b->mutex);

	if (b->parties == 0)
		return 0;

	i = b->parties - ++b->waiting;

	if (i == 0) {
		barrier_broadcast(b);
	} else {
		g = b->generation;
		while (g == b->generation)
			pthread_cond_wait(&b->cond, &b->mutex);
	}

	pthread_mutex_unlock(&b->mutex);

	return i;
}

void barrier_reset(struct barrier *b) {
	if (b == NULL) {
		fprintf(stderr, "mvm: barrier not initialized!\n");
		mvm_halt();
	}

	barrier_reset_parties(b, b->parties);
}

void barrier_reset_parties(struct barrier *b, int p) {
	if (b == NULL) {
		fprintf(stderr, "mvm: barrier not initialized!\n");
		mvm_halt();
	}

	b->waiting = 0;
	b->parties = p;
}

int barrier_get_waiting(struct barrier *b) {
	if (b == NULL) {
		fprintf(stderr, "mvm: barrier not initialized!\n");
		mvm_halt();
	}

	return b->waiting;
}

int barrier_get_parties(struct barrier *b) {
	if (b == NULL) {
		fprintf(stderr, "mvm: barrier not initialized!\n");
		mvm_halt();
	}

	return b->parties;
}

int barrier_inc_parties(struct barrier *b) {
	if (b == NULL) {
		fprintf(stderr, "mvm: barrier not initialized!\n");
		mvm_halt();
	}

	pthread_mutex_lock(&b->mutex);
	b->parties++;
	pthread_mutex_unlock(&b->mutex);

	return 0;
}

int barrier_dec_parties(struct barrier *b) {
	int i;

	if (b == NULL) {
		fprintf(stderr, "mvm: barrier not initialized!\n");
		mvm_halt();
	}

	pthread_mutex_lock(&b->mutex);

	i = --b->parties - b->waiting;
	
	if (i == 0) {
		mvm_print("removing thread caused barrier to broadcast!\n");
		barrier_broadcast(b);
	}

	pthread_mutex_unlock(&b->mutex);

	return 0;
}

/* caller must have acquired b->mutex! */
int barrier_broadcast(struct barrier *b) {
	if (b == NULL) {
		fprintf(stderr, "mvm: barrier not initialized!\n");
		mvm_halt();
	}

	if (b->hook != NULL) (*b->hook)(0, b->hook_arg);

	b->generation++;
	b->waiting = 0;
	pthread_cond_broadcast(&b->cond);

	return 0;
}
