/* Niels Widger
 * Time-stamp: <03 Sep 2012 at 21:48:12 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "class.h"
#include "class_table.h"
#include "constants.h"
#include "globals.h"
#include "heap.h"
#include "integer.h"
#include "object.h"
#include "string.h"

/* struct definitions */
struct string {
	char *chars;
	int length;
};

struct string * string_create(char *c) {
	struct string *s;

	if ((s = (struct string *)heap_malloc(heap, sizeof(struct string))) == NULL)
		mvm_halt();

	s->length = strlen(c);

	if ((s->chars = (char *)heap_malloc(heap, sizeof(char)*(s->length+1))) == NULL)
		mvm_halt();
	strcpy(s->chars, c);

	return s;
}

void string_destroy(struct string *s) {
	if (s != NULL) {
		string_clear(s);
		if (s->chars != NULL)
			heap_free(heap, s->chars);
		heap_free(heap, s);
	}
}

void string_clear(struct string *s) {
	if (s != NULL) {

	}
}

char * string_get_chars(struct string *s) {
	if (s == NULL) {
		fprintf(stderr, "mvm: string has not been initialized!\n");
		mvm_halt();
	}

	return s->chars;
}

int string_length(struct object *o) {
	int ref;
	struct string *s;

	s = object_get_string(o);

	if (s == NULL) {
		fprintf(stderr, "mvm: string has not been initialized!\n");
		mvm_halt();
	}

	if ((ref = class_table_new_integer(class_table, (int32_t)s->length, NULL)) == 0)
		mvm_halt();

	return ref;
}

