/* Niels Widger
 * Time-stamp: <25 Jun 2012 at 16:45:16 by nwidger on macros.local>
 */

#ifndef _MVM_MDB_H
#define _MVM_MDB_H

/* enums */
/** indicates to mdb what caused it to run.
 * used by mdb_hook
 * @see mdb_hook
 */
enum mdb_hook {
	/** program has not been started */
	startup_hook			= 0,
	/** program has just entered a new method */
	enter_method_hook		= 1,
	/** program has just left a method */
	leave_method_hook		= 2,
	/** program about to execute an instruction */
	execute_instruction_hook	= 3,
	/** program encountered an error and is about to halt */
	halt_vm_hook			= 4,
	/** program has just terminated */
	terminate_hook			= 5
};

/** initialize the debugger.
 * 
 * @return 0 on success, non-zero on failure
 */

int mdb_initialize();

/** cleanup the debugger.
 *
 * @return 0 on success, non-zero on failure
 */

int mdb_cleanup();

/** give debugger the opportunity to take control of the virtual
 * machine.  Hook h provides the debugger with the specific event that
 * caused it to run.
 *
 * @param h - the hook to run.  Must be one of 'startup_hook'
 * 'enter_method_hook', 'leave_method_hook',
 * 'execute_instruction_hook' 'halt_vm_hook', or 'terminate_hook'
 *
 * @return For 'execute_instruction_hook', the actual opcode to
 * execute or zero if no enabled breakpoint at current instruction.
 * For all other hooks, 0 on success, non-zero on failure.
 * @see mdb_hook
 */

uint32_t mdb_hook(enum mdb_hook h);

/** temporarily uninsert all enabled breakpoints.  This should be
 * followed by a call to mdb_insert_enabled_breakpoints as soon as
 * possible and before the program continues execution.  Used when the
 * method area needs to be accessed without worrying about inserted
 * breakpoints.
 *
 * @return 0 on success, non-zero on failure
 */

int mdb_uninsert_enabled_breakpoints();

/** reinsert all enabled breakpoints.  See
 * mdb_uninsert_enabled_breakpoints description for more information.
 *
 * @return 0 on success, non-zero on failure
 */

int mdb_insert_enabled_breakpoints();

#endif
