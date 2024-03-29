/* Niels Widger
 * Time-stamp: <03 Sep 2013 at 20:50:53 by nwidger on macros.local>
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

/* struct definitions */
struct table_entry {
	int key;
	int value;
	int hash;
	struct table_entry *next;
};

struct table {
	struct object *object;
	
	int num_entries;
	float load_factor;
	int current_capacity;
	struct table_entry **buckets;

	int iterator_is_running;
	int iterator_bucket;
	struct table_entry *iterator_entry;

#ifdef TABLE_USE_RWLOCK
	pthread_rwlock_t rwlock;
#endif

#ifdef DMP
	struct table_dmp *dmp;
#endif
};

/* forward declarations */
int table_put_aux(struct table *t, struct object *k, struct object *v, int hash);
int table_entries_exceeds_load_factor(int e, int l, int c);
int table_resize(struct table *t, int n, struct table **nt);
int table_run_hash_code(struct table *t, struct object *o);
int table_run_equals(struct table *t, struct object *o, struct object *p);
int table_dump(struct table *t);
struct table_entry * table_entry_create(int k, int v, int h);
void table_entry_destroy(struct table_entry *r);
int table_wrlock(struct table *t);
int table_rdlock(struct table *t);
int table_unlock(struct table *t);

struct table * table_create(int c, struct object *o) {
	struct table *t;

	if ((t = (struct table *)heap_malloc(heap, sizeof(struct table))) == NULL)
		mvm_halt();

	t->object = o;

	t->num_entries = 0;
	t->current_capacity = c;
	t->load_factor = TABLE_DEFAULT_LOAD_FACTOR;

	t->iterator_is_running = 0;
	t->iterator_bucket = 0;
	t->iterator_entry = NULL;

	if ((t->buckets = (struct table_entry **)heap_malloc(heap, sizeof(struct table_entry *)*c)) == NULL)
		mvm_halt();

	memset(t->buckets, 0, sizeof(struct table_entry *)*c);

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
	if (t != NULL) {
		table_clear(t);
		heap_free(heap, t->buckets);
		heap_free(heap, t);
#ifdef TABLE_USE_RWLOCK
		pthread_rwlock_destroy(&t->rwlock);
#endif
	}
}

void table_clear(struct table *t) {
	int i;
	struct table_entry *r, *s;

	if (t != NULL) {
		for (i = 0; i < t->current_capacity; i++) {
			r = t->buckets[i];
			while (r != NULL) {
				s = r;
				r = r->next;
				table_entry_destroy(s);
			}
		}

		memset(t->buckets, 0,
		       sizeof(struct table_entry *)*t->current_capacity);

		t->num_entries = 0;
		t->iterator_is_running = 0;
		t->iterator_bucket = 0;
		t->iterator_entry = NULL;
	}
}

int table_populate_ref_set(struct table *t, struct ref_set *r) {
	int i;
	struct table_entry *p;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	table_rdlock(t);

	for (i = 0; i < t->current_capacity; i++) {
		for (p = t->buckets[i]; p != NULL; p = p->next) {
			if (p->key != 0)
				ref_set_add(r, p->key);
			if (p->value != 0)
				ref_set_add(r, p->value);
		}
	}

	/* unlock */
	table_unlock(t);

	return 0;
}

int table_entries_exceeds_load_factor(int e, int l, int c) {
	return (e * l) > c;
}

int table_get(struct table *t, struct object *k) {
	struct table_entry *r;
	int n, hash, value;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	hash = abs(table_run_hash_code(t, k));

#ifdef DMP
	if (t->dmp != NULL)
		table_dmp_load(t->dmp);
#endif

	/* lock */
	table_rdlock(t);

	n = hash % t->current_capacity;

	for (r = t->buckets[n]; r != NULL; r = r->next) {
		if (table_run_equals(t, k, heap_fetch_object(heap, r->key)) == 1) {
#ifdef DMP
			if (t->dmp != NULL)
				table_dmp_load(t->dmp);
#endif

			value = r->value;

			/* unlock */
			table_unlock(t);
			return value;
		}
		
#ifdef DMP
		if (t->dmp != NULL)
			table_dmp_load(t->dmp);
#endif
	}

	/* unlock */
	table_unlock(t);

	return 0;
}

