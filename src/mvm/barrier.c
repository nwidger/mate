/* Niels Widger
 * Time-stamp: <19 Dec 2010 at 21:59:55 by nwidger on macros.local>
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
};

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

	return b;
}

void barrier_destroy(struct barrier *b) {
	if (b != NULL) {
		if (pthread_cond_destroy(&b->cond) != 0) {
			perror("mvm: pthread_cond_destroy");
			mvm_halt();
		}
		if (pthread_mutex_destroy(&b->mutex) != 0) {
			perror("mvm: pthread_mutex_destroy");
			mvm_halt();
		}
	}
}

void barrier_clear(struct barrier *b) {
	if (b != NULL) {
		b->generation = 0;
		b->waiting = 0;
	}
}

int barrier_await(struct barrier *b) {
	int i;
	uint64_t g;

	if (b == NULL) {
		fprintf(stderr, "mvm: barrier not initialized!\n");
		mvm_halt();
	}

	pthread_mutex_lock(&b->mutex);

	i = b->parties - ++b->waiting;

	if (i == 0) {
		b->generation++;
		b->waiting = 0;
		pthread_cond_broadcast(&b->cond);
	} else {
		g = b->generation;
		while (g == b->generation);
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

	b->waiting = 0;
}

int barrier_get_waiting(struct barrier *b) {
	if (b == NULL) {
		fprintf(stderr, "mvm: barrier not initialized!\n");
		mvm_halt();
	}

	return b->waiting;
}
