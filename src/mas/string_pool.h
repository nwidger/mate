/* Niels Widger
 * Time-stamp: <20 Feb 2010 at 12:34:22 by nwidger on macros.local>
 */

#ifndef _STRINGPOOL_H
#define _STRINGPOOL_H

struct string_pool;

/** create a new string pool
 * 
 * @return pointer to new table on success, NULL on failure
 */

struct string_pool * string_pool_create();

/** destroy given table
 * 
 * @param p - table to destroy
 */

void string_pool_destroy(struct string_pool *p);

/** remove all strings from given table
 * 
 * @param p - table to clear
 */

void string_pool_clear(struct string_pool *p);

/** add string to given table
 * 
 * @param p - table to add the string to
 * @param s - string as c-string
 * 
 * @return pointer to saved string on success, NULL on failure
 */

char * string_pool_add(struct string_pool *p, char *s);

#endif
