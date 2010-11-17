/* Niels Widger
 * Time-stamp: <17 Nov 2010 at 10:54:53 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "globals.h"
#include "nlock.h"

/* struct definitions */
struct nlock {
	int locks;
	pthread_t owner;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	uint64_t notifies;
};

struct nlock * nlock_create() {
	struct nlock *n;
	pthread_mutexattr_t attr;

	if ((n = (struct nlock *)malloc(sizeof(struct nlock))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	n->locks = 0;

	if (pthread_mutexattr_init(&attr) != 0) {
		perror("mvm: pthread_mutexattr_init");
		mvm_halt();
	}

	if (pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE) != 0) {
		perror("mvm: pthread_mutexattr_settype");
		mvm_halt();
	}

	if (pthread_mutex_init(&n->mutex, &attr) != 0) {
		perror("mvm: pthread_mutex_init");
		mvm_halt();
	}

	if (pthread_mutexattr_destroy(&attr) != 0) {
		perror("mvm: pthread_mutexattr_destroy");
		mvm_halt();
	}

	if (pthread_cond_init(&n->cond, NULL) != 0) {
		perror("mvm: pthread_cond_init");
		mvm_halt();
	}

	n->notifies = 0UL;

	return n;
}

void nlock_destroy(struct nlock *n) {
	if (n != NULL) {
		pthread_mutex_destroy(&n->mutex);
		pthread_cond_destroy(&n->cond);
		free(n);
	}
}

void nlock_clear(struct nlock *n) {
	if (n != NULL) {
	}
}

int nlock_lock(struct nlock *n) {
	int err = 0;

	if (n == NULL) {
		fprintf(stderr, "mvm: nlock has not been initialized!\n");
		mvm_halt();
	}

	/* lock */
	if ((err = pthread_mutex_lock(&n->mutex)) != 0) {
		fprintf(stderr, "mvm: pthread_mutex_lock: %s\n", strerror(err));
		mvm_halt();
	}

	if (n->locks == 0)
		n->owner = pthread_self();

	n->locks++;
	
	return 0;
}

int nlock_unlock(struct nlock *n) {
	int err = 0;

	if (n == NULL) {
		fprintf(stderr, "mvm: nlock has not been initialized!\n");
		mvm_halt();
	}

	/* unlock */
	if ((err = pthread_mutex_unlock(&n->mutex)) != 0) {
		fprintf(stderr, "mvm: pthread_mutex_unlock: %s\n", strerror(err));
		mvm_halt();
	}

	n->locks--;
	return 0;
}

int nlock_release(struct nlock *n) {
	int locks;

	if (n == NULL) {
		fprintf(stderr, "mvm: nlock has not been initialized!\n");
		mvm_halt();
	}

	locks = n->locks;

	while (n->locks > 0)
		nlock_unlock(n);

	return locks;
}

int nlock_reacquire(struct nlock *n, int l) {
	if (n == NULL) {
		fprintf(stderr, "mvm: nlock has not been initialized!\n");
		mvm_halt();
	}

	while (n->locks < l)
		nlock_lock(n);

	return 0;
}

int nlock_wait(struct nlock *n) {
	if (n->locks == 0 || pthread_equal(pthread_self(), n->owner) == 0) {
		fprintf(stderr, "mvm: nlock_wait called but thread does not own mutex!\n");
		mvm_halt();
	}
	
	return nlock_timedwait(n, 0L);
}

int nlock_timedwait(struct nlock *n, long t) {
	uint64_t notifies;
	struct timespec abstime;

	if (n->locks == 0 || pthread_equal(pthread_self(), n->owner) == 0) {
		fprintf(stderr, "mvm: nlock_timedwait called but thread does not own mutex!\n");
		mvm_halt();
	}

	abstime.tv_sec = time(NULL);
	abstime.tv_nsec = t * 1000000;
	
	notifies = n->notifies;
	
	while (notifies == n->notifies) {
		/* unlock */
		n->locks--;
		
		if (t == 0L)
			pthread_cond_wait(&n->cond, &n->mutex);
		else
			pthread_cond_timedwait(&n->cond, &n->mutex, &abstime);
		
		/* lock */
		n->locks++;
	}

	return 0;
}

int nlock_notify(struct nlock *n) {
	if (n->locks == 0 || pthread_equal(pthread_self(), n->owner) == 0) {
		fprintf(stderr, "mvm: nlock_notify called but thread does not own mutex!\n");
		mvm_halt();
	}
	
	pthread_cond_signal(&n->cond);
	n->notifies++;

	return 0;
}

int nlock_notify_all(struct nlock *n) {
	if (n->locks == 0 || pthread_equal(pthread_self(), n->owner) == 0) {
		fprintf(stderr, "mvm: nlock_notify_all called but thread does not own mutex!\n");
		mvm_halt();
	}
	
	pthread_cond_broadcast(&n->cond);
	n->notifies++;	

	return 0;
}
