/* Niels Widger
 * Time-stamp: <14 Mar 2013 at 19:48:28 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "class.h"
#include "class_table.h"
#include "constants.h"
#include "frame.h"
#include "globals.h"
#include "heap.h"
#include "integer.h"
#include "invoke_method.h"
#include "object.h"
#include "operand_stack.h"
#include "ref_set.h"
#include "string.h"
#include "table.h"
#include "thread.h"
#include "vm_stack.h"

#ifdef DMP
#include "dmp.h"
#include "table_dmp.h"
#endif

/* enums */
/* field indices for table entry objects */
enum entry_field {
	key_field   = 0,
	value_field = 1,
	next_field  = 2,
};

/* struct definitions */
struct table_entry {
	int ref;
	pthread_rwlock_t rwlock;
};

struct table {
	struct object *object;

	int num_entries;
	float load_factor;
	int current_capacity;
	struct table_entry *buckets;

	int iterator_is_running;
	int iterator_bucket;
	int iterator_entry;

#ifdef TABLE_USE_RWLOCK
	pthread_rwlock_t rwlock;
#endif

#ifdef DMP
	struct table_dmp *dmp;
#endif
};

/* forward declarations */
int table_entries_exceeds_load_factor(int e, int l, int c);
int table_resize(struct table *t, int n);
int table_run_hash_code(struct table *t, struct object *o);
int table_run_equals(struct table *t, struct object *o, struct object *p);
int table_dump(struct table *t);
int table_wrlock(struct table *t);
int table_rdlock(struct table *t);
int table_unlock(struct table *t);

struct table * table_create(int c, struct object *o) {
	int i;
	struct table *t;

	if ((t = (struct table *)heap_malloc(heap, sizeof(struct table))) == NULL)
		mvm_halt();

	t->object = o;

	t->num_entries = 0;
	t->current_capacity = c;
	t->load_factor = TABLE_DEFAULT_LOAD_FACTOR;

	t->iterator_is_running = 0;
	t->iterator_bucket = 0;
	t->iterator_entry = 0;

	if ((t->buckets = (struct table_entry *)heap_malloc(heap, sizeof(struct table_entry)*c)) == NULL)
		mvm_halt();

	for (i = 0; i < t->current_capacity; i++) {
		t->buckets[i].ref = 0;
		if (pthread_rwlock_init(&t->buckets[i].rwlock, NULL) != 0) {
			perror("mvm: pthread_rwlock_init");
			mvm_halt();
		}
	}

#ifdef TABLE_USE_RWLOCK
	if (pthread_rwlock_init(&t->rwlock, NULL) != 0) {
		perror("mvm: pthread_rwlock_init");
		mvm_halt();
	}
#endif

#ifdef DMP
	if (dmp == NULL)
		t->dmp = NULL;
	else
		t->dmp = dmp_create_table_dmp(dmp, t, object_get_dmp(o));
#endif

	return t;
}

void table_destroy(struct table *t) {
	int i;
	
	if (t != NULL) {
		table_clear(t);

		for (i = 0; i < t->current_capacity; i++)
			pthread_rwlock_destroy(&t->buckets[i].rwlock);

		heap_free(heap, t->buckets);
#ifdef DMP
		if (t->dmp != NULL)
			table_dmp_destroy(t->dmp);
#endif
#ifdef TABLE_USE_RWLOCK
		pthread_rwlock_destroy(&t->rwlock);
#endif
		heap_free(heap, t);
	}
}

void table_clear(struct table *t) {
	int i;

	if (t != NULL) {
		for (i = 0; i < t->current_capacity; i++)
			t->buckets[i].ref = 0;

		t->num_entries = 0;
		t->iterator_is_running = 0;
		t->iterator_bucket = 0;
		t->iterator_entry = 0;
#ifdef DMP
		if (t->dmp != NULL)
			table_dmp_clear(t->dmp);
#endif
	}
}

