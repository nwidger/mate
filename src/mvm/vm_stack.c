/* Niels Widger
 * Time-stamp: <04 Dec 2012 at 13:57:20 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#include "frame.h"
#include "globals.h"
#include "heap.h"
#include "nlock.h"
#include "vm_stack.h"

/* struct definitions */
struct vm_stack {
	int size;
	struct frame *head;
	struct nlock *nlock;
};

struct vm_stack * vm_stack_create() {
	struct vm_stack *s;

	if ((s = (struct vm_stack *)malloc(sizeof(struct vm_stack))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	s->size = 0;
	s->head = NULL;

	if ((s->nlock = nlock_create()) == NULL)
		mvm_halt();

	return s;
}

void vm_stack_destroy(struct vm_stack *s) {
	if (s != NULL) {
		/* lock */
		vm_stack_lock(s);
		
		vm_stack_clear(s);

		/* unlock */
		vm_stack_unlock(s);

		nlock_destroy(s->nlock);
		free(s);
	}
}

void vm_stack_clear(struct vm_stack *s) {
	if (s != NULL) {
		/* lock */
		vm_stack_lock(s);
		
		vm_stack_pop_n(s, vm_stack_size(s));
		s->size = 0;

		/* unlock */
		vm_stack_unlock(s);
	}
}

struct frame * vm_stack_push(struct vm_stack *s, char *e, int n, int m,
			     uint32_t a, uint32_t b, uint32_t r) {
	struct frame *f;
	
	if (s == NULL) {
		fprintf(stderr, "mvm: stack not initialized!\n");
		mvm_halt();
	}

	if (n < 0) {
		fprintf(stderr, "mvm: argument count must be >= 0!\n");
		mvm_halt();
	}
	
	if (m < 0) {
		fprintf(stderr, "mvm: max locals count must be >= 0!\n");
		mvm_halt();
	}

	if ((f = frame_create(e, n, m, a, b, r, s->head)) == NULL) {
		fprintf(stderr, "mvm: error pushing vm_stack frame!\n");
		mvm_halt();
	}


	/* lock */
	vm_stack_lock(s);

	s->head = f;
	s->size++;

	/* unlock */
	vm_stack_unlock(s);

	return f;
}

struct frame * vm_stack_peek(struct vm_stack *s) {
	struct frame *head;
	
	if (s == NULL) {
		fprintf(stderr, "mvm: stack not initialized!\n");
		mvm_halt();
	}

	/* lock */
	/* vm_stack_lock(s); */
	
	if (vm_stack_empty(s) == 1) {
		/* unlock */
		/* vm_stack_unlock(s); */
		return NULL;
	}

	head = s->head;

	/* unlock */
	/* vm_stack_unlock(s); */

	return head;
}

uint32_t vm_stack_pop(struct vm_stack *s) {
	struct frame *f, *cf;
	uint32_t return_address;
	
	if (s == NULL) {
		fprintf(stderr, "mvm: stack not initialized!\n");
		mvm_halt();
	}

	if (s->size <= 0) {
		fprintf(stderr, "mvm: stack underflow!\n");
		mvm_halt();
	}

	f = s->head;
	cf = frame_get_calling_frame(f);
	return_address = frame_get_return_address(f);

	/* lock */
	vm_stack_lock(s);

	s->head = cf;
	s->size--;

	/* unlock */
	vm_stack_unlock(s);

	frame_destroy(f);
	
	return return_address;
}

uint32_t vm_stack_pop_n(struct vm_stack *s, int n) {
	uint32_t return_address;
	
	if (s == NULL) {
		fprintf(stderr, "mvm: stack not initialized!\n");
		mvm_halt();
	}

	return_address = 0;

	/* lock */
	/* vm_stack_lock(s); */

	while (n-- > 0)
		return_address = vm_stack_pop(s);

	/* unlock */
	/* vm_stack_unlock(s); */

	return return_address;
}

int vm_stack_empty(struct vm_stack *s) {
	int empty;
	
	if (s == NULL) {
		fprintf(stderr, "mvm: stack not initialized!\n");
		mvm_halt();
	}

	/* lock */
	/* vm_stack_lock(s); */

	empty = s->size == 0;
	
	/* unlock */
	/* vm_stack_unlock(s); */

	return empty;
}

int vm_stack_size(struct vm_stack *s) {
	int size;
	
	if (s == NULL) {
		fprintf(stderr, "mvm: stack not initialized!\n");
		mvm_halt();
	}

	/* lock */
	/* vm_stack_lock(s); */

	size = s->size;

	/* unlock */
	/* vm_stack_unlock(s); */

	return size;
}

int vm_stack_dump(struct vm_stack *s, int o, int l) {
	if (s == NULL) {
		fprintf(stderr, "mvm: stack not initialized!\n");
		mvm_halt();
	}

	return vm_stack_dump_n(s, 0, o, l);
}

int vm_stack_dump_n(struct vm_stack *s, int n, int o, int l) {
	int i;
	struct frame *f;
	
	if (s == NULL) {
		fprintf(stderr, "mvm: stack not initialized!\n");
		mvm_halt();
	}

	if (n >= s->size) {
		fprintf(stderr, "mvm: invalid stack index!\n");
		mvm_halt();
	}

	i = 0;
	for (f = vm_stack_peek(s); f != NULL; f = frame_get_calling_frame(f)) {
		if (i >= n) {
			fprintf(stderr, "#%d  ", i);
			frame_dump(f, o, l);
		}
		if (o != 0 || l != 0)
			fprintf(stderr, "\n");
		i++;
	}
	
	return 0;
}

int vm_stack_lock(struct vm_stack *s) {
	if (s == NULL) {
		fprintf(stderr, "mvm: vm_stack not initialized!\n");
		mvm_halt();
	}

	/* lock */
	nlock_lock(s->nlock);
	
	return 0;
}

int vm_stack_unlock(struct vm_stack *s) {
	if (s == NULL) {
		fprintf(stderr, "mvm: vm_stack not initialized!\n");
		mvm_halt();
	}

	/* unlock */
	nlock_unlock(s->nlock);
	
	return 0;
}

