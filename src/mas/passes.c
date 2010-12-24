/* Niels Widger
 * Time-stamp: <23 Dec 2010 at 21:12:40 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <arpa/inet.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include <constants.h>

#include "instruction_table.h"
#include "passes.h"
#include "string_pool.h"
#include "symbol_table.h"

/* globals */
int pc;
struct instruction_table *instruction_table;
struct symbol_table *symbol_table;
struct string_pool *string_pool;

/* forward declarations */
int fwrite_big_endian(uint32_t value);

/* first pass */

int first_pass_init() {
	instruction_table = instruction_table_create(NUM_INSTRUCTIONS, MAX_OPCODE);
	symbol_table = symbol_table_create();
	string_pool = string_pool_create();

	pc = 0;
	return 0;
}

int first_pass_cleanup() {
	return 0;
}

/* second pass */

int second_pass_init() {
	instruction_table_add(instruction_table, ACONST_NULL_NAME, ACONST_NULL_OPCODE);
	instruction_table_add(instruction_table, ALOAD_NAME, ALOAD_OPCODE);
	instruction_table_add(instruction_table, ARETURN_NAME, ARETURN_OPCODE);
	instruction_table_add(instruction_table, ASTORE_NAME, ASTORE_OPCODE);
	instruction_table_add(instruction_table, CHECKCAST_NAME, CHECKCAST_OPCODE);
	instruction_table_add(instruction_table, DUP_NAME, DUP_OPCODE);
	instruction_table_add(instruction_table, GETFIELD_NAME, GETFIELD_OPCODE);
	instruction_table_add(instruction_table, GOTO_NAME, GOTO_OPCODE);
	instruction_table_add(instruction_table, IFEQ_NAME, IFEQ_OPCODE);
	instruction_table_add(instruction_table, IN_NAME, IN_OPCODE);
	instruction_table_add(instruction_table, INVOKESPECIAL_NAME, INVOKESPECIAL_OPCODE);
	instruction_table_add(instruction_table, INVOKENATIVE_NAME, INVOKENATIVE_OPCODE);
	instruction_table_add(instruction_table, INVOKEVIRTUAL_NAME, INVOKEVIRTUAL_OPCODE);
	instruction_table_add(instruction_table, NEW_NAME, NEW_OPCODE);
	instruction_table_add(instruction_table, NEWINT_NAME, NEWINT_OPCODE);
	instruction_table_add(instruction_table, NEWSTR_NAME, NEWSTR_OPCODE);
	instruction_table_add(instruction_table, OUT_NAME, OUT_OPCODE);
	instruction_table_add(instruction_table, PUTFIELD_NAME, PUTFIELD_OPCODE);
	instruction_table_add(instruction_table, REFCMP_NAME, REFCMP_OPCODE);
	instruction_table_add(instruction_table, RETURN_NAME, RETURN_OPCODE);
	instruction_table_add(instruction_table, POP_NAME, POP_OPCODE);
	instruction_table_add(instruction_table, DUP_X1_NAME, DUP_X1_OPCODE);
	instruction_table_add(instruction_table, MONITORENTER_NAME, MONITORENTER_OPCODE);
	instruction_table_add(instruction_table, MONITOREXIT_NAME, MONITOREXIT_OPCODE);
	instruction_table_add(instruction_table, NEWREAL_NAME, NEWREAL_OPCODE);
	
	return 0;
}

int second_pass_cleanup() {
	string_pool_destroy(string_pool);
	symbol_table_destroy(symbol_table);
	instruction_table_destroy(instruction_table);

	return 0;
}

/* writers */

int fwrite_big_endian(uint32_t value) {
	uint32_t tmp;
	tmp = htonl(value);
	return fwrite(&tmp, sizeof(uint32_t), 1, stdout);
}

int second_pass_process_symbol(int line, char *symbol) {
	uint32_t value;

	if ((value = symbol_table_find_address(symbol_table, symbol)) == 0) {
		fprintf(stderr, "mas: %d: undefined symbol %s\n", line, symbol);
		return 1;
	}

	fwrite_big_endian(value);
	return 0;
}

int second_pass_process_instruction(int line, char *instruction) {
	uint32_t value;

	if ((value = instruction_table_find(instruction_table, instruction)) == 0) {
		fprintf(stderr, "mas: %d: invalid instruction %s\n",
			line, instruction);
		return 1;
	}

	fwrite_big_endian(value);
	return 0;
}

int second_pass_process_integer(int line, char *integer) {
	char *ptr;
	uint32_t value;
	long long result;

	errno = 0;
	result = strtoll(integer, &ptr, 10);
	if (errno != 0 || ptr == integer ||
	    strspn(ptr, " \n\r\t\v\f") != strlen(ptr)) {
		return 1;
	}

	value = (uint32_t)result;

	fwrite_big_endian(value);
	return 0;
}

int second_pass_process_real(int line, char *real) {
	return second_pass_process_string(line, real);
}

int second_pass_process_string(int line, char *string) {
	uint8_t *c;

	for (c = (uint8_t *)string; *c != '\0'; c++)
		fwrite_big_endian(*c);
	fwrite_big_endian(*c);

	return 0;
}

/* readers */

int first_pass_process_label(int line, char *label) {
	char *tmp;

	tmp = string_pool_add(string_pool, label);
	return symbol_table_add(symbol_table, tmp, pc);
}

int first_pass_process_symbol(int line, char *symbol) {
	pc += 4;
	return 0;
}

int first_pass_process_instruction(int line, char *instruction) {
	pc += 4;
	return 0;
}

int first_pass_process_integer(int line, char *integer) {
	pc += 4;
	return 0;
}

int first_pass_process_real(int line, char *real) {
	pc += (strlen(real)+1)*4;
	return 0;
}
int first_pass_process_string(int line, char *string) {
	pc += (strlen(string)+1)*4;
	return 0;
}

void dump_symbol_table() {
	symbol_table_dump(symbol_table);
}