int table_populate_ref_set(struct table *t, struct ref_set *r) {
	struct object *object;
	struct table_entry *bucket;
	int i, ref, current_capacity;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	table_rdlock(t);
	current_capacity = t->current_capacity;
	table_unlock(t);
	/* unlock */

	for (i = 0; i < current_capacity; i++) {
		bucket = &t->buckets[i];

		/* lock */
		pthread_rwlock_rdlock(&bucket->rwlock);

		ref = bucket->ref;

		if (ref != 0) {
			ref_set_add(r, ref);
			object = heap_fetch_object(heap, ref);
			object_populate_ref_set(object, r);
		}

		/* unlock */
		pthread_rwlock_unlock(&bucket->rwlock);
	}

	return 0;
}

int table_entries_exceeds_load_factor(int e, int l, int c) {
	return (e * l) > c;
}

int table_get(struct table *t, struct object *k) {
	struct table_entry *bucket;
	struct object *entry, *key;
	int n, hash, entry_ref, key_ref, value_ref;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	value_ref = 0;
	hash = abs(table_run_hash_code(t, k));

	/* lock */
	table_rdlock(t);
	n = hash % t->current_capacity;
	table_unlock(t);
	/* unlock */

	bucket = &t->buckets[n];

	/* lock */
	pthread_rwlock_rdlock(&bucket->rwlock);

	entry_ref = bucket->ref;

	for (; entry_ref != 0; entry_ref = object_load_field(entry, next_field)) {
		entry = heap_fetch_object(heap, entry_ref);

		key_ref = object_load_field(entry, key_field);
		key = heap_fetch_object(heap, key_ref);

		if (table_run_equals(t, k, key) == 1) {
			value_ref = object_load_field(entry, value_field);
			break;
		}
	}

	/* unlock */
	pthread_rwlock_unlock(&bucket->rwlock);

	return value_ref;
}

int table_put(struct table *t, struct object *k, struct object *v) {
	struct class *object_class;
	struct table_entry *bucket;
	struct object *prev, *curr, *key, *new;
	int prev_ref, curr_ref, key_ref, new_ref, hash, n, head, old_value,
		iterator_is_running, current_capacity;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	table_rdlock(t);
	iterator_is_running = t->iterator_is_running;
	table_unlock(t);
	/* unlock */

	/* check if iterator is running */
	if (iterator_is_running == 1) {
		fprintf(stderr, "mvm: cannot put, Table's iterator is running!\n");
		mvm_halt();
	}

	hash = abs(table_run_hash_code(t, k));

	/* lock */
	table_rdlock(t);
	current_capacity = t->current_capacity;
	table_unlock(t);
	/* unlock */

	if (current_capacity <= 0)
		table_resize(t, TABLE_DEFAULT_INITIAL_CAPACITY);

	n = hash % current_capacity;
	old_value = 0;
	prev_ref = 0;

	bucket = &t->buckets[n];

	/* lock */
	pthread_rwlock_wrlock(&bucket->rwlock);

	curr_ref = bucket->ref;

	for (; curr_ref != 0; prev_ref = curr_ref, curr_ref = object_load_field(curr, next_field)) {
		curr = heap_fetch_object(heap, curr_ref);

		key_ref = object_load_field(curr, key_field);
		key = heap_fetch_object(heap, key_ref);

		if (table_run_equals(t, k, key) == 1)
			break;
	}

	object_class = class_table_find_predefined(class_table, object_type);
	new_ref = object_create(object_class, 3, &new);

	object_store_field(new, key_field, object_get_ref(k));
	object_store_field(new, value_field, object_get_ref(v));
	object_store_field(new, next_field, 0);

	new_ref = object_get_ref(new);

	if (curr_ref == 0) {
		/* lock */
		table_wrlock(t);

		head = bucket->ref;
		bucket->ref = new_ref;
		t->num_entries++;

		/* unlock */
		table_unlock(t);

		object_store_field(new, next_field, head);
	} else {
		old_value = object_load_field(curr, value_field);
		object_store_field(new, next_field, object_load_field(curr, next_field));

		/* lock */
		table_rdlock(t);
		head = bucket->ref;
		table_unlock(t);
		/* unlock */

		if (curr_ref != head) {
			prev = heap_fetch_object(heap, prev_ref);
			object_store_field(prev, next_field, new_ref);
		} else {
			/* lock */
			table_wrlock(t);
			bucket->ref = new_ref;
			table_unlock(t);
			/* unlock */
		}
	}

	/* unlock */
	pthread_rwlock_unlock(&bucket->rwlock);

	if (table_entries_exceeds_load_factor(t->num_entries,
					      t->load_factor,
					      t->current_capacity)) {
		table_resize(t, t->current_capacity*2);
	}

	return old_value;
}

