/* Niels Widger
 * Time-stamp: <27 Jan 2011 at 19:45:15 by nwidger on macros.local>
 */

#ifndef _MVM_GLOBALS_H
#define _MVM_GLOBALS_H

#include <inttypes.h>
#include <pthread.h>
#include <stdarg.h>

/* forward declarations */
struct class_table;
struct garbage_collector;
struct heap;
struct instruction_table;
struct method_area;
struct native_method_array;
struct symbol_table;
struct vm_stack;

/** called when mvm encounters a serious error.  If debugging, enter
 * debugger prompt.  If not debugger, terminate the program, possibly
 * with a stack trace is print_trace != 0.
 */

void mvm_halt();

/** print debug message if verbose != 0
 *
 * @param f - fprintf format string
 * @param ... - 0 or more arguments to format string
 */

void mvm_print(const char *f, ...);

/** return new malloc'd copy of s.
 * @param s - the string to copy
 *
 * @return new pointer to malloc'd c-string, or NULL on failure
 */

char * mvm_strdup(const char *s);

/** cleans up mvm.  Must be called before terminating.
 *
 * @return 0 on success, non-zero on failure
 */

int mvm_cleanup();

/** clears the heap and vm_stack, resets the garbage_collector so that
 * program can be rerun.  If input is a regular file, it will be
 * rewound.  If output is a regular file, it will be truncated to
 * length 0.
 */

void mvm_clear();

/* globals */

/** input stream used by in instruction, default is stdin */
extern FILE *input;
/** output stream used by out instruction, default is stdout */
extern FILE *output;
/** currently loaded class file, or NULL if none */
extern char *class_file;
/** currently loaded symbol file, or NULL if none */
extern char *symbol_file;
extern struct barrier *pbarrier;
extern struct barrier *sbarrier;
extern struct class_table *class_table;
extern struct garbage_collector *garbage_collector;
extern struct heap *heap;
extern struct instruction_table *instruction_table;
extern struct method_area *method_area;
extern struct native_method_array *native_method_array;
extern struct symbol_table *symbol_table;
pthread_key_t key;
/** instruction pointer */
/* extern uint32_t pc; */
extern uint32_t main_block_address;
extern uint32_t main_block_end;
extern uint32_t main_block_max_locals;
extern int32_t main_block_return_value;
/** if non-zero, stack trace printed in mvm_halt.
 * @see mvm_halt
 */
extern int print_trace;
extern int verbose;
extern int debug;
/* set by debugger to signal a program restart */
extern int restart;

#endif
