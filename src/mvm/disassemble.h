/* Niels Widger
 * Time-stamp: <18 Feb 2010 at 13:34:57 by nwidger on macros.local>
 */

#include <inttypes.h>
#include <stdarg.h>

/** returns the nth argument of the instruction at address a.  The
 * first argument is at index 0.
 *
 * @param a - the address of the instruction
 * @param n - the number of the instruction to return
 *
 * @returns nth argument of instruction
 */

uint32_t mvm_disassemble_argument(uint32_t a, int n);

/** stores the first n arguments of the instruction at address a.  The
 * caller must provide n int pointers, into which each argument shall
 * be saved.
 *
 * @param a - the address of the instruction
 * @param n - the number of arguments to fetch
 * @param ... - n int pointers into which the values shall be stored
 *
 * @return 0 on success, non-zero on failure
 */

int mvm_disassemble_arguments(uint32_t a, int n, ...);

/** returns a malloc'd c-string representing the first and only
 * argument to the instruction at address a.
 *
 * @param a - the address of the instruction
 *
 * @return pointer to malloc'd c-string.
 */

char * mvm_disassemble_string_argument(uint32_t a);

/** Disassemble the current class file, including the class table.
 *
 * @param p - if non-zero, print address in front of each instruction    
 *
 * @return 0 on success, non-zero on failure
 */

int mvm_disassemble_class_file(int p);

/** Disassemble the given region of the class file from s to e and
 * output it to standard error.  Address b must be after the class
 * table.  Function will stop before decoding the first instruction
 * with a start address greater than e.
 *
 * @param p - if non-zero, print address in front of each instruction 
 * @param s - address to begin decoding
 * @param e - address to end decoding
 *
 * @return 0 on success, non-zero on failure
 */

int mvm_disassemble_region(int p, uint32_t s, uint32_t e);

/** disassembles the instruction at address a to stderr.
 *
 * @param p - if non-zero, print address in front of each instruction  
 * @param a - the address of the instruction to decode
 *
 * @return size of decoded instruction in words
 */

int mvm_disassemble_instruction(int p, uint32_t a);

/** returns the address of the last instruction of the
 * method/constructor at address a.
 *
 * @param a - the beginning of the method/constructor
 *
 * @return address of last instruction, or 0 on failure
 */

uint32_t mvm_disassemble_method_end(uint32_t a);

/** returns a null-terminated array of c-strings containing all valid
 * addresses in the current class file.  A valid address is one at
 * which an instruction begins and must occur after the class table.
 * Array will be sorted numerically by address.  Both the array and
 * each string are allocated by malloc, and must be free'd when no
 * longer needed.
 *
 * @return pointer to array of c-strings, or NULL on failure
 */

char ** mvm_disassemble_addresses_array();

/** returns a 0-terminated array of addresses containing all valid
 * addresses in the current class file.  A valid address is one at
 * which an instruction begins and must occur after the class table.
 * Array will be sorted numerically by address.  The array is
 * allocated by malloc, and must be free'd when no longer needed.
 *
 * @return pointer to array of addresses, or NULL on failure
 */

uint32_t * mvm_disassemble_addresses_value_array();
