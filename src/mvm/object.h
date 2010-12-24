/* Niels Widger
 * Time-stamp: <23 Dec 2010 at 17:44:59 by nwidger on macros.local>
 */

#ifndef _MVM_OBJECT_H
#define _MVM_OBJECT_H

#include <inttypes.h>

struct class;
struct integer;
struct object;
struct ref_set;
struct string;
struct table;
struct thread;

/* enum */
enum ownership_status {
	owned_status   = 0,
	unowned_status = 1,
};

enum shared_status {
	private_status = 0,
	shared_status  = 1
};

/** creates a new object from class c with n fields. Memory for the new
 * object shall be allocated from the heap.  Returns a reference to
 * the new object.  This reference must also be saved inside the
 * object for later use.
 *
 * @param c - the class type of the new object
 * @param n - the number of fields in the new object
 * @param o - if non-NULL, pointer to object will be saved in this variable
 *
 * @return reference to new object, or 0 on failure
 */

int object_create(struct class *c, uint32_t n, struct object **o);

/** destroys the given object.
 *
 * @param o - the object to destroy
 */

void object_destroy(struct object *o);

/** clears the given object.
 *
 * @param o - the object to clear
 */

void object_clear(struct object *o);

/** causes the calling thread to acquire the monitor of the given
 * object.  If another thread currently owns it, the thread will block
 * until it is able to acquire it.  A thread is allowed to acquire the
 * same monitor multiple times, but must release the given monitor the
 * same number of times before other threads will be able to acquire
 * it.
 *
 * @param o - the object whose monitor will be acquired
 *
 * @return 0 on success, non-zero on failure
 */

int object_acquire_monitor(struct object *o);

/** causes the calling thread to release the monitor of the given
 * object.
 *
 * @param o - the object whose monitor will be released
 *
 * @return 0 on success, non-zero on failure
 */

int object_release_monitor(struct object *o);

/** causes the calling thread to wait until another thread calls
 * object_notify or object_notify_all on the given object.  The
 * calling thread must acquire the given object's monitor via a call
 * to object_acquire_monitor before calling this function.
 *
 * @param o - the object to wait on
 *
 * @return 0 on success, non-zero on failure
 */

int object_wait(struct object *o);

/** causes the calling thread to wait until another thread calls
 * object_notify or object_notify_all on the given object or t
 * milliseconds have gone by.  The calling thread must acquire the
 * given object's monitor via a call to object_acquire_monitor before
 * calling this function.
 *
 * @param o - the object to wait on
 * @param p - Integer object indicating wait timeout in milliseconds
 *
 * @return 0 on success, non-zero on failure
 */

int object_timedwait(struct object *o, struct object *p);

/** wakes up a single thread waiting on the given object via a call to
 * object_wait or object_timedwait.  The calling thread must acquire
 * the given object's monitor via a call to object_acquire_monitor
 * before calling this function.
 *
 * @param o - notify threads waiting on this object's monitor
 *
 * @return 0 on success, non-zero on failure 
 */

int object_notify(struct object *o);

/** wakes up all threads waiting on the given object via a call to
 * object_wait or object_timedwait.  The calling thread must acquire
 * the given object's monitor via a call to object_acquire_monitor
 * before calling this function.
 *
 * @param o - notify threads waiting on this object's monitor
 *
 * @return 0 on success, non-zero on failure 
 */

int object_notify_all(struct object *o);

/** returns the given object's reference.
 *
 * @param o - the object to access
 *
 * @return the object's reference
 */

int object_get_ref(struct object *o);

/** returns pointer to the object's class type.
 *
 * @param o - the object to access
 *
 * @return pointer to the object's class type
 */

struct class * object_get_class(struct object *o);

/** returns the reference stored in field i of the given object.  Field
 * indices begin at 0.
 *
 * @param o - the object to load from
 * @param i - index of the field to load
 *
 * @return reference stored at field index i
 */

int object_load_field(struct object *o, int i);

/** stores reference r into field index i of the given object.  Field
 * indices begin at 0.  Returns the old reference at the given index.
 *
 * @param o - the object to store into
 * @param i - index of the field to store into
 * @param r - the reference to store
 *
 * @return old reference stored at the given index
 */

int object_store_field(struct object *o, int i, int r);

/** returns then number of fields in the given object.
 *
 * @param o - the object to access
 *
 * @return the number of fields in the given object.
 */

uint32_t object_get_num_fields(struct object *o);

