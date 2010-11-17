/* Niels Widger
 * Time-stamp: <11 Apr 2010 at 01:25:54 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "class.h"
#include "class_table.h"
#include "constants.h"
#include "disassemble.h"
#include "globals.h"
#include "method_area.h"
#include "nlock.h"

/* struct definitions */
struct method_area {
	char *class_file;
	int fd;
	uint8_t *code;
	int size;
	struct nlock *nlock;
};

/* forward declarations */
int method_area_read_string(struct method_area *m, uint32_t a, char **b);

struct method_area * method_area_create() {
	struct method_area *m;

	if ((m = (struct method_area *)malloc(sizeof(struct method_area))) == NULL) {
		perror("mvm: malloc");
		mvm_halt();
	}

	m->class_file = NULL;
	m->fd = 0;
	m->code = NULL;
	m->size = 0;

	if ((m->nlock = nlock_create()) == NULL)
		mvm_halt();

	return m;
}

void method_area_destroy(struct method_area *m) {
	if (m != NULL) {
		/* lock */
		method_area_lock(m);

		method_area_clear(m);

		/* unlock */
		method_area_unlock(m);

		nlock_destroy(m->nlock);
		free(m);
	}
}

void method_area_clear(struct method_area *m) {
	if (m != NULL) {
		/* lock */
		method_area_lock(m);

		m->class_file = NULL;

		if (munmap(m->code, m->size) != 0) {
			perror("mvm: munmap");
			mvm_halt();
		}
		m->code = NULL;

		close(m->fd);
		m->fd = 0;

		m->size = 0;

		/* unlock */
		method_area_unlock(m);
	}
}

int method_area_load_class_file(struct method_area *m, char *c) {
	struct stat file_stat;

	if (m == NULL) {
		fprintf(stderr, "mvm: method area not initialized!\n");
		mvm_halt();
	}

	/* lock */
	method_area_lock(m);

	if (m->class_file != NULL) {
		fprintf(stderr, "mvm: class file already loaded!\n");
		/* unlock */
		method_area_unlock(m);
		mvm_halt();
	}

	m->class_file = c;

	/* stat class file to find size */
	if (stat(m->class_file, &file_stat) == -1) {
		fprintf(stderr, "mvm: %s: %s\n", m->class_file, strerror(errno));
		m->class_file = NULL;
		/* unlock */
		method_area_unlock(m);
		mvm_halt();
	}

	/* grab class file size in bytes */
	m->size = file_stat.st_size;

	/* open class file */
	if ((m->fd = open(m->class_file, O_RDONLY)) == -1) {
		fprintf(stderr, "mvm: %s: %s\n", m->class_file, strerror(errno));
		m->class_file = NULL;
		/* unlock */
		method_area_unlock(m);
		mvm_halt();
	}

	/* map the file into memory */
	if ((m->code = mmap(NULL, m->size, PROT_READ|PROT_WRITE, MAP_PRIVATE, m->fd, 0)) == MAP_FAILED) {
		perror("mvm: mmap");
		m->class_file = NULL;
		close(m->fd);
		/* unlock */
		method_area_unlock(m);
		mvm_halt();
	}

	/* unlock */
	method_area_unlock(m);

	return 0;
}

uint32_t method_area_fetch(struct method_area *m, uint32_t a) {
	uint32_t word;

	if (m == NULL) {
		fprintf(stderr, "mvm: method area not initialized!\n");
		mvm_halt();
	}

	/* lock */
	method_area_lock(m);

	if (m->class_file == NULL) {
		fprintf(stderr, "mvm: no class file loaded!\n");
		/* unlock */
		method_area_unlock(m);
		mvm_halt();
	}

	if (method_area_address_is_valid(m, a) == 0) {
		fprintf(stderr, "mvm: invalid address %" PRIu32 "\n", a);
		/* unlock */
		method_area_unlock(m);
		mvm_halt();
	}

	word = ntohl(*((uint32_t *)((m->code)+a)));

	/* unlock */
	method_area_unlock(m);

	return word;
}

int method_area_write(struct method_area *m, uint32_t a, uint32_t n) {
	uint32_t word;
	
	if (m == NULL) {
		fprintf(stderr, "mvm: method area not initialized!\n");
		mvm_halt();
	}

	/* lock */
	method_area_lock(m);

	if (m->class_file == NULL) {
		fprintf(stderr, "mvm: no class file loaded!\n");
		/* unlock */
		method_area_unlock(m);
		mvm_halt();
	}

	if (method_area_address_is_valid(m, a) == 0) {
		fprintf(stderr, "mvm: invalid address %" PRIu32 "\n", a);
		/* unlock */
		method_area_unlock(m);
		mvm_halt();
	}

	word = ntohl(*((uint32_t *)((m->code)+a)));
	*((uint32_t *)((m->code)+a)) = htonl(n);

	/* unlock */
	method_area_unlock(m);
	
	return word;
}

