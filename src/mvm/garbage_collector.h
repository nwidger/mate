/* Niels Widger
 * Time-stamp: <19 Feb 2010 at 15:16:58 by nwidger on macros.local>
 */

#ifndef _MVM_GARBAGECOLLECTOR_H
#define _MVM_GARBAGECOLLECTOR_H

/* enums */
/** defines concurrency of garbage collector */
enum garbage_collector_type {
	/** run in new thread,
	 * collect at regular intervals or when explicitly invoked
	 */
	concurrent_type = 0,
	/** run in same thread,
	 *  collect only when explicitly invoked
	 */
	serial_type     = 1
};

struct garbage_collector;

/** creates a new garbage_collector.
 *
 * @return pointer to the new garbage_collector, or NULL on failure
 */

struct garbage_collector * garbage_collector_create();

/** destroys the given garbage_collector.
 *
 * @param g - the garbage_collector to destroy
 */

void garbage_collector_destroy(struct garbage_collector *g);

/** clears the given garbage_collector.
 *
 * @param g - the garbage_collector to clear
 */

void garbage_collector_clear(struct garbage_collector *g);

/** checks if the given garbage_collector is currently running.
 *
 * @param g - the garbage_collector to check
 *
 * @return 1 if the garbage_collector is running, 0 otherwise
 */

int garbage_collector_is_running(struct garbage_collector *g);

/** starts the garbage_collector.  If t is 'concurrent_type', the
 * garbage_collector will perform a collection every i seconds.  If t
 * is 'serial_type', the garbage_collector will only perform a
 * collection by an explicit call to garbage_collector_collect_now.
 *
 * @param g - the garbage_collector to start
 * @param t - the type of the garbage_collector.  Must be one of
 * 'concurrent_type' or 'serial_type'
 * @param i - the sleep interval of the garbage collector if t is
 * 'concurrent_type'.  Otherwise this argument is ignored.
 *
 * @return 0 on success, non-zero on failure
 * @see garbage_collector_type
 */

int garbage_collector_start(struct garbage_collector *g,
			    enum garbage_collector_type t, int i);

/** stops the garbage_collector.
 *
 * @param g - the garbage_collector to stop
 *
 * @return 0 on success, non-zero on failure
 */

int garbage_collector_stop(struct garbage_collector *g);

/** schedules an immediate collection phase, blocking the calling
 * thread until collection has finished.
 *
 * @param g - the garbage_collector to use
 *
 * @return 0 on success, non-zero on failure
 */

int garbage_collector_collect_now(struct garbage_collector *g);

/** dumps the current state of the garbage_collector to standard
 * error.
 *
 * @param g - the garbage_collector to dump
 *
 * @return 0 on success, non-zero on failure
 */

int garbage_collector_dump(struct garbage_collector *g);

int garbage_collector_lock(struct garbage_collector *g);
int garbage_collector_unlock(struct garbage_collector *g);
int garbage_collector_release(struct garbage_collector *g);
int garbage_collector_reacquire(struct garbage_collector *g, int l);

#endif
