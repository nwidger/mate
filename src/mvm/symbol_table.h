/* Niels Widger
 * Time-stamp: <18 Feb 2010 at 14:06:12 by nwidger on macros.local>
 */

#ifndef _SYMBOLTABLE_H
#define _SYMBOLTABLE_H

struct symbol_table;

/** create a new symbol table
 * table is sorted by address in ascending order
 * 
 * @return pointer to new table on success, NULL on failure
 */

struct symbol_table * symbol_table_create();

/** destroy given table
 * 
 * @param t - table to destroy
 */

void symbol_table_destroy(struct symbol_table *t);

/** remove all symbols from given table
 * 
 * @param t - table to clear
 */

void symbol_table_clear(struct symbol_table *t);

/** add symbol to given table
 * 
 * @param t - the table to add the symbol to
 * @param l - label (symbol) name as c-string
 * @param a - address of the label (symbol)
 * 
 * @return 0 on success, 1 on failure
 */

int symbol_table_add(struct symbol_table *t, char *l, uint32_t a);

/** find address of symbol with given name in given table
 * 
 * @param t - the table to search
 * @param l - label (symbol) name as c-string
 * 
 * @return opcode of given label (symbol) on success, or 0 on failure
 */

uint32_t symbol_table_find_address(struct symbol_table *t, char *l);

/** find address of symbol with given name in given table
 * 
 * @param t - the table to search
 * @param a - address of the label (symbol) to find
 * 
 * @return label of given address on success, or NULL on failure
 */

char * symbol_table_find_label(struct symbol_table *t, uint32_t a);

/** returns the label of the i-th element in the symbol table
 * 
 * @param t - the table to access
 * @param i - the index to use
 * 
 * @return label of the element with the given address, or NULL on failure
 */

char * symbol_table_get_label_at_index(struct symbol_table *t, int i);

/** returns the address of the i-th element in the symbol table
 * 
 * @param t - the table to access
 * @param i - the index to use
 * 
 * @return address of the element with the given address, or 0 on failure
 */

uint32_t symbol_table_get_address_at_index(struct symbol_table *t, int i);

/** returns the current size of the table
 * 
 * @param t - the table to return the size of
 * 
 * @return size of table, or -1 on error.
 */

int symbol_table_size(struct symbol_table *t);

/** dumps the size of the symbol table on a single line, then dumps the
 * contents of the symbol table, one per line, in the following form:
 *
 * symbol_address $symbol_name
 *
 * where symbol_address and $symbol_name are separated by
 * a space character.
 *
 * @param t - the table to dump
 * 
 * @return 0 on success, 1 on failure
 */

int symbol_table_dump(struct symbol_table *t);

/** reads in all label/address pairs from the given file
 * into the symbol table.  The file must be in the form
 * defined by symbol_table_dump.
 * 
 * @param t - the table to load into
 * @param f - file containing label/address pairs, as c-string
 * 
 * @return 0 on success, 1 on failure
 */

int symbol_table_load_dump(struct symbol_table *t, char *f);

/** returns a null-terminated array of c-strings containing all labels
 * in the symbol table.  Array will be sorted lexicographically by
 * label name.  Both the array and each string are allocated by
 * malloc, and must be free'd when no longer needed.
 *
 * @param t - the table to access
 *
 * @return pointer to array of c-strings, or NULL on failure
 */

char ** symbol_table_label_array(struct symbol_table *t);

#endif
