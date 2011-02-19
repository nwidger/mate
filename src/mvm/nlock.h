/* Niels Widger
 * Time-stamp: <03 Feb 2011 at 19:26:01 by nwidger on macros.local>
 */

#ifndef _MVM_NLOCK_H
#define _MVM_NLOCK_H

struct nlock;

/** creates a new, unlocked nlock.
 *
 * An nlock is a slightly modified recursive mutex.  With a recursive
 * mutex, a thread attempting to relock the mutex without first
 * unlocking it will succeed.  Multiple locks require the same number
 * of unlocks to release the mutex before another thread can acquire
 * the mutex.
 *
 * An nlock differs in that it can be explicitly released ("fully
 * unlocked") by the owning thread with a single call to
 * nlock_release.  Doing so allows another thread to acquire the
 * mutex.  This is useful in situations where the thread would like to
 * temporarily release the mutex, but several recursive locks have
 * taken place and therefore a single unlock will not work.  An nlock
 * solves this problem by keeping a count of the number of recursive
 * locks made by a thread.  A call to nlock_release unlocks the mutex
 * the same number of times, therefore releasing the mutex from the
 * thread's control.  When the thread needs to reacquire the mutex, a
 * call to nlock_reacquire is made which relocks the mutex the correct
 * number of times.  A call to nlock_reacquire must be made after the
 * call to nlock_release to reacquire the mutex before any further
 * nlock operations are performed by the calling thread.
 *
 * @return pointer to new nlock, or NULL on failure
 */

struct nlock * nlock_create();

/** destroys the given nlock.
 */

void nlock_destroy(struct nlock *n);

/** clears the given nlock.
 */

void nlock_clear(struct nlock *n);

/** locks the given nlock
 *
 * @param n - the nlock to lock
 *
 * @return 0 on success, non-zero on failure
 */

int nlock_lock(struct nlock *n);

/** non-blocking attempt to lock the given nlock
 *
 * @param n - the nlock to try to lock
 *
 * @return 0 on success, non-zero on failure
 */
int nlock_trylock(struct nlock *n);

/** unlocks the given nlock.
 *
 * @param n - the nlock to unlock
 *
 * @return 0 on success, non-zero on failure
 */

int nlock_unlock(struct nlock *n);

/** releases the given nlock.
 *
 * @param n - the nlock to release
 *
 * @return lock count on success, or -1 on failure
 */

int nlock_release(struct nlock *n);

/** reacquires the given nlock.
 *
 * @param n - the nlock to reacquire
 * @param l - lock count, as returned by nlock_release
 *
 * @return 0 on success, non-zero on failure
 */

int nlock_reacquire(struct nlock *n, int l);

/** causes the calling thread to wait until another thread calls
 * nlock_notify or nlock_notify_all on the given nlock.
 *
 * @param n - the nlock to wait on
 *
 * @return 0 on success, non-zero on failure 
 */

int nlock_wait(struct nlock *n);

/** causes the calling thread to wait until another thread calls
 * nlock_notify or nlock_notify_all on the given nlock or t
 * milliseconds have gone by.
 *
 * @param n - the nlock to wait on
 * @param t - wait timeout in milliseconds
 *
 * @return 0 on success, non-zero on failure 
 */

int nlock_timedwait(struct nlock *n, long t);

/** wakes up a single thread waiting on the given nlock via a call to
 * nlock_wait or nlock_timedwait.
 *
 * @param n - notify threads waiting on this nlock
 *
 * @return 0 on success, non-zero on failure 
 */

int nlock_notify(struct nlock *n);


/** wakes up all threads waiting on the given nlock via a call to
 * nlock_wait or nlock_timedwait.
 *
 * @param n - notify threads waiting on this nlock 
 *
 * @return 0 on success, non-zero on failure 
 */

int nlock_notify_all(struct nlock *n);

#endif
