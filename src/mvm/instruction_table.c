/* Niels Widger
 * Time-stamp: <20 Dec 2012 at 17:59:42 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "instruction_table.h"
#include "mdb.h"

/* struct definitions */
struct instruction_record {
	char *name;
	int (*function)(uint32_t, struct thread *);
	int (*decode)(uint32_t);
	int (*size)(uint32_t);	
};

struct instruction_table {
	uint32_t max_opcode;
	struct instruction_record **instructions;
};

/* forward declarations */
struct instruction_record * instruction_record_create(char *n, int (*f)(uint32_t, struct thread *),
						      int (*d)(uint32_t),
						      int (*s)(uint32_t));
void instruction_record_destroy(struct instruction_record *r);

struct instruction_table * instruction_table_create(uint32_t m) {
	struct instruction_table *i;

	if ((i = (struct instruction_table *)malloc(sizeof(struct instruction_table))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	i->max_opcode = m;

	if ((i->instructions = (struct instruction_record **)
	     calloc(i->max_opcode+1, sizeof(struct instruction_record *))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	return i;
}

void instruction_table_destroy(struct instruction_table *i) {
	if (i != NULL) {
		instruction_table_clear(i);
		free(i->instructions);
		free(i);
	}
}

void instruction_table_clear(struct instruction_table *i) {
	int j;

	if (i != NULL) {
		for (j = 0; j < i->max_opcode+1; j++) {
			if (i->instructions[j] != NULL)
				instruction_record_destroy(i->instructions[j]);
		}
	}
}

int instruction_table_add(struct instruction_table *i, char *n, uint32_t o,
			  int (*f)(uint32_t, struct thread *), int (*d)(uint32_t), int (*s)(uint32_t)) {
	if (i == NULL) {
		fprintf(stderr, "mvm: instruction table not initialized!\n");
		mvm_halt();
	}

	if (o > i->max_opcode) {
		fprintf(stderr, "mvm: invalid opcode 0x%" PRIx32 "!\n", o);
		mvm_halt();
	}

	if (i->instructions[o] != NULL)
		instruction_record_destroy(i->instructions[o]);

	if ((i->instructions[o] = instruction_record_create(n, f, d, s)) == NULL)
		mvm_halt();

	return 0;
}

int instruction_table_execute(struct instruction_table *i, uint32_t o, struct thread *t) {
	struct instruction_record *r;

	if (i == NULL) {
		fprintf(stderr, "mvm: instruction table not initialized!\n");
		mvm_halt();
	}

	if (o > i->max_opcode) {
		fprintf(stderr, "mvm: invalid opcode 0x%" PRIx32 "!\n", o);
		mvm_halt();
	}

	if ((r = i->instructions[o]) == NULL) {
		fprintf(stderr, "mvm: instruction 0x%" PRIx32 " was not set!\n", o);
		mvm_halt();
	}

	return (*r->function)(o, t);
}

int instruction_table_decode(struct instruction_table *i, int p, uint32_t o, uint32_t a) {
	struct instruction_record *r;

	if (i == NULL) {
		fprintf(stderr, "mvm: instruction table not initialized!\n");
		mvm_halt();
	}

	if (o > i->max_opcode) {
		fprintf(stderr, "mvm: invalid opcode 0x%" PRIx32 "!\n", o);
		mvm_halt();
	}

	if ((r = i->instructions[o]) == NULL) {
		if (debug == 0)
			fprintf(stderr, "mvm: instruction 0x%" PRIx32 " was not set!\n", o);
		mvm_halt();
	}

	if (p != 0)
		fprintf(stderr, "%-14" PRIu32, a);
	return (*r->decode)(a);
}

int instruction_table_decode_size(struct instruction_table *i, uint32_t o, uint32_t a) {
	struct instruction_record *r;

	if (i == NULL) {
		fprintf(stderr, "mvm: instruction table not initialized!\n");
		mvm_halt();
	}

	if (o > i->max_opcode) {
		fprintf(stderr, "mvm: invalid opcode 0x%" PRIx32 "!\n", o);
		mvm_halt();
	}

	if ((r = i->instructions[o]) == NULL) {
		if (debug == 0)
			fprintf(stderr, "mvm: instruction 0x%" PRIx32 " was not set!\n", o);
		mvm_halt();
	}

	return (*r->size)(a);
}

char * instruction_table_get_name(struct instruction_table *i, uint32_t o) {
	struct instruction_record *r;
	
	if (i == NULL) {
		fprintf(stderr, "mvm: instruction table not initialized!\n");
		mvm_halt();
	}

	if (o > i->max_opcode) {
		fprintf(stderr, "mvm: invalid opcode 0x%" PRIx32 "!\n", o);
		mvm_halt();
	}

	if ((r = i->instructions[o]) == NULL) {
		if (debug == 0)
			fprintf(stderr, "mvm: instruction 0x%" PRIx32 " was not set!\n", o);
		mvm_halt();
	}
	
	return r->name;
}

int instruction_table_dump(struct instruction_table *i) {
	uint32_t j;
	struct instruction_record *r;
	
	if (i == NULL) {
		fprintf(stderr, "mvm: instruction table not initialized!\n");
		mvm_halt();
	}

	for (j = 0; j < i->max_opcode+1; j++) {
		if ((r = i->instructions[j]) != NULL)
			fprintf(stderr, "%s (0x%" PRIx32 ")\n", r->name, j);
	}

	return 0;
}

struct instruction_record * instruction_record_create(char *n, int (*f)(uint32_t, struct thread *),
						      int (*d)(uint32_t),
						      int (*s)(uint32_t)) {
	struct instruction_record *r;

	if ((r = (struct instruction_record *)malloc(sizeof(struct instruction_record))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	if ((r->name = (char *)malloc(sizeof(char)*(strlen(n)+1))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	strcpy(r->name, n);
	r->function = f;
	r->decode = d;
	r->size = s;

	return r;
}

void instruction_record_destroy(struct instruction_record *r) {
	if (r != NULL) {
		free(r->name);
		free(r);
	}
}
