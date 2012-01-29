/* Niels Widger
 * Time-stamp: <29 Jan 2012 at 14:09:31 by nwidger on macros.local>
 */

#ifndef _MVM_TABLE_H
#define _MVM_TABLE_H

struct object;
struct ref_set;
struct table;

/** creates a new table with initial capacity c.
 *
 * @param c - the initial capacity of the new table
 * @param o - the object associated with this table
 *
 * @return pointer to new table, or NULL on failure
 */

struct table * table_create(int c, struct object *o);

/** destroys the given table.
 *
 * @param t - the table to destroy
 */

void table_destroy(struct table *t);

/** clears the given table by removing all entries from it.
 *
 * @param t - the table to clear
 */

void table_clear(struct table *t);

/** adds all references currently in the given table (that is, all key
 * and value pairs) to the ref_set r.
 *
 * @param t - the table to scan
 * @param r - the ref_set to populate
 *
 * @return 0 on success, non-zero on failure
 */

int table_populate_ref_set(struct table *t, struct ref_set *r);

/** returns the value reference of the entry in the given table whose
 * key equals k.  The equals method of the Object class is invoked to
 * determine if two keys are equal.
 *
 * @param t - the table to access
 * @param k - an Object pointer
 *
 * @return value reference of retrieved entry, or 0 if none found
 */

int table_get(struct table *t, struct object *k);

/** creates a new entry with key k and value v in the given table.
 * Returns the value reference of the old entry with key k, or 0 if
 * none existed.
 *
 * @param t - the table to access
 * @param k - the key, must be an Object pointer
 * @param v - the value, must be an Object pointer
 *
 * @return old value reference, or 0 if none existed 
 */

int table_put(struct table *t, struct object *k, struct object *v);

/** removes from the given table the entry with key k.  Returns the
 * value reference of the removed entry, or 0 if none existed.
 *
 * @param t - the table to access
 * @param k - the key, must be an Object pointer
 *
 * @return old value reference, or 0 if none existed
 */

int table_remove(struct table *t, struct object *k);

/** resets the given table's iterator to point to the table's first
 * entry.  returns a reference to a new Integer object whose value is
 * 1 if the table is empty, and 0 otherwise.
 *
 * @param t - the table to access
 *
 * @return reference to new Integer object
 */

int table_first_key(struct table *t);

/** returns the key reference pointed to by the given table's iterator
 * and advances the iterator to the next entry.  Returns 0 if the
 * iterator is at the end of the table.
 *
 * @param t - the table to access
 *
 * @return key reference, or 0 is iterator is at end of the table
 */

int table_next_key(struct table *t);

/** dumps the contents of the table to standard error.
 *
 * @param t - the table to dump
 *
 * @return 0 on success, non-zero on failure
 */

int table_dump(struct table *t);

#endif
