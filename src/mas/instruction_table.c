/* Niels Widger
 * Time-stamp: <08 Sep 2009 at 12:16:44 by nwidger on macros.local>
 */

#include "instruction_table.h"

/* struct definitions */
struct instruction_record {
	char *name;
	uint32_t opcode;
	struct instruction_record *next;
};

struct instruction_table {
	struct instruction_record *head;
};

/* forward declarations */
void instruction_table_init(struct instruction_table *i);

struct instruction_table * instruction_table_create(int n, int m) {
	struct instruction_table *i;

	if ((i = (struct instruction_table *)malloc(sizeof(struct instruction_table))) == NULL) {
		perror("mas: malloc");
		exit(1);
	}

	instruction_table_init(i);
	return i;
}

void instruction_table_destroy(struct instruction_table *i) {
	if (i != NULL) {
		instruction_table_clear(i);
		free(i);
	}
}

void instruction_table_init(struct instruction_table *i) {
	i->head = NULL;
}

void instruction_table_clear(struct instruction_table *i) {
	struct instruction_record *r, *s;

	if (i == NULL) {
		fprintf(stderr, "mas: instruction table not initialized!\n");
		return;
	}

	r = i->head;
	while (r != NULL) {
		s = r;
		r = r->next;

		free(s);
	}
}

int instruction_table_add(struct instruction_table *i, char *n, uint32_t o) {
	struct instruction_record *r;

	if (i == NULL) {
		fprintf(stderr, "mas: instruction table not initialized!\n");
		return 1;
	}

	if ((r = (struct instruction_record *)
	     malloc(sizeof(struct instruction_record))) == NULL) {
		perror("mas: malloc");
		return 1;
	}

	r->name = n;
	r->opcode = o;
	r->next = i->head;
	
	i->head = r;
	return 0;
}

uint32_t instruction_table_find(struct instruction_table *i, char *n) {
	struct instruction_record *r;

	if (i == NULL) {
		fprintf(stderr, "mas: instruction table not initialized!\n");
		return 0;
	}

	for (r = i->head; r != 0; r = r->next) {
		if (strcmp(r->name, n) == 0)
			return r->opcode;
	}

	return 0;
}