int table_remove(struct table *t, struct object *k) {
	struct table_entry *bucket;
	struct object *curr, *prev, *key;
	int hash, n, old_value, curr_ref, prev_ref, key_ref,
		iterator_is_running, current_capacity;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	table_rdlock(t);
	iterator_is_running = t->iterator_is_running;
	table_unlock(t);
	/* unlock */

	/* check if iterator is running */
	if (iterator_is_running == 1) {
		fprintf(stderr, "mvm: cannot remove, Table's iterator is running!\n");
		mvm_halt();
	}

	hash = abs(table_run_hash_code(t, k));

	/* lock */
	table_rdlock(t);
	current_capacity = t->current_capacity;
	table_unlock(t);
	/* unlock */

	n = hash % current_capacity;
	old_value = 0;
	prev_ref = 0;

	bucket = &t->buckets[n];

	/* lock */
	pthread_rwlock_rdlock(&bucket->rwlock);
	curr_ref = bucket->ref;

	for (; curr_ref != 0; prev_ref = curr_ref, curr_ref = object_load_field(curr, next_field)) {
		curr = heap_fetch_object(heap, curr_ref);

		key_ref = object_load_field(curr, key_field);
		key = heap_fetch_object(heap, key_ref);

		if (table_run_equals(t, k, key) == 1) {
			old_value = object_load_field(curr, value_field);
			break;
		}
	}

	if (curr_ref != 0) {
		if (curr_ref == bucket->ref) {
			bucket->ref = object_load_field(curr, next_field);
		} else {
			prev = heap_fetch_object(heap, prev_ref);
			object_store_field(prev, next_field,
					   object_load_field(curr, next_field));
		}
	}

	/* unlock */
	pthread_rwlock_unlock(&bucket->rwlock);

	/* lock */
	table_wrlock(t);
	t->num_entries--;
	table_unlock(t);
	/* unlock */

	return old_value;
}

