/* Niels Widger
 * Time-stamp: <29 Jan 2012 at 14:18:36 by nwidger on macros.local>
 */

#ifndef _MVM_TABLE_DMP_H
#define _MVM_TABLE_DMP_H

struct object_dmp;
struct table;
struct table_dmp;
struct table_dmp_ops;

/* struct definitions */
struct table_dmp_ops {
	int  (*load)(struct table_dmp *td);
	int (*store)(struct table_dmp *td);
};

struct table_dmp_attr {
	struct table_dmp_ops *ops;
};

struct table_dmp * table_dmp_create(struct table *o, struct table_dmp_attr *a,
				    struct object_dmp *od);
void table_dmp_destroy(struct table_dmp *td);
void table_dmp_clear(struct table_dmp *td);
int table_dmp_load(struct table_dmp *td);
int table_dmp_store(struct table_dmp *td);

/* default ops */
extern struct table_dmp_ops table_dmp_default_ops;

/* default attr */
extern struct table_dmp_attr table_dmp_default_attr;

/* default functions */
int table_dmp_default_load(struct table_dmp *td);
int table_dmp_default_store(struct table_dmp *td);

#endif
