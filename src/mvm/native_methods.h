/* Niels Widger
 * Time-stamp: <21 Nov 2010 at 23:03:19 by nwidger on macros.local>
 */

#ifndef _MVM_NATIVEMETHODS_H
#define _MVM_NATIVEMETHODS_H

#include <inttypes.h>

/* these functions implement each native method of the maTe language */

int native_object_constructor(uint32_t i);
int native_object_equals(uint32_t i);
int native_object_hash_code(uint32_t i);
int native_object_to_string(uint32_t i);

int native_integer_constructor(uint32_t i);
int native_integer_constructor_integer(uint32_t i);
int native_integer_add(uint32_t i);
int native_integer_subtract(uint32_t i);
int native_integer_multiply(uint32_t i);
int native_integer_divide(uint32_t i);
int native_integer_greater_than(uint32_t i);
int native_integer_less_than(uint32_t i);
int native_integer_not(uint32_t i);
int native_integer_minus(uint32_t i);
int native_integer_equals(uint32_t i);
int native_integer_hash_code(uint32_t i);
int native_integer_to_string(uint32_t i);

int native_string_constructor_string(uint32_t i);
int native_string_length(uint32_t i);
int native_string_substr(uint32_t i);
int native_string_to_integer(uint32_t i);
int native_string_concat(uint32_t i);
int native_string_greater_than(uint32_t i);
int native_string_less_than(uint32_t i);
int native_string_hash_code(uint32_t i);
int native_string_equals(uint32_t i);
int native_string_to_string(uint32_t i);

int native_table_constructor(uint32_t i);
int native_table_constructor_integer(uint32_t i);
int native_table_get(uint32_t i);
int native_table_put(uint32_t i);
int native_table_remove(uint32_t i);
int native_table_first_key(uint32_t i);
int native_table_next_key(uint32_t i);

int native_thread_constructor(uint32_t i);
int native_thread_start(uint32_t i);
int native_thread_run(uint32_t i);
int native_thread_join(uint32_t i);

/** adds all necessary native methods to the given native_method_array.
 *
 * @param n - the native_method_array to add to
 *
 * @return 0 on success, non-zero on failure
 */

int add_native_methods(struct native_method_array *n);

#endif
