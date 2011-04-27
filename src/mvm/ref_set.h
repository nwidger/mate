/* Niels Widger
 * Time-stamp: <10 Apr 2011 at 20:33:32 by nwidger on macros.local>
 */

#ifndef _MVM_REFSET_H
#define _MVM_REFSET_H

struct ref_set;

/** creates a new, empty ref set.
 *
 * @return pointer to new ref_set, or NULL on failure
 */

struct ref_set * ref_set_create();

/** destroys the given ref_set.
 *
 * @param h - the ref_set to destroy
 */

void ref_set_destroy(struct ref_set *h);

/** clears the given ref_set by removing all references from it.
 *
 * @param h - the ref_set to clear
 */

void ref_set_clear(struct ref_set *h);

/** adds the reference r to the given ref_set.
 *
 * @param h - the ref_set to add to
 * @param r - the reference to add
 *
 * @return 0 on success, non-zero on failure
 */

int ref_set_add(struct ref_set *h, int r);

/** removes the reference r from the given ref_set.
 *
 * @param h - the ref_set to remove from
 * @param r - the reference to remove
 *
 * @return 0 on success, non-zero on failure
 */

int ref_set_remove(struct ref_set *h, int r);

/** returns the size of the given ref_set.
 *
 * @param h - the ref_set to access
 *
 * @return size of the ref_set
 */

int ref_set_size(struct ref_set *h);

/** returns 1 if the given ref_set contains the reference r, and 0
 * otherwise.
 *
 * @param h - the ref_set to search
 * @param r - the reference to search for
 *
 * @return 1 if reference is found, 0 otherwise
 */

int ref_set_contains(struct ref_set *h, int r);

/** initializes the given ref_set so that the next call to
 * ref_set_iterator_next will return the first reference in the
 * ref_set.
 *
 * @param h - the ref_set to access
 *
 * @return 0 on success, non-zero on failure
 */

int ref_set_iterator_init(struct ref_set *h);

/** returns the next reference in the given ref_set.  A call to
 * ref_set_iterator_init must be made before the first call to
 * ref_set_iterator_next to initialize the internal iterator.
 *
 * @param h - the ref_set to access
 *
 * @return next reference, or 0 if no more references
 */

int ref_set_iterator_next(struct ref_set *h);

/** dumps the given ref_set for debugging purposes.  Dump format is
 * implementation dependent.
 *
 * @param h - the ref_set to dump
 */

int ref_set_dump(struct ref_set *h);

/** dumps the given ref_set iterator for debugging purposes.  Dump
 * format is implementation dependent.
 *
 * @param h - the ref_set to dump
 */

int ref_set_dump_iterator(struct ref_set *h);

int ref_set_lock(struct ref_set *h);

int ref_set_unlock(struct ref_set *h);

#endif
