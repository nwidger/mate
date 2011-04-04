/* Niels Widger
 * Time-stamp: <04 Apr 2011 at 18:55:56 by nwidger on macros.local>
 */

#ifndef _MVM_NLOCK_DMP_H
#define _MVM_NLOCK_DMP_H

struct nlock;
struct nlock_dmp;
struct nlock_dmp_ops;

/* struct definitions */
struct nlock_dmp_ops {
	int  (*lock)(struct nlock_dmp *nd);
	int  (*unlock)(struct nlock_dmp *nd);	
};

struct nlock_dmp_attr {
	struct nlock_dmp_ops *ops;
};

struct nlock_dmp * nlock_dmp_create(struct nlock *n, struct nlock_dmp_attr *a);
void nlock_dmp_destroy(struct nlock_dmp *od);
void nlock_dmp_clear(struct nlock_dmp *od);
struct nlock * nlock_dmp_get_nlock(struct nlock_dmp *nd);
int nlock_dmp_set_nlock(struct nlock_dmp *nd, struct nlock *n);
int nlock_dmp_lock(struct nlock_dmp *nd);
int nlock_dmp_unlock(struct nlock_dmp *nd);

/* default ops */
extern struct nlock_dmp_ops nlock_dmp_default_ops;

/* default attr */
extern struct nlock_dmp_attr nlock_dmp_default_attr;

/* default functions */
int nlock_dmp_default_lock(struct nlock_dmp *nd);
int nlock_dmp_default_unlock(struct nlock_dmp *nd);

#endif
