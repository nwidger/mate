/* Niels Widger
 * Time-stamp: <04 Apr 2011 at 18:55:49 by nwidger on macros.local>
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
	created_state   = 0,
	running_state   = 1,
	blocking_state  = 2,
	waiting_state   = 3,
	destroyed_state = 4
};

/* struct definitions */
struct thread_dmp_ops {
	int  (*thread_creation)(struct thread_dmp *td);
	int  (*thread_start)(struct thread_dmp *td);
	int  (*thread_destruction)(struct thread_dmp *td);
	int  (*thread_join)(struct thread_dmp *td);
	int  (*execute_instruction)(struct thread_dmp *td, uint32_t o);
};

struct thread_dmp_attr {
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
int thread_dmp_get_quantum_size(struct thread_dmp *td);
int thread_dmp_set_quantum_size(struct thread_dmp *td, int q);
int thread_dmp_thread_creation(struct thread_dmp *td);
int thread_dmp_thread_start(struct thread_dmp *td);
int thread_dmp_thread_destruction(struct thread_dmp *td);
int thread_dmp_thread_join(struct thread_dmp *td);
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
int thread_dmp_default_execute_instruction(struct thread_dmp *td, uint32_t o);
#endif
