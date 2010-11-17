/* Niels Widger
 * Time-stamp: <18 Feb 2010 at 13:46:55 by nwidger on macros.local>
 */

#ifndef _MVM_LOCALVARIABLEARRAY_H
#define _MVM_LOCALVARIABLEARRAY_H

struct local_variable_array;

/** creates a new local_variable_array of size m.
 *
 * @param m - the size of the new local_variable_array
 *
 * @return pointer to new local_variable_array, or NULL on failure
 */

struct local_variable_array * local_variable_array_create(int m);

/** destroys the given local_variable_array
 *
 * @param l - the local_variable_array to destroy
 */

void local_variable_array_destroy(struct local_variable_array *l);

/** clears the given local_variable_array by setting all of its
 * references to 0.
 *
 * @param l - the local_variable_array to clear
 */

void local_variable_array_clear(struct local_variable_array *l);

/** returns the size of the local_variable_array
 *
 * @param l - the local_variable_array to check
 *
 * @return size of local_variable_array
 */

int local_variable_array_size(struct local_variable_array *l);

/** returns the reference stored at index i of the given
 * local_variable_array.  Indices start at 0.
 *
 * @param l - the local_variable_array to operate on
 * @param i - the index to read
 *
 * @return reference stored at given index
 */

int local_variable_array_load(struct local_variable_array *l, int i);

/** stores reference r in index i of the given local_variable_array.
 * Indices start at 0.
 *
 * @param l - the local_variable_array to operate on
 * @param i - the index to write
 * @param r - the reference to store
 *
 * @return old reference stored at given index
 */

int local_variable_array_store(struct local_variable_array *l, int i, int r);

/** dumps the reference, class name and toString representation of each
 * element of the given local_variable_array on a single line.
 *
 * @param l - the local_variable_array to dump
 *
 * @return 0 on success, non-zero on failure
 */

int local_variable_array_dump(struct local_variable_array *l);

/** acquires a lock which prevents other threads from accessing the
 * given local_variable_array.  When done, release lock with
 * local_variable_array_lock.
 *
 * @param l - the local_variable_array to lock
 *
 * @return 0 on success, non-zero on failure.
 */

int local_variable_array_lock(struct local_variable_array *l);

/** releases the lock acquired by local_variable_array_lock, allowing
 * other threads to access the given local_variable_array.
 *
 * @param l - the local_variable_array to unlock
 *
 * @return 0 on success, non-zero on failure.
 */

int local_variable_array_unlock(struct local_variable_array *l);

#endif
