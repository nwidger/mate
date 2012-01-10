/* Niels Widger
 * Time-stamp: <09 Jan 2012 at 20:47:40 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <errno.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "class_table.h"
#include "constants.h"
#include "disassemble.h"
#include "dmp.h"
#include "garbage_collector.h"
#include "heap.h"
#include "instruction_table.h"
#include "instructions.h"
#include "invoke_method.h"
#include "integer.h"
#include "mdb.h"
#include "method_area.h"
#include "native_method_array.h"
#include "native_methods.h"
#include "nlock.h"
#include "nlock_dmp.h"
#include "object.h"
#include "object_dmp.h"
#include "symbol_table.h"
#include "thread.h"
#include "thread_dmp.h"
#include "vm_stack.h"

#define OPTARGS ":m:s:i:daqcvhpQ:g:r"

/* globals */
FILE *input;
FILE *output;

char *class_file;
char *symbol_file;

struct class_table *class_table;
struct garbage_collector *garbage_collector;
struct heap *heap;
struct instruction_table *instruction_table;
struct method_area *method_area;
struct native_method_array *native_method_array;
struct symbol_table *symbol_table;
int garbage_collector_interval;
enum garbage_collector_type garbage_collector_type;
struct dmp *dmp;
struct object_dmp_attr object_dmp_attr;
struct thread_dmp_attr thread_dmp_attr;
struct nlock_dmp_attr nlock_dmp_attr;

uint32_t main_block_address;
uint32_t main_block_end;
uint32_t main_block_max_locals;
int32_t main_block_return_value;
int print_trace;
int verbose;
int debug;
int restart;

/* forward declarations */
void usage();
int mvm_initialize(uint64_t h);
void mvm_halt();
int mvm_cleanup();
void mvm_clear();
void mvm_print(const char *f, ...);
uint64_t parse_heap_size(char *s);

/** prints usage text to stderr. */

void usage() {
	fprintf(stderr, "usage: mvm [-h] [-v] [-q] [-c] [-d] [-a] [-i NUM] [-s FILE] [-m NUM] [-p] [-Q NUM] [-g NUM] [-r] FILE\n");
	fprintf(stderr, "Executes the specified maTe class file.\n");
	fprintf(stderr, "  -h       Print this help message and exit.\n");
	fprintf(stderr, "  -v       Verbose output.  Decodes each executed instruction to stderr.\n");
	fprintf(stderr, "           Also, stack traces will output the contents of each frame's\n");
	fprintf(stderr, "           operand stack and local variable array.\n");
	fprintf(stderr, "  -d       Run in debugger mode.\n");
	fprintf(stderr, "  -a       Do not execute class file.  Instead, disassemble it to stderr\n");
        fprintf(stderr, "           and exit.\n");
	fprintf(stderr, "  -q       Be quiet, do not print stack trace on error.\n");
	fprintf(stderr, "  -c       Use a concurrent garbage collector.  Default is to use a %s.\n",
		GARBAGE_COLLECTOR_DEFAULT_TYPE == serial_type ? "serial gargage collector" : "concurrent garbage collector");
	fprintf(stderr, "  -i NUM   With -c, specify seconds between garbage collection phases.  Default is %d seconds.\n",
		GARBAGE_COLLECTOR_DEFAULT_INTERVAL);
	fprintf(stderr, "  -s FILE  Specify FILE as a symbol table.  Used with -v to resolve addresses\n");
	fprintf(stderr, "           when decoding instructions.  Symbol tables can be generated by\n");
	fprintf(stderr, "           passing the -s switch to the assembler.\n");
	fprintf(stderr, "  -m NUM   Use a heap with a size of NUM bytes.  Default is %d bytes.\n",
		HEAP_DEFAULT_SIZE);
	fprintf(stderr, "  -p       Enable DMP (cannot be used with -c).\n");
	fprintf(stderr, "  -Q NUM   With -p, specify quantum size.  Default is %d.\n",
		THREAD_DMP_DEFAULT_QUANTUM_SIZE);
	fprintf(stderr, "  -g NUM   With -p, specify ownership table granularity.  Default is %d.\n",
		OBJECT_DMP_DEFAULT_DEPTH);
	fprintf(stderr, "  -r       With -p, enable reduced serial mode.  Default is %s.\n",
		THREAD_DMP_DEFAULT_REDUCED_SERIAL_MODE == full_mode ? "full serial mode" : "reduced serial mode");
}

/** initializes mvm components.
 *
 * @param h - heap will be created with a max size of h bytes
 *
 * @return 0 on success, non-zero on failure
 */

