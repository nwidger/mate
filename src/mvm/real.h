/* Niels Widger
 * Time-stamp: <23 Dec 2010 at 17:50:18 by nwidger on macros.local>
 */

#ifndef _MVM_REAL_H
#define _MVM_REAL_H

struct real;
struct object;

/** creates a new real with value v.
 *
 * @param v - the value of the new real
 *
 * @return pointer to new real, or NULL on failure
 */

struct real * real_create(float v);

/** destroys the given real.
 *
 * @param r - the real to destroy
 */

void real_destroy(struct real *r);

/** clears the given real.
 *
 * @param r - the real to clear
 */

void real_clear(struct real *r);

/** returns the current value of the given real.
 *
 * @param r - the real to access
 *
 * @return real's current value
 */

float real_get_value(struct real *r);

/** returns a reference to a new Real object whose value is the
 * addition of the value of o and p.  o and p must be pointers to
 * Real objects.
 *
 * @param o - first Real object
 * @param p - second Real object
 *
 * @return reference to new Real object, or 0 on failure
 */

int real_add(struct object *o, struct object *p);

/** returns a reference to a new Real object whose value is the
 * subtraction of the value of p from o.  o and p must be pointers to
 * Real objects.
 *
 * @param o - first Real object
 * @param p - second Real object
 *
 * @return reference to new Real object, or 0 on failure
 */

int real_subtract(struct object *o, struct object *p);

/** returns a reference to a new Real object whose value is the
 * multiplication of the value of o and p.  o and p must be pointers
 * to Real objects.
 *
 * @param o - first Real object
 * @param p - second Real object
 *
 * @return reference to new Real object, or 0 on failure
 */

int real_multiply(struct object *o, struct object *p);

/** returns a reference to a new Real object whose value is the
 * division of the value of o by p.  o and p must be pointers
 * to Real objects.  Division by zero causes a failure.
 *
 * @param o - first Real object
 * @param p - second Real object
 *
 * @return reference to new Real object, or 0 on failure
 */

int real_divide(struct object *o, struct object *p);

/** returns a reference to a new Real object whose value is 1 if the
 * value of o is greater than the value of p, and 0 otherwise.  o and
 * p must be pointers to Real objects.
 *
 * @param o - first Real object
 * @param p - second Real object
 *
 * @return reference to new Real object, or 0 on failure
 */

int real_greater_than(struct object *o, struct object *p);

/** returns a reference to a new Real object whose value is 1 if the
 * value of o is less than the value of p, and 0 otherwise.  o and
 * p must be pointers to Real objects.
 *
 * @param o - first Real object
 * @param p - second Real object
 *
 * @return reference to new Real object, or 0 on failure
 */

int real_less_than(struct object *o, struct object *p);

/** returns a reference to a new Real object whose value is 1 if the
 * value of o is 0, and 0 otherwise.  o must be a pointer to an
 * Real object.
 *
 * @param o - an Real object
 *
 * @return reference to new Real object, or 0 on failure
 */

int real_not(struct object *o);

/** returns a reference to a new Real object whose value is the
 * arithmetic negation of the value of o.  o must be a pointer to an
 * Real object.
 *
 * @param o - an Real object
 *
 * @return reference to new Real object, or 0 on failure
 */

int real_minus(struct object *o);

/** returns a reference to a new Real object whose value is 1 if the
 * value of o is equal to the value of p, and 0 otherwise.  o and p
 * must be pointers to Real objects.
 *
 * @param o - first Real object
 * @param p - second Real object
 *
 * @return reference to new Real object, or 0 on failure
 */

int real_equals(struct object *o, struct object *p);

/** returns a reference to a new Real object whose value is the
 * value of o.  o must be a pointer to an Real object.
 *
 * @param o - an Real object
 *
 * @return reference to new Real object, or 0 on failure
 */

int real_hash_code(struct object *o);

/** returns a reference to a new String object whose characters are the
 * string representation of the value of o.  o must be a pointer
 * to an Real object.
 *
 * @param o - an Real object
 *
 * @return reference to new String object, or 0 on failure
 */

int real_to_string(struct object *o);

#endif