/** returns the predefined type of the given object.  Return value
 * will be one of 'object_type', 'integer_type', 'string_type',
 * 'table_type', 'thread_type' or 'real_type'.
 *
 * @param o - the object to access
 *
 * @return the predefined type of the object
 */

int object_get_predefined_type(struct object *o);

/** returns pointer to the integer stored in the given object.  This
 * function should only be called after ensuring that
 * object_get_predefined_type returns 'integer_type'.
 *
 * @param o - the object to access
 *
 * @return pointer to the integer stored in the object
 */

struct integer * object_get_integer(struct object *o);

/** returns pointer to the string stored in the given object.  This
 * function should only be called after ensuring that
 * object_get_predefined_type returns 'string_type'.
 *
 * @param o - the object to access
 *
 * @return pointer to the string stored in the object
 */

struct string * object_get_string(struct object *o);

/** returns pointer to the table stored in the given object.  This
 * function should only be called after ensuring that
 * object_get_predefined_type returns 'table_type'.
 *
 * @param o - the object to access
 *
 * @return pointer to the table stored in the object
 */

struct table * object_get_table(struct object *o);

/** returns pointer to the thread stored in the given object.  This
 * function should only be called after ensuring that
 * object_get_predefined_type returns 'thread_type'.
 *
 * @param o - the object to access
 *
 * @return pointer to the thread stored in the object
 */

struct thread * object_get_thread(struct object *o);

/** returns pointer to the real stored in the given object.  This
 * function should only be called after ensuring that
 * object_get_predefined_type returns 'real_type'.
 *
 * @param o - the object to access
 *
 * @return pointer to the real stored in the object
 */

struct real * object_get_real(struct object *o);

/** stores the integer i into the given object.  Calling
 * object_get_predefined_type immediately after calling this function
 * should return 'integer_type'.
 *
 * @param o - the object to store into
 * @param i - the integer to store
 *
 * @return 0 on success, non-zero on failure
 */

int object_set_integer(struct object *o, struct integer *i);

/** stores the string s into the given object.  Calling
 * object_get_predefined_type immediately after calling this function
 * should return 'string_type'.
 *
 * @param o - the object to store into
 * @param s - the string to store
 *
 * @return 0 on success, non-zero on failure
 */

int object_set_string(struct object *o, struct string *s);

/** stores the table t into the given object.  Calling
 * object_get_predefined_type immediately after calling this function
 * should return 'table_type'.
 *
 * @param o - the object to store into
 * @param t - the table to store
 *
 * @return 0 on success, non-zero on failure
 */

int object_set_table(struct object *o, struct table *t);

/** stores the thread t into the given object.  Calling
 * object_get_predefined_type immediately after calling this function
 * should return 'thread_type'.
 *
 * @param o - the object to store into
 * @param t - the thread to store
 *
 * @return 0 on success, non-zero on failure
 */

int object_set_thread(struct object *o, struct thread *t);

/** stores the real i into the given object.  Calling
 * object_get_predefined_type immediately after calling this function
 * should return 'real_type'.
 *
 * @param o - the object to store into
 * @param f - the real to store
 *
 * @return 0 on success, non-zero on failure
 */

int object_set_real(struct object *o, struct real *f);

/** adds all references stored inside the given object to the ref_set
 * r.  Any references stored inside predefined types (Table in
 * particular) are also added.
 *
 * @param o - the object to scan
 * @param r - the ref_set to add to
 *
 * @return 0 on success, non-zero on failure
 */

int object_populate_ref_set(struct object *o, struct ref_set *r);

/** returns a reference to a new Integer object whose value is 1 if the
 * pointers o and p are the same, and 0 otherwise.
 *
 * @param o - first Object
 * @param p - second Object
 *
 * @return reference to new Integer object, or 0 on failure
 */

int object_equals(struct object *o, struct object *p);

/** returns a reference to a new Integer object whose value is value of
 * the pointer o.
 *
 * @param o - an Object
 *
 * @return reference to new Integer object, or 0 on failure
 */

int object_hash_code(struct object *o);

/** returns a reference to a new String object whose characters shall
 * be "Object".
 *
 * @param o - an Object
 *
 * @return reference to new String object, or 0 on failure
 */

int object_to_string(struct object *o);

/** dumps the reference and class name of the given object on a single
 * line.
 *
 * @param o - the Object to dump
 * @param f - if non-zero, dump fields as well
 *
 * @return 0 on success, non-zero on failure
 */

int object_dump(struct object *o, int f);

#endif
