/* Niels Widger
 * Time-stamp: <07 Mar 2011 at 11:57:27 by nwidger on macros.local>
 */

#ifndef _MVM_THREAD_H
#define _MVM_THREAD_H

struct object;
struct thread;
struct thread_dmp;

/* enum */
/** specifies current state of the thread */
enum thread_state {
	/** the thread has not yet started */
	new_state		= 0,
	/** the thread is executing */
	runnable_state		= 1,
	/** the thread is blocked waiting for a monitor */
	blocked_state		= 2,
	/** the thread has exited */
	terminated_state	= 5
};

#endif

struct thread * thread_create();

void thread_destroy(struct thread *t);

void thread_clear(struct thread *t);

struct thread_dmp * thread_get_dmp();

struct thread_dmp * _thread_get_dmp(struct thread *t);

struct thread * thread_get_current();

int thread_get_ref();

int _thread_get_ref(struct thread *t);

struct vm_stack * thread_get_vm_stack();

struct vm_stack * _thread_get_vm_stack(struct thread *t);

int thread_get_state();

int _thread_get_state(struct thread *t);

uint32_t thread_get_pc();

uint32_t _thread_get_pc(struct thread *t);

uint32_t thread_set_pc(uint32_t p);

int thread_start(struct object *o);

int thread_start_main(struct object *o);

int thread_join(struct object *o);

int thread_join_main(struct object *o);

int thread_sleep(struct object *o, struct object *p);

int thread_run(struct object *o);
