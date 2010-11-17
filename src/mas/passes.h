/* Niels Widger
 * Time-stamp: <20 Feb 2010 at 12:37:55 by nwidger on macros.local>
 */

#ifndef PASSES_H
#define PASSES_H

/* constants */

/* first pass */

/** initializes any resources needed for first pass
 * 
 * @return 0 on success, non-zero on failure
 */

int first_pass_init();

/** cleans up any resources after first pass is completed
 * 
 * @return 0 on success, non-zero on failure
 */

int first_pass_cleanup();

/* second_pass */

/** initializes any resources needed for second pass
 * 
 * @return 0 on success, non-zero on failure
 */

int second_pass_init();

/** cleans up any resources after second pass is completed
 * 
 * @return 0 on success, non-zero on failure
 */

int second_pass_cleanup();

/* readers */

/** processes label read on first pass
 * 
 * @param line - current line of scanner
 * @param label - label name as c-string
 * 
 * @return 0 on success, non-zero on failure
 */

int first_pass_process_label(int line, char *label);

/** processes symbol read on first pass
 * 
 * @param line - current line of scanner
 * @param symbol - symbol name as c-string
 * 
 * @return 0 on success, non-zero on failure
 */

int first_pass_process_symbol(int line, char *symbol);

/** processes instruction read on first pass
 * 
 * @param line - current line of scanner
 * @param instruction - instruction name as c-string
 * 
 * @return 0 on success, non-zero on failure
 */

int first_pass_process_instruction(int line, char *instruction);

/** processes integer literal read on first pass
 * 
 * @param line - current line of scanner
 * @param integer - integer as c-string
 * 
 * @return 0 on success, non-zero on failure
 */

int first_pass_process_integer(int line, char *integer);

/** processes string literal read on first pass
 * 
 * @param line - current line of scanner
 * @param string - string as c-string
 * 
 * @return 0 on success, non-zero on failure
 */

int first_pass_process_string(int line, char *string);

/* writers */

/** processes symbol read on second pass
 * 
 * @param line - current line of scanner
 * @param symbol - symbol as c-string
 * 
 * @return 0 on success, non-zero on failure
 */

int second_pass_process_symbol(int line, char *symbol);

/** processes instruction read on second pass
 * 
 * @param line - current line of scanner
 * @param instruction - instruction as c-string
 * 
 * @return 0 on success, non-zero on failure
 */

int second_pass_process_instruction(int line, char *instruction);

/** processes integer literal read on second pass
 * 
 * @param line - current line of scanner
 * @param integer - integer as c-string
 * 
 * @return 0 on success, non-zero on failure
 */

int second_pass_process_integer(int line, char *integer);

/** processes string literal read on second pass
 * 
 * @param line - current line of scanner
 * @param string - string as c-string
 * 
 * @return 0 on success, non-zero on failure
 */

int second_pass_process_string(int line, char *string);

/** prints out contents of the symbol table.
 * Symbols are printed one per line, in the
 * following form:
 *
 * symbol_address $symbol_name
 *
 * where symbol_address and $symbol_name are separated by a single space
 * character.
 */

void dump_symbol_table();

#endif