int string_substr(struct object *o, struct object *p, struct object *q) {
	int ref;
	char *buf;
	struct string *s;
	struct integer *b, *e;
	int32_t beg_index, end_index;

	s = object_get_string(o);

	if (s == NULL) {
		fprintf(stderr, "mvm: string has not been initialized!\n");
		mvm_halt();
	}

	b = object_get_integer(p);
	e = object_get_integer(q);

	beg_index = integer_get_value(b);
	end_index = integer_get_value(e);

	if (s->length <= 0) {
		fprintf(stderr, "mvm: cannot invoke substr, String has length 0!\n");
		mvm_halt();
	}

	if (beg_index < 0 || beg_index >= s->length) {
		fprintf(stderr, "mvm: invalid beginning index!\n");
		mvm_halt();
	}

	if (end_index < beg_index || end_index < 0 || end_index >= s->length) {
		fprintf(stderr, "mvm: invalid end index!\n");
		mvm_halt();
	}

	if ((buf = (char *)malloc(sizeof(char)*(end_index-beg_index+2))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	strncpy(buf, s->chars+beg_index, (end_index-beg_index)+2);
	buf[(end_index-beg_index)+1] = '\0';

	if ((ref = class_table_new_string(class_table, buf, NULL)) == 0)
		mvm_halt();

	free(buf);
	return ref;
}

int string_concat(struct object *o, struct object *p) {
	char *buf;
	int ref, retval_length;
	struct string *s, *t;

	s = object_get_string(o);
	t = object_get_string(p);

	if (s == NULL || t == NULL) {
		fprintf(stderr, "mvm: string has not been initialized!\n");
		mvm_halt();
	}

	retval_length = s->length+t->length;

	if ((buf = (char *)malloc(sizeof(char)*(retval_length+1))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	strcpy(buf, s->chars);
	strcat(buf, t->chars);

	if ((ref = class_table_new_string(class_table, buf, NULL)) == 0)
		mvm_halt();

	free(buf);
	return ref;
}

int string_to_integer(struct object *o) {
	int ref;
	int64_t value;
	char *ptr;
	struct string *s;

	s = object_get_string(o);

	if (s == NULL) {
		fprintf(stderr, "mvm: string has not been initialized!\n");
		mvm_halt();
	}

	if (s->length <= 0) {
		fprintf(stderr, "mvm: cannot invoke toInteger on String of length zero!\n");
		mvm_halt();
	}

	ptr = (s->chars[0] == '-') ? s->chars+1 : s->chars;
	if (strspn(ptr, "0123456789") != strlen(ptr)) {
		fprintf(stderr, "mvm: invalid characters, cannot invoke toInteger\n");
		mvm_halt();
	}

        errno = 0;
	value = strtol(s->chars, &ptr, 10);
	if (errno != 0 || ptr == s->chars) {
		perror("mvm: strtol");
		mvm_halt();
	}

	if (value > INTEGER_MAX_INTEGER || value < INTEGER_MIN_INTEGER) {
		fprintf(stderr, "mvm: String integer value out of range!\n");
		mvm_halt();
	}

	if ((ref = class_table_new_integer(class_table, (int32_t)value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int string_to_real(struct object *o) {
	int ref;
	char *ptr;
	float value;
	struct string *s;

	s = object_get_string(o);

	if (s == NULL) {
		fprintf(stderr, "mvm: string has not been initialized!\n");
		mvm_halt();
	}

	if (s->length <= 0) {
		fprintf(stderr, "mvm: cannot invoke toReal on String of length zero!\n");
		mvm_halt();
	}

	ptr = (s->chars[0] == '-') ? s->chars+1 : s->chars;
	if (strspn(ptr, "0123456789.eE+-") != strlen(ptr)) {
		fprintf(stderr, "mvm: invalid characters, cannot invoke toReal\n");
		mvm_halt();
	}

        errno = 0;
	value = strtof(s->chars, &ptr);
	if (errno != 0 || ptr == s->chars) {
		perror("mvm: strtof");
		mvm_halt();
	}

	if ((ref = class_table_new_real(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int string_greater_than(struct object *o, struct object *p) {
	int ref;
	int32_t value;
	struct string *s, *t;

	s = object_get_string(o);
	t = object_get_string(p);

	if (s == NULL || t == NULL) {
		fprintf(stderr, "mvm: string has not been initialized!\n");
		mvm_halt();
	}

	value = (strcmp(s->chars, t->chars) > 0) ? 1 : 0;
	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int string_less_than(struct object *o, struct object *p) {
	int ref;
	int32_t value;
	struct string *s, *t;

	s = object_get_string(o);
	t = object_get_string(p);

	if (s == NULL || t == NULL) {
		fprintf(stderr, "mvm: string has not been initialized!\n");
		mvm_halt();
	}

	value = (strcmp(s->chars, t->chars) < 0) ? 1 : 0;
	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int string_hash_code(struct object *o) {
	int ref, i;
	int32_t value;
	struct string *s;

	s = object_get_string(o);

	if (s == NULL) {
		fprintf(stderr, "mvm: string has not been initialized!\n");
		mvm_halt();
	}

	value = 0;
	for (i = 0; i < s->length; i++)
		value += (int32_t)s->chars[i];

	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int string_equals(struct object *o, struct object *p) {
	int ref;
	int32_t value;
	struct class *string_class;
	struct string *s, *t;

	string_class = class_table_find_predefined(class_table, string_type);
	
	if (class_can_cast_to(object_get_class(p), string_class) == 0) {
		value = 0;
	} else {
		s = object_get_string(o);
		t = object_get_string(p);

		if (s == NULL || t == NULL) {
			fprintf(stderr, "mvm: string has not been initialized!\n");
			mvm_halt();
		}

		value = (strcmp(s->chars, t->chars) == 0) ? 1 : 0;
	}

	if ((ref = class_table_new_integer(class_table, value, NULL)) == 0)
		mvm_halt();

	return ref;
}

int string_to_string(struct object *o) {
	int ref;
	struct string *s;

	s = object_get_string(o);

	if (s == NULL) {
		fprintf(stderr, "mvm: string has not been initialized!\n");
		mvm_halt();
	}

	if ((ref = class_table_new_string(class_table, s->chars, NULL)) == 0)		
		mvm_halt();

	return ref;
}
