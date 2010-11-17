/* Niels Widger
 * Time-stamp: <09 Apr 2010 at 16:36:40 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "constants.h"
#include "class.h"
#include "globals.h"

/* struct definitions */
struct method {
	char *name;
	uint32_t address;
	uint32_t end;
	uint32_t max_locals;
};

struct class {
	char *name;
	
	uint32_t vmt;
	uint32_t super_vmt;
	struct class *super_class;
	uint32_t num_methods;
	struct method **methods;
	uint32_t num_fields;
};

/* forward declarations */
int class_is_descended_from(struct class *c, struct class *t);
int class_can_widen_to(struct class *c, struct class *t);
int class_can_narrow_to(struct class *c, struct class *t);
int class_equals(struct class *c, struct class *t);
struct method * method_create(char *n, uint32_t a, uint32_t e, uint32_t m);
void method_destroy(struct method *r);

struct class * class_create(char *s, uint32_t v, uint32_t n) {
	struct class *cl;

	if ((cl = (struct class *)malloc(sizeof(struct class))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	if ((cl->name = strdup(s)) == NULL) {
		perror("mvm: strdup");
		mvm_halt();
	}

	cl->vmt = v;
	cl->super_vmt = 0;
	cl->super_class = NULL;

	cl->num_methods = n;

	if ((cl->methods = (struct method **)
	     calloc(n, sizeof(struct method *))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	cl->num_fields = 0;

	return cl;
}

void class_destroy(struct class *c) {
	uint32_t i;

	if (c != NULL) {
		class_clear(c);
		if (c->num_methods > 0) {
			for (i = 0; i < c->num_methods; i++)
				free(c->methods[i]);
			free(c->methods);
		}
		free(c->name);
		free(c);
	}
}

void class_clear(struct class *c) {
	int i;

	if (c != NULL) {
		c->vmt = 0;
		c->super_vmt = 0;
		c->super_class = NULL;

		for (i = 0; i < c->num_methods; i++) {
			if (c->methods[i] != NULL)
				method_destroy(c->methods[i]);
		}

		memset(c->methods, 0, sizeof(struct method *)*c->num_methods);
	}
}

int class_set_super_vmt(struct class *c, uint32_t s) {
	if (c == NULL) {
		fprintf(stderr, "mvm: class not initialized!\n");
		mvm_halt();
	}

	c->super_vmt = s;
	return 0;
}

int class_set_super_class(struct class *c, struct class *s) {
	if (c == NULL) {
		fprintf(stderr, "mvm: class not initialized!\n");
		mvm_halt();
	}

	c->super_class = s;
	return 0;
}

int class_set_method(struct class *c, int i, char *n,
		     uint32_t a, uint32_t e, uint32_t m) {
	if (c == NULL) {
		fprintf(stderr, "mvm: class not initialized!\n");
		mvm_halt();
	}

	if (i < 0 || i >= c->num_methods) {
		fprintf(stderr, "mvm: invalid method index %d!\n", i);
		mvm_halt();
	}

	if ((c->methods[i] = method_create(n, a, e, m)) == NULL)
		mvm_halt();

	return 0;
}

int class_set_native_method(struct class *c, int i, char *s, uint32_t n) {
	if (c == NULL) {
		fprintf(stderr, "mvm: class not initialized!\n");
		mvm_halt();
	}

	if (i < 0 || i >= c->num_methods) {
		fprintf(stderr, "mvm: invalid method index!\n");
		mvm_halt();
	}

	if ((c->methods[i] = method_create(s, NATIVE_ADDRESS,
					   NATIVE_ADDRESS, n)) == NULL)
		mvm_halt();

	return 0;
}

int class_set_num_fields(struct class *c, uint32_t n) {
	if (c == NULL) {
		fprintf(stderr, "mvm: class not initialized!\n");
		mvm_halt();
	}

	c->num_fields = n;
	
	return 0;
}

uint32_t class_get_vmt(struct class *c) {
	if (c == NULL) {
		fprintf(stderr, "mvm: class not initialized!\n");
		mvm_halt();
	}

	return c->vmt;
}

char * class_get_name(struct class *c) {
	if (c == NULL) {
		fprintf(stderr, "mvm: class not initialized!\n");
		mvm_halt();
	}

	return c->name;
}

uint32_t class_get_super_vmt(struct class *c) {
	if (c == NULL) {
		fprintf(stderr, "mvm: class not initialized!\n");
		mvm_halt();
	}

	return c->super_vmt;
}

struct class * class_get_super_class(struct class *c) {
	if (c == NULL) {
		fprintf(stderr, "mvm: class not initialized!\n");
		mvm_halt();
	}

	return c->super_class;
}


uint32_t class_get_num_fields(struct class *c) {
	if (c == NULL) {
		fprintf(stderr, "mvm: class not initialized!\n");
		mvm_halt();
	}

	return c->num_fields;
}

uint32_t class_get_num_methods(struct class *c) {
	if (c == NULL) {
		fprintf(stderr, "mvm: class not initialized!\n");
		mvm_halt();
	}

	return c->num_methods;
}

int class_method_is_native(struct class *c, int i) {
	struct method *r;
	
	if (c == NULL) {
		fprintf(stderr, "mvm: class not initialized!\n");
		mvm_halt();
	}

	if (i < 0 || i >= c->num_methods) {
		fprintf(stderr, "mvm: invalid method index!\n");
		mvm_halt();
	}

	if ((r = c->methods[i]) == NULL) {
		fprintf(stderr, "mvm: method was not set!\n");
		mvm_halt();
	}

	return r->address == NATIVE_ADDRESS;
}

uint32_t class_get_method_field(struct class *c, int i, enum class_field f) {
	struct method *r;
	
	if (c == NULL) {
		fprintf(stderr, "mvm: class not initialized!\n");
		mvm_halt();
	}

	if (i < 0 || i >= c->num_methods) {
		fprintf(stderr, "mvm: invalid method index!\n");
		mvm_halt();
	}

	if ((r = c->methods[i]) == NULL) {
		fprintf(stderr, "mvm: method was not set!\n");
		mvm_halt();
	}

	switch (f) {
	case address_field:
		return r->address;
	case end_field:
		return r->end;
	case max_locals_field:
		return r->max_locals;
	default:
		return 0;
	}
}

char * class_get_method_name(struct class *c, int i) {
	struct method *r;
	
	if (c == NULL) {
		fprintf(stderr, "mvm: class not initialized!\n");
		mvm_halt();
	}

	if (i < 0 || i >= c->num_methods) {
		fprintf(stderr, "mvm: invalid method index!\n");
		mvm_halt();
	}

	if ((r = c->methods[i]) == NULL) {
		fprintf(stderr, "mvm: method was not set!\n");
		mvm_halt();
	}

	return r->name;
}

int class_is_descended_from(struct class *c, struct class *t) {
	struct class *p;

	if (c == NULL || t == NULL) {
		fprintf(stderr, "mvm: class not initialized!\n");
		mvm_halt();
	}

	for (p = c; p != NULL; p = class_get_super_class(p)) {
		if (p == t)
			return 1;
	}

	return 0;
}

int class_can_cast_to(struct class *c, struct class *t) {
	if (c == NULL || t == NULL) {
		fprintf(stderr, "mvm: class not initialized!\n");
		mvm_halt();
	}

	if (class_equals(c, t))
		return 1;
	if (class_can_widen_to(c, t))
		return 1;
/* 	if (class_can_narrow_to(c, t)) */
/* 		return 1; */

	return 0;
}

int class_can_widen_to(struct class *c, struct class *t) {
	if (c == NULL || t == NULL) {
		fprintf(stderr, "mvm: class not initialized!\n");
		mvm_halt();
	}

	if (class_get_super_vmt(t) == 0)
		return 1;
	if (class_is_descended_from(c, t))
		return 1;

	return 0;
}

int class_can_narrow_to(struct class *c, struct class *t) {
	if (c == NULL || t == NULL) {
		fprintf(stderr, "mvm: class not initialized!\n");
		mvm_halt();
	}

	if (class_get_super_vmt(c) == 0)
		return 1;
	if (class_is_descended_from(t, c))
		return 1;

	return 0;
}

int class_equals(struct class *c, struct class *t) {
	if (c == NULL || t == NULL) {
		fprintf(stderr, "mvm: class not initialized!\n");
		mvm_halt();
	}

	return class_get_vmt(c) == class_get_vmt(t);
}

struct method * method_create(char *n, uint32_t a, uint32_t e, uint32_t m) {	
	struct method *r;

	if ((r = (struct method *)malloc(sizeof(struct method))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	if ((r->name = strdup(n)) == NULL) {
		perror("mvm: strdup");
		mvm_halt();
	}

	r->address = a;
	r->end = e;
	r->max_locals = m;

	return r;
}

void method_destroy(struct method *r) {
	if (r != NULL) {
		free(r->name);
		free(r);
	}
}
