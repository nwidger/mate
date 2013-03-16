/* Niels Widger
 * Time-stamp: <16 Mar 2013 at 10:52:20 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <errno.h>
#include <inttypes.h>
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
#include "real.h"
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
	key_field	= 0,
	value_field	= 1,
	next_field	= 2
};

/* field indices for Table objects */
enum table_field {
	lock_field                      = 0,
	num_entries_field		= 1,
	load_factor_field		= 2,
	current_capacity_field		= 3,
	iterator_is_running_field	= 4,
	iterator_bucket_field		= 5,
	iterator_entry_field		= 6
};

/* struct definitions */
struct table_entry {
	int ref;
	int lock;
};

struct table {
	struct object *object;
	struct table_entry *buckets;

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
int table_lock(struct table *t);
int table_unlock(struct table *t);

struct table * table_create(int c, struct object *o) {
	int i, ref;
	struct table *t;
	struct class *object_class;

	if ((t = (struct table *)heap_malloc(heap, sizeof(struct table))) == NULL)
		mvm_halt();

	t->object = o;

	object_class = class_table_find_predefined(class_table, object_type);
	ref = class_table_new(class_table, class_get_vmt(object_class), NULL);
	object_store_field(t->object, lock_field, ref);

	ref = class_table_new_integer(class_table, 0, NULL);
	object_store_field(t->object, num_entries_field, ref);
	object_store_field(t->object, iterator_is_running_field, ref);
	object_store_field(t->object, iterator_bucket_field, ref);
	object_store_field(t->object, iterator_entry_field, ref);

	ref = class_table_new_integer(class_table, c, NULL);
	object_store_field(t->object, current_capacity_field, ref);

	ref = class_table_new_real(class_table, TABLE_DEFAULT_LOAD_FACTOR, NULL);
	object_store_field(t->object, load_factor_field, ref);

	if ((t->buckets = (struct table_entry *)heap_malloc(heap, sizeof(struct table_entry)*(c+1))) == NULL)
		mvm_halt();

	for (i = 0; i < c; i++) {
		t->buckets[i].ref = 0;
		t->buckets[i].lock = 0;
	}

	/* ref of -1 is sentinel for end of buckets */
	t->buckets[c].ref = -1;
	t->buckets[c].lock = -1;

#ifdef DMP
	if (dmp == NULL)
		t->dmp = NULL;
	else
		t->dmp = dmp_create_table_dmp(dmp, t, object_get_dmp(o));
#endif

	return t;
}

void table_destroy(struct table *t) {
	if (t != NULL) {
		table_clear(t);
		heap_free(heap, t->buckets);
#ifdef DMP
		if (t->dmp != NULL)
			table_dmp_destroy(t->dmp);
#endif
		heap_free(heap, t);
	}
}

void table_clear(struct table *t) {
	if (t != NULL) {
#ifdef DMP
		if (t->dmp != NULL)
			table_dmp_clear(t->dmp);
#endif
	}
}

int32_t table_get_integer_field(struct table *t, enum table_field f) {
	int ref;
	int32_t value;
	struct object *object;
	struct integer *integer;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	switch (f) {
	case num_entries_field:
	case current_capacity_field:
	case iterator_is_running_field:
	case iterator_bucket_field:
	case iterator_entry_field:
		break;
	default:
		fprintf(stderr, "mvm: invalid integer table field!\n");
		mvm_halt();
	}

	ref = object_load_field(t->object, f);
	object = heap_fetch_object(heap, ref);
	integer = object_get_integer(object);
	value = integer_get_value(integer);

	return value;
}

int table_set_integer_field(struct table *t, enum table_field f, int32_t v) {
	int ref;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	ref = class_table_new_integer(class_table, v, NULL);
	object_store_field(t->object, f, ref);

	return 0;
}

float table_get_real_field(struct table *t, enum table_field f) {
	int ref;
	float value;
	struct real *real;
	struct object *object;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	switch (f) {
	case load_factor_field:
		break;
	default:
		fprintf(stderr, "mvm: invalid real table field!\n");
		mvm_halt();
	}

	ref = object_load_field(t->object, f);
	object = heap_fetch_object(heap, ref);
	real = object_get_real(object);
	value = real_get_value(real);

	return value;
}

int table_set_real_field(struct table *t, enum table_field f, float v) {
	int ref;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	ref = class_table_new_real(class_table, v, NULL);
	object_store_field(t->object, f, ref);

	return 0;
}

struct table_entry * table_get_bucket(struct table *t, int i, struct object **l) {
	struct class *object_class;
	struct table_entry *bucket;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	bucket = &t->buckets[i];

	if (bucket->ref == -1)
		return NULL;

	if (bucket->lock == 0) {
		object_class = class_table_find_predefined(class_table, object_type);
		bucket->lock = class_table_new(class_table, class_get_vmt(object_class), NULL);
	}

