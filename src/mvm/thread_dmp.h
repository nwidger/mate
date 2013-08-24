/* Niels Widger
 * Time-stamp: <22 Aug 2013 at 19:17:47 by nwidger on macros.local>
 */

#ifndef _MVM_THREAD_DMP_H
#define _MVM_THREAD_DMP_H

#include <inttypes.h>

#include "globals.h"
#include "object.h"
#include "thread.h"

struct thread_dmp;

/* enums */
enum thread_dmp_state {
	/* thread created but not yet executing byte-code */
	created_state   = 0,
	/* currently executing byte-code */
	running_state   = 1,
        /* reached end of current mode */
	blocking_state  = 2,
	/* waiting to be awoken during serial mode */
	waiting_state   = 3,
	/* thread done executing, may be joined */
	destroyed_state = 4
};

static inline const char * thread_dmp_state_to_string(enum thread_dmp_state s) {
	static char *thread_dmp_state_strings[] = {
		"created_state",
		"running_state",
		"blocking_state",
		"waiting_state",
		"destroyed_state"
	};
	
	if (s < 0 ||
	    s > (sizeof(thread_dmp_state_strings)/sizeof(const char *))) {
		return "unknown_state";
        }

	return thread_dmp_state_strings[s];
}

enum thread_dmp_serial_mode {
	full_mode    = 0,
	reduced_mode = 1
};

/* struct definitions */
struct thread_dmp_ops {
	int  (*thread_creation)(struct thread_dmp *td);
	int  (*thread_start)(struct thread_dmp *td);
	int  (*thread_destruction)(struct thread_dmp *td);
	int  (*thread_join)(struct thread_dmp *td);
	int  (*thread_sleep)(struct thread_dmp *td, int32_t m);
	int  (*execute_instruction)(struct thread_dmp *td, uint32_t o);
};

struct thread_dmp_attr {
	enum thread_dmp_serial_mode serial_mode;
	int lock_count;
	int quantum_size;
	uint64_t instruction_counter;
	struct thread_dmp_ops *ops;
};

struct thread_dmp * thread_dmp_create(struct thread *t, struct thread_dmp_attr *a);
void thread_dmp_destroy(struct thread_dmp *td);
void thread_dmp_clear(struct thread_dmp *td);
enum thread_dmp_state thread_dmp_get_state(struct thread_dmp *td);
enum thread_dmp_state thread_dmp_get_state_nonblock(struct thread_dmp *td);
int thread_dmp_set_state(struct thread_dmp *td, enum thread_dmp_state s);
int thread_dmp_wait(struct thread_dmp *td);
int thread_dmp_signal(struct thread_dmp *td);
struct thread * thread_dmp_get_thread(struct thread_dmp *td);
int thread_dmp_set_thread(struct thread_dmp *td, struct thread *t);
enum thread_dmp_serial_mode thread_dmp_get_serial_mode(struct thread_dmp *td);
int thread_dmp_incr_lock_count(struct thread_dmp *td);
int thread_dmp_decr_lock_count(struct thread_dmp *td);
int thread_dmp_get_lock_count(struct thread_dmp *td);
int thread_dmp_get_quantum_size(struct thread_dmp *td);
int thread_dmp_set_quantum_size(struct thread_dmp *td, int q);
int thread_dmp_thread_creation(struct thread_dmp *td);
int thread_dmp_thread_start(struct thread_dmp *td);
int thread_dmp_thread_destruction(struct thread_dmp *td);
int thread_dmp_thread_join(struct thread_dmp *td);
int thread_dmp_thread_sleep(struct thread_dmp *td, int32_t m);
int thread_dmp_execute_instruction(struct thread_dmp *td, uint32_t o);

/* default ops */
extern struct thread_dmp_ops thread_dmp_default_ops;

/* default attr */
extern struct thread_dmp_attr thread_dmp_default_attr;

/* default functions */
int thread_dmp_default_thread_creation(struct thread_dmp *td);
int thread_dmp_default_thread_start(struct thread_dmp *td);
int thread_dmp_default_thread_destruction(struct thread_dmp *td);
int thread_dmp_default_thread_join(struct thread_dmp *td);
int thread_dmp_default_thread_sleep(struct thread_dmp *td, int32_t m);
int thread_dmp_default_execute_instruction(struct thread_dmp *td, uint32_t o);
#endif
