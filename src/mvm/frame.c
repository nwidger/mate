/* Niels Widger
 * Time-stamp: <30 Aug 2010 at 17:24:49 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "constants.h"
#include "frame.h"
#include "globals.h"
#include "operand_stack.h"
#include "local_variable_array.h"
#include "nlock.h"

/* struct definitions */
struct frame {
	char *method_name;
	struct operand_stack *operand_stack;
	struct local_variable_array *local_variable_array;
	struct frame *calling_frame;
	uint32_t start_address;
	uint32_t end_address;
	uint32_t current_address;
	uint32_t return_address;
	struct nlock *nlock;
};

/* forward declarations */
int frame_copy_arguments(struct frame *f, struct frame *c, int n);

struct frame * frame_create(char *e, int n, int m, uint32_t s,
			    uint32_t b, uint32_t r, struct frame *c) {
	struct frame *f;

	if ((f = (struct frame *)malloc(sizeof(struct frame))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	if ((f->method_name = strdup(e)) == NULL) {
		perror("mvm: strdup");
		mvm_halt();
	}

	f->start_address = s;
	f->end_address = b;
	f->current_address = s;
	f->return_address = r;

	if ((f->operand_stack = operand_stack_create()) == NULL)
		mvm_halt();

	if ((f->local_variable_array = local_variable_array_create(m)) == NULL)
		mvm_halt();

	f->calling_frame = c;

	if ((f->nlock = nlock_create()) == NULL)
		mvm_halt();

	if (f->calling_frame == NULL && n > 0)
		mvm_halt();
	if (f->calling_frame != NULL && frame_copy_arguments(f, c, n) != 0)
		mvm_halt();

	return f;
}

void frame_destroy(struct frame *f) {
	if (f != NULL) {
		/* lock */
		frame_lock(f);

		frame_clear(f);
		operand_stack_destroy(f->operand_stack);
		local_variable_array_destroy(f->local_variable_array);

		/* unlock */
		frame_unlock(f);

		nlock_destroy(f->nlock);
		free(f->method_name);
		free(f);
	}
}

void frame_clear(struct frame *f) {
	if (f != NULL) {
		/* lock */
		frame_lock(f);

		operand_stack_clear(f->operand_stack);
		local_variable_array_clear(f->local_variable_array);

		/* unlock */
		frame_unlock(f);
	}
}

struct operand_stack * frame_get_operand_stack(struct frame *f) {
	struct operand_stack *operand_stack;

	if (f == NULL) {
		fprintf(stderr, "mvm: frame has not been initialized!\n");
		mvm_halt();
	}

	/* lock */
	frame_lock(f);

	operand_stack = f->operand_stack;

	/* unlock */
	frame_unlock(f);

	return operand_stack;
}

struct local_variable_array * frame_get_local_variable_array(struct frame *f) {
	struct local_variable_array *local_variable_array;

	if (f == NULL) {
		fprintf(stderr, "mvm: frame has not been initialized!\n");
		mvm_halt();
	}

	/* lock */
	frame_lock(f);

	local_variable_array = f->local_variable_array;

	/* unlock */
	frame_unlock(f);

	return local_variable_array;
}

struct frame * frame_get_calling_frame(struct frame *f) {
	struct frame *calling_frame;

	if (f == NULL) {
		fprintf(stderr, "mvm: frame has not been initialized!\n");
		mvm_halt();
	}

	/* lock */
	frame_lock(f);

	calling_frame = f->calling_frame;

	/* unlock */
	frame_unlock(f);

	return calling_frame;
}

uint32_t frame_get_start_address(struct frame *f) {
	uint32_t start_address;

	if (f == NULL) {
		fprintf(stderr, "mvm: frame has not been initialized!\n");
		mvm_halt();
	}

	/* lock */
	frame_lock(f);

	start_address = f->start_address;

	/* unlock */
	frame_unlock(f);

	return start_address;
}

uint32_t frame_get_end_address(struct frame *f) {
	uint32_t end_address;

	if (f == NULL) {
		fprintf(stderr, "mvm: frame has not been initialized!\n");
		mvm_halt();
	}

	/* lock */
	frame_lock(f);

	end_address = f->end_address;

	/* unlock */
	frame_unlock(f);

	return end_address;
}

uint32_t frame_get_current_address(struct frame *f) {
	uint32_t current_address;

	if (f == NULL) {
		fprintf(stderr, "mvm: frame has not been initialized!\n");
		mvm_halt();
	}

	/* lock */
	frame_lock(f);

	current_address = f->current_address;

	/* unlock */
	frame_unlock(f);

	return current_address;
}

uint32_t frame_get_return_address(struct frame *f) {
	uint32_t return_address;

	if (f == NULL) {
		fprintf(stderr, "mvm: frame has not been initialized!\n");
		mvm_halt();
	}

	/* lock */
	frame_lock(f);

	return_address = f->return_address;

	/* unlock */
	frame_unlock(f);

	return return_address;
}

int frame_set_current_address(struct frame *f, uint32_t c) {
	uint32_t old_current;

	if (f == NULL) {
		fprintf(stderr, "mvm: frame has not been initialized!\n");
		mvm_halt();
	}

	/* lock */
	frame_lock(f);

	old_current = f->current_address;
	f->current_address = c;

	/* unlock */
	frame_unlock(f);

	return old_current;
}

int frame_copy_arguments(struct frame *f, struct frame *c, int n) {
	int i, ref;
	struct operand_stack *calling_frame_operand_stack;

	if (f == NULL) {
		fprintf(stderr, "mvm: frame has not been initialized!\n");
		mvm_halt();
	}

	/* lock */
	frame_lock(f);

	if (n < 0) {
		fprintf(stderr, "mvm: argument count must be >= 0!\n");
		/* unlock */
		frame_unlock(f);
		mvm_halt();
	}

	calling_frame_operand_stack = frame_get_operand_stack(c);
	operand_stack_reverse(calling_frame_operand_stack, n);

	for (i = 0; i < n; i++ ) {
		ref = operand_stack_peek_n(calling_frame_operand_stack, i);
		local_variable_array_store(f->local_variable_array, i, ref);
	}

	operand_stack_pop_n(calling_frame_operand_stack, n);

	/* unlock */
	frame_unlock(f);

	return 0;
}

char * frame_get_method_name(struct frame *f) {
	char *method_name;

	if (f == NULL) {
		fprintf(stderr, "mvm: frame has not been initialized!\n");
		mvm_halt();
	}

	/* lock */
	frame_lock(f);

	method_name = f->method_name;

	/* unlock */
	frame_unlock(f);

	return method_name;
}

int frame_dump(struct frame *f, int o, int l) {
	if (f == NULL) {
		fprintf(stderr, "mvm: frame has not been initialized!\n");
		mvm_halt();
	}

	if (f->start_address == NATIVE_ADDRESS) {
		fprintf(stderr, "%s <native-method> (return=%" PRIu32 ")\n",
			f->method_name, f->return_address);
	} else {
		fprintf(stderr, "%s (start=%" PRIu32 ", end=%" PRIu32
			", current=%" PRIu32 ", return=%" PRIu32 ")\n",
			f->method_name, f->start_address, f->end_address,
			f->current_address, f->return_address);
	}

	if (o != 0) {
		fprintf(stderr, "operand stack:\n");
		operand_stack_dump(f->operand_stack);
	}

	if (l != 0) {
		fprintf(stderr, "local variable array:\n");
		local_variable_array_dump(f->local_variable_array);
	}

	return 0;
}

int frame_lock(struct frame *f) {
	if (f == NULL) {
		fprintf(stderr, "mvm: frame not initialized!\n");
		mvm_halt();
	}

	/* lock */
	nlock_lock(f->nlock);

	return 0;
}

int frame_unlock(struct frame *f) {
	if (f == NULL) {
		fprintf(stderr, "mvm: frame not initialized!\n");
		mvm_halt();
	}

	/* unlock */
	nlock_unlock(f->nlock);

	return 0;
}
