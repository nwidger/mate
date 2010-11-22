/* Niels Widger
 * Time-stamp: <19 Nov 2010 at 21:02:22 by nwidger on macros.local>
 */

#ifndef _MVM_VMSTACK_H
#define _MVM_VMSTACK_H

#include "frame.h"

struct vm_stack;

/** creates a new, empty vm_stack.
 * 
 * @return pointer to new vm_stack, or NULL on failure
 */

struct vm_stack * vm_stack_create();

/** destroys the given vm_stack.
 * 
 * @param s - the vm_stack to destroy
 */

void vm_stack_destroy(struct vm_stack *s);

/** clears the given vm_stack by popping off all frames.
 * 
 * @param s - the vm_stack to clear
 */

void vm_stack_clear(struct vm_stack *s);

/** creates a new frame with the given values and pushes it onto the
 * top of the vm_stack.  The new frame will have a local variable
 * array of size m, and n arguments will be removed from the calling
 * frame and placed into the new frame's local variable array in the
 * proper order.  When the newly created frame is eventually popped
 * using vm_stack_pop, its return address r will be returned.
 * 
 * @param s - the vm_stack to operate on
 * @param e - the name of the method of the new frame as c-string
 * @param n - the number of arguments of the new frame
 * @param m - the max locals count of the new frame
 * @param a - the start address of the new frame
 * @param b - the end address of the new frame
 * @param r - the return address of the new frame
 * 
 * @return pointer to new frame, or NULL on failure
 */

struct frame * vm_stack_push(struct vm_stack *s, char *e, int n, int m,
			     uint32_t a, uint32_t b, uint32_t r);

/** returns the top frame on the given vm_stack, or NULL if empty.
 * 
 * @param s - the vm_stack to peek
 * 
 * @return pointer to top frame, or NULL if empty
 */

struct frame * vm_stack_peek(struct vm_stack *s);

/** pops the top frame off the given vm_stack.
 * 
 * @param s - the vm_stack to pop
 * 
 * @return return address of popped frame,
 * 0 if vm_stack is empty
 */

uint32_t vm_stack_pop(struct vm_stack *s);

/** pops the top n frames off the given vm_stack.
 * 
 * @param s - the vm_stack to pop
 * @param n - the number of frames to pop
 * 
 * @return the return address of the last frame popped,
 * or 0 if vm_stack is empty
 */

uint32_t vm_stack_pop_n(struct vm_stack *s, int n);

/** checks if vm_stack is empty.
 * 
 * @param s - the vm_stack to check
 * 
 * @return 0 if vm_stack is empty, non-zero otherwise
 */

int vm_stack_empty(struct vm_stack *s);

/** returns the number of frames in the vm_stack.
 * 
 * @param s - the vm_stack to check
 * 
 * @return current size of vm_stack
 */

int vm_stack_size(struct vm_stack *s);

/** dumps the contents of the vm_stack.
 *
 * @param s - the vm_stack to dump
 * @param o - if non-zero, dump operand stack of each frame
 * @param l - if non-zero, dump local variable array of each frame
 *
 * @return 0 on success, non-zero on failure.
 */

int vm_stack_dump(struct vm_stack *s, int o, int l);

/** dumps the contents of the vm_stack, starting from frame n.
 *
 * @param s - the vm_stack to dump
 * @param n - the frame to begin from
 * @param o - if non-zero, dump operand stack of each frame
 * @param l - if non-zero, dump local variable array of each frame
 *
 * @return 0 on success, non-zero on failure.
 */
int vm_stack_dump_n(struct vm_stack *s, int n, int o, int l);

/** acquires a lock which prevents other threads from accessing the
 * given vm_stack.  When done, release lock with vm_stack_lock.
 *
 * @param s - the vm_stack to lock
 *
 * @return 0 on success, non-zero on failure.
 */

int vm_stack_lock(struct vm_stack *s);

/** releases the lock acquired by vm_stack_lock, allowing other threads
 * to access the given vm_stack.
 *
 * @param s - the vm_stack to unlock
 *
 * @return 0 on success, non-zero on failure.
 */

int vm_stack_unlock(struct vm_stack *s);

#endif
