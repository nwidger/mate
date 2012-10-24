/* Niels Widger
 * Time-stamp: <21 Oct 2012 at 10:36:51 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "barrier.h"
#include "constants.h"
#include "dmp.h"
#include "garbage_collector.h"
#include "globals.h"
#include "heap.h"
#include "nlock_dmp.h"
#include "object_dmp.h"
#include "ref_set.h"
#include "table_dmp.h"
#include "thread_dmp.h"

/* struct definitions */
struct dmp_shm_stats {
	uint32_t total;

	uint32_t blocks;
	uint32_t proceeds;
	
	uint32_t shared;
	uint32_t private_mine;
	uint32_t private_not_mine;
};

struct dmp_stats {
	uint32_t rounds;

	uint32_t parallel_modes;
	uint32_t serial_modes;

	struct dmp_shm_stats reads;
	struct dmp_shm_stats writes;
};

struct dmp {
	enum dmp_mode mode;

	struct ref_set *thread_set;

	int first;
	struct barrier *barrier;

	/* object_dmp defaults */
	struct object_dmp_attr *od_attr;

	/* thread_dmp defaults */
	struct thread_dmp_attr *td_attr;

	/* nlock_dmp defaults */
	struct nlock_dmp_attr *nd_attr;

	/* table_dmp defaults */
	struct table_dmp_attr *tb_attr;

	struct dmp_stats *stats;
};

/* forward declarations */
int dmp_acquiesce(struct dmp *d, int r, enum thread_dmp_state s);
int dmp_toggle_mode(struct dmp *d);
void * dmp_barrier_parallel_hook(int i, void *a);
void * dmp_barrier_serial_hook(int i, void *a);

