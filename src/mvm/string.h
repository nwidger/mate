/* Niels Widger
 * Time-stamp: <18 Feb 2010 at 14:05:34 by nwidger on macros.local>
 */

#ifndef _MVM_STRING_H
#define _MVM_STRING_H

#include <inttypes.h>

struct object;
struct string;

/** creates a new string whose characters are the characters in c.
 *
 * @param c - characters in the new string as a c-string
 *
 * @return pointer to new string, or NULL on failure
 */

struct string * string_create(char *c);

/** destroys the given string.
 *
 * @param s - the string destroy
 */

void string_destroy(struct string *s);

/** clears the given string.
 *
 * @param s - the string to clear
 */

void string_clear(struct string *s);

/** returns a c-string containing the characters in the given string.
 *
 * @param s - the string to access
 *
 * @return c-string of characters in the given string
 */

char * string_get_chars(struct string *s);

/** returns a reference to a new Integer object whose value is the
 * length of the string o.  o must be a pointer to a String object.
 *
 * @param o - a String object
 *
 * @return reference to new Integer object, or 0 on failure
 */

int string_length(struct object *o);

/** returns a reference to a new String object whose characters are the
 * characters in o whose indices are between the value of p and q.  o
 * must be a pointer to a String object.  p and q must be pointers to
 * Integer objects.
 *
 * @param o - a String object
 * @param p - first Integer object
 * @param q - second Integer object
 *
 * @return reference to new String object, or 0 on failure
 */

int string_substr(struct object *o, struct object *p, struct object *q);

/** returns a reference to a new String object whose characters are the
 * characters in o concatenated with the characters in p.  o and p
 * must be pointers to String objects.
 *
 * @param o - first String object
 * @param p - second String object
 *
 * @return reference to new String object, or 0 on failure
 */

int string_concat(struct object *o, struct object *p);

/** returns a reference to a new Integer object whose value is the
 * decimal value obtained by parsing the characters in o as a signed
 * decimal integer.  o must be a pointer to a String object.
 *
 * @param o - a String object
 *
 * @return reference to new Integer object, or 0 on failure
 */

int string_to_integer(struct object *o);

/** returns a reference to a new Integer object whose value is 1 if the
 * characters in p lexicographically precede the characters in o, and
 * 0 otherwise.  o and p must be pointers to String objects.
 *
 * @param o - first String object
 * @param p - second String object
 *
 * @return reference to new Integer object, or 0 on failure
 */

int string_greater_than(struct object *o, struct object *p);

/** returns a reference to a new Integer object whose value is 1 if the
 * characters in o lexicographically precede the characters in p, and
 * 0 otherwise.  o and p must be pointers to String objects.
 *
 * @param o - first String object
 * @param p - second String object
 *
 * @return reference to new Integer object, or 0 on failure
 */

int string_less_than(struct object *o, struct object *p);

/** returns a reference to a new Integer object whose value is the
 * summation of the ordinal values of the characters in o.  o must be
 * a pointer to a String object.
 *
 * @param o - a String object
 *
 * @return reference to new Integer object, or 0 on failure
 */

int string_hash_code(struct object *o);

/** returns a reference to a new Integer object whose value is 1 if the
 * characters in o are lexicographically equal to the characters in p,
 * and 0 otherwise.  o and p must be pointers to String objects.
 *
 * @param o - first String object
 * @param p - second String object
 *
 * @return reference to new Integer object, or 0 on failure
 */

int string_equals(struct object *o, struct object *p);

/** returns a reference to a new String object whose characters are
 * exactly the characters in o. o must be a pointer to a String
 * object.
 *
 * @param o - a String object
 *
 * @return reference to new String object, or 0 on failure
 */

int string_to_string(struct object *o);

#endif
