/* Niels Widger
 * Time-stamp: <30 Nov 2009 at 19:28:05 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string_pool.h"

/* struct definitions */
struct string_record {
	char *str;
	struct string_record *next;
};

struct string_pool {
	struct string_record *head;
};

/* forward declarations */
void string_pool_init(struct string_pool *p);

struct string_pool * string_pool_create() {
	struct string_pool *p;

	if ((p = (struct string_pool *)malloc(sizeof(struct string_pool))) == NULL) {
		perror("mas: malloc");
		exit(1);
	}

	string_pool_init(p);
	return p;
}

void string_pool_destroy(struct string_pool *p) {
	if (p != NULL) {
		string_pool_clear(p);
		free(p);
	}
}

void string_pool_init(struct string_pool *p) {
	p->head = NULL;
}

void string_pool_clear(struct string_pool *p) {
	struct string_record *r, *s;

	if (p == NULL) {
		fprintf(stderr, "mas: string pool not initialized!\n");		
		return;
	}

	r = p->head;
	while (r != NULL) {
		s = r;
		r = r->next;
		
		if (s->str != NULL)
			free(s->str);
		free(s);
	}
}

char * string_pool_add(struct string_pool *p, char *s) {
	struct string_record *r;

	if (p == NULL) {
		fprintf(stderr, "mas: string pool not initialized!\n");
		return NULL;
	}

	if ((r = (struct string_record *)
	     malloc(sizeof(struct string_record))) == NULL) {
		perror("mas: malloc");
		return NULL;
	}

	if ((r->str = (char *)malloc(strlen(s)+1)) == NULL) {
		perror("mas: malloc");
		return NULL;
	}

	strcpy(r->str, s);
	r->next = p->head;
	p->head = r;
	
	return r->str;
}