struct dmp * dmp_create(struct object_dmp_attr *a,
			struct thread_dmp_attr *t,
			struct nlock_dmp_attr *n,
			struct table_dmp_attr *tb,
			int s) {
	struct dmp *d;

	if ((d = (struct dmp *)malloc(sizeof(struct dmp))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	d->mode = parallel_mode;

	d->thread_set = ref_set_create();

	d->first = 0;
	d->barrier = barrier_create(0);
	barrier_set_hook(d->barrier, dmp_barrier_parallel_hook, d);

	d->od_attr = a;
	d->td_attr = t;
	d->nd_attr = n;
	d->tb_attr = tb;

	if (s == 0) {
		d->stats = NULL;
	} else {
		if ((d->stats = (struct dmp_stats *)calloc(1, sizeof(struct dmp_stats))) == NULL) {
			perror("mvm: malloc");
			mvm_halt();
		}
	}

	return d;
}

void dmp_destroy(struct dmp *d) {
	if (d != NULL) {
		ref_set_destroy(d->thread_set);
		barrier_destroy(d->barrier);
		free(d);
	}
}

void dmp_clear(struct dmp *d) {
	if (d != NULL) {
		ref_set_clear(d->thread_set);
		barrier_clear(d->barrier);
		d->od_attr = NULL;
		d->td_attr = NULL;
		d->nd_attr = NULL;
		d->tb_attr = NULL;
	}
}

float dmp_percentage(float part, float total) {
	return (part / total) * 100.0;
}

void dmp_dump_stats(struct dmp *d) {
	struct dmp_stats *stats;
	
	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	if (d->stats == NULL)
		return;

	stats = d->stats;

	fprintf(stdout, "\n");

	fprintf(stdout, "%-30s %10" PRIu32 "\n", "Rounds:", stats->rounds);

	fprintf(stdout, "\n");

	fprintf(stdout, "%-30s %10" PRIu32 " (%5.5f%%)\n", "Reads:", stats->reads.total,
		dmp_percentage(stats->reads.total, stats->reads.total+stats->writes.total));
	fprintf(stdout, "%-30s %10" PRIu32 " (%5.5f%%)\n", "Writes:", stats->writes.total,
		dmp_percentage(stats->writes.total, stats->reads.total+stats->writes.total));
	fprintf(stdout, "============================================================\n");
	fprintf(stdout, "%-30s %10" PRIu32 " (100.0%%)\n", "Total:", stats->reads.total+stats->writes.total);

	fprintf(stdout, "\n");

	fprintf(stdout, "%-30s %10" PRIu32 " (%5.5f%%)\n", "Total (shared):", stats->reads.shared+stats->writes.shared,
		dmp_percentage(stats->reads.shared+stats->writes.shared, stats->reads.total+stats->writes.total));
	fprintf(stdout, "%-30s %10" PRIu32 " (%5.5f%%)\n", "Total (private mine):", stats->reads.private_mine+stats->writes.private_mine,
		dmp_percentage(stats->reads.private_mine+stats->writes.private_mine, stats->reads.total+stats->writes.total));
	fprintf(stdout, "%-30s %10" PRIu32 " (%5.5f%%)\n", "Total (private not mine):", stats->reads.private_not_mine+stats->writes.private_not_mine,
		dmp_percentage(stats->reads.private_not_mine+stats->writes.private_not_mine, stats->reads.total+stats->writes.total));
	fprintf(stdout, "============================================================\n");
	fprintf(stdout, "%-30s %10" PRIu32 " (100.0%%)\n", "Total:", stats->reads.total+stats->writes.total);

	fprintf(stdout, "\n");

	fprintf(stdout, "%-30s %10" PRIu32 " (%5.5f%%)\n", "Reads (shared):", stats->reads.shared,
		dmp_percentage(stats->reads.shared, stats->reads.total));
	fprintf(stdout, "%-30s %10" PRIu32 " (%5.5f%%)\n", "Reads (private mine):", stats->reads.private_mine,
		dmp_percentage(stats->reads.private_mine, stats->reads.total));
	fprintf(stdout, "%-30s %10" PRIu32 " (%5.5f%%)\n", "Reads (private not mine):", stats->reads.private_not_mine,
		dmp_percentage(stats->reads.private_not_mine, stats->reads.total));
	fprintf(stdout, "============================================================\n");
	fprintf(stdout, "%-30s %10" PRIu32 " (100.0%%)\n", "Total:", stats->reads.total);

	fprintf(stdout, "\n");

	fprintf(stdout, "%-30s %10" PRIu32 " (%5.5f%%)\n", "Writes (shared):", stats->writes.shared,
		dmp_percentage(stats->writes.shared, stats->writes.total));
	fprintf(stdout, "%-30s %10" PRIu32 " (%5.5f%%)\n", "Writes (private mine):", stats->writes.private_mine,
		dmp_percentage(stats->writes.private_mine, stats->writes.total));
	fprintf(stdout, "%-30s %10" PRIu32 " (%5.5f%%)\n", "Writes (private not mine):", stats->writes.private_not_mine,
		dmp_percentage(stats->writes.private_not_mine, stats->writes.total));
	fprintf(stdout, "============================================================\n");
	fprintf(stdout, "%-30s %10" PRIu32 " (100.0%%)\n", "Total:", stats->writes.total);

	fprintf(stdout, "\n");
}

int dmp_get_mode(struct dmp *d) {
	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	return d->mode;
}

int dmp_toggle_mode(struct dmp *d) {
	struct dmp_stats *stats;
	
	void * (*hook)(int, void *);

	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	stats = d->stats;

	if (stats != NULL) stats->rounds++;

	if (d->mode == parallel_mode) {
		mvm_print("thread %" PRIu32 ": entering serial mode\n", thread_get_ref(NULL));
		d->mode = serial_mode;
		hook = dmp_barrier_serial_hook;
	} else {
		mvm_print("thread %" PRIu32 ": entering parallel mode\n", thread_get_ref(NULL));
		d->mode = parallel_mode;
		hook = dmp_barrier_parallel_hook;
	}

	barrier_set_hook(d->barrier, hook, (void *)d);
	barrier_reset(d->barrier);

	if (debug == 0 && verbose != 0)
		ref_set_dump_iterator(d->thread_set);

	return 0;
}

int dmp_add_thread(struct dmp *d, int r) {
	struct thread *t;

	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	if (r != 1 && dmp_get_mode(dmp) == parallel_mode) {
		/* assertion fail */
		mvm_print("adding thread %d to thread_set in parallel mode!\n", r);
		mvm_halt();
	}

	if ((t = thread_get_current(NULL)) == NULL)
		mvm_print("adding thread %d to thread_set!\n", r);
	else
		mvm_print("thread %" PRIu32 ": adding thread %d to thread_set!\n", thread_get_ref(t), r);

	ref_set_add(d->thread_set, r);
	barrier_inc_parties(d->barrier);

	if (debug == 0 && verbose != 0)
		ref_set_dump_iterator(d->thread_set);

	return 0;
}

int dmp_remove_thread(struct dmp *d, int r) {
	struct thread *t;

	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	if (dmp_get_mode(dmp) == serial_mode) {
		mvm_print("removing thread %d from thread_set in serial mode!\n", r);
		mvm_halt();
	}

	if ((t = thread_get_current(NULL)) == NULL)
		mvm_print("removing thread %d from thread_set!\n", r);
	else
		mvm_print("thread %" PRIu32 ": removing thread %d from thread_set!\n", thread_get_ref(t), r);

	ref_set_remove(d->thread_set, r);
	barrier_dec_parties(d->barrier);

	if (debug == 0 && verbose != 0)
		ref_set_dump_iterator(d->thread_set);

	return 0;
}

struct object_dmp * dmp_create_object_dmp(struct dmp *d, struct object *o) {
	struct object_dmp *od;

	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	od = object_dmp_create(o, d->od_attr);
	return od;
}

struct thread_dmp * dmp_create_thread_dmp(struct dmp *d, struct thread *t) {
	struct thread_dmp *td;

	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	td = thread_dmp_create(t, d->td_attr);
	return td;
}

struct nlock_dmp * dmp_create_nlock_dmp(struct dmp *d, struct nlock *n) {
	struct nlock_dmp *nd;

	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	nd = nlock_dmp_create(n, d->nd_attr);
	return nd;
}

struct table_dmp * dmp_create_table_dmp(struct dmp *d, struct table *t, struct object_dmp *od) {
	struct table_dmp *td;

	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	td = table_dmp_create(t, d->tb_attr, od);
	return td;
}

int dmp_shm_read(struct dmp *d, int c, int r, enum dmp_thread_action *ta, enum dmp_owner_action *oa) {
	struct dmp_stats *stats;
	enum dmp_owner_action oaction;
	enum dmp_thread_action taction;

	stats = d->stats;

	if (DMP_SHARED(c)) {
		taction = proceed_action;
		oaction = no_action;
		mvm_print("thread %" PRIu32 ":     shared: proceed!\n", r);

		if (stats != NULL) stats->reads.shared++;
	} else if (c != r) {
		taction = (d->mode == parallel_mode) ? block_action : proceed_action;
		oaction = shared_action;
		mvm_print("thread %" PRIu32 ":     private not owned by me (%d): %sset shared, proceed!\n", r, c,
			  taction == block_action ? "block, " : "");

		if (stats != NULL) stats->reads.private_not_mine++;
	} else {
		taction = proceed_action;
		oaction = no_action;
                mvm_print("thread %" PRIu32 ":     private owned by me: proceed!\n", r);

		if (stats != NULL) stats->reads.private_mine++;
	}

	if (ta != NULL) *ta = taction;
	if (oa != NULL) *oa = oaction;

	if (stats != NULL) {
		stats->reads.total++;
		switch (taction) {
		case proceed_action: stats->reads.proceeds++; break;
		case block_action: stats->reads.blocks++; break;
		}
	}

	return 0;
}

int dmp_shm_write(struct dmp *d, int c, int r, enum dmp_thread_action *ta, enum dmp_owner_action *oa) {
	struct dmp_stats *stats;
	enum dmp_owner_action oaction;
	enum dmp_thread_action taction;

	stats = d->stats;

	if (DMP_SHARED(c)) {
		taction = (d->mode == parallel_mode) ? block_action : proceed_action;
		oaction = private_action;
		mvm_print("thread %" PRIu32 ":     shared: %sset private owned by me, proceed!\n", r,
			  taction == block_action ? "block, " : "");

		if (stats != NULL) stats->writes.shared++;
	} else if (c != r) {
		taction = (d->mode == parallel_mode) ? block_action : proceed_action;
		oaction = private_action;
		mvm_print("thread %" PRIu32 ":     private not owned by me (%d): %sset private owned by me, proceed!\n", r, c,
			  taction == block_action ? "block, " : "");

		if (stats != NULL) stats->writes.private_not_mine++;
	} else {
		taction = proceed_action;
		oaction = no_action;
		mvm_print("thread %" PRIu32 ":     private owned by me: proceed!\n", r);

		if (stats != NULL) stats->writes.private_mine++;
	}

	if (ta != NULL) *ta = taction;
	if (oa != NULL) *oa = oaction;

	if (stats != NULL) {
		stats->writes.total++;
		switch (taction) {
		case proceed_action: stats->writes.proceeds++; break;
		case block_action: stats->writes.blocks++; break;
		}
	}

	return 0;
}

int dmp_acquiesce(struct dmp *d, int r, enum thread_dmp_state s) {
	struct object *o;
	struct thread *t;
	struct thread_dmp *ud;

	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	if (r == thread_get_ref(NULL)) {
		mvm_print("thread %" PRIu32 ": tried to acquiesce on ourselves!\n", thread_get_ref(NULL));
		mvm_halt();
	}

	mvm_print("thread %" PRIu32 ": blocking until thread %" PRIu32 " is in state %s\n", thread_get_ref(NULL), r,
		  thread_dmp_state_to_string(s));

	o = heap_fetch_object(heap, r);
	t = object_get_thread(o);
	ud = thread_get_dmp(t);

	while (thread_dmp_get_state_nonblock(ud) != s);

	return 0;
}

int dmp_thread_block(struct dmp *d, struct thread_dmp *td) {
	int me, ref;
	struct object *o;
	struct thread *t;
	struct thread_dmp *ud;

	ref = me = -1;

	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	mvm_print("thread %" PRIu32 ": in dmp_thread_block\n", thread_get_ref(NULL));

	thread_dmp_set_state(td, blocking_state);

	if (d->mode == serial_mode) {
		ref = ref_set_iterator_next(d->thread_set);

		if (ref != 0) {
			dmp_acquiesce(d, ref, waiting_state);
			o = heap_fetch_object(heap, ref);
			t = object_get_thread(o);
			ud = thread_get_dmp(t);
			thread_dmp_signal(ud);
		}
	}

	barrier_await(d->barrier);

	if (d->mode == serial_mode) {
		me = thread_get_ref(NULL);
		if (me != d->first) thread_dmp_wait(td);
	}

	thread_dmp_set_state(td, running_state);

	return 0;
}

void * dmp_barrier_parallel_hook(int i, void *a) {
	struct dmp *d;

	d = (struct dmp *)a;

	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	if (i == 0) {
		/* determine first thread */
		ref_set_iterator_init(d->thread_set);
		d->first = ref_set_iterator_next(d->thread_set);
                /* end of parallel mode */
		dmp_toggle_mode(d);
	}

	return NULL;
}

void * dmp_barrier_serial_hook(int i, void *a) {
	double load;
	int nthreads;
	struct dmp *d;
	uint64_t size, free;

	d = (struct dmp *)a;

	if (d == NULL) {
		fprintf(stderr, "mvm: dmp not initialized!\n");
		mvm_halt();
	}

	if (i == 0) {
		size = heap_get_size(heap);
		free = heap_get_free(heap);

		load = (size - free) / size;

		if (load > DMP_GARBAGE_COLLECTOR_LOAD_FACTOR) {
			/* run garbage collector */
			mvm_print("thread %" PRIu32 ": calling garbage collector\n", thread_get_ref(NULL));

			if (garbage_collector_collect_now(garbage_collector) != 0)
				mvm_halt();

			mvm_print("thread %" PRIu32 ": returned from garbage collector\n", thread_get_ref(NULL));
		}

		ref_set_iterator_init(d->thread_set);
		d->first = 0;
		/* update barrier */
		nthreads = ref_set_size(d->thread_set);
		barrier_reset_parties(d->barrier, nthreads);
                /* end of serial mode */
		dmp_toggle_mode(d);
	}

	return NULL;
}
