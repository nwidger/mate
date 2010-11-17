/* Niels Widger
 * Time-stamp: <23 Feb 2010 at 11:23:57 by nwidger on macros.local>
 */

#ifndef _INSTRUCTIONTABLE_H
#define _INSTRUCTIONTABLE_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct instruction_table;

/** create a new instruction table
 * 
 * @param n - maximum possible instructions
 * @param m - maximum possible opcode
 * 
 * @return pointer to new table on success, NULL on failure
 */

struct instruction_table * instruction_table_create(int n, int m);

/** destroy given table
 * 
 * @param i - table to destroy
 */

void instruction_table_destroy(struct instruction_table *i);

/** remove all instructions from given table
 * 
 * @param i - table to clear
 */

void instruction_table_clear(struct instruction_table *i);

/** add instruction to given table
 * 
 * @param i - the table to add the instruction to
 * @param n - instruction name as c-string
 * @param o - opcode of the given instruction
 * 
 * @return zero on success, non-zero on failure
 */

int instruction_table_add(struct instruction_table *i, char *n, uint32_t o);

/** find opcode of instruction with given name in given table
 * 
 * @param i - the table to search
 * @param n - instruction name as c-string
 * 
 * @return opcode of given instruction, or zero on failure
 */

uint32_t instruction_table_find(struct instruction_table *i, char *n);

#endif