	if (l != NULL)
		*l = heap_fetch_object(heap, bucket->lock);
	
	return bucket;
}

int table_populate_ref_set(struct table *t, struct ref_set *r) {
	int i, lock, ref;
	struct object *object;
	struct table_entry *bucket;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	for (i = 0; t->buckets[i].ref != -1; i++) {
		bucket = &t->buckets[i];
		lock = bucket->lock;

		if (lock != 0)
			ref_set_add(r, lock);

		ref = bucket->ref;

		if (ref != 0) {
			ref_set_add(r, ref);
			object = heap_fetch_object(heap, ref);
			object_populate_ref_set(object, r);
		}
	}

	return 0;
}

int table_entries_exceeds_load_factor(int e, int l, int c) {
	return (e * l) > c;
}

int table_get(struct table *t, struct object *k) {
	struct table_entry *bucket;
	struct object *entry, *key, *lock;
	int n, hash, entry_ref, key_ref, value_ref;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	value_ref = 0;
	hash = abs(table_run_hash_code(t, k));

	n = hash % table_get_integer_field(t, current_capacity_field);

	bucket = table_get_bucket(t, n, &lock);

	/* lock */
	object_acquire_monitor(lock);

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
	object_release_monitor(lock);

	return value_ref;
}

int table_put(struct table *t, struct object *k, struct object *v) {
	struct class *object_class;
	struct table_entry *bucket;
	struct object *prev, *curr, *key, *new, *lock;
	int prev_ref, curr_ref, key_ref, new_ref, hash, n, head, old_value,
		current_capacity;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	/* check if iterator is running */
	if (table_get_integer_field(t, iterator_is_running_field) == 1) {
		fprintf(stderr, "mvm: cannot put, Table's iterator is running!\n");
		mvm_halt();
	}

	hash = abs(table_run_hash_code(t, k));

	/* lock */
	table_lock(t);
	current_capacity = table_get_integer_field(t, current_capacity_field);
	table_unlock(t);
	/* unlock */

	if (current_capacity <= 0)
		table_resize(t, TABLE_DEFAULT_INITIAL_CAPACITY);

	n = hash % current_capacity;
	old_value = 0;
	prev_ref = 0;

	bucket = table_get_bucket(t, n, &lock);

	/* lock */
	object_acquire_monitor(lock);

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
		table_lock(t);

		head = bucket->ref;
		bucket->ref = new_ref;
		table_set_integer_field(t, num_entries_field, 
					table_get_integer_field(t, num_entries_field) + 1);

		/* unlock */
		table_unlock(t);

		object_store_field(new, next_field, head);
	} else {
		old_value = object_load_field(curr, value_field);
		object_store_field(new, next_field, object_load_field(curr, next_field));

		/* lock */
		table_lock(t);
		head = bucket->ref;
		table_unlock(t);
		/* unlock */

		if (curr_ref != head) {
			prev = heap_fetch_object(heap, prev_ref);
			object_store_field(prev, next_field, new_ref);
		} else {
			/* lock */
			table_lock(t);
			bucket->ref = new_ref;
			table_unlock(t);
			/* unlock */
		}
	}

	/* unlock */
	object_release_monitor(lock);

	if (table_entries_exceeds_load_factor(table_get_integer_field(t, num_entries_field),
					      table_get_real_field(t, load_factor_field),
					      table_get_integer_field(t, current_capacity_field))) {
		table_resize(t, table_get_integer_field(t, current_capacity_field)*2);
	}

	return old_value;
}

int table_remove(struct table *t, struct object *k) {
	struct table_entry *bucket;
	struct object *curr, *prev, *key, *lock;
	int hash, n, old_value, curr_ref, prev_ref, key_ref,
		current_capacity;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	/* check if iterator is running */
	if (table_get_integer_field(t, iterator_is_running_field) == 1) {
		fprintf(stderr, "mvm: cannot remove, Table's iterator is running!\n");
		mvm_halt();
	}

	hash = abs(table_run_hash_code(t, k));

	/* lock */
	table_lock(t);
	current_capacity = table_get_integer_field(t, current_capacity_field);
	table_unlock(t);
	/* unlock */

	n = hash % current_capacity;
	old_value = 0;
	prev_ref = 0;

	bucket = table_get_bucket(t, n, &lock);

	/* lock */
	object_acquire_monitor(lock);

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
	object_release_monitor(lock);

	/* lock */
	table_lock(t);
	table_set_integer_field(t, num_entries_field, table_get_integer_field(t, num_entries_field) - 1);
	table_unlock(t);
	/* unlock */

	return old_value;
}

