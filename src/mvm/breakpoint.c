/* Niels Widger
 * Time-stamp: <13 Apr 2010 at 13:27:01 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "breakpoint.h"
#include "constants.h"
#include "globals.h"
#include "method_area.h"

struct breakpoint * breakpoint_create(int n, uint32_t a, uint32_t i, char *w) {
	struct breakpoint *b;

	if ((b = (struct breakpoint *)malloc(sizeof(struct breakpoint))) == NULL) {
		perror("mvm: malloc");
		exit(1);
	}

	b->num = n;
	b->marks = 0;
	b->enabled = bp_disabled;
	b->inserted = 0;
	b->disposition = bp_donttouch_disp;
	b->address = 0;
	b->opcode = 0;
	b->native_index = 0;
	b->ignore_count = 0;
	b->hit_count = 0;
	b->next = NULL;

	if (a == NATIVE_ADDRESS) {
		b->type = bp_method;
		b->native_index = i;
	} else {
		b->type = bp_address;
		b->address = a;
		b->opcode = method_area_fetch(method_area, a);
	}

	b->where = mvm_strdup(w);

	return b;
}

void breakpoint_destroy(struct breakpoint *b) {
	if (b != NULL) {
		breakpoint_clear(b);
		if (b->where != NULL)
			free(b->where);
		free(b);
	}
}

void breakpoint_clear(struct breakpoint *b) {
	if (b != NULL) {
		breakpoint_disable(b);
		b->num = 0;
		b->type = bp_address;
		b->marks = 0;
		b->enabled = bp_enabled;
		b->inserted = 0;
		b->disposition = bp_donttouch_disp;
		b->address = 0;
		b->opcode = 0;
		b->native_index = 0;
		b->ignore_count = 0;
		b->hit_count = 0;
	}
}

int breakpoint_is_hit(struct breakpoint *b, uint32_t a, char *w) {
	if (b == NULL) {
		fprintf(stderr, "mvm: breakpoint not initialized!\n");
		exit(1);
	}


	return (b->type == bp_address && b->address == a) ||
		(b->type == bp_method && w != NULL && strcmp(b->where, w) == 0);
}

int breakpoint_enable(struct breakpoint *b) {
	if (b == NULL) {
		fprintf(stderr, "mvm: breakpoint not initialized!\n");
		exit(1);
	}

	b->enabled = bp_enabled;
	if (b->type == bp_address)
		breakpoint_insert(b);
	
	return 0;
}

int breakpoint_disable(struct breakpoint *b) {
	if (b == NULL) {
		fprintf(stderr, "mvm: breakpoint not initialized!\n");
		exit(1);
	}

	b->enabled = bp_disabled;
	if (b->type == bp_address)
		breakpoint_uninsert(b);
	
	return 0;
}

int breakpoint_insert(struct breakpoint *b) {
	if (b == NULL) {
		fprintf(stderr, "mvm: breakpoint not initialized!\n");
		exit(1);
	}

	method_area_write(method_area, b->address, 0);
	b->inserted = 1;

	return 0;
}

int breakpoint_uninsert(struct breakpoint *b) {
	if (b == NULL) {
		fprintf(stderr, "mvm: breakpoint not initialized!\n");
		exit(1);
	}

	method_area_write(method_area, b->address, b->opcode);
	b->inserted = 0;
	
	return 0;
}


int breakpoint_set_mark(struct breakpoint *b, enum bp_mark m) {
	if (b == NULL) {
		fprintf(stderr, "mvm: breakpoint not initialized!\n");
		exit(1);
	}

	b->marks |= m;

	return 0;
}

int breakpoint_has_mark(struct breakpoint *b, enum bp_mark m) {
	if (b == NULL) {
		fprintf(stderr, "mvm: breakpoint not initialized!\n");
		exit(1);
	}

	return b->marks & m;
}

int breakpoint_clear_mark(struct breakpoint *b, enum bp_mark m) {
	if (b == NULL) {
		fprintf(stderr, "mvm: breakpoint not initialized!\n");
		exit(1);
	}

	b->marks &= ~m;

	return 0;
}

int breakpoint_clear_all_marks(struct breakpoint *b) {
	if (b == NULL) {
		fprintf(stderr, "mvm: breakpoint not initialized!\n");
		exit(1);
	}

	b->marks = 0;

	return 0;
}