int table_put(struct table *t, struct object *k, struct object *v) {
	int hash;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

#ifdef DMP
	if (t->dmp != NULL)
		table_dmp_load(t->dmp);
#endif

	/* lock */
	table_rdlock(t);

	/* check if iterator is running */
	if (t->iterator_is_running == 1) {
		fprintf(stderr, "mvm: cannot put, Table's iterator is running!\n");
		mvm_halt();
	}

	/* unlock */
	table_unlock(t);

	hash = abs(table_run_hash_code(t, k));

#ifdef DMP
	if (t->dmp != NULL)
		table_dmp_load(t->dmp);
#endif

	/* lock */
	table_rdlock(t);

	if (t->current_capacity <= 0) {
		table_resize(t, TABLE_DEFAULT_INITIAL_CAPACITY, &t);
	}

	/* unlock */
	table_unlock(t);

#ifdef DMP
	if (t->dmp != NULL)
		table_dmp_load(t->dmp);
#endif

	return table_put_aux(t, k, v, hash);
}

int table_put_aux(struct table *t, struct object *k, struct object *v, int hash) {
	int value, n, old_value;
	struct table_entry *new_entry, *r;

	n = hash % t->current_capacity;
	old_value = 0;

	for (r = t->buckets[n]; r != NULL; r = r->next) {
		value = table_run_equals(t, k, heap_fetch_object(heap, r->key));
		if (value == 1)
			break;

#ifdef DMP
		if (t->dmp != NULL)
			table_dmp_load(t->dmp);
#endif
	}

#ifdef DMP
	if (t->dmp != NULL)
		table_dmp_store(t->dmp);
#endif

	/* lock */
	table_wrlock(t);

	if (r != NULL) {
		old_value = r->value;
		r->key = object_get_ref(k);
		r->value = object_get_ref(v);
		r->hash = hash;
	} else {
		if ((new_entry =
		     table_entry_create(object_get_ref(k), object_get_ref(v), hash)) == NULL) {
			mvm_halt();
		}

		new_entry->next = t->buckets[n];
		t->buckets[n] = new_entry;
		t->num_entries++;
	}

	if (table_entries_exceeds_load_factor(t->num_entries,
					      t->load_factor,
					      t->current_capacity)) {
		table_resize(t, t->current_capacity*2, &t);
		object_set_table(t->object, t);
	}

	/* unlock */
	table_unlock(t);

	return old_value;
}

int table_remove(struct table *t, struct object *k) {
	int hash, n, old_value;
	struct table_entry *r, *s;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}
	
#ifdef DMP
	if (t->dmp != NULL)
		table_dmp_load(t->dmp);
#endif

	/* lock */
	table_wrlock(t);

	/* check if iterator is running */
	if (t->iterator_is_running == 1) {
		fprintf(stderr, "mvm: cannot remove, Table's iterator is running!\n");
		mvm_halt();
	}

	hash = abs(table_run_hash_code(t, k));

#ifdef DMP
	if (t->dmp != NULL)
		table_dmp_load(t->dmp);
#endif
	
	n = hash % t->current_capacity;
	old_value = 0;

	for (s = NULL, r = t->buckets[n]; r != NULL; s = r, r = r->next) {
		if (table_run_equals(t, k, heap_fetch_object(heap, r->key)) == 1) {
#ifdef DMP
			if (t->dmp != NULL)
				table_dmp_load(t->dmp);
#endif
			
			old_value = r->value;
			break;
		}

#ifdef DMP
		if (t->dmp != NULL)
			table_dmp_load(t->dmp);
#endif
	}

	if (r == NULL) {
		/* unlock */
		table_unlock(t);
		return 0;
	}

#ifdef DMP
	if (t->dmp != NULL)
		table_dmp_store(t->dmp);
#endif

	if (r == t->buckets[n])
		t->buckets[n] = r->next;
	else
		s->next = r->next;

	table_entry_destroy(r);

	t->num_entries--;

	/* unlock */
	table_unlock(t);

	return old_value;
}

