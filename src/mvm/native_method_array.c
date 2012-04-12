/* Niels Widger
 * Time-stamp: <11 Apr 2012 at 20:18:54 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "native_method_array.h"

/* struct definitions */
struct native_method_record {
	char *name;
	int num_args;
	int (*method)(uint32_t);
};

struct native_method_array {
	int size;
	struct native_method_record **native_methods;
};

/* forward declarations */
int native_method_array_compare_names(const void *n, const void *o);
struct native_method_record * native_method_record_create(char *n, int (*m)(uint32_t));
void native_method_record_destroy(struct native_method_record *r);
int native_method_record_decode_num_args(char *n);

struct native_method_array * native_method_array_create(int s) {
	struct native_method_array *n;

	if ((n = (struct native_method_array *)
	     malloc(sizeof(struct native_method_array))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	n->size = s;

	if ((n->native_methods = (struct native_method_record **)
	     calloc(n->size, sizeof(struct native_method_record *))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	return n;
}

void native_method_array_destroy(struct native_method_array *n) {
	if (n != NULL) {
		native_method_array_clear(n);
		free(n->native_methods);
		free(n);
	}
}

void native_method_array_clear(struct native_method_array *n) {
	int i;

	if (n != NULL) {
		for (i = 0; i < n->size; i++) {
			if (n->native_methods[i] != NULL)
				native_method_record_destroy(n->native_methods[i]);
		}

		memset(n->native_methods, 0, sizeof(struct native_method_record *)*n->size);
		n->size = 0;
	}
}

int native_method_array_set(struct native_method_array *n, int i, char *s) {
	if (n == NULL) {
		fprintf(stderr, "mvm: native method array not initialized!\n");
		mvm_halt();
	}

	if (i < 0 || i >= n->size) {
		fprintf(stderr, "mvm: bad native method array index!\n");
		mvm_halt();
	}

	if (n->native_methods[i] != NULL)
		native_method_record_destroy(n->native_methods[i]);

	if ((n->native_methods[i] = native_method_record_create(s, NULL)) == NULL)
		mvm_halt();

	return 0;
}

int native_method_array_set_method(struct native_method_array *n, int i, int (*m)(uint32_t)) {
	struct native_method_record *r;
	
	if (n == NULL) {
		fprintf(stderr, "mvm: native method array not initialized!\n");
		mvm_halt();
	}

	if (i < 0 || i >= n->size) {
		fprintf(stderr, "mvm: bad native method array index!\n");
		mvm_halt();
	}

	if ((r = n->native_methods[i]) == NULL) {
		fprintf(stderr, "mvm: native method was not set!\n");
		mvm_halt();
	}

	r->method = m;

	return 0;
}

int native_method_array_execute(struct native_method_array *n, int i) {
	struct native_method_record *r;

	if (n == NULL) {
		fprintf(stderr, "mvm: native method array not initialized!\n");
		mvm_halt();
	}

	if (i < 0 || i >= n->size) {
		fprintf(stderr, "mvm: bad native method array index!\n");
		mvm_halt();
	}

	if ((r = n->native_methods[i]) == NULL) {
		fprintf(stderr, "mvm: native method was not set!\n");
		mvm_halt();
	}

	return (*r->method)(i);
}

char * native_method_array_get_name(struct native_method_array *n, int i) {
	struct native_method_record *r;

	if (n == NULL) {
		fprintf(stderr, "mvm: native method array not initialized!\n");
		mvm_halt();
	}

	if (i < 0 || i >= n->size) {
		fprintf(stderr, "mvm: bad native method array index!\n");
		mvm_halt();
	}

	if ((r = n->native_methods[i]) == NULL) {
		fprintf(stderr, "mvm: native method was not set!\n");
		mvm_halt();
	}

	return r->name;
}

int native_method_array_get_index(struct native_method_array *n, char *s) {
	int i;
	struct native_method_record *r;

	if (n == NULL) {
		fprintf(stderr, "mvm: native method array not initialized!\n");
		mvm_halt();
	}

	for (i = 0; i < n->size; i++) {
		r = n->native_methods[i];
		if (r != NULL && strcmp(r->name, s) == 0)
			return i;
	}

	return -1;
}

int native_method_array_get_num_args(struct native_method_array *n, int i) {
	struct native_method_record *r;

	if (n == NULL) {
		fprintf(stderr, "mvm: native method array not initialized!\n");
		mvm_halt();
	}

	if (i < 0 || i >= n->size) {
		fprintf(stderr, "mvm: bad native method array index!\n");
		mvm_halt();
	}

	if ((r = n->native_methods[i]) == NULL) {
		fprintf(stderr, "mvm: native method was not set!\n");
		mvm_halt();
	}

	return r->num_args;
}

int native_method_array_dump(struct native_method_array *n) {
	int i;
	struct native_method_record *r;

	if (n == NULL) {
		fprintf(stderr, "mvm: native method array not initialized!\n");
		mvm_halt();
	}

	for (i = 0; i < n->size; i++) {
		if ((r = n->native_methods[i]) != NULL)
			fprintf(stderr, "#%d %s\n", i, r->name);
	}

	return 0;
}

char ** native_method_array_methods_array(struct native_method_array *n) {
	int i;
	char **buf;
	struct native_method_record *r;

	if (n == NULL) {
		fprintf(stderr, "mvm: native method array not initialized!\n");
		mvm_halt();
	}

	if ((buf = (char **)malloc(sizeof(char *)*(n->size+1))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	for (i = 0; i < n->size; i++) {
		r = n->native_methods[i];
		if ((buf[i] = strdup(r->name)) == NULL) {
			perror("mvm: strdup");
			mvm_halt();

		}
	}

	qsort(buf, n->size, sizeof(char *), native_method_array_compare_names);

	buf[i] = NULL;
	return buf;
}

int native_method_array_compare_names(const void *n, const void *o) {
	return strcmp((char *)n, (char *)o);
}

struct native_method_record * native_method_record_create(char *n, int (*m)(uint32_t)) {
	struct native_method_record *r;

	if ((r = (struct native_method_record *)malloc(sizeof(struct native_method_record))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	if ((r->name = (char *)malloc(sizeof(char)*(strlen(n)+1))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	strcpy(r->name, n);
	r->num_args = native_method_record_decode_num_args(n);
	r->method = m;

	return r;
}

void native_method_record_destroy(struct native_method_record *r) {
	if (r != NULL) {
		free(r->name);
		free(r);
	}
}

int native_method_record_decode_num_args(char *n) {
	char *p, *q;
	int num_args;

	/* "CLASS_constructor" */
	if ((p = strchr(n, '$')) == NULL)
		return 1;

	if ((q = strchr(n, '_')) != NULL && q < p &&
	    strspn(q, "_constructor") == strlen("_constructor")) {
		/* "CLASS_constructor[$arg]..." */
		num_args = 1;
	} else {
		/* "CLASS$method[$arg]..." */
		num_args = 0;
	}

	while ((n = strchr(n, '$')) != NULL) {
		num_args++;
		n++;
	};

	return num_args;
}
