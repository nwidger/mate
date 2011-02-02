/* Niels Widger
 * Time-stamp: <02 Feb 2011 at 13:09:55 by nwidger on macros.local>
 */

#ifndef _MVM_OBJECT_DMP_H
#define _MVM_OBJECT_DMP_H

struct object;
struct object_dmp;
struct object_dmp_ops;
struct thread;

/* constants */
/** for accessing owner field */
#define SHARED(x) (x == 0)
#define PRIVATE(x) (x != 0)

/* struct definitions */
struct object_dmp_ops {
	int  (*load)(struct object_dmp *od, struct thread *t, int i);
	int (*store)(struct object_dmp *od, struct thread *t, int i);
	int (*chown)(struct object_dmp *od, struct thread *t, int n);	
};

struct object_dmp_attr {
	int owner;
	int depth;
	struct object_dmp_ops *ops;
};

struct object_dmp * object_dmp_create(struct object *o, struct object_dmp_attr *a);
void object_dmp_destroy(struct object_dmp *od);
void object_dmp_clear(struct object_dmp *od);
struct object * object_dmp_get_object(struct object_dmp *od);
int object_dmp_set_object(struct object_dmp *od, struct object *o);
int object_dmp_get_owner(struct object_dmp *od);
int object_dmp_set_owner(struct object_dmp *od, int n);
int object_dmp_is_shared(struct object_dmp *od);
int object_dmp_is_private(struct object_dmp *od);
int object_dmp_get_depth(struct object_dmp *od);
int object_dmp_set_depth(struct object_dmp *od, int d);
struct object_dmp_ops * object_dmp_get_ops(struct object_dmp *od);
int object_dmp_set_ops(struct object_dmp *od, struct object_dmp_ops *p);
int object_dmp_load(struct object_dmp *od, struct thread *t, int i);
int object_dmp_store(struct object_dmp *od, struct thread *t, int i);
int object_dmp_chown(struct object_dmp *od, struct thread *t, int i);

/* default ops */
extern struct object_dmp_ops object_dmp_default_ops;

int object_dmp_default_load(struct object_dmp *od, struct thread *t, int i);
int object_dmp_default_store(struct object_dmp *od, struct thread *t, int i);
int object_dmp_default_chown(struct object_dmp *od, struct thread *t, int n);

#endif
