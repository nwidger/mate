/* Niels Widger
 * Time-stamp: <29 Jan 2012 at 14:16:49 by nwidger on macros.local>
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
struct table;
struct table_dmp;
struct table_dmp_attr;
struct thread;
struct thread_dmp;
struct thread_dmp_attr;

/* enums */
enum dmp_mode {
	parallel_mode	= 0,
	serial_mode	= 1
};

enum dmp_thread_action {
	block_action   = 0,
	proceed_action = 1
};

enum dmp_owner_action {
	no_action      = 0,
	shared_action  = 1,
	private_action = 2
};

struct dmp * dmp_create(struct object_dmp_attr *a,
			struct thread_dmp_attr *t,
			struct nlock_dmp_attr *n,
			struct table_dmp_attr *tb);
void dmp_destroy(struct dmp *d);
void dmp_clear(struct dmp *d);
int dmp_get_mode(struct dmp *d);
int dmp_add_thread(struct dmp *d, int r);
int dmp_remove_thread(struct dmp *d, int r);
struct object_dmp * dmp_create_object_dmp(struct dmp *d, struct object *o);
struct thread_dmp * dmp_create_thread_dmp(struct dmp *d, struct thread *t);
struct nlock_dmp * dmp_create_nlock_dmp(struct dmp *d, struct nlock *n);
struct table_dmp * dmp_create_table_dmp(struct dmp *d, struct table *t, struct object_dmp *od);
int dmp_thread_block(struct dmp *d, struct thread_dmp *td);
int dmp_shm_read(struct dmp *d, int c, int r, enum dmp_thread_action *ta, enum dmp_owner_action *oa);
int dmp_shm_write(struct dmp *d, int c, int r, enum dmp_thread_action *ta, enum dmp_owner_action *oa);

#endif
