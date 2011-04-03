/* Niels Widger
 * Time-stamp: <03 Apr 2011 at 15:03:53 by nwidger on macros.local>
 */

#ifndef _MVM_HEAP_H
#define _MVM_HEAP_H

#include <stdint.h>

struct object;
struct heap;
struct ref_set;

/** creates a new heap of size m bytes.
 *
 * @param m - the size of the heap in bytes
 *
 * @return pointer to new heap, or NULL on failure
 */

struct heap * heap_create(uint64_t m);

/** destroys the given heap
 *
 * @param h - the heap to destroy
 */

void heap_destroy(struct heap *h);

/** clears the given heap by freeing all allocated space.
 *
 * @param h - the heap to clear
 */

void heap_clear(struct heap *h);

/** returns the current size of the given heap in bytes.
 *
 * @param h - the heap access
 *
 * @return the current size of the heap in bytes
 */ 

int heap_get_size(struct heap *h);

/** returns the current free space in the given heap in bytes.
 *
 * @param h - the heap access
 *
 * @return the current free space in the heap in bytes
 */

int heap_get_free(struct heap *h);

/** allocates b bytes of memory from the given heap and returns a
 * pointer to the allocated space.
 *
 * @param h - the heap to allocate from
 * @param b - the amount of memory to allocate in bytes
 *
 * @return pointer to allocated memory, or NULL on failure
 */

void * heap_malloc(struct heap *h, int b);

/** allocates b bytes of memory from the given heap, assigns a
 * reference to the allocated memory and returns a pointer to the
 * allocated space.
 *
 * @param h - the heap to allocate from
 * @param b - the amount of memory to allocate in bytes
 * @param r - if non-NULL, assigned reference will be saved to this variable
 *
 * @return pointer to allocated memory, or NULL on failure
 */

void * heap_malloc_ref(struct heap *h, int b, int *r);

/** frees the heap memory pointed to by p.
 *
 * @param h - the heap to operate on
 * @param p - pointer to heap memory to be freed
 *
 * @return 0 on success, non-zero on failure
 */

int heap_free(struct heap *h, void *p);

/** returns a pointer to the memory assigned the reference r.
 *
 * @param h - the heap to fetch from
 * @param r - the reference to fetch
 *
 * @return pointer to memory, or NULL on failure
 */

void * heap_fetch(struct heap *h, int r);

/** returns a pointer to an object in heap memory that has been
 * assigned the reference r.  This is a convenience function to be
 * used instead of heap_fetch when r is known to be an object.
 *
 * @param h - the heap to fetch from
 * @param r - the reference to fetch
 *
 * @return object pointer, or NULL on failure
 */

struct object * heap_fetch_object(struct heap *h, int r);

/** compacts memory used by the heap.  Function details are
 * implementation dependent.  If not implemented, simply return 0.
 *
 * @param h - the heap to compact
 *
 * @return 0 on success, non-zero on failure
 */

int heap_compact(struct heap *h);

/** resize the heap to be of size m bytes.
 *
 * @param h - the heap to resize
 * @param m - the new size of the heap in bytes
 *
 * @return 0 on success, non-zero on failure
 */

int heap_resize(struct heap *h, uint64_t m);

/** adds all references currently active in the given heap (but which
 * have not been excluded by a call to heap_exclude_ref) to the
 * ref_set r.  Care must be taken in this function to ensure that the
 * heap is not altered while this function is running.
 *
 * @param h - the heap to scan
 * @param r - the ref_set to populate
 *
 * @return 0 on success, non-zero on failure
 */

int heap_populate_ref_set(struct heap *h, struct ref_set *r);


/** adds all thread references currently active in the given heap to
 * the ref_set r.  Care must be taken in this function to ensure that
 * the heap is not altered while this function is running.
 *
 * @param h - the heap to scan
 * @param r - the ref_set to populate
 *
 * @return 0 on success, non-zero on failure
 */

int heap_populate_thread_set(struct heap *h, struct ref_set *r);

/** adds the given reference to the set of references which are
 * excluded from garbage collection.  Care must be taken to ensure
 * heap_include_ref is called on the given reference as soon as
 * possible so that its memory can eventually be reclaimed.
 *
 * @param h - the heap to access
 * @param r - the reference to exclude
 *
 * @return 0 on success, non-zero on failure
 */

int heap_exclude_ref(struct heap *h, int r);

/** sets the given reference to be included in future garbage
 * collection passes.
 *
 * @param h - the heap to access
 * @param r - the reference to include
 *
 * @return 0 on success, non-zero on failure
 */

int heap_include_ref(struct heap *h, int r);

/** adds the given reference to the set of currently active threads.
 *
 * @param h - the heap to access
 * @param r - the thread reference to add
 *
 * @return 0 on success, non-zero on failure
 */

int heap_add_thread_ref(struct heap *h, int r);

/** removes the given reference from the set of currently active
 * threads.
 *
 * @param h - the heap to access
 * @param r - the thread reference to remove
 *
 * @return 0 on success, non-zero on failure
 */

int heap_remove_thread_ref(struct heap *h, int r);

/** dumps the contents of the heap to standard error.
 *
 * @param h - the heap to dump
 *
 * @return 0 on success, non-zero on failure
 */

int heap_dump(struct heap *h);

/** acquires a lock which prevents other threads from accessing the
 * given heap.  When done, release the lock with heap_lock.
 *
 * @param h - the heap to lock
 *
 * @return 0 on success, non-zero on failure.
 */

int heap_lock(struct heap *h);

/** releases the lock acquired by heap_lock, allowing other threads to
 * access the given heap.
 *
 * @param h - the heap to unlock
 *
 * @return 0 on success, non-zero on failure.
 */

int heap_unlock(struct heap *h);

#endif