int mvm_initialize(uint64_t h) {
	int len;
	char *buf;

	input = stdin;
	output = stdout;

	if ((heap = heap_create(h)) == NULL) {
		fprintf(stderr, "mvm: error creating heap!\n");
		return 1;
	}

	if ((garbage_collector = garbage_collector_create()) == NULL) {
		fprintf(stderr, "mvm: error creating garbage collector!\n");
		return 1;
	}

	if ((symbol_table = symbol_table_create()) == NULL) {
		fprintf(stderr, "mvm: error creating symbol table!\n");
		return 1;
	}

	if ((method_area = method_area_create()) == NULL) {
		fprintf(stderr, "mvm: error creating method area!\n");
		return 1;
	}

	if ((native_method_array = native_method_array_create(NUM_NATIVES)) == NULL) {
		fprintf(stderr, "mvm: error creating native method array!\n");
		return 1;
	}

	if (add_native_methods(native_method_array) != 0) {
		fprintf(stderr, "mvm: error adding native methods!\n");
		return 1;
	}

	if ((instruction_table = instruction_table_create(MAX_OPCODE)) == NULL) {
		fprintf(stderr, "mvm: error creating instruction table!\n");
		return 1;
	}

	if (add_instructions(instruction_table) != 0) {
		fprintf(stderr, "mvm: error adding instructions!\n");
		return 1;
	}

	if (symbol_file != NULL &&
	    symbol_table_load_dump(symbol_table, symbol_file) != 0)
		return 1;

	if (access(class_file, F_OK) != 0) {
		len = strlen(class_file);
		if ((buf = (char *)malloc(len + 1 + strlen(".class"))) == NULL) {
			perror("malloc");
			return 1;
		}

		buf[0] = '\0';
		strcat(buf, class_file);
		strcat(buf, ".class");

		if (access(buf, F_OK) != 0) {
			fprintf(stderr, "mvm: %s: %s\n", class_file, strerror(errno));
			free(buf);
			return 1;
		}

		class_file = buf;
	}

	if (method_area_load_class_file(method_area, class_file) != 0)
		return 1;

	if ((class_table = method_area_generate_class_table(method_area)) == NULL)
		return 1;

	if (debug != 0 && mdb_initialize() != 0) {
		fprintf(stderr, "mvm: error initializing debugger!\n");
		return 1;
	}

	if (dmp != NULL &&
	    (dmp = dmp_create(&object_dmp_attr,
			      &thread_dmp_attr,
			      &nlock_dmp_attr)) == NULL) {
		fprintf(stderr, "mvm: error initializing DMP!\n");
		return 1;
	}

	garbage_collector_start(garbage_collector,
				garbage_collector_type,
				garbage_collector_interval);

	return 0;
}

int mvm_cleanup() {
	if (input != NULL && fclose(input) == EOF) {
		perror("fclose");
		exit(1);
	}

	if (output != NULL && fflush(output) == EOF) {
		perror("fflush");
		exit(1);
	}

	if (output != NULL && fclose(output) == EOF) {
		perror("fclose");
		exit(1);
	}

	if (dmp != NULL && dmp != (struct dmp *)1)
		dmp_destroy(dmp);

	if (garbage_collector != NULL) {
		garbage_collector_stop(garbage_collector);
		garbage_collector_destroy(garbage_collector);
	}

	if (heap != NULL)
		heap_destroy(heap);
	if (symbol_table != NULL)
		symbol_table_destroy(symbol_table);
	if (method_area != NULL)
		method_area_destroy(method_area);
	if (instruction_table != NULL)
		instruction_table_destroy(instruction_table);
	if (native_method_array != NULL)
		native_method_array_destroy(native_method_array);
	if (class_table != NULL)
		class_table_destroy(class_table);
	if (debug != 0)
		mdb_cleanup();

	return 0;
}

void mvm_clear() {
	int fd;
	struct stat buf;

	if ((fd = fileno(input)) == -1) {
		perror("mvm: fileno");
		exit(1);
	}

	if (fstat(fd, &buf) != 0) {
		perror("mvm: fstat");
		exit(1);
	}

	if (S_ISREG(buf.st_mode) && fseek(input, 0L, SEEK_SET) != 0) {
		perror("mvm: fseek");
		exit(1);
	}

	if ((fd = fileno(output)) == -1) {
		perror("mvm: fileno");
		exit(1);
	}

	if (fstat(fd, &buf) != 0) {
		perror("mvm: fstat");
		exit(1);
	}

	if (S_ISREG(buf.st_mode) && ftruncate(fd, 0) != 0) {
		perror("mvm: ftruncate");
		exit(1);
	}

	garbage_collector_stop(garbage_collector);

	heap_clear(heap);

	garbage_collector_start(garbage_collector,
				garbage_collector_type,
				garbage_collector_interval);
}

void mvm_halt() {
	uint32_t pc, opcode;
	struct vm_stack *vm_stack;
	char *class_file, *instruction_name;

	if (dmp != NULL)
		exit(1);

	pc = thread_get_pc(NULL);
	vm_stack = thread_get_vm_stack(NULL);

	if (debug != 0) {
		mdb_hook(halt_vm_hook);
		if (restart != 0)
			return;
	}

	if (print_trace == 0)
		exit(1);

	print_trace = 0;
	if ((class_file = method_area_get_class_file(method_area)) != NULL) {
		heap_resize(heap, INT32_MAX);
		opcode = method_area_fetch(method_area, pc);
		instruction_name = instruction_table_get_name(instruction_table, opcode);

		fprintf(stderr, "%s: executing %s (pc=%" PRIu32 ")\n",
			class_file, instruction_name, pc);
	}

	if (debug == 0 && verbose != 0)
		vm_stack_dump(vm_stack, 1, 1);
	else
		vm_stack_dump(vm_stack, 0, 0);

	exit(1);
}

