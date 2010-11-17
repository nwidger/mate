/* Niels Widger
 * Time-stamp: <25 Mar 2010 at 20:50:19 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "class_table.h"
#include "constants.h"
#include "disassemble.h"
#include "globals.h"
#include "instruction_table.h"
#include "mdb.h"
#include "method_area.h"
#include "symbol_table.h"

uint32_t mvm_disassemble_argument(uint32_t a, int n) {
	return method_area_fetch(method_area, a+(sizeof(uint32_t)*(n+1)));
}

int mvm_disassemble_arguments(uint32_t a, int n, ...) {
	int i;
	va_list ap;
	uint32_t *arg, value;

	va_start(ap, n);
	for (i = 0; i < n; i++) {
		arg = va_arg(ap, uint32_t *);
		value = mvm_disassemble_argument(a, i);
		if (arg != NULL)
			*arg = value;
	}
	va_end(ap);

	return 0;
}

char * mvm_disassemble_string_argument(uint32_t a) {
	int i, size;
	char c, *buf;

	size = INSTRUCTION_INITIAL_BUFFER_SIZE;
	if ((buf = (char *)malloc(sizeof(char)*size)) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	i = 0;
	do {
		c = (char)mvm_disassemble_argument(a, i);
		buf[i++] = c;

		if (i == size) {
			size *= INSTRUCTION_BUFFER_RESIZE_FACTOR;
			if ((buf = (char *)realloc(buf, sizeof(char)*size)) == NULL) {
				perror("mvm: realloc");
				mvm_halt();
			}
		}
	} while (c != '\0');

	return buf;
}

int mvm_disassemble_class_file(int p) {
	fprintf(stderr, "$mainBlock %" PRIu32 "\n",
		method_area_get_main_block_max_locals(method_area));
	class_table_decode(class_table);
	return mvm_disassemble_region(p, method_area_get_main_block_address(method_area),
				      method_area_size(method_area));
}

int mvm_disassemble_region(int p, uint32_t s, uint32_t e) {
	int inc;
	uint32_t address;

	address = s;

	while (address <= e && method_area_address_is_valid(method_area, address)) {
		inc = mvm_disassemble_instruction(p, address);
		address += (inc * sizeof(uint32_t));
	}

	return 0;
}

int mvm_disassemble_instruction(int p, uint32_t a) {
	int retval;
	char *label;
	uint32_t opcode;

	if ((label = symbol_table_find_label(symbol_table, a)) != NULL)
		fprintf(stderr, "%s:\n", label);

	mdb_uninsert_enabled_breakpoints();

	opcode = method_area_fetch(method_area, a);
	retval = instruction_table_decode(instruction_table, p, opcode, a);

	mdb_insert_enabled_breakpoints();

	return retval;
}

uint32_t mvm_disassemble_method_end(uint32_t a) {
	int inc;
	uint32_t opcode, address, max, branch;

	mdb_uninsert_enabled_breakpoints();

	address = max = a;
	while (method_area_address_is_valid(method_area, address) != 0) {
		opcode = method_area_fetch(method_area, address);
		inc = instruction_table_decode_size(instruction_table,
						    opcode, address);

		switch(opcode) {
		case ARETURN_OPCODE:
		case RETURN_OPCODE:
			if (address >= max) {
				mdb_insert_enabled_breakpoints();
				return address;
			}
		case IFEQ_OPCODE:
			branch = mvm_disassemble_argument(address, 0);
			if (branch > max) max = branch;
			branch = address + (inc * sizeof(uint32_t));
			if (branch > max) max = branch;
			break;
		case GOTO_OPCODE:
			branch = mvm_disassemble_argument(address, 0);
			if (branch > max) max = branch;
			break;

		}

		address += (inc * sizeof(uint32_t));
	}

	mdb_insert_enabled_breakpoints();
	return 0;
}

char ** mvm_disassemble_addresses_array() {
	int inc, size, i;
	char **buf, str[INTEGER_TO_STRING_BUFFER_SIZE];
	uint32_t opcode, address;

	mdb_uninsert_enabled_breakpoints();
	address = method_area_get_main_block_address(method_area);

	size = 0;
	while (method_area_address_is_valid(method_area, address)) {
		opcode = method_area_fetch(method_area, address);
		inc = instruction_table_decode_size(instruction_table, opcode, address);
		address += (inc * sizeof(uint32_t));
		size++;
	}

	if ((buf = (char **)malloc(sizeof(char *)*(size+1))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	address = method_area_get_main_block_address(method_area);

	for (i = 0; i < size; i++) {
		opcode = method_area_fetch(method_area, address);
		inc = instruction_table_decode_size(instruction_table, opcode, address);

		snprintf(str, INTEGER_TO_STRING_BUFFER_SIZE, "%" PRIu32, address);

		if ((buf[i] = strdup(str)) == NULL) {
			perror("mvm: strdup");
			mvm_halt();
		}

		address += (inc * sizeof(uint32_t));
	}

	buf[size] = NULL;

	mdb_insert_enabled_breakpoints();
	return buf;
}

uint32_t * mvm_disassemble_addresses_value_array() {
	int inc, size, i;
	uint32_t address, opcode, *buf;

	mdb_uninsert_enabled_breakpoints();
	address = method_area_get_main_block_address(method_area);

	size = 0;
	while (method_area_address_is_valid(method_area, address)) {
		opcode = method_area_fetch(method_area, address);
		inc = instruction_table_decode_size(instruction_table, opcode, address);
		address += (inc * sizeof(uint32_t));
		size++;
	}

	if ((buf = (uint32_t *)malloc(sizeof(uint32_t)*(size+1))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	address = method_area_get_main_block_address(method_area);

	for (i = 0; i < size; i++) {
		opcode = method_area_fetch(method_area, address);
		inc = instruction_table_decode_size(instruction_table, opcode, address);
		buf[i] = address;
		address += (inc * sizeof(uint32_t));
	}

	buf[size] = 0;

	mdb_insert_enabled_breakpoints();
	return buf;
}
