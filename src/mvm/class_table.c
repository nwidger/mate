/* Niels Widger
 * Time-stamp: <23 Dec 2012 at 20:53:00 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "class.h"
#include "class_table.h"
#include "constants.h"
#include "garbage_collector.h"
#include "globals.h"
#include "heap.h"
#include "integer.h"
#include "native_method_array.h"
#include "nlock.h"
#include "object.h"
#include "real.h"
#include "string.h"
#include "table.h"
#include "thread.h"

/* struct definitions */
struct string_cache_record {
	int ref;
	char *chars;

	struct string_cache_record *next;
};

struct class_table {
	int num_classes;
	int size;
	struct class **classes;
	struct class *predefined_classes[NUM_PREDEFINED_CLASSES];
	struct nlock *nlock;

	struct string_cache_record *string_cache[CLASS_TABLE_STRING_CACHE_SIZE];
	int integer_cache[CLASS_TABLE_INTEGER_CACHE_SIZE];
};

/* forwared declarations */
int class_table_compare_strings(const void *s, const void *t);
struct string_cache_record * string_cache_record_create(int r, char *c);
void string_cache_record_destroy(struct string_cache_record *r);

struct class_table * class_table_create(int n) {
	struct class_table *c;

	if ((c = (struct class_table *)malloc(sizeof(struct class_table))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	c->num_classes = 0;
	c->size = n;

	if ((c->classes = (struct class **)calloc(n, sizeof(struct class *))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	memset(c->predefined_classes, 0, sizeof(struct class *)*NUM_PREDEFINED_CLASSES);

	if ((c->nlock = nlock_create()) == NULL)
		mvm_halt();

	memset(c->integer_cache, 0, sizeof(int)*CLASS_TABLE_INTEGER_CACHE_SIZE);
	memset(c->string_cache, 0, sizeof(struct string_cache_record *)*CLASS_TABLE_STRING_CACHE_SIZE);

	return c;
}

void class_table_destroy(struct class_table *c) {
	if (c != NULL) {
		/* lock */
		/* class_table_lock(c); */

		class_table_clear(c);

		/* unlock */
		/* class_table_unlock(c); */

		nlock_destroy(c->nlock);
		free(c->classes);
		free(c);
	}
}

void class_table_clear(struct class_table *c) {
	int i;

	if (c != NULL) {
		/* lock */
		/* class_table_lock(c); */

		for (i = 0; i < c->size; i++) {
			if (c->classes[i] != NULL) {
				class_destroy(c->classes[i]);
				c->classes[i] = NULL;
			}
		}

		c->num_classes = 0;

		memset(c->predefined_classes, 0, sizeof(struct class *)*NUM_PREDEFINED_CLASSES);

		memset(c->integer_cache, 0, sizeof(int) * CLASS_TABLE_INTEGER_CACHE_SIZE);
		memset(c->string_cache, 0, sizeof(struct string_cache_record *) * CLASS_TABLE_STRING_CACHE_SIZE);

		/* unlock */
		/* class_table_unlock(c); */
	}
}

int class_table_add(struct class_table *c, enum class_type t, struct class *n) {
	if (c == NULL) {
		fprintf(stderr, "mvm: class table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	/* class_table_lock(c); */

	if (c->num_classes >= c->size) {
		fprintf(stderr, "mvm: class table full!\n");
		/* unlock */
		/* class_table_unlock(c); */
		mvm_halt();
	}

	if (t != user_type)
		c->predefined_classes[t] = n;

	c->classes[c->num_classes++] = n;

	/* unlock */
	/* class_table_unlock(c); */

	return 0;
}

struct class * class_table_find(struct class_table *c, uint32_t v) {
	int i;

	if (c == NULL) {
		fprintf(stderr, "mvm: class table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	/* class_table_lock(c); */

	for (i = 0; i < c->num_classes; i++) {
		if (class_get_vmt(c->classes[i]) == v) {
			/* unlock */
			/* class_table_unlock(c); */
			return c->classes[i];
		}
	}

	/* unlock */
	/* class_table_unlock(c); */

	return NULL;
}

struct class * class_table_find_predefined(struct class_table *c, enum class_type t) {
	struct class *class;

	if (c == NULL) {
		fprintf(stderr, "mvm: class table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	/* class_table_lock(c); */

	class = (t == user_type) ? NULL : c->predefined_classes[t];

	/* unlock */
	/* class_table_unlock(c); */

	return class;
}

int class_table_new(struct class_table *c, uint32_t v, struct object **o) {
	int ref;
	uint32_t num_fields;
	struct object *object;
	struct class *class;

	if (c == NULL) {
		fprintf(stderr, "mvm: class table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	/* class_table_lock(c); */

	if ((class = class_table_find(c, v)) == NULL) {
		/* unlock */
		/* class_table_unlock(c); */
		mvm_halt();
	}

	num_fields = class_get_num_fields(class);

	if ((ref = object_create(class, num_fields, &object)) == 0) {
		/* unlock */
		/* class_table_unlock(c); */
		mvm_halt();
	}

	if (o != NULL)
		*o = object;

	/* unlock */
	/* class_table_unlock(c); */

	return ref;
}

int class_table_new_object(struct class_table *c, struct object **o) {
	int ref;
	uint32_t vmt;
	struct object *object;
	struct class *object_class;

	if (c == NULL) {
		fprintf(stderr, "mvm: class table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	/* class_table_lock(c); */

	object_class = c->predefined_classes[object_type];
	vmt = class_get_vmt(object_class);
	ref = class_table_new(class_table, vmt, &object);

	if (o != NULL)
		*o = object;

	/* unlock */
	/* class_table_unlock(c); */

	return ref;
}

int class_table_new_integer(struct class_table *c, int32_t v, struct object **o) {
	int ref;
	uint32_t vmt;
	struct object *object;
	struct integer *integer;
	struct class *integer_class;

	if (c == NULL) {
		fprintf(stderr, "mvm: class table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	/* class_table_lock(c); */

	if (v >= 0 && v <= CLASS_TABLE_INTEGER_CACHE_SIZE && c->integer_cache[(int)v] != 0) {
		ref = c->integer_cache[(int)v];
	} else {
		integer_class = c->predefined_classes[integer_type];
		vmt = class_get_vmt(integer_class);

		/* lock */
		garbage_collector_pause(garbage_collector);

		ref = class_table_new(class_table, vmt, &object);
		heap_exclude_ref(heap, ref);

		/* unlock */
		garbage_collector_unpause(garbage_collector);

		integer = integer_create(v);
		if (v < 0 || v > CLASS_TABLE_INTEGER_CACHE_SIZE) heap_include_ref(heap, ref);
		object_set_integer(object, integer);
		if (v >= 0 && v <= CLASS_TABLE_INTEGER_CACHE_SIZE) c->integer_cache[(int)v] = ref;
	}

	if (o != NULL)
		*o = object;

	/* unlock */
	/* class_table_unlock(c); */

	return ref;
}

int class_table_new_string(struct class_table *c, char *b, struct object **o, int s) {
	uint32_t vmt;
	struct object *object;
	struct string *string;
	int ref, i, hash, n, len;
	struct class *string_class;
	struct string_cache_record *r, *new_entry;

	if (c == NULL) {
		fprintf(stderr, "mvm: class table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	/* class_table_lock(c); */

	string_class = c->predefined_classes[string_type];
	vmt = class_get_vmt(string_class);

	n = 0;
	r = NULL;

	if (s != 0 && CLASS_TABLE_STRING_CACHE_SIZE != 0) {
		len = strlen(b);
		hash = 0; for (i = 0; i < len; i++) hash = (31*hash) + (int32_t)b[i];
		hash = abs(hash);
		
		n = hash % CLASS_TABLE_STRING_CACHE_SIZE;

		for (r = c->string_cache[n]; r != NULL; r = r->next) {
			if (strcmp(b, r->chars) == 0)
				break;
		}
	}

	if (r != NULL) {
		ref = r->ref;
	} else {
		/* lock */
		garbage_collector_pause(garbage_collector);

		ref = class_table_new(class_table, vmt, &object);
		heap_exclude_ref(heap, ref);

		/* unlock */
		garbage_collector_unpause(garbage_collector);

		string = string_create(b);
		if (s == 0 || CLASS_TABLE_STRING_CACHE_SIZE == 0) heap_include_ref(heap, ref);
		object_set_string(object, string);
		if (s != 0 && CLASS_TABLE_STRING_CACHE_SIZE != 0) {
			new_entry = string_cache_record_create(ref, b);
			new_entry->next = c->string_cache[n];
			c->string_cache[n] = new_entry;
		}
	}

	if (o != NULL)
		*o = object;

	/* unlock */
	/* class_table_unlock(c); */

	return ref;
}

int class_table_new_table(struct class_table *c, int n, struct object **o) {
	int ref;
	uint32_t vmt;
	struct object *object;
	struct table *table;
	struct class *table_class;

	if (c == NULL) {
		fprintf(stderr, "mvm: class table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	/* class_table_lock(c); */

	table_class = c->predefined_classes[table_type];
	vmt = class_get_vmt(table_class);

	/* lock */
	garbage_collector_pause(garbage_collector);

	ref = class_table_new(class_table, vmt, &object);
	heap_exclude_ref(heap, ref);

	/* unlock */
	garbage_collector_unpause(garbage_collector);

	table = table_create(n, object);
	heap_include_ref(heap, ref);
	object_set_table(object, table);

	if (o != NULL)
		*o = object;

	/* unlock */
	/* class_table_unlock(c); */

	return ref;
}

int class_table_new_thread(struct class_table *c, struct object **o) {
	int ref;
	uint32_t vmt;
	struct object *object;
	struct thread *thread;
	struct class *thread_class;

	if (c == NULL) {
		fprintf(stderr, "mvm: class table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	/* class_table_lock(c); */

	thread_class = c->predefined_classes[thread_type];
	vmt = class_get_vmt(thread_class);

	/* lock */
	garbage_collector_pause(garbage_collector);

	ref = class_table_new(class_table, vmt, &object);
	heap_exclude_ref(heap, ref);

	/* unlock */
	garbage_collector_unpause(garbage_collector);

	thread = thread_create();
	/* thread instances are not collectable until run method terminates */
	/* heap_include_ref(heap, ref); */
	object_set_thread(object, thread);
	heap_add_thread_ref(heap, ref);

	if (o != NULL)
		*o = object;

	/* unlock */
	/* class_table_unlock(c); */

	return ref;
}

int class_table_new_real(struct class_table *c, float v, struct object **o) {
	int ref;
	uint32_t vmt;
	struct object *object;
	struct real *real;
	struct class *real_class;

	if (c == NULL) {
		fprintf(stderr, "mvm: class table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	/* class_table_lock(c); */

	real_class = c->predefined_classes[real_type];
	vmt = class_get_vmt(real_class);

	/* lock */
	garbage_collector_pause(garbage_collector);

	ref = class_table_new(class_table, vmt, &object);
	heap_exclude_ref(heap, ref);

	/* unlock */
	garbage_collector_unpause(garbage_collector);

	real = real_create(v);
	heap_include_ref(heap, ref);
	object_set_real(object, real);

	if (o != NULL)
		*o = object;

	/* unlock */
	/* class_table_unlock(c); */

	return ref;
}

int class_table_analyze(struct class_table *c) {
	int i;
	uint32_t super_vmt;
	struct class *class, *super_class;

	if (c == NULL) {
		fprintf(stderr, "mvm: class table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	/* class_table_lock(c); */

	for (i = 0; i < c->num_classes; i++) {
		class = c->classes[i];

		super_vmt = class_get_super_vmt(class);
		if (super_vmt == 0) /* object class */
			continue;

		super_class = class_table_find(c, super_vmt);
		if (super_class == NULL) {
			fprintf(stderr, "mvm: invalid super class!\n");
			/* unlock */
			/* class_table_unlock(c); */
			mvm_halt();
		}

		class_set_super_class(class, super_class);
	}

	/* unlock */
	/* class_table_unlock(c); */

	return 0;
}

int class_table_lock(struct class_table *c) {
	if (c == NULL) {
		fprintf(stderr, "mvm: class table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	/* nlock_lock(c->nlock); */

	return 0;
}

int class_table_unlock(struct class_table *c) {
	if (c == NULL) {
		fprintf(stderr, "mvm: class table not initialized!\n");
		mvm_halt();
	}

	/* unlock */
	/* nlock_unlock(c->nlock); */

	return 0;
}

void class_table_dump(struct class_table *c) {
	int i;
	uint32_t j, native_index, address, end, max_locals;
	char *method_name;
	struct class *class, *super_class;

	if (c == NULL)
		return;

	/* lock */
	/* class_table_lock(c); */

	for (i = 0; i < c->num_classes; i++) {
		class = c->classes[i];
		super_class = class_get_super_class(class);

		fprintf(stderr, "%s:\n", class_get_name(class));
		fprintf(stderr, "    super_class: %s\n",
			(super_class == NULL) ? "Object" : class_get_name(super_class));

		for (j = 0; j < class_get_num_methods(class); j++) {
			method_name = class_get_method_name(class, j);

			fprintf(stderr, "    method %s:\n", method_name);

			address = class_get_method_field(class, j, address_field);
			end = class_get_method_field(class, j, end_field);
			max_locals = class_get_method_field(class, j, max_locals_field);

			if (class_method_is_native(class, j) != 0) {
				native_index = native_method_array_get_index(native_method_array, method_name);
				fprintf(stderr, "        native_index: %" PRIu32 "\n", native_index);
			} else {
				fprintf(stderr, "        address: %" PRIu32 "\n", address);
				fprintf(stderr, "        end: %" PRIu32 "\n", end);
				fprintf(stderr, "        max_locals: %" PRIu32 "\n", max_locals);
			}
		}
	}

	/* unlock */
	/* class_table_unlock(c); */
}

void class_table_decode(struct class_table *c) {
	int i;
	uint32_t j, address, max_locals;
	char *method_name;
	struct class *class, *super_class;

	if (c == NULL) {
		fprintf(stderr, "mvm: class table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	/* class_table_lock(c); */

	fprintf(stderr, "%d\n", c->num_classes);

	for (i = 0; i < c->num_classes; i++) {
		class = c->classes[i];
		fprintf(stderr, "$%s \"%s\"\n", class_get_name(class), class_get_name(class));
	}

	for (i = 0; i < c->num_classes; i++) {
		class = c->classes[i];
		super_class = class_get_super_class(class);

		fprintf(stderr, "%s:\n", class_get_name(class));
		if (super_class == NULL)
			fprintf(stderr, "  0\n");
		else
			fprintf(stderr, "  $%s\n", class_get_name(super_class));
		fprintf(stderr, "  %" PRIu32 "\n", class_get_num_fields(class));
		fprintf(stderr, "  %" PRIu32 "\n", class_get_num_methods(class));

		for (j = 0; j < class_get_num_methods(class); j++) {
			method_name = class_get_method_name(class, j);
			address = class_get_method_field(class, j, address_field);
			max_locals = class_get_method_field(class, j, max_locals_field);

			fprintf(stderr, "  %" PRIu32 " %" PRIu32 " \"%s\"\n",
				address, max_locals, method_name);
		}
	}

	/* unlock */
	/* class_table_unlock(c); */
}

char ** class_table_classes_array(struct class_table *c) {
	int i;
	char **buf;

	if (c == NULL) {
		fprintf(stderr, "mvm: class table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	/* class_table_lock(c); */

	if ((buf = (char **)malloc(sizeof(char *)*(c->num_classes+1))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	for (i = 0; i < c->num_classes; i++) {
		if ((buf[i] = strdup(class_get_name(c->classes[i]))) == NULL) {
			perror("mvm: strdup");
			mvm_halt();
		}
	}

	buf[c->num_classes] = NULL;

	qsort(buf, c->num_classes, sizeof(char *), class_table_compare_strings);

	/* unlock */
	/* class_table_unlock(c); */

	return buf;
}

char ** class_table_methods_array(struct class_table *c) {
	int size, i, j, k;
	char **buf;
	struct class *class;

	if (c == NULL) {
		fprintf(stderr, "mvm: class table not initialized!\n");
		mvm_halt();
	}

	/* lock */
	/* class_table_lock(c); */

	size = 0;
	for (i = 0; i < c->num_classes; i++)
		size += class_get_num_methods(c->classes[i]);

	if ((buf = (char **)malloc(sizeof(char *)*(size+1))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	k = 0;
	for (i = 0; i < c->num_classes; i++) {
		class = c->classes[i];
		for (j = 0; j < class_get_num_methods(class); j++) {
			if ((buf[k++] = strdup(class_get_method_name(class, j))) == NULL) {
				perror("mvm: strdup");
				mvm_halt();
			}
		}
	}

	buf[k] = NULL;

	qsort(buf, size, sizeof(char *), class_table_compare_strings);

	/* unlock */
	/* class_table_unlock(c); */

	return buf;
}

int class_table_compare_strings(const void *s, const void *t) {
	return strcmp((char *)s, (char *)t);
}


struct string_cache_record * string_cache_record_create(int r, char *c) {
	struct string_cache_record *rec;

	if ((rec = (struct string_cache_record *)malloc(sizeof(struct string_cache_record))) == NULL)
		mvm_halt();

	rec->ref = r;
	
	if ((rec->chars = strdup(c)) == NULL) {
		perror("mvm: strdup");
		mvm_halt();
	}
	
	rec->next = NULL;

	return rec;
}

void string_cache_record_destroy(struct string_cache_record *rec) {
	if (rec != NULL) {
		free(rec->chars);
		free(rec);
	}
}
