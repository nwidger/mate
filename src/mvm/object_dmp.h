/* Niels Widger
 * Time-stamp: <04 Apr 2011 at 14:12:32 by nwidger on macros.local>
 */

#ifndef _MVM_OBJECT_DMP_H
#define _MVM_OBJECT_DMP_H

struct object;
struct object_dmp;
struct object_dmp_ops;

/* constants */
#define OBJECT_DMP_DEFAULT_OWNER -1 /* -1 == allocating thread */
#define OBJECT_DMP_DEFAULT_DEPTH 1

/** for accessing owner field */
#define SHARED(x) (x == 0)
#define PRIVATE(x) (x != 0)

/* struct definitions */
struct object_dmp_ops {
	int  (*load)(struct object_dmp *od, int i);
	int (*store)(struct object_dmp *od, int i, int r);
	int (*chown)(struct object_dmp *od, int n);	
};

struct object_dmp_attr {
	int owner;
	int depth;
	struct object_dmp_ops *ops;
};

struct object_dmp * object_dmp_create(struct object *o, struct object_dmp_attr *a);
void object_dmp_destroy(struct object_dmp *od);
void object_dmp_clear(struct object_dmp *od);
int object_dmp_get_owner(struct object_dmp *od);
int object_dmp_load(struct object_dmp *od, int i);
int object_dmp_store(struct object_dmp *od, int i, int r);
int object_dmp_chown(struct object_dmp *od, int i);

/* default ops */
extern struct object_dmp_ops object_dmp_default_ops;

/* default attr */
extern struct object_dmp_attr object_dmp_default_attr;

/* default functions */
int object_dmp_default_load(struct object_dmp *od, int i);
int object_dmp_default_store(struct object_dmp *od, int i, int r);
int object_dmp_default_chown(struct object_dmp *od, int n);

#endif