int table_first_key(struct table *t) {
	int32_t value;
	struct object *lock;
	struct table_entry *bucket;
	int entry_ref, ref, i, current_capacity;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	table_lock(t);
	current_capacity = table_get_integer_field(t, current_capacity_field);
	table_unlock(t);
	/* unlock */

	for (i = 0; i < current_capacity; i++) {
		bucket = table_get_bucket(t, i, &lock);

		/* lock */
		object_acquire_monitor(lock);

		entry_ref = bucket->ref;

		/* unlock */
		object_release_monitor(lock);

		if (entry_ref != 0)
			break;
	}

	/* lock */
	table_lock(t);

	if (i >= current_capacity) {
		value = 1;
		table_set_integer_field(t, iterator_is_running_field, 0);
	} else {
		value = 0;
		table_set_integer_field(t, iterator_is_running_field, 1);
		table_set_integer_field(t, iterator_bucket_field, i);
		table_set_integer_field(t, iterator_entry_field, entry_ref);
	}

	/* unlock */
	table_unlock(t);

	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int table_next_key(struct table *t) {
	struct table_entry *bucket;
	struct object *entry, *lock;
	int n, entry_ref, prev_ref, current_capacity;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	if (table_get_integer_field(t, iterator_is_running_field) == 0)
		return 0;


	/* lock */
	table_lock(t);
	entry_ref = table_get_integer_field(t, iterator_entry_field);
	table_unlock(t);
	/* unlock */

	entry = heap_fetch_object(heap, entry_ref);
	prev_ref = object_load_field(entry, key_field);
	entry_ref = object_load_field(entry, next_field);

	/* lock */
	table_lock(t);
	table_set_integer_field(t, iterator_entry_field, entry_ref);
	table_unlock(t);
	/* unlock */

	if (entry_ref != 0) {
		return prev_ref;
	}

	/* lock */
	table_lock(t);
	current_capacity = table_get_integer_field(t, current_capacity_field);
	n = table_get_integer_field(t, iterator_bucket_field) + 1;
	table_unlock(t);
	/* unlock */

	for (; n < current_capacity; n++) {
		bucket = table_get_bucket(t, n, &lock);

		/* lock */
		object_acquire_monitor(lock);

		entry_ref = bucket->ref;

		/* unlock */
		object_release_monitor(lock);

		if (entry_ref != 0) {
			/* lock */
			table_lock(t);
			table_set_integer_field(t, iterator_bucket_field, n);
			table_set_integer_field(t, iterator_entry_field, entry_ref);
			table_unlock(t);
			/* unlock */

			break;
		}
	}

	if (n >= current_capacity) {
		/* lock */
		table_lock(t);
		table_set_integer_field(t, iterator_is_running_field, 0);
		table_unlock(t);
		/* unlock */
	}

	return prev_ref;
}

int table_resize(struct table *t, int n) {
	struct table *new_table;
	struct table_entry *bucket;
	int i, entry_ref, key_ref, value_ref;
	struct object *entry, *key, *value, *lock;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	table_lock(t);

	if ((new_table = table_create(n, t->object)) == NULL)
		mvm_halt();

	for (i = 0; ; i++) {
		bucket = table_get_bucket(t, i, &lock);

		if (bucket == NULL)
			break;

		/* lock */
		object_acquire_monitor(lock);

		for (entry_ref = bucket->ref; entry_ref != 0; entry_ref = object_load_field(entry, next_field)) {
			entry = heap_fetch_object(heap, entry_ref);

			key_ref = object_load_field(entry, key_field);
			key = heap_fetch_object(heap, key_ref);

			value_ref = object_load_field(entry, key_field);
			value = heap_fetch_object(heap, value_ref);

			table_put(new_table, key, value);
		}

		/* unlock */
		object_release_monitor(lock);
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
	int i, entry_ref, key_ref, value_ref;
	struct object *entry, *key, *value, *lock;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	table_lock(t);

	fprintf(stderr, "num_entries = %d\n", table_get_integer_field(t, num_entries_field));
	fprintf(stderr, "load_factor = %f\n", table_get_real_field(t, load_factor_field));
	fprintf(stderr, "current_capacity = %d\n", table_get_integer_field(t, current_capacity_field));

	for (i = 0; ; i++) {
		bucket = table_get_bucket(t, i, &lock);

		if (bucket == NULL)
			break;

		/* lock */
		object_acquire_monitor(lock);

		fprintf(stderr, "bucket %d =\n", i);

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
		object_release_monitor(lock);
	}

	/* unlock */
	table_unlock(t);

	return 0;
}

int table_lock(struct table *t) {
	int ref;
	struct object *object;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	ref = object_load_field(t->object, lock_field);
	object = heap_fetch_object(heap, ref);
	object_acquire_monitor(object);

	return 0;
}

int table_unlock(struct table *t) {
	int ref;
	struct object *object;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	/* unlock */
	ref = object_load_field(t->object, lock_field);
	object = heap_fetch_object(heap, ref);
	object_release_monitor(object);

	return 0;
}