int method_area_address_is_valid(struct method_area *m, uint32_t a) {
	int retval;
	
	if (m == NULL) {
		fprintf(stderr, "mvm: method area not initialized!\n");
		mvm_halt();
	}

	/* lock */
	method_area_lock(m);

	retval = (a >= 0 && a <= ((m->size)-sizeof(uint32_t)));

	/* unlock */
	method_area_unlock(m);
	
	return retval;
}

int method_area_size(struct method_area *m) {
	int size;

	if (m == NULL) {
		fprintf(stderr, "mvm: method area not initialized!\n");
		mvm_halt();
	}

	/* lock */
	method_area_lock(m);

	if (m->class_file == NULL) {
		fprintf(stderr, "mvm: no class file loaded!\n");
		/* unlock */
		method_area_unlock(m);
		mvm_halt();
	}

	size = m->size;

	/* unlock */
	method_area_unlock(m);

	return size;
}

char * method_area_get_class_file(struct method_area *m) {
	char *class_file;

	if (m == NULL) {
		fprintf(stderr, "mvm: method area not initialized!\n");
		mvm_halt();
	}

	/* lock */
	method_area_lock(m);

	class_file = m->class_file;

	/* unlock */
	method_area_unlock(m);

	return class_file;
}

struct class_table * method_area_generate_class_table(struct method_area *m) {
	int type;
	char *class_name, *method_name;
	uint32_t i, j, d, n, p, num_classes, num_methods, num_fields, vmt, super_vmt,
		method_address, method_end, method_max_locals;
	struct class_table *c;
	struct class *cl;

	if (m == NULL) {
		fprintf(stderr, "mvm: method area not initialized!\n");
		mvm_halt();
	}

	/* lock */
	method_area_lock(m);

	if (m->class_file == NULL) {
		fprintf(stderr, "mvm: no class file loaded!\n");
		/* unlock */
		method_area_unlock(m);
		mvm_halt();
	}

	d = CLASS_TABLE_SIZE_ADDRESS;
	num_classes = method_area_fetch(m, d);

	if ((c = class_table_create(num_classes)) == NULL) {
		fprintf(stderr, "mvm: error creating class table!\n");
		/* unlock */
		method_area_unlock(m);
		mvm_halt();
	}

	d += sizeof(uint32_t);
	for (i = 0; i < num_classes; i++) {
		/* vmt */
		p = method_area_fetch(m, d);
		vmt = p;
		d += sizeof(uint32_t);

		/* name */
		n = method_area_read_string(m, d, &class_name);
		d += sizeof(uint32_t)*n;

		/* class type */
		if (strcmp(class_name, OBJECT_PREDEFINED_CLASS_NAME) == 0)
			type = object_type;
		else if (strcmp(class_name, INTEGER_PREDEFINED_CLASS_NAME) == 0)
			type = integer_type;
		else if (strcmp(class_name, STRING_PREDEFINED_CLASS_NAME) == 0)
			type = string_type;
		else if (strcmp(class_name, TABLE_PREDEFINED_CLASS_NAME) == 0)
			type = table_type;
		else
			type = user_type;

		/* super_vmt */
		super_vmt = method_area_fetch(m, p);
		p += sizeof(uint32_t);

		/* num_fields */
		num_fields = method_area_fetch(m, p);
		p += sizeof(uint32_t);

		/* num_methods */
		num_methods = method_area_fetch(m, p);
		p += sizeof(uint32_t);

		/* create class, set super_vmt and num_fields */
		if ((cl = class_create(class_name, vmt, num_methods)) == NULL) {
			fprintf(stderr, "mvm: error creating class!\n");
			/* unlock */
			method_area_unlock(m);
			mvm_halt();
		}

		class_set_super_vmt(cl, super_vmt);
		class_set_num_fields(cl, num_fields);

		/* methods */
		for (j = 0; j < num_methods; j++) {
			method_address = method_end = method_area_fetch(m, p);
			p += sizeof(uint32_t);
			if (method_address != NATIVE_ADDRESS)
				method_end = mvm_disassemble_method_end(method_address);
			method_max_locals = method_area_fetch(m, p);
			p += sizeof(uint32_t);
			n = method_area_read_string(m, p, &method_name);
			p += sizeof(uint32_t)*n;

			class_set_method(cl, j, method_name,
					 method_address, method_end, method_max_locals);
			free(method_name);
		}

		class_table_add(c, type, cl);
		free(class_name);
	}

