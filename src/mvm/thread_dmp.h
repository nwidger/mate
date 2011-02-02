/* Niels Widger
 * Time-stamp: <02 Feb 2011 at 13:35:49 by nwidger on macros.local>
 */

#ifndef _MVM_THREAD_DMP_H
#define _MVM_THREAD_DMP_H

#include <inttypes.h>

#include "globals.h"
#include "object.h"
#include "thread.h"

struct thread_dmp;

/* struct definitions */
struct thread_dmp_ops {
	int  (*round_start)(struct thread_dmp *td);
	int  (*pmode_start)(struct thread_dmp *td);
	int  (*pmode_end)(struct thread_dmp *td);
	int  (*smode_start)(struct thread_dmp *td);
	int  (*smode_end)(struct thread_dmp *td);
	int  (*round_end)(struct thread_dmp *td);
	int  (*thread_creation)(struct thread_dmp *td);
	int  (*thread_destruction)(struct thread_dmp *td);
	int  (*execute_instruction)(struct thread_dmp *td, uint32_t p, uint32_t o);
};

struct thread_dmp_attr {
	int quantum_size;
	uint64_t instruction_counter;
	struct thread_dmp_ops *ops;
};

struct thread_dmp * thread_dmp_create(struct thread *t, struct thread_dmp_attr *a);
void thread_dmp_destroy(struct thread_dmp *td);
void thread_dmp_clear(struct thread_dmp *td);
struct thread * thread_dmp_get_thread(struct thread_dmp *td);
int thread_dmp_set_thread(struct thread_dmp *td, struct thread *t);
int thread_dmp_get_quantum_size(struct thread_dmp *td);
int thread_dmp_set_quantum_size(struct thread_dmp *td, int q);
int thread_dmp_round_start(struct thread_dmp *td);
int thread_dmp_pmode_start(struct thread_dmp *td);
int thread_dmp_pmode_end(struct thread_dmp *td);
int thread_dmp_smode_start(struct thread_dmp *td);
int thread_dmp_smode_end(struct thread_dmp *td);
int thread_dmp_round_end(struct thread_dmp *td);
int thread_dmp_thread_creation(struct thread_dmp *td);
int thread_dmp_thread_destruction(struct thread_dmp *td);
int thread_dmp_execute_instruction(struct thread_dmp *td, uint32_t p, uint32_t o);

/* default ops */
extern struct thread_dmp_ops thread_dmp_default_ops;

int thread_dmp_default_round_start(struct thread_dmp *td);
int thread_dmp_default_pmode_start(struct thread_dmp *td);
int thread_dmp_default_pmode_end(struct thread_dmp *td);
int thread_dmp_default_smode_start(struct thread_dmp *td);
int thread_dmp_default_smode_end(struct thread_dmp *td);
int thread_dmp_default_round_end(struct thread_dmp *td);
int thread_dmp_default_thread_creation(struct thread_dmp *td);
int thread_dmp_default_thread_destruction(struct thread_dmp *td);
int thread_dmp_default_execute_instruction(struct thread_dmp *td, uint32_t p, uint32_t o);
#endif
