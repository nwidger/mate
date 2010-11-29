/* Niels Widger
 * Time-stamp: <28 Nov 2010 at 21:59:40 by nwidger on macros.local>
 */

#ifndef _MVM_NATIVEMETHODARRAY_H
#define _MVM_NATIVEMETHODARRAY_H

#include <inttypes.h>

struct native_method_array;

/** creates a new native_method_array with size s.
 *
 * @param s - the size of the new native_method_array
 *
 * @return pointer to new native_method_array, or NULL on failure
 */

struct native_method_array * native_method_array_create(int s);

/** destroys the given native_method_array
 *
 * @param n - the native_method_array to destroy
 */

void native_method_array_destroy(struct native_method_array *n);

/** clears the given native_method_array by removing all its native
 * methods.
 *
 * @param n - the native_method_array to clear
 */

void native_method_array_clear(struct native_method_array *n);

/** adds the native method to the given native_method_array at index
 * i.
 *
 * @param n - the native_method_array to add to
 * @param i - the index to set
 * @param s - native method name as c-string
 * @param a - the number of arguments of the native method
 * @param m - pointer to native method
 *
 * @return 0 on success, non-zero on failure
 */

int native_method_array_set(struct native_method_array *n, int i, char *s,
			    int (*m)(uint32_t));

/** executes the native method at index i of the given
 * native_method_array.
 *
 * @param n - the native_method_array to access
 * @param i - the index of the native method to invoke
 *
 * @return return value of the native method function, or -1 on failure
 */

int native_method_array_execute(struct native_method_array *n, int i);

/** returns the name of the native method at index i of the given
 * native_method_array.
 *
 * @param n - the native_method_array to access
 * @param i - the index of the native method
 *
 * @return name of given native method as a c-string, or NULL on failure
 */

char * native_method_array_get_name(struct native_method_array *n, int i);

/** returns the index of the native method with name s in the given
 * native_method_array.
 *
 * @param n - the native_method_array to access
 * @param s - the name of the native method to find
 *
 * @return index of the native method, or -1 on failure
 */

int native_method_array_get_index(struct native_method_array *n, char *s);

/** returns the number of arguments of the native method at index i of
 * the given native_method_array.
 *
 * @param n - the native_method_array to access
 * @param i - the index of the native method
 *
 * @return number of arguments of given native method, or -1 on failure
 */

int native_method_array_get_num_args(struct native_method_array *n, int i);

/** dump the contents of the native_method_array to standard error.
 *
 * @param n - the native_method_array to dump
 *
 * @return 0 on success, non-zero on failure
 */

int native_method_array_dump(struct native_method_array *n);

/** returns a null-terminated array of c-strings containing the names
 * of all native methods in the native_method_array.  Array will be
 * sorted lexicographically by native method name.  Both the array and
 * each string are allocated by malloc, and must be free'd when no
 * longer needed.
 *
 * @param n - the native_method_array to access
 *
 * @return pointer to array of c-strings, or NULL on failure
 */

char ** native_method_array_methods_array(struct native_method_array *n);

#endif
