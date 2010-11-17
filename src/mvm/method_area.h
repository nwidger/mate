/* Niels Widger
 * Time-stamp: <18 Feb 2010 at 13:55:43 by nwidger on macros.local>
 */

#ifndef _MVM_METHODAREA_H
#define _MVM_METHODAREA_H

#include <inttypes.h>

struct class_table;
struct method_area;

/** creates a new method_area.
 *
 * @return pointer to new method_area, or NULL on error
 */

struct method_area * method_area_create();

/** destroys the given method_area
 *
 * @param m - the method_area to destroy
 */

void method_area_destroy(struct method_area *m);

/** clears the given method_area by unloading the currently loaded
 * class file.
 *
 * @param m - the method_area to clear
 */

void method_area_clear(struct method_area *m);

/** loads the class file c into the method area.  Must perform
 * byte-order swapping when loading class file if host architecture is
 * not big-endian.
 *
 * @param m - the method area to load into
 * @param c - path to class file as a c-string
 *
 * @return 0 on success, non-zero on failure
 */

int method_area_load_class_file(struct method_area *m, char *c);

/** returns the 32-bit unsigned integer at address a in the class file.
 * Addresses are given in byte-offsets from the start of the class,
 * starting at 0.
 *
 * @param m - the method_area to fetch from
 * @param a - the address to fetch
 *
 * @return value at requested address
 */

uint32_t method_area_fetch(struct method_area *m, uint32_t a);

/** writes the word n to address a of the given method_area.  Returns
 * the old value at the given address.
 *
 * @param m - the method_area to modify
 * @param a - the address to write to
 * @param n - the new value to be written
 *
 * @return the old value at address a
 */

int method_area_write(struct method_area *m, uint32_t a, uint32_t n);

/** checks if a is a valid address in the given method_area.
 *
 * @param m - the method_area to access
 * @param a - the address to check
 *
 * @return 0 if the address is invalid, non-zero otherwise
 */

int method_area_address_is_valid(struct method_area *m, uint32_t a);

/** returns the size of the method area in bytes.
 *
 * @param m - the method_area to access
 *
 * @return method_area size in bytes
 */

int method_area_size(struct method_area *m);

/** returns the name of the currently loaded class file.
 *
 * @param m - the method_area to access
 *
 * @return name of loaded class file as c-string, or NULL if none
 * currently loaded
 */

char * method_area_get_class_file(struct method_area *m);

/** processes the class table in the currently loaded class file and
 * generates a new class_table from it.  Implementations must ensure
 * that class_table_analyze is called on the new class_table before
 * returning it.
 *
 * @param m - the method_area to generate a class_table from
 *
 * @return pointer to new class_table, or NULL on error
 */

struct class_table * method_area_generate_class_table(struct method_area *m);

/** returns the address of the main block as indicated by the currently
 * loaded class file.
 *
 * @param m - the method_area to access
 *
 * @return address of the main block
 */

uint32_t method_area_get_main_block_address(struct method_area *m);

/** returns the max locals count of the main block as indicated by the
 * currently loaded class file.
 *
 * @param m - the method_area to access
 *
 * @return max locals count of the main block
 */

uint32_t method_area_get_main_block_max_locals(struct method_area *m);

/** dumps the current state of the method_area.
 *
 * @param m - the method_area to dump
 *
 * @return 0 on success, non-zero on failure
 */

int method_area_dump(struct method_area *m);

/** acquires a lock which prevents other threads from accessing the
 * given method_area.  When done, release lock with method_area_lock.
 *
 * @param m - the method_area to lock
 *
 * @return 0 on success, non-zero on failure.
 */

int method_area_lock(struct method_area *m);

/** releases the lock acquired by method_area_lock, allowing other
 * threads to access the given method_area.
 *
 * @param m - the method_area to unlock
 *
 * @return 0 on success, non-zero on failure.
 */

int method_area_unlock(struct method_area *m);

#endif
