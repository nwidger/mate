/* Niels Widger
 * Time-stamp: <18 Feb 2010 at 14:04:10 by nwidger on macros.local>
 */

#ifndef _MVM_OPERANDSTACK_H
#define _MVM_OPERANDSTACK_H

struct operand_stack;

/** creates a new, empty operand_stack.
 * 
 * @return pointer to new operand_stack, NULL on failure
 */

struct operand_stack * operand_stack_create();

/** destroys the given operand_stack.
 * 
 * @param s - the operand_stack to destroy
 */

void operand_stack_destroy(struct operand_stack *s);

/** clears the given operand_stack by popping off all operands.
 * 
 * @param s - the operand_stack to clear
 */

void operand_stack_clear(struct operand_stack *s);

/** pushes reference r onto the given operand_stack.
 * 
 * @param s - the operand_stack to operate on
 * @param r - the reference to push
 * 
 * @return r on success, 0 on failure
 */

int operand_stack_push(struct operand_stack *s, int r);

/** returns the reference on the top of the given operand_stack.
 * 
 * @param s - the operand_stack to peek
 * 
 * @return reference on top of stack, 0 if operand_stack is empty
 */

int operand_stack_peek(struct operand_stack *s);

/** returns the n'th reference from the top of the operand_stack.
 * operand_stack_peek_n(s, 0) is analogous to operand_stack_peek(s).
 * n=1 returns the second reference from the top, n=2 returns the
 * third reference from the top, and so on.
 * 
 * @param s - the operand_stack to peek
 * @param n - the number of elements
 * 
 * @return the requested reference, or 0 if n is >= operand_stack
 * size
 */

int operand_stack_peek_n(struct operand_stack *s, int n);

/** pops the top reference off of the operand_stack and returns it.
 *
 * @param s - the operand_stack to pop
 *
 * @return the popped reference, or 0 is operand_stack is empty
 */

int operand_stack_pop(struct operand_stack *s);

/** pops the top n references off of the operand_stack and returns
 * the last reference popped.
 *
 * @param s - the operand_stack to pop
 * @param n - the number of references to pop
 *
 * @return last popped reference, or 0 if n is > operand_stack
 * size
 */

int operand_stack_pop_n(struct operand_stack *s, int n);

/** checks if operand_stack is empty.
 *
 * @param s - the operand_stack to check
 *
 * @return 0 if operand_stack is empty, non-zero otherwise.
 */

int operand_stack_empty(struct operand_stack *s);

/** returns the size of the operand_stack
 *
 * @param s - the operand_stack to check
 *
 * @return number of references on the operand_stack.
 */

int operand_stack_size(struct operand_stack *s);

/** reverses the first n references on the operand_stack.
 *
 * @param s - the operand_stack to operate on
 * @param n - the number of references to reverse
 *
 * @return 0 on success, non-zero on failure.
 */

int operand_stack_reverse(struct operand_stack *s, int n);

/** dump the reference, class name and toString representation of each
 * element of the given operand_stack on a single line.
 *
 * @param s - the operand_stack to dump
 *
 * @return 0 on success, non-zero on failure.
 */

int operand_stack_dump(struct operand_stack *s);

/** acquires a lock which prevents other threads from accessing the
 * given operand_stack.  When done, release lock with
 * operand_stack_lock.
 *
 * @param s - the operand_stack to lock
 *
 * @return 0 on success, non-zero on failure.
 */

int operand_stack_lock(struct operand_stack *s);

/** releases the lock acquired by operand_stack_lock, allowing other
 * threads to access the given operand_stack.
 *
 * @param s - the operand_stack to unlock
 *
 * @return 0 on success, non-zero on failure.
 */

int operand_stack_unlock(struct operand_stack *s);

#endif