void mvm_print(const char *f, ...) {
	va_list ap;

	if (debug == 0 && verbose != 0) {
		va_start(ap, f);
		vfprintf(stderr, f, ap);
		va_end(ap);
	}
}

char * mvm_strdup(const char *s) {
	char *retval;

	if ((retval = strdup(s)) == NULL) {
		perror("mvm: strdup");
		exit(1);
	}

	return retval;
}

uint64_t parse_heap_size(char *s) {
	char m;
	uint64_t len, factor, size;

	len = strlen(s);
	m = s[len-1];

	switch (m) {
	case 'k': case 'K':
		factor = 1024;
		break;
	case 'm': case 'M':
		factor = 1048576;
		break;
	case 'g': case 'G':
		factor = 1073741824;
		break;
	default:
		factor = 1;
	}

	if (factor != 1) s[len-1] = '\0';
	size = atoi(s);

	return size * factor;
}

int main(int argc, char *argv[]) {
	struct object *object;
	uint64_t heap_size;
	int err, c, disassemble;

	dmp = NULL;

	memcpy(&object_dmp_attr, &object_dmp_default_attr,
	       sizeof(struct object_dmp_attr));
	memcpy(&thread_dmp_attr, &thread_dmp_default_attr,
	       sizeof(struct thread_dmp_attr));
	memcpy(&nlock_dmp_attr, &nlock_dmp_default_attr,
	       sizeof(struct nlock_dmp_attr));

	class_table = NULL;
	garbage_collector = NULL;
	heap = NULL;
	instruction_table = NULL;
	method_area = NULL;
	native_method_array = NULL;
	symbol_table = NULL;

	garbage_collector_type = GARBAGE_COLLECTOR_DEFAULT_TYPE;
	garbage_collector_interval = GARBAGE_COLLECTOR_DEFAULT_INTERVAL;
	heap_size = HEAP_DEFAULT_SIZE;
	print_trace = 1;
	verbose = 0;
	symbol_file = NULL;
	disassemble = 0;
	debug = 0;

	if (argc < 2) {
		usage();
		return 1;
	}

	err = 0;
	while ((c = getopt(argc, argv, OPTARGS)) != -1) {
		switch (c) {
		case 'h':
			usage();
			return 0;
		case 'v':
			verbose++;
			break;
		case 's':
			symbol_file = optarg;
			break;
		case 'c':
			garbage_collector_type = concurrent_type;
			break;
		case 'd':
			debug = 1;
			break;
		case 'a':
			disassemble = 1;
			break;
		case 'i':
			if (garbage_collector_type != concurrent_type) {
				fprintf(stderr, "-i must be used with -c.\n");
				err = 1;
			}
			garbage_collector_interval = atoi(optarg);
			if (garbage_collector_interval <= 0) {
				fprintf(stderr, "-i must be a positive integer.\n");
				err = 1;
			}
			break;
		case 'm':
			heap_size = parse_heap_size(optarg);
			if (heap_size <= 0) {
				fprintf(stderr, "-m must be a positive integer.\n");
				err = 1;
			}
			break;
		case 'p':
			dmp = (struct dmp *)1;
			break;
		case 'Q':
			thread_dmp_attr.quantum_size = atoi(optarg);
			break;
		case 'g':
			object_dmp_attr.depth = atoi(optarg);
			break;
		case 'r':
			thread_dmp_attr.serial_mode = reduced_mode;
			break;
		case 'q':
			print_trace = 0;
			break;
		case ':':
			fprintf(stderr, "Missing parameter to switch '%c'\n",
				optopt);
			usage();
			return 1;
		case '?':
			fprintf(stderr, "Invalid switch option '%c'\n",
				optopt);
			usage();
			return 1;
		}
	}

	if (dmp != NULL && garbage_collector_type == concurrent_type) {
		fprintf(stderr, "mvm: cannot use -p and -c together!\n");
		mvm_cleanup();
		return 1;
	}

	if (dmp != NULL && debug != 0) {
		fprintf(stderr, "mvm: cannot use -p and -d together!\n");
		mvm_cleanup();
		return 1;
	}

	if (err == 1 || optind >= argc) {
		usage();
		return 1;
	}

	class_file = argv[optind];

	if (mvm_initialize(heap_size) != 0) {
		fprintf(stderr, "mvm: error initializing VM!\n");
	        mvm_cleanup();
		return 1;
	}

	if (disassemble != 0) {
		mvm_disassemble_class_file(0);
		mvm_cleanup();
		return 0;
	}

	main_block_address = method_area_get_main_block_address(method_area);
	main_block_end = mvm_disassemble_method_end(main_block_address);
	main_block_max_locals = method_area_get_main_block_max_locals(method_area);

	do {
		mvm_clear();
		restart = 0;

		class_table_new_thread(class_table, &object);
		thread_start_main(object);
		thread_join_main(object);
	} while (restart != 0);

	mvm_cleanup();
	return main_block_return_value;
}
