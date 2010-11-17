/* Niels Widger
 * Time-stamp: <18 Feb 2010 at 13:15:08 by nwidger on macros.local>
 */

#ifndef _MVM_CLASS_H
#define _MVM_CLASS_H

#include <inttypes.h>

/* enums */
/** used by class_get_method_field
 * @see class_get_method_field
 */
enum class_field {
	/** method start address */
	address_field		= 0,
	/** method end address */
	end_field               = 1,
	/** method max locals */
	max_locals_field	= 2,
	/** method native index */
	native_index_field	= 2
};

struct class;

/** creates a new class with the given vmt and method table size.
 *
 * @param s - the name of the class as a c-string 
 * @param v - the vmt of the new class
 * @param n - the number of methods in the new class's method table
 *
 * @return pointer to new class, or NULL on failure
 */

struct class * class_create(char *s, uint32_t v, uint32_t n);

/** destroys the given class.
 *
 * @param c - the class to destroy
 */

void class_destroy(struct class *c);

/** clears the given class by setting all fields to 0
 *
 * @param c - the class to clear
 */

void class_clear(struct class *c);

/** sets the super class vmt of the given class to s.
 *
 * @param c - the class to operate on
 * @param s - the super class vmt to set
 *
 * @return 0 on success, non-zero on failure
 */

int class_set_super_vmt(struct class *c, uint32_t s);

/** sets the super class pointer of the given class to s.
 *
 * @param c - the class to operate on
 * @param s - pointer to super class
 * 
 * @return 0 on success, non-zero on failure 
 */

int class_set_super_class(struct class *c, struct class *s);

/** sets the address and max locals count of the method at method table
 * index i of the given class to a and m, respectively.  Use
 * class_set_native_method if method is implemented natively.
 *
 * @param c - the class to operate on
 * @param i - the method at this method table index will be set
 * @param n - name of the method as a c-string
 * @param a - address of the specified method
 * @param e - end address of the specified method
 * @param m - max locals count of the specified method
 * 
 * @return 0 on success, non-zero on failure 
 */

int class_set_method(struct class *c, int i, char *n,
		     uint32_t a, uint32_t e, uint32_t m);

/** sets the native index of the method at method table index i of the
 * given class to n.
 *
 * @param c - the class to operate on
 * @param i - the method at this method table index will be set
 * @param s - name of the method as a c-string
 * @param n - native index of the specified method
 * 
 * @return 0 on success, non-zero on failure
 */

int class_set_native_method(struct class *c, int i, char *s, uint32_t n);

/** sets the number of fields in the given class to n.
 *
 * @param c - the class to operate on
 * @param n - the number of fields in this class
 * 
 * @return 0 on success, non-zero on failure
 */

int class_set_num_fields(struct class *c, uint32_t n);

/** returns the vmt of the given class.
 *
 * @param c - the class to operate on
 *
 * @return vmt of the given class
 */

uint32_t class_get_vmt(struct class *c);

/** returns name of class as c-string.
 *
 * @param c - the class to access
 *
 * @return name of class as c-string, or NULL on failure.
 */
char * class_get_name(struct class *c);

/** returns pointer to super class of given class.
 *
 * @param c - the class to operate on
 *
 * @return pointer to class's super class.
 */

struct class * class_get_super_class(struct class *c);

/** returns vmt of super class of given class.
 *
 * @param c - the class to operate on
 *
 * @return vmt of class's super class.
 */

uint32_t class_get_super_vmt(struct class *c);

/** returns the given class's field count.
 *
 * @param c - the class to operate on
 *
 * @return number of fields in given class.
 */

uint32_t class_get_num_fields(struct class *c);

/** returns the size of the given class's method table.
 *
 * @param c - the class to operate on
 *
 * @return size of class's method table.
 */

uint32_t class_get_num_methods(struct class *c);

/** checks if method at given method table index is implemented natively.
 *
 * @param c - the class to operate on
 * @param i - method at this method table index will be checked
 *
 * @return non-zero if method is native, 0 otherwise.
 */

int class_method_is_native(struct class *c, int i);

/** returns the requested method field of the method at method table
 * index i of the given class.
 *
 * @param c - the class to operate on
 * @param i - method at this method table index will be used
 * @param f - field to return.  Must be either 'address_field', 'end_field',
 * 'max_locals_field' or 'native_index_field'
 *
 * @return requested field of class's method.
 * @see class_field
 */

uint32_t class_get_method_field(struct class *c, int i, enum class_field f);

/** returns the name of the requested method at method table index i of
 * the given class.
 *
 * @param c - the class to operate on
 * @param i - method at this method table index will be accessed
 *
 * @return requested method name as c-string
 */

char * class_get_method_name(struct class *c, int i);

/** checks to see if given class can be casted to class t.
 *
 * @param c - the class to check
 * @param t - the target class to cast to
 *
 * @return 0 of cast fails, 1 if cast succeeds
 */

int class_can_cast_to(struct class *c, struct class *t);

#endif
