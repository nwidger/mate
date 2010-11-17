/* Niels Widger
 * Time-stamp: <18 Feb 2010 at 13:45:15 by nwidger on macros.local>
 */

#ifndef _MVM_INTEGER_H
#define _MVM_INTEGER_H

#include <inttypes.h>

struct integer;
struct object;

/** creates a new integer with value v.
 *
 * @param v - the value of the new integer
 *
 * @return pointer to new integer, or NULL on failure
 */

struct integer * integer_create(int32_t v);

/** destroys the given integer.
 *
 * @param i - the integer to destroy
 */

void integer_destroy(struct integer *i);

/** clears the given integer.
 *
 * @param i - the integer to clear
 */

void integer_clear(struct integer *i);

/** returns the current value of the given integer.
 *
 * @param i - the integer to access
 *
 * @return integer's current value
 */

int32_t integer_get_value(struct integer *i);

/** returns a reference to a new Integer object whose value is the
 * addition of the value of o and p.  o and p must be pointers to
 * Integer objects.
 *
 * @param o - first Integer object
 * @param p - second Integer object
 *
 * @return reference to new Integer object, or 0 on failure
 */

int integer_add(struct object *o, struct object *p);

/** returns a reference to a new Integer object whose value is the
 * subtraction of the value of p from o.  o and p must be pointers to
 * Integer objects.
 *
 * @param o - first Integer object
 * @param p - second Integer object
 *
 * @return reference to new Integer object, or 0 on failure
 */

int integer_subtract(struct object *o, struct object *p);

/** returns a reference to a new Integer object whose value is the
 * multiplication of the value of o and p.  o and p must be pointers
 * to Integer objects.
 *
 * @param o - first Integer object
 * @param p - second Integer object
 *
 * @return reference to new Integer object, or 0 on failure
 */

int integer_multiply(struct object *o, struct object *p);

/** returns a reference to a new Integer object whose value is the
 * division of the value of o by p.  o and p must be pointers
 * to Integer objects.  Division by zero causes a failure.
 *
 * @param o - first Integer object
 * @param p - second Integer object
 *
 * @return reference to new Integer object, or 0 on failure
 */

int integer_divide(struct object *o, struct object *p);

/** returns a reference to a new Integer object whose value is 1 if the
 * value of o is greater than the value of p, and 0 otherwise.  o and
 * p must be pointers to Integer objects.
 *
 * @param o - first Integer object
 * @param p - second Integer object
 *
 * @return reference to new Integer object, or 0 on failure
 */

int integer_greater_than(struct object *o, struct object *p);

/** returns a reference to a new Integer object whose value is 1 if the
 * value of o is less than the value of p, and 0 otherwise.  o and
 * p must be pointers to Integer objects.
 *
 * @param o - first Integer object
 * @param p - second Integer object
 *
 * @return reference to new Integer object, or 0 on failure
 */

int integer_less_than(struct object *o, struct object *p);

/** returns a reference to a new Integer object whose value is 1 if the
 * value of o is 0, and 0 otherwise.  o must be a pointer to an
 * Integer object.
 *
 * @param o - an Integer object
 *
 * @return reference to new Integer object, or 0 on failure
 */

int integer_not(struct object *o);

/** returns a reference to a new Integer object whose value is the
 * arithmetic negation of the value of o.  o must be a pointer to an
 * Integer object.
 *
 * @param o - an Integer object
 *
 * @return reference to new Integer object, or 0 on failure
 */

int integer_minus(struct object *o);

/** returns a reference to a new Integer object whose value is 1 if the
 * value of o is equal to the value of p, and 0 otherwise.  o and p
 * must be pointers to Integer objects.
 *
 * @param o - first Integer object
 * @param p - second Integer object
 *
 * @return reference to new Integer object, or 0 on failure
 */

int integer_equals(struct object *o, struct object *p);

/** returns a reference to a new Integer object whose value is the
 * value of o.  o must be a pointer to an Integer object.
 *
 * @param o - an Integer object
 *
 * @return reference to new Integer object, or 0 on failure
 */

int integer_hash_code(struct object *o);

/** returns a reference to a new String object whose characters are the
 * string representation of the value of o.  o must be a pointer
 * to an Integer object.
 *
 * @param o - an Integer object
 *
 * @return reference to new String object, or 0 on failure
 */

int integer_to_string(struct object *o);

#endif
