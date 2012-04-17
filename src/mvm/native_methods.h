/* Niels Widger
 * Time-stamp: <12 Apr 2012 at 19:33:13 by nwidger on macros.local>
 */

#ifndef _MVM_NATIVEMETHODS_H
#define _MVM_NATIVEMETHODS_H

#include <inttypes.h>

struct native_method_array;

/* these functions implement each native method of the maTe language */

int native_object_constructor(uint32_t i);
int native_object_equals(uint32_t i);
int native_object_hash_code(uint32_t i);
int native_object_to_string(uint32_t i);
int native_object_notify(uint32_t i);
int native_object_notify_all(uint32_t i);
int native_object_wait(uint32_t i);
int native_object_wait_integer(uint32_t i);

int native_integer_constructor(uint32_t i);
int native_integer_constructor_integer(uint32_t i);
int native_integer_add(uint32_t i);
int native_integer_subtract(uint32_t i);
int native_integer_multiply(uint32_t i);
int native_integer_divide(uint32_t i);
int native_integer_greater_than(uint32_t i);
int native_integer_less_than(uint32_t i);
int native_integer_greater_than_equal(uint32_t i);
int native_integer_less_than_equal(uint32_t i);
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
int native_thread_sleep(uint32_t i);

int native_real_constructor(uint32_t i);
int native_real_constructor_real(uint32_t i);
int native_real_add(uint32_t i);
int native_real_subtract(uint32_t i);
int native_real_multiply(uint32_t i);
int native_real_divide(uint32_t i);
int native_real_greater_than(uint32_t i);
int native_real_less_than(uint32_t i);
int native_real_greater_than_equal(uint32_t i);
int native_real_less_than_equal(uint32_t i);
int native_real_not(uint32_t i);
int native_real_minus(uint32_t i);
int native_real_equals(uint32_t i);
int native_real_hash_code(uint32_t i);
int native_real_to_string(uint32_t i);

/** adds all necessary native methods to the given native_method_array.
 *
 * @param n - the native_method_array to add to
 *
 * @return 0 on success, non-zero on failure
 */

int add_native_methods(struct native_method_array *n);

int native_methods_parse_predefined_classes_file();
int native_methods_load_predefined_classes_file(struct native_method_array *n);

/* private code used by parse.y */
struct native_class;
struct native_method;

struct native_class * native_class_create(char *n, struct native_method *m, struct native_class *x);
int native_class_set_next(struct native_class *c, struct native_class *x);

struct native_method * native_method_create(unsigned long i, char *n, struct native_method *x);
int native_method_set_next(struct native_method *c, struct native_method *x);

#endif