int table_first_key(struct table *t) {
	int ref, i;
	int32_t value;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

#ifdef DMP
	if (t->dmp != NULL)
		table_dmp_store(t->dmp);
#endif

	/* lock */
	table_wrlock(t);

	for (i = 0; i < t->current_capacity; i++) {
		if (t->buckets[i] != NULL)
			break;
	}

	if (i >= t->current_capacity) {
		value = 1;
		t->iterator_is_running = 0;
	} else {
		value = 0;
		t->iterator_is_running = 1;
		t->iterator_bucket = i;
		t->iterator_entry = t->buckets[i];
	}

	/* unlock */
	table_unlock(t);

	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int table_next_key(struct table *t) {
	int n, prev;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

#ifdef DMP
	if (t->dmp != NULL)
		table_dmp_load(t->dmp);
#endif

	/* lock */
	table_wrlock(t);

	if (t->iterator_is_running == 0) {
		/* unlock */
		table_unlock(t);
		return 0;
	}

#ifdef DMP
	if (t->dmp != NULL)
		table_dmp_store(t->dmp);
#endif

	prev = t->iterator_entry->key;

	t->iterator_entry = t->iterator_entry->next;
	if (t->iterator_entry != NULL) {
		/* unlock */
		table_unlock(t);
		return prev;
	}

	for (n = t->iterator_bucket+1; n < t->current_capacity; n++) {
		if (t->buckets[n] != NULL) {
			t->iterator_bucket = n;
			t->iterator_entry = t->buckets[n];
			break;
		}
	}

	if (n >= t->current_capacity)
		t->iterator_is_running = 0;

	/* unlock */
	table_unlock(t);

	return prev;
}

int table_resize(struct table *t, int n, struct table **nt) {
	int i;
	struct table_entry *r;
	struct table *new_table;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	table_wrlock(t);

	if ((new_table = table_create(n, t->object)) == NULL)
		mvm_halt();

	for (i = 0; i < t->current_capacity; i++ ) {
		for (r = t->buckets[i]; r != NULL; r = r->next) {
			table_put_aux(new_table,
				      heap_fetch_object(heap, r->key),
				      heap_fetch_object(heap, r->value),
				      r->hash);
		}
	}

	table_clear(t);
	heap_free(heap, t->buckets);
	memcpy(t, new_table, sizeof(struct table));
	heap_free(heap, new_table);

	if (nt != NULL)
		*nt = new_table;

	object_set_table(t->object, t);

	/* unlock */
	table_unlock(t);

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
	int i;
	struct table_entry *r;
	struct object *key_object, *value_object;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	fprintf(stderr, "num_entries = %d\n", t->num_entries);
	fprintf(stderr, "load_factor = %f\n", t->load_factor);
	fprintf(stderr, "current_capacity = %d\n", t->current_capacity);

	for (i = 0; i < t->current_capacity; i++) {
		fprintf(stderr, "bucket %d =\n", i);

		for (r = t->buckets[i]; r != NULL; r = r->next) {
			key_object = heap_fetch_object(heap, r->key);
			value_object = heap_fetch_object(heap, r->value);

			fprintf(stderr, "               (");
			object_dump(key_object, 0);
			fprintf(stderr, ", ");
			object_dump(value_object, 0);
			fprintf(stderr, ")\n");
		}
	}

	return 0;
}

struct table_entry * table_entry_create(int k, int v, int h) {
	struct table_entry *r;

	if ((r = (struct table_entry *)heap_malloc(heap, sizeof(struct table_entry))) == NULL)
		mvm_halt();

	r->key = k;
	r->value = v;
	r->hash = h;

	return r;
}

void table_entry_destroy(struct table_entry *r) {
	if (r != NULL) {
		heap_free(heap, r);
	}
}

int table_wrlock(struct table *t) {
#ifdef TABLE_USE_RWLOCK
	int err;

	if (t == NULL) {
		fprintf(stderr, "mvm: table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	if ((err = pthread_rwlock_wrlock(&t->rwlock)) != 0) {
		fprintf(stderr, "mvm: pthread_rwlock_wrlock: %s\n", strerror(err));
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
	if ((err = pthread_rwlock_rdlock(&t->rwlock)) != 0) {
		fprintf(stderr, "mvm: pthread_rwlock_rdlock: %s\n", strerror(err));
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
