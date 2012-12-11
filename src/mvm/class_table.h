/* Niels Widger
 * Time-stamp: <09 Dec 2012 at 20:03:12 by nwidger on macros.local>
 */

#ifndef _MVM_CLASSTABLE_H
#define _MVM_CLASSTABLE_H

#include <inttypes.h>

/* enums */
/** used by class_table_add and
 * class_table_find_predefined
 * @see class_table_add
 * @see class_table_find_predefined
 */
enum class_type {
	/** class is predefined Object class */
	object_type     = 0,
	/** class is predefined Integer class */	
	integer_type	= 1,
	/** class is predefined String class */	
	string_type	= 2,
	/** class is predefined Table class */	
	table_type	= 3,
	/** class is predefined Thread class */	
	thread_type	= 4,
	/** class is predefined Real class */
	real_type      = 5,
	/** class is user defined class */
	user_type       = 6
};

struct class;
struct class_table;
struct object;

/** creates a new class_table able to store up to n classes.
 *
 * @param n - the size of the new class_table
 *
 * @return pointer to new class_table, NULL on failure
 */

struct class_table * class_table_create(int n);

/** destroys the given class_table.
 *
 * @param c - the class_table to destroy
 */

void class_table_destroy(struct class_table *c);

/** clears the given class_table by removing all classes stored in it.
 *
 * @param c - the class_table to clear
 */

void class_table_clear(struct class_table *c);

/** adds a new class n to the given class_table.
 *
 * @param c - the class_table to add to @param t - the type of the new
 * class.  Must be one of 'object_type', 'integer_type',
 * 'string_type', 'table_type', 'thread_type', 'real_type' or
 * 'user_type'.  All classes besides Object, Integer, String, Table,
 * Thread and Real should be added as type 'user_type'.
 * 
 * @param n - the new class to add
 *
 * @return 0 on success, non-zero on failure.
 * @see class_type
 */

int class_table_add(struct class_table *c, enum class_type t, struct class *n);

/** find a class based on vmt.
 *
 * @param c - the class_table to search
 * @param v - vmt of the class to find
 *
 * @return pointer to class if found, NULL otherwise.
 */

struct class * class_table_find(struct class_table *c, uint32_t v);

/** find a predefined class.
 *
 * @param c - the class_table to search
 * @param t - predefined class type to find.  Must be one of
 * 'object_type', 'integer_type', 'string_type', 'table_type',
 * 'thread_type' or 'real_type'.
 *
 * @return pointer to class if found, NULL otherwise.
 * @see class_type
 */

struct class * class_table_find_predefined(struct class_table *c, enum class_type t);

/** instantiates a new object whose class type has the given vmt, and
 * returns a reference to the new object.  To create an instance of a
 * predefined class, use class_table_new_object,
 * class_table_new_integer, class_table_new_string,
 * class_table_new_table, class_table_new_thread or
 * class_table_new_real.
 *
 * @param c - the class_table to use
 * @param v - the vmt of the class to create an instance of
 * @param o - if non-NULL, pointer to object will be saved to this variable
 *
 * @return reference to the new object, or 0 on failure
 */

int class_table_new(struct class_table *c, uint32_t v, struct object **o);

/** creates a new instance of the predefined Object class and returns
 * a reference to the new object.
 *
 * @param c - the class_table to use
 * @param o - if non-NULL, pointer to object will be saved to this variable
 *
 * @return reference to the new Object instance, or 0 on failure
 */

int class_table_new_object(struct class_table *c, struct object **o);

/** creates a new instance of the predefined Integer class with value v
 * and returns a reference to the new object.
 *
 * @param c - the class_table to use
 * @param v - the new Integer instance will have this value
 * @param o - if non-NULL, pointer to object will be saved to this variable
 *
 * @return reference to the new Integer instance, or 0 on failure
 */

int class_table_new_integer(struct class_table *c, int32_t v, struct object **o);

/** creates a new instance of the predefined String class with
 * characters b and returns a reference to the new object.
 *
 * @param c - the class_table to use
 * @param b - the new String instance will contain all characters in this C-string
 * @param o - if non-NULL, pointer to object will be saved to this variable
 * @param s - non-zero if string is being created due to a newstr instruction
 *
 * @return reference to the new String instance, or 0 on failure
 */

int class_table_new_string(struct class_table *c, char *b, struct object **o, int s);

/** creates a new instance of the predefined Table class with initial
 * capacity n and returns a reference to the new object.
 *
 * @param c - the class_table to use
 * @param n - the new Table instance will have this initial capacity
 * @param o - if non-NULL, pointer to object will be saved to this variable
 *
 * @return reference to the new Table instance, or 0 on failure
 */

int class_table_new_table(struct class_table *c, int n, struct object **o);

/** creates a new instance of the predefined Thread class and returns
 * a reference to the new object.
 *
 * @param c - the class_table to use
 * @param o - if non-NULL, pointer to object will be saved to this variable
 *
 * @return reference to the new Thread instance, or 0 on failure
 */

int class_table_new_thread(struct class_table *c, struct object **o);

/** creates a new instance of the predefined Real class with value v
 * and returns a reference to the new object.
 *
 * @param c - the class_table to use
 * @param v - the new Real instance will have this value
 * @param o - if non-NULL, pointer to object will be saved to this variable
 *
 * @return reference to the new Real instance, or 0 on failure
 */

int class_table_new_real(struct class_table *c, float v, struct object **o);

/** performs analysis on the class_table in order to determine class
 * hierarchy.  In particular, calls class_set_super_class on each
 * class (except Object) so that each class will have a pointer to its
 * super class.
 *
 * @param c - the class_table to analyze
 * 
 * @return 0 on succcess, non-zero on failure
 */

int class_table_analyze(struct class_table *c);

/** acquires a lock which prevents other threads from accessing the
 * given class_table.  When done, release the lock with
 * class_table_lock.
 *
 * @param c - the class_table to lock
 *
 * @return 0 on success, non-zero on failure.
 */

int class_table_lock(struct class_table *c);

/** releases the lock acquired by class_table_lock, allowing other
 * threads to access the given class_table.
 *
 * @param c - the class_table to unlock
 *
 * @return 0 on success, non-zero on failure.
 */

int class_table_unlock(struct class_table *c);

/** dumps the class_table for debugging purposes.  Dump format is
 * implementation dependent.
 *
 * @param c - the class_table to dump
 */

void class_table_dump(struct class_table *c);

/** decodes the class table to maTe assembler.
 *
 * @param c - the class_table to decode
 */

void class_table_decode(struct class_table *c);


/** returns a null-terminated array of c-strings containing the names
 * of all classes in the class table.  Array will be sorted
 * lexicographically by class name.  Both the array and each string
 * are allocated by malloc, and must be free'd when no longer needed.
 *
 * @param c - the class_table to access
 *
 * @return pointer to array of c-strings, or NULL on failure
 */

char ** class_table_classes_array(struct class_table *c);

/** returns a null-terminated array of c-strings containing all methods
 * in the class table.  Array will be sorted lexicographically by
 * method name.  Both the array and each string are allocated by
 * malloc, and must be free'd when no longer needed.
 *
 * @param c - the class_table to access
 *
 * @return pointer to array of c-strings, or NULL on failure
 */

char ** class_table_methods_array(struct class_table *c);

#endif