int table_first_key(struct table *t) {
	int32_t value;
	struct table_entry *bucket;
	int entry_ref, ref, i, current_capacity;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	table_rdlock(t);
	current_capacity = t->current_capacity;
	table_unlock(t);
	/* unlock */

	for (i = 0; i < current_capacity; i++) {
		bucket = &t->buckets[i];

		/* lock */
		pthread_rwlock_rdlock(&bucket->rwlock);
		entry_ref = bucket->ref;
		pthread_rwlock_unlock(&bucket->rwlock);
		/* unlock */

		if (entry_ref != 0)
			break;
	}

	/* lock */
	table_wrlock(t);

	if (i >= current_capacity) {
		value = 1;
		t->iterator_is_running = 0;
	} else {
		value = 0;
		t->iterator_is_running = 1;
		t->iterator_bucket = i;
		t->iterator_entry = entry_ref;
	}

	/* unlock */
	table_unlock(t);

	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int table_next_key(struct table *t) {
	struct object *entry;
	struct table_entry *bucket;
	int n, entry_ref, prev_ref, iterator_is_running, current_capacity;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	table_rdlock(t);
	iterator_is_running = t->iterator_is_running;
	table_unlock(t);
	/* unlock */

	if (iterator_is_running == 0)
		return 0;


	/* lock */
	table_rdlock(t);
	entry_ref = t->iterator_entry;
	table_unlock(t);
	/* unlock */

	entry = heap_fetch_object(heap, entry_ref);
	prev_ref = object_load_field(entry, key_field);
	entry_ref = object_load_field(entry, next_field);

	/* lock */
	table_wrlock(t);
	t->iterator_entry = entry_ref;
	table_unlock(t);
	/* unlock */

	if (entry_ref != 0) {
		return prev_ref;
	}

	/* lock */
	table_rdlock(t);
	current_capacity = t->current_capacity;
	n = t->iterator_bucket + 1;
	table_unlock(t);
	/* unlock */

	for (; n < current_capacity; n++) {
		bucket = &t->buckets[n];

		/* lock */
		pthread_rwlock_rdlock(&bucket->rwlock);
		entry_ref = bucket->ref;
		pthread_rwlock_unlock(&bucket->rwlock);
		/* unlock */

		if (entry_ref != 0) {
			/* lock */
			table_wrlock(t);
			t->iterator_bucket = n;
			t->iterator_entry = entry_ref;
			table_unlock(t);
			/* unlock */

			break;
		}
	}

	if (n >= current_capacity) {
		/* lock */
		table_wrlock(t);
		t->iterator_is_running = 0;
		table_unlock(t);
		/* unlock */
	}

	return prev_ref;
}

int table_resize(struct table *t, int n) {
	struct table *new_table;
	struct table_entry *bucket;
	struct object *entry, *key, *value;
	int i, entry_ref, key_ref, value_ref;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	table_wrlock(t);

	if ((new_table = table_create(n, t->object)) == NULL)
		mvm_halt();

	for (i = 0; i < t->current_capacity; i++ ) {
		bucket = &t->buckets[i];

		/* lock */
		pthread_rwlock_wrlock(&bucket->rwlock);

		for (entry_ref = bucket->ref; entry_ref != 0; entry_ref = object_load_field(entry, next_field)) {
			entry = heap_fetch_object(heap, entry_ref);

			key_ref = object_load_field(entry, key_field);
			key = heap_fetch_object(heap, key_ref);

			value_ref = object_load_field(entry, key_field);
			value = heap_fetch_object(heap, value_ref);

			table_put(new_table, key, value);
		}

		/* unlock */
		pthread_rwlock_unlock(&bucket->rwlock);
	}

	object_set_table(t->object, new_table);

	/* unlock */
	table_unlock(t);

	table_destroy(t);

	return 0;
}

int table_run_hash_code(struct table *t, struct object *o) {
	int ref;
	int32_t value;
	uint32_t pc;
	struct integer *integer;
	struct object *object;
	struct vm_stack *vm_stack;
	struct frame *frame;
	struct operand_stack *operand_stack;
	struct thread *thread;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	thread = thread_get_current();

	pc = thread_get_pc(thread);
	vm_stack = thread_get_vm_stack(thread);

	frame = vm_stack_peek(vm_stack);
	operand_stack = frame_get_operand_stack(frame);
	operand_stack_push(operand_stack, object_get_ref(o));

	invoke_virtual_method_by_name(thread, object_get_ref(o), pc, "hashCode", 0);

	ref = operand_stack_peek(operand_stack);
	object = heap_fetch_object(heap, ref);
	integer = object_get_integer(object);
	value = integer_get_value(integer);

	operand_stack_pop(operand_stack);

	return value;
}

int table_run_equals(struct table *t, struct object *o, struct object *p) {
	int ref;
	int32_t value;
	uint32_t pc;
	struct integer *integer;
	struct object *object;
	struct vm_stack *vm_stack;
	struct frame *frame;
	struct operand_stack *operand_stack;
	struct thread *thread;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	thread = thread_get_current();

	pc = thread_get_pc(thread);
	vm_stack = thread_get_vm_stack(thread);

	frame = vm_stack_peek(vm_stack);
	operand_stack = frame_get_operand_stack(frame);
	operand_stack_push(operand_stack, object_get_ref(o));
	operand_stack_push(operand_stack, object_get_ref(p));

	invoke_virtual_method_by_name(thread, object_get_ref(o), pc, "equals", 1, "Object");

	ref = operand_stack_peek(operand_stack);
	object = heap_fetch_object(heap, ref);
	integer = object_get_integer(object);
	value = integer_get_value(integer);

	operand_stack_pop(operand_stack);

	return value;
}

int table_dump(struct table *t) {
	struct table_entry *bucket;
	struct object *entry, *key, *value;
	int i, entry_ref, key_ref, value_ref;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	table_rdlock(t);

	fprintf(stderr, "num_entries = %d\n", t->num_entries);
	fprintf(stderr, "load_factor = %f\n", t->load_factor);
	fprintf(stderr, "current_capacity = %d\n", t->current_capacity);

	for (i = 0; i < t->current_capacity; i++) {
		bucket = &t->buckets[i];

		fprintf(stderr, "bucket %d =\n", i);

		/* lock */
		pthread_rwlock_rdlock(&bucket->rwlock);

		for (entry_ref = bucket->ref; entry_ref != 0; entry_ref = object_load_field(entry, next_field)) {
			entry = heap_fetch_object(heap, entry_ref);

			key_ref = object_load_field(entry, key_field);
			key = heap_fetch_object(heap, key_ref);

			value_ref = object_load_field(entry, value_field);
			value = heap_fetch_object(heap, value_ref);

			fprintf(stderr, "               (");
			object_dump(key, 0);
			fprintf(stderr, ", ");
			object_dump(value, 0);
			fprintf(stderr, ")\n");
		}

		/* unlock */
		pthread_rwlock_unlock(&bucket->rwlock);
	}

	/* unlock */
	table_unlock(t);

	return 0;
}

int table_wrlock(struct table *t) {
#ifdef TABLE_USE_RWLOCK
	int err;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	err = pthread_rwlock_trywrlock(&t->rwlock);

	switch (err) {
	case 0:			/* acquired lock */
	case EDEADLK:		/* we already own the lock */
		break;
	case EBUSY:		/* someone else owns the lock, block */
		if ((err = pthread_rwlock_wrlock(&t->rwlock)) != 0) {
			fprintf(stderr, "mvm: pthread_rwlock_wrlock: %s\n", strerror(err));
			mvm_halt();
		}
		break;
	default:
		fprintf(stderr, "mvm: pthread_rwlock_trywrlock: %s\n", strerror(err));
		mvm_halt();
	}
#endif

	return 0;
}

int table_rdlock(struct table *t) {
#ifdef TABLE_USE_RWLOCK
	int err;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	err = pthread_rwlock_tryrdlock(&t->rwlock);

	switch (err) {
	case 0:			/* acquired lock */
	case EDEADLK:		/* we already own the lock */
		break;
	case EBUSY:		/* someone else owns the lock, block */
		if ((err = pthread_rwlock_rdlock(&t->rwlock)) != 0) {
			fprintf(stderr, "mvm: pthread_rwlock_rdlock: %s\n", strerror(err));
			mvm_halt();
		}
		break;
	default:
		fprintf(stderr, "mvm: pthread_rwlock_tryrdlock: %s\n", strerror(err));
		mvm_halt();
	}

#endif

	return 0;
}

int table_unlock(struct table *t) {
#ifdef TABLE_USE_RWLOCK
	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	pthread_rwlock_unlock(&t->rwlock);
#endif

	return 0;
}
