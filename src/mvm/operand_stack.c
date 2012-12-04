/* Niels Widger
 * Time-stamp: <04 Dec 2012 at 09:59:08 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#include "globals.h"
#include "heap.h"
#include "object.h"
#include "nlock.h"
#include "operand_stack.h"

/* struct definitions */
struct operand_record {
	int ref;
	struct operand_record *next;
};

struct operand_stack {
	struct operand_record *head;
	int size;
	struct nlock *nlock;
};

/* forward declarations */
struct operand_record * operand_record_create(int r, struct operand_record *n);
void operand_record_destroy(struct operand_record *r);

struct operand_stack * operand_stack_create() {
	struct operand_stack *s;

	if ((s = (struct operand_stack *)malloc(sizeof(struct operand_stack))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	s->head = NULL;
	s->size = 0;

	if ((s->nlock = nlock_create()) == NULL)
		mvm_halt();

	return s;
}

void operand_stack_destroy(struct operand_stack *s) {
	if (s != NULL) {
		/* lock */
		operand_stack_lock(s);

		operand_stack_clear(s);

		/* unlock */
		operand_stack_unlock(s);

		nlock_destroy(s->nlock);
		free(s);
	}
}

void operand_stack_clear(struct operand_stack *s) {
	if (s != NULL) {
		/* lock */
		operand_stack_lock(s);

		operand_stack_pop_n(s, operand_stack_size(s));

		/* unlock */
		operand_stack_unlock(s);
	}
}

int operand_stack_push(struct operand_stack *s, int r) {
	struct operand_record *o;

	if (s == NULL) {
		fprintf(stderr, "mvm: operand stack not initialized!\n");
		mvm_halt();
	}

	/* lock */
	operand_stack_lock(s);

	if ((o = operand_record_create(r, s->head)) == NULL) {
		/* unlock */
		operand_stack_unlock(s);
		mvm_halt();
	}

	s->head = o;
	s->size++;

	/* unlock */
	operand_stack_unlock(s);

	return r;
}

int operand_stack_peek(struct operand_stack *s) {
	int ref;

	if (s == NULL) {
		fprintf(stderr, "mvm: operand stack not initialized!\n");
		mvm_halt();
	}

	/* lock */
	operand_stack_lock(s);

	if (operand_stack_empty(s) == 1)
		return 0;

	ref = s->head->ref;

	/* unlock */
	operand_stack_unlock(s);

	return ref;
}

int operand_stack_peek_n(struct operand_stack *s, int n) {
	int ref;
	struct operand_record *o;

	if (s == NULL) {
		fprintf(stderr, "mvm: operand stack not initialized!\n");
		mvm_halt();
	}

	/* lock */
	operand_stack_lock(s);

	if (n < 0 || n >= s->size) {
		fprintf(stderr, "mvm: invalid operand stack index!\n");
		/* unlock */
		operand_stack_unlock(s);
		mvm_halt();
	}

	for (o = s->head; --n >= 0; o = o->next);
	ref = o->ref;

	/* unlock */
	operand_stack_unlock(s);

	return ref;
}

int operand_stack_pop(struct operand_stack *s) {
	int ref;
	struct operand_record *o;

	if (s == NULL) {
		fprintf(stderr, "mvm: operand stack not initialized!\n");
		mvm_halt();
	}

	/* lock */
	operand_stack_lock(s);

	if (s->size <= 0) {
		fprintf(stderr, "mvm: operand stack underflow!\n");
		/* unlock */
		operand_stack_unlock(s);
		mvm_halt();
	}

	o = s->head;
	s->head = o->next;
	ref = o->ref;
	operand_record_destroy(o);

	s->size--;

	/* unlock */
	operand_stack_unlock(s);

	return ref;
}

int operand_stack_pop_n(struct operand_stack *s, int n) {
	int ref;

	if (s == NULL) {
		fprintf(stderr, "mvm: operand stack not initialized!\n");
		mvm_halt();
	}

	/* lock */
	operand_stack_lock(s);

	if (n < 0 || n > s->size) {
		fprintf(stderr, "mvm: invalid operand stack index!\n");
		/* unlock */
		operand_stack_unlock(s);
		mvm_halt();
	}

	ref = 0;

	while (n-- > 0) 
		ref = operand_stack_pop(s);

	/* unlock */
	operand_stack_unlock(s);

	return ref;
}

int operand_stack_empty(struct operand_stack *s) {
	int empty;

	if (s == NULL) {
		fprintf(stderr, "mvm: operand stack not initialized!\n");
		mvm_halt();
	}

	/* lock */
	operand_stack_lock(s);

	empty = s->size == 0;

	/* unlock */
	operand_stack_unlock(s);

	return empty;
}

int operand_stack_size(struct operand_stack *s) {
	int size;

	if (s == NULL) {
		fprintf(stderr, "mvm: operand stack not initialized!\n");
		mvm_halt();
	}

	/* lock */
	operand_stack_lock(s);

	size = s->size;

	/* unlock */
	operand_stack_unlock(s);

	return size;
}

int operand_stack_reverse(struct operand_stack *s, int n) {
	int *args, i;

	if (s == NULL) {
		fprintf(stderr, "mvm: operand stack not initialized!\n");
		mvm_halt();
	}

	/* lock */
	operand_stack_lock(s);

	if (n > s->size) {
		fprintf(stderr, "mvm: too many operands to reverse!\n");
		/* unlock */
		operand_stack_unlock(s);
		mvm_halt();
	}

	if ((args = (int *)malloc(sizeof(int)*n)) == NULL) {
		perror("mvm: malloc");
		/* unlock */
		operand_stack_unlock(s);
		mvm_halt();
	}

	for (i = 0; i < n; i++)
		args[i] = operand_stack_pop(s);
	for (i = 0; i < n; i++)
		operand_stack_push(s, args[i]);

	/* unlock */
	operand_stack_unlock(s);

	free(args);
	return 0;
}

int operand_stack_dump(struct operand_stack *s) {
	int n;
	struct object *object;
	struct operand_record *o;

	if (s == NULL) {
		fprintf(stderr, "mvm: operand stack not initialized!\n");
		mvm_halt();
	}

	if (s->size == 0) {
		fprintf(stderr, "empty!\n");
		return 0;
	}

	for (n = 0, o = s->head; n < s->size; o = o->next, n++) {
		fprintf(stderr, "#%d  ", n);
		object = heap_fetch_object(heap, o->ref);
		object_dump(object, 0);
		fprintf(stderr, "\n");
	}

	return 0;
}

int operand_stack_lock(struct operand_stack *s) {
	if (s == NULL) {
		fprintf(stderr, "mvm: operand stack not initialized!\n");
		mvm_halt();
	}

	/* lock */
	/* nlock_lock(s->nlock); */

	return 0;
}

int operand_stack_unlock(struct operand_stack *s) {
	if (s == NULL) {
		fprintf(stderr, "mvm: operand stack not initialized!\n");
		mvm_halt();
	}

	/* unlock */
	/* nlock_unlock(s->nlock); */

	return 0;
}

struct operand_record * operand_record_create(int r, struct operand_record *n) {
	struct operand_record *o;

	if ((o = (struct operand_record *)malloc(sizeof(struct operand_record))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	o->ref = r;
	o->next = n;

	return o;
}

void operand_record_destroy(struct operand_record *r) {
	if (r != NULL)
		free(r);
}
