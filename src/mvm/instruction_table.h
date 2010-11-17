/* Niels Widger
 * Time-stamp: <18 Feb 2010 at 13:40:53 by nwidger on macros.local>
 */

#ifndef _MVM_INSTRUCTIONTABLE_H
#define _MVM_INSTRUCTIONTABLE_H

#include <inttypes.h>

struct instruction_table;

/** creates a new instruction_table capable of storing n instructions.
 * The largest opcode of any instruction added must be <= m.
 *
 * @param m - max allowed opcode of the new instruction table
 *
 * @return pointer to new instruction_table, or NULL on failure
 */

struct instruction_table * instruction_table_create(uint32_t m);

/** destroys the given instruction_table.
 *
 * @param i - the instruction_table to destroy
 */

void instruction_table_destroy(struct instruction_table *i);

/** clears the given instruction_table by removing all instructions.
 *
 * @param i - the instruction_table to clear
 */

void instruction_table_clear(struct instruction_table *i);

/** adds a new instruction of opcode o to the instruction_table.
 *
 * @param i - the instruction_table to add to
 * @param n - the name of the new instruction as a c-string
 * @param o - the opcode of the new instruction
 * @param f - pointer to function that implements the instruction
 * @param d - pointer to function that decodes the instruction
 * @param s - pointer to function that decodes size of the instruction
 *
 * @return 0 on success, non-zero on failure
 */

int instruction_table_add(struct instruction_table *i, char *n, uint32_t o,
			  int (*f)(uint32_t), int (*d)(uint32_t), int (*s)(uint32_t));

/** executes the instruction with opcode o.
 *
 * @param i - the instruction_table to search
 * @param o - the opcode to execute
 *
 * @return return value of the instruction function
 */

int instruction_table_execute(struct instruction_table *i, uint32_t o);

/** decodes the instruction with opcode o at address a.
 *
 * @param i - the instruction_table to search
 * @param p - if non-zero, print address in front of each instruction  
 * @param o - the opcode to decode
 * @param a - the address of the instruction
 *
 * @return return value of the decode function
 */

int instruction_table_decode(struct instruction_table *i, int p, uint32_t o, uint32_t a);

/** returns of the size in bytes of the instruction with opcode o at
 * address a.
 *
 * @param i - the instruction_table to search
 * @param o - the opcode to decode
 * @param a - the address of the instruction
 *
 * @return size of decoded instruction
 */

int instruction_table_decode_size(struct instruction_table *i, uint32_t o, uint32_t a);

/** returns the name of the instruction with the opcode o.
 *
 * @param i - the instruction_table to access
 * @param o - the opcode of the instruction name to fetch
 *
 * @return name of instruction as a c-string, or NULL on failure
 */

char * instruction_table_get_name(struct instruction_table *i, uint32_t o);

/** dumps the contents of the instruction_table to standard error.
 *
 * @param i - the instruction_table to dump
 *
 * @return 0 on success, non-zero on failure
 */

int instruction_table_dump(struct instruction_table *i);

#endif