	if (class_table_analyze(c) != 0) {
		fprintf(stderr, "mvm: error analyzing class table!\n");
		/* unlock */
		method_area_unlock(m);
		mvm_halt();
	}

#ifdef DEBUG
	class_table_dump(c);
#endif

	/* unlock */
	method_area_unlock(m);

	return c;
}

uint32_t method_area_get_main_block_address(struct method_area *m) {
	uint32_t address;

	if (m == NULL) {
		fprintf(stderr, "mvm: method area not initialized!\n");
		mvm_halt();
	}

	/* lock */
	method_area_lock(m);

	if (m->class_file == NULL) {
		fprintf(stderr, "mvm: no class file loaded!\n");
		/* unlock */
		method_area_unlock(m);
		mvm_halt();
	}

	address = method_area_fetch(m, MAIN_BLOCK_DEC_ADDRESS);

	/* unlock */
	method_area_unlock(m);

	return address;
}

uint32_t method_area_get_main_block_max_locals(struct method_area *m) {
	uint32_t locals;

	if (m == NULL) {
		fprintf(stderr, "mvm: method area not initialized!\n");
		mvm_halt();
	}

	/* lock */
	method_area_lock(m);

	if (m->class_file == NULL) {
		fprintf(stderr, "mvm: no class file loaded!\n");
		/* unlock */
		method_area_unlock(m);
		mvm_halt();
	}

	locals = method_area_fetch(m, MAIN_BLOCK_DEC_ADDRESS+sizeof(uint32_t));

	/* unlock */
	method_area_unlock(m);

	return locals;
}

int method_area_read_string(struct method_area *m, uint32_t a, char **b) {
	int i, size;
	char c, *buf;

	if (m == NULL) {
		fprintf(stderr, "mvm: method area not initialized!\n");
		mvm_halt();
	}

	/* lock */
	method_area_lock(m);

	if (b == NULL) {
		/* unlock */
		method_area_unlock(m);
		return 0;
	}

	size = METHOD_AREA_INITIAL_BUFFER_SIZE;
	if ((buf = (char *)malloc(sizeof(char)*size)) == NULL) {
		perror("mvm: malloc");
		/* unlock */
		method_area_unlock(m);
		mvm_halt();
	}

	i = 0;
	do {
		c = (char)method_area_fetch(m, a);
		buf[i++] = c;

		if (i == size) {
			size *= METHOD_AREA_BUFFER_RESIZE_FACTOR;
			if ((buf = (char *)realloc(buf, sizeof(char)*size)) == NULL) {
				perror("mvm: realloc");
				/* unlock */
				method_area_unlock(m);
				mvm_halt();
			}
		}

		a += sizeof(uint32_t);
	} while (c != '\0');

	*b = buf;

	/* unlock */
	method_area_unlock(m);

	return i;
}

int method_area_dump(struct method_area *m) {
	if (m == NULL) {
		fprintf(stderr, "mvm: method area not initialized!\n");
		mvm_halt();
	}

	if (m->class_file != NULL) {
		fprintf(stderr, "class_file = %s\n", m->class_file);
		fprintf(stderr, "size = %d\n", m->size);
	}

	return 0;
}

int method_area_lock(struct method_area *m) {
	if (m == NULL) {
		fprintf(stderr, "mvm: method area not initialized!\n");
		mvm_halt();
	}

	/* lock */
	nlock_lock(m->nlock);

	return 0;
}

int method_area_unlock(struct method_area *m) {
	if (m == NULL) {
		fprintf(stderr, "mvm: method area not initialized!\n");
		mvm_halt();
	}

	/* unlock */
	nlock_unlock(m->nlock);
	
	return 0;
}

#ifdef DEBUG_METHOD_AREA
int main(int argc, char *argv[]) {
	uint32_t value;
	int size;
	struct method_area *m;
	struct class_table *c;

	m = method_area_create();

	method_area_load_class_file(m, argv[1]);

	c = method_area_generate_class_table(m);

	class_table_dump(c);

	size = method_area_size(m);
	fprintf(stderr, "size = %d\n", size);
	/* should not cause an error */
	fprintf(stderr, "fetching address %lu, should not fail\n", size-sizeof(uint32_t)-1);
	value = method_area_fetch(m, size-sizeof(uint32_t));
	fprintf(stderr, "value = %d\n", value);
	/* should cause an error */
	fprintf(stderr, "fetching %lu, should fail\n", size-sizeof(uint32_t));
	value = method_area_fetch(m, size-sizeof(uint32_t)+1);

	method_area_destroy(m);
	class_table_destroy(c);
	return 0;
}
#endif
