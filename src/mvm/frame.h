/* Niels Widger
 * Time-stamp: <19 Feb 2010 at 13:34:08 by nwidger on macros.local>
 */

#ifndef _MVM_FRAME_H
#define _MVM_FRAME_H

#include <inttypes.h>

struct frame;
struct local_variable_array;
struct operand_stack;

/** creates a new frame with an empty operand stack and local variable
 * array.  The local variable array of the new frame will be of size
 * m.  n arguments will be popped off of the calling frame c's operand
 * stack and stored in the new frame's local variable array
 * appropriately.
 *
 * @param e - the name of the invoked method as a c-string 
 * @param n - the number of arguments of the invoked method
 * @param m - the max locals count of the invoked method
 * @param s - the start address of the invoked method
 * @param b - the end address of the invoked method
 * @param r - the return address of the invoked method
 * @param c - pointer to calling frame
 *
 * @return pointer to new frame, or NULL on failure
 */

struct frame * frame_create(char *e, int n, int m, uint32_t s,
			    uint32_t b, uint32_t r, struct frame *c);

/** destroys the given frame
 *
 * @param f - the frame to destroy
 */

void frame_destroy(struct frame *f);

/** clears the given frame by popping all operands off its operand
 * stack and clearing its local variable array of references.
 *
 * @param f - the frame to clear
 *
 */

void frame_clear(struct frame *f);

/** returns a pointer to the operand stack of the given frame.
 *
 * @param f - the frame to access
 *
 * @return pointer to the frame's operand stack
 */

struct operand_stack * frame_get_operand_stack(struct frame *f);

/** returns a pointer to the local variable array of the given frame.
 *
 * @param f - the frame to access
 *
 * @return pointer to the frame's local variable array
 */

struct local_variable_array * frame_get_local_variable_array(struct frame *f);

/** returns a pointer to the given frame's calling frame.
 *
 * @param f - the frame to access
 *
 * @return pointer to the frame's calling frame
 */

struct frame * frame_get_calling_frame(struct frame *f);

/** returns the start address of the given frame.
 *
 * @param f - the frame to access
 *
 * @return the frame's start address
 */

uint32_t frame_get_start_address(struct frame *f);

/** returns the end address of the given frame.
 *
 * @param f - the frame to access
 *
 * @return the frame's end address
 */

uint32_t frame_get_end_address(struct frame *f);

/** returns the current address of the given frame.
 *
 * @param f - the frame to access
 *
 * @return the frame's current address
 */

uint32_t frame_get_current_address(struct frame *f);

/** returns the return address of the given frame.
 *
 * @param f - the frame to access
 *
 * @return the frame's return address
 */

uint32_t frame_get_return_address(struct frame *f);

/** sets the current address of the given frame.
 *
 * @param f - the frame to access
 * @param c - the new current address
 *
 * @return old current address.
 */

int frame_set_current_address(struct frame *f, uint32_t c);

/** returns the method name of the given frame.
 *
 * @param f - the frame to access
 *
 * @return the method name of the frame as a c-string
 */

char * frame_get_method_name(struct frame *f);

/** dumps the contents of the frame.
 *
 * @param f - the frame to access
 * @param o - if non-zero, also dump operand stack
 * @param l - if non-zero, also dump local variable array
 *
 * @return 0 on success, non-zero on failure
 */

int frame_dump(struct frame *f, int o, int l);

/** acquires a lock which prevents other threads from accessing the
 * given frame.  When done, release the lock with frame_lock.
 *
 * @param f - the frame to lock
 *
 * @return 0 on success, non-zero on failure.
 */

int frame_lock(struct frame *f);

/** releases the lock acquired by frame_lock, allowing other threads to
 * access the given frame.
 *
 * @param f - the frame to unlock
 *
 * @return 0 on success, non-zero on failure.
 */

int frame_unlock(struct frame *f);

#endif
