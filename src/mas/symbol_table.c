/* Niels Widger
 * Time-stamp: <21 Dec 2009 at 13:53:17 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "symbol_table.h"

/* constants */
#define BUFFER_SIZE 4096

/* struct definitions */
struct table_record {
	char *label;
	uint32_t address;
	struct table_record *next;
};

struct symbol_table {
	int size;
	struct table_record *head;
};

/* forward declarations */
void symbol_table_init(struct symbol_table *t);

struct symbol_table * symbol_table_create() {
	struct symbol_table *t;

	if ((t = (struct symbol_table *)malloc(sizeof(struct symbol_table))) == NULL) {
		perror("mas: malloc");
		exit(1);
	}

	symbol_table_init(t);
	return t;
}

void symbol_table_destroy(struct symbol_table *t) {
	if (t != NULL) {
		symbol_table_clear(t);
		free(t);
	}
}

void symbol_table_init(struct symbol_table *t) {
	t->head = NULL;
	t->size = 0;
}

void symbol_table_clear(struct symbol_table *t) {
	struct table_record *r, *s;

	if (t == NULL) {
		fprintf(stderr, "mas: symbol table not initialized!\n");
		return;
	}

	r = t->head;
	while (r != 0) {
		s = r;
		r = r->next;
		free(s->label);
		free(s);
	}

	t->size = 0;
}

int symbol_table_add(struct symbol_table *t, char *l, uint32_t a) {
	char *m;
	struct table_record *r, *p, *q;

	if (t == NULL) {
		fprintf(stderr, "mas: symbol table not initialized!\n");
		return 1;
	}

	if (symbol_table_find_address(t, l) != 0) {
		fprintf(stderr, "mas: duplicate label %s\n", l);
		return 1;
	}

	if ((r = (struct table_record *)malloc(sizeof(struct table_record))) == NULL) {
		perror("mas: malloc");
		return 1;
	}

	if ((m = (char *)malloc(sizeof(char)*(strlen(l)+1))) == NULL) {
		perror("mas: malloc");
		return 1;
	}

	strcpy(m, l);

	r->label = m;
	r->address = a;
	r->next = NULL;

	if (t->head == NULL) {
		t->head = r;
	} else if (r->address <= t->head->address) {
		r->next = t->head;
		t->head = r;
	} else {
		for (p = t->head, q = NULL; p != NULL && r->address >= p->address; q = p, p = p->next);
		r->next = q->next;
		q->next = r;
	}

	t->size++;
	return 0;
}

uint32_t symbol_table_find_address(struct symbol_table *t, char *l) {
	struct table_record *r;

	if (t == NULL) {
		fprintf(stderr, "mas: symbol table not initialized!\n");
		return 0;
	}

	for (r = t->head; r != NULL; r = r->next) {
		if (strcmp(r->label, l) == 0)
			return r->address;
	}

	return 0;
}

char * symbol_table_find_label(struct symbol_table *t, uint32_t a) {
	struct table_record *r;

	if (t == NULL) {
		fprintf(stderr, "mas: symbol table not initialized!\n");
		return NULL;
	}

	for (r = t->head; r != NULL; r = r->next) {
		if (r->address == a)
			return r->label;
	}

	return NULL;
}

int symbol_table_size(struct symbol_table *t) {
	if (t == NULL) {
		fprintf(stderr, "mas: symbol table not initialized!\n");
		return -1;
	}

	return t->size;
}

char * symbol_table_get_label_at_index(struct symbol_table *t, int i) {
	struct table_record *r;

	if (t == NULL) {
		fprintf(stderr, "mas: symbol table not initialized!\n");
		return NULL;
	}

	if (i < 0 || i >= t->size) {
		fprintf(stderr, "mas: invalid index!\n");
		return NULL;
	}

	for (r = t->head; r != NULL && --i >= 0; r = r->next);

	return r->label;
}

uint32_t symbol_table_get_address_at_index(struct symbol_table *t, int i) {
	struct table_record *r;

	if (t == NULL) {
		fprintf(stderr, "mas: symbol table not initialized!\n");
		return 0;
	}

	if (i < 0 || i >= t->size) {
		fprintf(stderr, "mas: invalid index!\n");
		return 0;
	}

	for (r = t->head; r != NULL && --i >= 0; r = r->next);

	return r->address;
}

int symbol_table_dump(struct symbol_table *t) {
	struct table_record *r;

	if (t == NULL) {
		fprintf(stderr, "mas: symbol table not initialized!\n");
		return 0;
	}

	for (r = t->head; r != NULL; r = r->next)
		fprintf(stdout, "%" PRIu32 " %s\n", r->address, r->label);

	return 0;
}

int symbol_table_load_dump(struct symbol_table *t, char *f) {
	FILE *file;
	uint32_t address;
	char label[BUFFER_SIZE];

	if (t == NULL) {
		fprintf(stderr, "mas: symbol table not initialized!\n");
		return 1;
	}

	if ((file = fopen(f, "r")) == NULL) {
		perror("mas: fopen");
		return 1;
	}

	while (fscanf(file, "%" SCNu32 " %s\n", &address, label) != EOF)
		symbol_table_add(t, label, address);	

	fclose(file);
	return 0;
}

#ifdef DEBUG_SYMBOLTABLE
int main(int argc, char *argv[]) {
	int size, i;
	struct symbol_table *t;

        t = symbol_table_create();
	symbol_table_load_dump(t, argv[1]);

	fprintf(stderr, "------------------------------------------------------------------------------\n");
	fprintf(stderr, "calling dump\n");
	
	symbol_table_dump(t);

	fprintf(stderr, "------------------------------------------------------------------------------\n");	
	fprintf(stderr, "printing by index\n");	

	size = symbol_table_size(t);

	for (i = 0; i < size; i++) {
		fprintf(stderr, "%u %s\n",
			symbol_table_get_address_at_index(t, i),
			symbol_table_get_label_at_index(t, i));
	}


	return 0;
}
#endif
