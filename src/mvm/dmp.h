/* Niels Widger
 * Time-stamp: <27 Jan 2012 at 20:22:39 by nwidger on macros.local>
 */

#ifndef _MVM_DMP_H
#define _MVM_DMP_H

struct dmp;
struct nlock;
struct nlock_dmp;
struct nlock_dmp_attr;
struct object;
struct object_dmp;
struct object_dmp_attr;
struct thread;
struct thread_dmp;
struct thread_dmp_attr;

/* enums */
enum dmp_mode {
	parallel_mode	= 0,
	serial_mode	= 1
};

struct dmp * dmp_create(struct object_dmp_attr *a,
			struct thread_dmp_attr *t,
			struct nlock_dmp_attr *n);
void dmp_destroy(struct dmp *d);
void dmp_clear(struct dmp *d);
int dmp_get_mode(struct dmp *d);
int dmp_add_thread(struct dmp *d, int r);
int dmp_remove_thread(struct dmp *d, int r);
struct object_dmp * dmp_create_object_dmp(struct dmp *d, struct object *o);
struct thread_dmp * dmp_create_thread_dmp(struct dmp *d, struct thread *t);
struct nlock_dmp * dmp_create_nlock_dmp(struct dmp *d, struct nlock *n);
int dmp_thread_block(struct dmp *d, struct thread_dmp *td);

#endif
