/* Niels Widger
 * Time-stamp: <18 Feb 2010 at 13:39:46 by nwidger on macros.local>
 */

#ifndef _MVM_BREAKPOINT_H
#define _MVM_BREAKPOINT_H

#include <inttypes.h>

/* enums */

/** breakpoint dispositions.  Specifies action to take when breakpoint
 * is hit.
 */
enum bp_disp {
	/** delete immediately */
	bp_del_disp			= 0,
	/** delete at next stop */
	bp_del_at_next_stop_disp	= 1,
	/** disable immediately */
	bp_disable_disp			= 2,
	/** do nothing */
	bp_donttouch_disp		= 3
};

/** breakpoint enabled state.
 */
enum bp_enable_state {
	/** active, can trigger */
	bp_enabled	= 0,
	/** inactive, cannot trigger */
	bp_disabled	= 1
};

/** breakpoint type.  Indicates whether to trigger using address or
 * method name.
 */
enum bp_type {
	/** trigger on address in class file */
	bp_address	= 0,
	/** trigger on frame method name */
	bp_method	= 1
};

/** breakpoint marks.  OR'd together to list future actions for
 * breakpoint.
 */
enum bp_mark {
	/** has been hit */
	bp_hit		= 0x001,
	/** need to print hit */
	bp_print_hit	= 0x002,
};

/* struct definitions */
struct breakpoint {
	/** number displayed to user */
	int num;

	/** type of this breakpoint.
	 * bp_address is triggered by a specific address
	 * bp_method is triggered by a specific method
	 */
	enum bp_type type;

	/** marks for future actions
	 * OR'ing enum bp_mark values
	 */
	int marks;

	/** is this bp active */
	enum bp_enable_state enabled;

	/** is this bp inserted */
	int inserted;

	/** what to do when this bp is triggered */
	enum bp_disp disposition;

	/** address of bp if type is bp_address, or 0 if type is
	 * bp_method. */
	uint32_t address;

	/** original opcode at address if type is bp_address, or 0 if
	 * type is bp_method
	 */
	uint32_t opcode;

	/** native index of method if type is bp_method, or 0 if type
	 * is bp_address. */
	uint32_t native_index;

	/** method name if type is bp_method, "???" or symbol name if
	 * type is bp_address (malloc'd). */
	char *where;

	/** times to ignore bp before stopping. */
	int ignore_count;

	/** times bp has been hit. */
	int hit_count;

	/** next bp in this chain, or NULL if at end. */
	struct breakpoint *next;
};

/** create a new breakpoint.  If a is non-zero, type will be
 * bp_address, otherwise it will be bp_method.  If type is bp_method,
 * address will be set to 0 and native_index will be set to i.  If
 * type is bp_address, native_index will be set to 0, address will be
 * set to a and opcode will be set to the opcode at address a.  By
 * default, newly created breakpoints are not enabled (enabled is set
 * to bp_disabled) and not inserted.
 *
 * @param n - number associated with breakpoint
 * @param a - address in class file of breakpoint (or zero if native method)
 * @param i - native method index, or 0 if a if non-zero
 * @param w - method name is a is zero or symbol name if a is non-zero
 *
 * @return pointer to new breakpoint, or NULL on failure.
 */

struct breakpoint * breakpoint_create(int n, uint32_t a, uint32_t i, char *w);

/** destroys the given breakpoint.
 *
 * @param b - the breakpoint to destroy
 */

void breakpoint_destroy(struct breakpoint *b);

/** clear the given breakpoint.
 *
 * @param b - the breakpoint to clear
 */

void breakpoint_clear(struct breakpoint *b);

/** checks if a breakpoints is set for the given address or frame.  For
 * breakpoints of type bp_address, this means that address == a.  For
 * breakpoints of type bp_method, this means that where == w.
 *
 * @param b - the breakpoint to check 
 * @param a - the address to check, or zero if in native method
 * @param w - the method of the current frame
 *
 * @return non-zero iff the breakpoint matches, or 0 otherwise.
 */

int breakpoint_is_hit(struct breakpoint *b, uint32_t a, char *w);

/** enables the given breakpoint.  If the breakpoint's address is
 * non-zero, also inserts the breakpoint with a call to
 * breakpoint_insert.  An enabled breakpoint will cause the debugger
 * to stop if reached.
 *
 * @param b - the breakpoint to enable
 *
 * @return 0 on success, non-zero on failure.
 */

int breakpoint_enable(struct breakpoint *b);

/** disables the given breakpoint.  If the breakpoint's address is
 * non-zero, also uninserts the breakpoint with a call to
 * breakpoint_uninsert.  A disabled breakpoint will not cause the
 * debugger to stop if reached.
 *
 * @param b - the breakpoint to disable
 *
 * @return 0 on success, non-zero on failure.
 */

int breakpoint_disable(struct breakpoint *b);

/** inserts the given breakpoint into the class file if the address
 * assocated with the breakpoint is non-zero.
 *
 * @param b - the breakpoint to insert
 *
 * @return 0 on success, non-zero on failure.
 */

int breakpoint_insert(struct breakpoint *b);

/** uninserts the given breakpoint into the class file if the address
 * assocated with the breakpoint is non-zero.
 *
 * @param b - the breakpoint to uninsert
 *
 * @return 0 on success, non-zero on failure.
 */

int breakpoint_uninsert(struct breakpoint *b);

/** sets the specified mark on the given breakpoint.
 *
 * @param b - the breakpoint to mark
 * @param m - the mark to set
 *
 * @return 0 on success, non-zero on failure.
 */

int breakpoint_set_mark(struct breakpoint *b, enum bp_mark m);

/** checks if the specified mark is set for the given breakpoint.
 *
 * @param b - the breakpoint to access
 * @param m - the mark to check
 *
 * @return non-zero if set, or 0 if not set.
 */

int breakpoint_has_mark(struct breakpoint *b, enum bp_mark m);

/** clears the specified mark for the given breakpoint.
 *
 * @param b - the breakpoint to access
 * @param m - the mark to clear
 *
 * @return 0 on success, non-zero on failure.
 */

int breakpoint_clear_mark(struct breakpoint *b, enum bp_mark m);

/** clears all marks for the given breakpoint.
 *
 * @param b - the breakpoint to access
 *
 * @return 0 on success, non-zero on failure.
 */

int breakpoint_clear_all_marks(struct breakpoint *b);

#endif
