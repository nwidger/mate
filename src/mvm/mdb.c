/* Niels Widger
 * Time-stamp: <20 Dec 2012 at 18:15:15 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <errno.h>
#include <inttypes.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "breakpoint.h"
#include "class_table.h"
#include "constants.h"
#include "disassemble.h"
#include "frame.h"
#include "garbage_collector.h"
#include "globals.h"
#include "heap.h"
#include "instruction_table.h"
#include "integer.h"
#include "local_variable_array.h"
#include "mdb.h"
#include "mdb_help.h"
#include "method_area.h"
#include "native_method_array.h"
#include "object.h"
#include "operand_stack.h"
#include "ref_set.h"
#include "string.h"
#include "symbol_table.h"
#include "table.h"
#include "thread.h"
#include "vm_stack.h"

/* constants */
#define BUFFER_SIZE 4096
#define DISASSEMBLE_LINES 10	/* disassemble command lists 10 instructions */

/* used by completion system */
#define	COMMANDS       0x0001
#define	SYMBOLS        0x0002
#define	CLASSES        0x0004
#define	METHODS        0x0008
#define	NATIVE_METHODS 0x0010
#define ADDRESSES      0x0020
#define INFO_COMMANDS  0x0040
#define FILENAMES      0x0080
#define NONE           0x0100
#define DEFAULT        (COMMANDS|SYMBOLS|CLASSES|METHODS|\
			NATIVE_METHODS|ADDRESSES|INFO_COMMANDS|\
			FILENAMES)

/* completions for each command */
#define ADD_SYMBOL_FILE_COMPLETION_TYPE FILENAMES
#define BACKTRACE_COMPLETION_TYPE NONE
#define BREAK_COMPLETION_TYPE SYMBOLS|METHODS
#define CONTINUE_COMPLETION_TYPE NONE
#define DELETE_COMPLETION_TYPE NONE
#define DISABLE_COMPLETION_TYPE NONE
#define DISASSEMBLE_COMPLETION_TYPE SYMBOLS
#define DOWN_COMPLETION_TYPE NONE
#define ENABLE_COMPLETION_TYPE NONE
#define FINISH_COMPLETION_TYPE NONE
#define FRAME_COMPLETION_TYPE NONE
#define HELP_COMPLETION_TYPE COMMANDS
#define INFO_COMPLETION_TYPE INFO_COMMANDS
#define JUMP_COMPLETION_TYPE SYMBOLS
#define NEXTI_COMPLETION_TYPE NONE
#define PRINT_COMPLETION_TYPE NONE
#define RUN_COMPLETION_TYPE FILENAMES
#define SET_COMPLETION_TYPE SYMBOLS
#define STEPI_COMPLETION_TYPE NONE
#define UNTIL_COMPLETION_TYPE SYMBOLS
#define UP_COMPLETION_TYPE NONE
#define QUIT_COMPLETION_TYPE NONE
#define X_COMPLETION_TYPE SYMBOLS

/* enums */
enum mdb_mode {
	startup_mode    = 0, /* program not started */
	continue_mode   = 1, /* run until terminate or breakpoint */
	nexti_mode      = 2, /* next instruction, skip method calls */
	stepi_mode      = 3, /* next instruction, enter method calls  */
	finish_mode     = 4, /* finish selected frame */
	until_mode      = 5, /* execute until bigger or given address */
	terminated_mode = 6  /* program has terminated */
};

enum match_type {
	partial_type  = 0, /* "a", "ab", "abc" or "abcd" matches "abcd" */
	complete_type = 1  /* only "abcd" matches "abcd" */
};

/* struct declarations */
struct mdb_command {
	char *name;
	int max_args;
	const char *short_help;
	const char *help;
	int (*function)(char **, int);
	int completion_type;
	int alias;		/* is command an alias for another command */
};

struct breakpoint_list {
	int num_breakpoints;
	struct breakpoint *head;
	int next_breakpoint_num;
};

/* forward declarations */
struct breakpoint_list * breakpoint_list_create();
void breakpoint_list_destroy(struct breakpoint_list *l);
void breakpoint_list_clear(struct breakpoint_list *l);
struct breakpoint * breakpoint_list_add(struct breakpoint_list *l,
					uint32_t a, uint32_t i, char *w);
int breakpoint_list_remove_by_address(struct breakpoint_list *l, uint32_t a);
int breakpoint_list_remove_by_num(struct breakpoint_list *l, int n);
int breakpoint_list_hit(struct breakpoint_list *l, uint32_t a, char *w);
struct breakpoint * breakpoint_list_find(struct breakpoint_list *l, uint32_t a, char *w);
struct breakpoint * breakpoint_list_find_by_where(struct breakpoint_list *l, char *w);
struct breakpoint * breakpoint_list_find_by_address(struct breakpoint_list *l, uint32_t a);
struct breakpoint * breakpoint_list_find_by_num(struct breakpoint_list *l, int n);
int breakpoint_list_mark_hits(struct breakpoint_list *l, uint32_t a, char *w);
int breakpoint_list_clear_mark(struct breakpoint_list *l, enum bp_mark m);
int breakpoint_list_clear_hits(struct breakpoint_list *l);
int breakpoint_list_print_hits(struct breakpoint_list *l);
int breakpoint_list_reload_symbols(struct breakpoint_list *l);
int breakpoint_list_uninsert_enabled(struct breakpoint_list *l);
int breakpoint_list_insert_enabled(struct breakpoint_list *l);
int breakpoint_list_dump(struct breakpoint_list *l);
int breakpoint_print_hit(struct breakpoint *b);

int add_symbol_file_function(char **t, int n);
int backtrace_function(char **t, int n);
int break_function(char **t, int n);
int continue_function(char **t, int n);
int delete_function(char **t, int n);
int disable_function(char **t, int n);
int disassemble_function(char **t, int n);
int down_function(char **t, int n);
int enable_function(char **t, int n);
int finish_function(char **t, int n);
int frame_function(char **t, int n);
int help_function(char **t, int n);
int info_function(char **t, int n);
int jump_function(char **t, int n);
int nexti_function(char **t, int n);
int print_function(char **t, int n);
int run_function(char **t, int n);
int set_function(char **t, int n);
int stepi_function(char **t, int n);
int until_function(char **t, int n);
int up_function(char **t, int n);
int quit_function(char **t, int n);
int x_function(char **t, int n);

void mdb_dump_threads();
int mdb_print_location(int d);
int mdb_restart();
int mdb_update_frame();
void mdb_sigint_handler(int s);
void mdb_sigint(void (*h)(int));
int mdb_command_processor();
int mdb_flip_breakpoints(int f);
char ** mdb_completion(const char *l, int s, int e);
char * mdb_command_completion_function(const char *k, int c);
char ** mdb_tokenize(char *l, int *n);
int mdb_disambugiate_command(char *c, char *t, struct mdb_command a[]);
int mdb_do_match(char *c, char *d, enum match_type t);
int mdb_command_num_matches(char *c, enum match_type t, struct mdb_command a[]);
int mdb_command_is_ambiguous(char *c, struct mdb_command a[]);
int mdb_command_find_index(char *c, struct mdb_command a[]);
int mdb_address_find_index(uint32_t a);
int mdb_complete(int t);
char * mdb_completion_function(const char *k, int c);

/* globals */

enum mdb_mode mode;	       /* current debugging mode */
enum mdb_hook last_hook;       /* last reason mdb was called, see mdb.h */
int repeat;	   /* if non-zero, empty input repeats last command */
char *last_line;   /* last line of valid input */
struct breakpoint_list *breakpoints;

/* these always point to selected frame */
int current_frame_num;		/* 0 is top of stack */
struct frame *frame;
char *where;
struct operand_stack *operand_stack;
struct local_variable_array *local_variable_array;

/* mode globals */
uint32_t until_address;
int nexti_mode_stack_size;
int finish_mode_stack_size;

/* disassemble globals */
uint32_t old_start;
uint32_t start;
uint32_t end;
int start_index;
int end_index;

/* used by completion mechanism */
int completion_type;
char **addresses;		/* array of valid addresses as strings */
uint32_t *addresses_value;	/* array of valid addresses */
char **classes;			/* array of class names */
char **methods;			/* array of method names */
char **native_methods;		/* array of native method names */
char **symbols;			/* array of symbol names */

/* info commands jump table */
struct mdb_command info_commands[] = {
	{
		"breakpoints", 0, INFO_BREAKPOINTS_SHORT_HELP,
		INFO_BREAKPOINTS_HELP, info_function,
		NONE, 0
	},
	{
		"class-table", 0, INFO_CLASS_TABLE_SHORT_HELP,
		INFO_CLASS_TABLE_HELP, info_function,
		NONE, 0
	},
	{
		"frame", 0, INFO_FRAME_SHORT_HELP,
		INFO_FRAME_HELP, info_function,
		NONE, 0
	},
	{
		"garbage-collector", 0, INFO_GARBAGE_COLLECTOR_SHORT_HELP,
		INFO_GARBAGE_COLLECTOR_HELP, info_function,
		NONE, 0
	},
	{
		"heap", 0, INFO_HEAP_SHORT_HELP,
		INFO_HEAP_HELP, info_function,
		NONE, 0
	},
	{
		"instruction-table", 0, INFO_INSTRUCTION_TABLE_SHORT_HELP,
		INFO_INSTRUCTION_TABLE_HELP, info_function,
		NONE, 0
	},
	{
		"local-variable-array", 0, INFO_LOCAL_VARIABLE_ARRAY_SHORT_HELP,
		INFO_LOCAL_VARIABLE_ARRAY_HELP, info_function,
		NONE, 0
	},
	{
		"method-area", 0, INFO_METHOD_AREA_SHORT_HELP,
		INFO_METHOD_AREA_HELP, info_function,
		NONE, 0
	},
	{
		"native-method-array", 0, INFO_NATIVE_METHOD_ARRAY_SHORT_HELP,
		INFO_NATIVE_METHOD_ARRAY_HELP, info_function,
		NONE, 0
	},
	{
		"operand-stack", 0, INFO_OPERAND_STACK_SHORT_HELP,
		INFO_OPERAND_STACK_HELP, info_function,
		NONE, 0
	},
	{
		"registers", 0, INFO_REGISTERS_SHORT_HELP,
		INFO_REGISTERS_HELP, info_function,
		NONE, 0
	},
	{
		"symbol-table", 0, INFO_SYMBOL_TABLE_SHORT_HELP,
		INFO_SYMBOL_TABLE_HELP, info_function,
		NONE, 0
	},
	{
		"threads", 0, INFO_THREADS_SHORT_HELP,
		INFO_THREADS_HELP, info_function,
		NONE, 0
	},
	{
		"vm-stack", 0, INFO_VM_STACK_SHORT_HELP,
		INFO_VM_STACK_HELP, info_function,
		NONE, 0
	},
	{ NULL, 0, NULL, NULL, NULL, NONE, 0 }
};

/* commands jump table */
struct mdb_command commands[] = {
	{
		"add-symbol-file", 1, ADD_SYMBOL_FILE_SHORT_HELP,
		ADD_SYMBOL_FILE_HELP, add_symbol_file_function,
		ADD_SYMBOL_FILE_COMPLETION_TYPE, 0
	},
	{
		"b", 1, B_SHORT_HELP,
		B_HELP, break_function,
		BREAK_COMPLETION_TYPE, 1
	},
	{
		"backtrace", 0, BACKTRACE_SHORT_HELP,
		BACKTRACE_HELP, backtrace_function,
		BACKTRACE_COMPLETION_TYPE, 0
	},
	{
		"break", 1, BREAK_SHORT_HELP,
		BREAK_HELP, break_function,
		BREAK_COMPLETION_TYPE, 0
	},
	{
		"bt", 0, BT_SHORT_HELP,
		BT_HELP, backtrace_function,
		BACKTRACE_COMPLETION_TYPE, 1
	},
	{
		"continue", 0, CONTINUE_SHORT_HELP,
		CONTINUE_HELP, continue_function,
		CONTINUE_COMPLETION_TYPE, 0
	},
	{
		"d", -1, D_SHORT_HELP,
		D_HELP, delete_function,
		DELETE_COMPLETION_TYPE, 1
	},
	{
		"delete", -1, DELETE_SHORT_HELP,
		DELETE_HELP, delete_function,
		DELETE_COMPLETION_TYPE, 0
	},
	{
		"dis", -1, DIS_SHORT_HELP,
		DIS_HELP, disable_function,
		DISABLE_COMPLETION_TYPE, 1
	},
	{
		"disable", -1, DISABLE_SHORT_HELP,
		DISABLE_HELP, disable_function,
		DISABLE_COMPLETION_TYPE, 0
	},
	{
		"disassemble", 1, DISASSEMBLE_SHORT_HELP,
		DISASSEMBLE_HELP, disassemble_function,
		DISASSEMBLE_COMPLETION_TYPE, 0
	},
	{
		"down", 1, DOWN_SHORT_HELP,
		DOWN_HELP, down_function,
		DOWN_COMPLETION_TYPE, 0
	},
	{
		"enable", -1, ENABLE_SHORT_HELP,
		ENABLE_HELP, enable_function,
		ENABLE_COMPLETION_TYPE, 0
	},
	{
		"f", 1, F_SHORT_HELP,
		F_HELP, frame_function,
		FRAME_COMPLETION_TYPE, 1
	},
	{
		"finish", 0, FINISH_SHORT_HELP,
		FINISH_HELP, finish_function,
		FINISH_COMPLETION_TYPE, 0
	},
	{
		"frame", 1, FRAME_SHORT_HELP,
		FRAME_HELP, frame_function,
		FRAME_COMPLETION_TYPE, 0
	},
	{
		"help", 1, HELP_SHORT_HELP,
		HELP_HELP, help_function,
		HELP_COMPLETION_TYPE, 0
	},
	{
		"info", 1, INFO_SHORT_HELP,
		INFO_HELP, info_function,
		INFO_COMPLETION_TYPE, 0
	},
	{
		"jump", 1, JUMP_SHORT_HELP,
		JUMP_HELP, jump_function,
		JUMP_COMPLETION_TYPE, 0
	},
	{
		"list", 1, LIST_SHORT_HELP,
		LIST_HELP, disassemble_function,
		DISASSEMBLE_COMPLETION_TYPE, 1
	},
	{
		"nexti", 0, NEXTI_SHORT_HELP,
		NEXTI_HELP, nexti_function,
		NEXTI_COMPLETION_TYPE, 0
	},
	{
		"print", 2, PRINT_SHORT_HELP,
		PRINT_HELP, print_function,
		PRINT_COMPLETION_TYPE, 0
	},
	{
		"run", 4, RUN_SHORT_HELP,
		RUN_HELP, run_function,
		RUN_COMPLETION_TYPE, 0
	},
	{
		"s", 0, S_SHORT_HELP,
		S_HELP, stepi_function,
		STEPI_COMPLETION_TYPE, 1
	},
	{
		"set", 2, SET_SHORT_HELP,
		SET_HELP, set_function,
		SET_COMPLETION_TYPE, 0
	},
	{
		"stepi", 0, STEPI_SHORT_HELP,
		STEPI_HELP, stepi_function,
		STEPI_COMPLETION_TYPE, 0
	},
	{
		"u", 1, U_SHORT_HELP,
		U_HELP, up_function,
		UP_COMPLETION_TYPE, 1
	},
	{
		"until", 1, UNTIL_SHORT_HELP,
		UNTIL_HELP, until_function,
		UNTIL_COMPLETION_TYPE, 0
	},
	{
		"up", 1, UP_SHORT_HELP,
		UP_HELP, up_function,
		UP_COMPLETION_TYPE, 0
	},
	{
		"quit", 0, QUIT_SHORT_HELP,
		QUIT_HELP, quit_function,
		QUIT_COMPLETION_TYPE, 0
	},
	{
		"x", 2, X_SHORT_HELP,
		X_HELP, x_function,
		X_COMPLETION_TYPE, 0
	},
	{ NULL, 0, NULL, NULL, NULL, NONE, 0 }
};

/* breakpoint_list */

struct breakpoint_list * breakpoint_list_create() {
	struct breakpoint_list *l;

	if ((l = (struct breakpoint_list *)
	     malloc(sizeof(struct breakpoint_list))) == NULL) {
		perror("mvm: malloc");
		exit(1);
	}

	l->next_breakpoint_num = 1;
	l->num_breakpoints = 0;
	l->head = NULL;

	return l;
}

void breakpoint_list_destroy(struct breakpoint_list *l) {
	if (l != NULL) {
		breakpoint_list_clear(l);
		free(l);
	}
}

void breakpoint_list_clear(struct breakpoint_list *l) {
	struct breakpoint *b, *c;

	if (l != NULL) {
		b = l->head;
		while (b != NULL) {
			c = b;
			b = b->next;
			breakpoint_disable(c);
			breakpoint_destroy(c);
		}

		l->next_breakpoint_num = 1;
		l->head = NULL;
		l->num_breakpoints = 0;
	}
}

struct breakpoint * breakpoint_list_add(struct breakpoint_list *l,
					uint32_t a, uint32_t i, char *w) {
	struct breakpoint *b, *c, *new;

	if (l == NULL) {
		fprintf(stderr, "mvm: breakpoint_list not initialized!\n");
		exit(1);
	}

	for (b = l->head, c = NULL; b != NULL && b->num < l->next_breakpoint_num; c = b, b = b->next);

	if (b != NULL && b->num == l->next_breakpoint_num)
		return NULL;

	if ((new = breakpoint_create(l->next_breakpoint_num, a, i, w)) == NULL)
		exit(1);

	if (b == l->head) {
		new->next = l->head;
		l->head = new;
	} else {
		c->next = new;
		new->next = b;
	}

	l->next_breakpoint_num++;
	l->num_breakpoints++;
	return new;
}

int breakpoint_list_remove_by_num(struct breakpoint_list *l, int n) {
	struct breakpoint *b, *c;

	if (l == NULL) {
		fprintf(stderr, "mvm: breakpoint_list not initialized!\n");
		exit(1);
	}

	for (b = l->head, c = NULL; b != NULL && b->num < n; c = b, b = b->next);

	if (b == NULL || b->num != n)
		return 1;

	if (b == l->head)
		l->head = b->next;
	else
		c->next = b->next;

	breakpoint_disable(b);
	breakpoint_destroy(b);
	l->num_breakpoints--;
	return 0;
}

struct breakpoint * breakpoint_list_find(struct breakpoint_list *l, uint32_t a, char *w) {
	struct breakpoint *b;

	if (l == NULL) {
		fprintf(stderr, "mvm: breakpoint_list not initialized!\n");
		exit(1);
	}

	b = breakpoint_list_find_by_address(l, a);
	return (b != NULL) ? b : breakpoint_list_find_by_where(l, w);
}

struct breakpoint * breakpoint_list_find_by_where(struct breakpoint_list *l, char *w) {
	struct breakpoint *b;

	if (l == NULL) {
		fprintf(stderr, "mvm: breakpoint_list not initialized!\n");
		exit(1);
	}

	if (w == NULL)
		return NULL;

	for (b = l->head; b != NULL; b = b->next) {
		if (strcmp(b->where, w) == 0)
			return b;
	}

	return NULL;
}

struct breakpoint * breakpoint_list_find_by_address(struct breakpoint_list *l, uint32_t a) {
	struct breakpoint *b;

	if (l == NULL) {
		fprintf(stderr, "mvm: breakpoint_list not initialized!\n");
		exit(1);
	}

	for (b = l->head; b != NULL; b = b->next) {
		if (b->address == a)
			return b;
	}

	return NULL;
}

struct breakpoint * breakpoint_list_find_by_num(struct breakpoint_list *l, int n) {
	struct breakpoint *b;

	if (l == NULL) {
		fprintf(stderr, "mvm: breakpoint_list not initialized!\n");
		exit(1);
	}

	for (b = l->head; b != NULL && b->num <= n; b = b->next) {
		if (b->num == n)
			return b;
	}

	return NULL;
}

int breakpoint_list_mark_hits(struct breakpoint_list *l, uint32_t a, char *w) {
	int hits;
	struct breakpoint *b;

	if (l == NULL) {
		fprintf(stderr, "mvm: breakpoint_list not initialized!\n");
		exit(1);
	}

	hits = 0;

	for (b = l->head; b != NULL; b = b->next) {
		if (b->enabled == bp_enabled && breakpoint_is_hit(b, a, w)) {
			b->hit_count++;
			breakpoint_set_mark(b, bp_hit);
			hits++;
		}
	}

	return hits;
}

int breakpoint_list_print_hits(struct breakpoint_list *l) {
	struct breakpoint *b;

	if (l == NULL) {
		fprintf(stderr, "mvm: breakpoint_list not initialized!\n");
		exit(1);
	}

	for (b = l->head; b != NULL; b = b->next) {
		if (breakpoint_has_mark(b, bp_hit))
			breakpoint_print_hit(b);
	}

	return 0;
}

int breakpoint_list_reload_symbols(struct breakpoint_list *l) {
	char *where;
	struct breakpoint *b;

	if (l == NULL) {
		fprintf(stderr, "mvm: breakpoint_list not initialized!\n");
		exit(1);
	}

	for (b = l->head; b != NULL; b = b->next) {
		where = symbol_table_find_label(symbol_table, b->address);
		if (b->type == bp_address && where != NULL) {
			if (b->where != NULL)
				free(b->where);
			b->where = mvm_strdup(where);
		}
	}

	return 0;
}

int breakpoint_list_clear_hits(struct breakpoint_list *l) {
	struct breakpoint *b;

	if (l == NULL) {
		fprintf(stderr, "mvm: breakpoint_list not initialized!\n");
		exit(1);
	}

	for (b = l->head; b != NULL; b = b->next)
		b->hit_count = 0;

	return 0;
}

int breakpoint_list_clear_mark(struct breakpoint_list *l, enum bp_mark m) {
	struct breakpoint *b;

	if (l == NULL) {
		fprintf(stderr, "mvm: breakpoint_list not initialized!\n");
		exit(1);
	}

	for (b = l->head; b != NULL; b = b->next)
		breakpoint_clear_mark(b, m);

	return 0;
}

int breakpoint_list_uninsert_enabled(struct breakpoint_list *l) {
	struct breakpoint *b;

	if (l == NULL) {
		fprintf(stderr, "mvm: breakpoint_list not initialized!\n");
		exit(1);
	}

	for (b = l->head; b != NULL; b = b->next) {
		if (b->enabled == bp_enabled)
			breakpoint_uninsert(b);
	}

	return 0;
}

int breakpoint_list_insert_enabled(struct breakpoint_list *l) {
	struct breakpoint *b;

	if (l == NULL) {
		fprintf(stderr, "mvm: breakpoint_list not initialized!\n");
		exit(1);
	}

	for (b = l->head; b != NULL; b = b->next) {
		if (b->enabled == bp_enabled)
			breakpoint_insert(b);
	}

	return 0;
}

int breakpoint_list_dump(struct breakpoint_list *l) {
	struct breakpoint *b;

	if (l == NULL) {
		fprintf(stderr, "mvm: breakpoint_list not initialized!\n");
		exit(1);
	}

	if (l->head == NULL) {
		fprintf(stderr, "No breakpoints.\n");
		return 0;
	}

	fprintf(stderr, "%-3s %-14s %-3s %-10s %-s\n",
		"Num", "Type", "Enb", "Address", "What");

	for (b = l->head; b != NULL; b = b->next) {
		if (b->type == bp_method) {
			fprintf(stderr, "%-3d %-14s %-3s %-10" PRIu32 " in %-s <native-method>\n",
				b->num, "breakpoint", (b->enabled == bp_enabled) ? "y" : "n",
				b->native_index, b->where);
		} else if (b->type == bp_address) {
			fprintf(stderr, "%-3d %-14s %-3s %-10" PRIu32 " in %-s\n",
				b->num, "breakpoint", (b->enabled == bp_enabled) ? "y" : "n",
				b->address, b->where);
		}

		if (b->hit_count > 0) {
			fprintf(stderr, "        breakpoint already hit %d %s\n",
				b->hit_count, (b->hit_count > 1) ? "times" : "time");
		}
	}

	return 0;
}

int breakpoint_print_hit(struct breakpoint *b) {
	if (b == NULL) {
		fprintf(stderr, "mvm: breakpoint not initialized!\n");
		exit(1);
	}

	fprintf(stderr, "\nBreakpoint %d, %s (%" PRIu32 "), at %s\n",
		b->num, b->where, b->address,
		method_area_get_class_file(method_area));

	return 0;
}

/* commands */

int add_symbol_file_function(char **t, int n) {
	int i;

	repeat = 0;

	if (n != 2) {
		fprintf(stderr, "usage: add-symbol-file FILE\n");
		return 0;
	}

	if (symbol_table_load_dump(symbol_table, t[1]) != 0)
		return 0;

	for (i = 0; symbols[i] != NULL; i++)
		free(symbols[i]);
	free(symbols);

	if ((symbols = symbol_table_label_array(symbol_table)) == NULL)
		exit(1);

	breakpoint_list_reload_symbols(breakpoints);

	return 0;
}

int backtrace_function(char **t, int n) {
	struct vm_stack *vm_stack;

	vm_stack = thread_get_vm_stack(NULL);
	
	repeat = 1;

	if (vm_stack_empty(vm_stack)) {
		fprintf(stderr, "No stack.\n");
		return 0;
	}

	if (n == 1) {
		vm_stack_dump(vm_stack, 0, 0);
	} else if (n == 2 && strncmp(t[1], "full", strlen(t[1])) == 0) {
		vm_stack_dump(vm_stack, 1, 1);
	} else {
		/* print help text */
		return 0;
	}

	return 0;
}

int break_function(char **t, int n) {
	char *where;
	int is_native;
	struct breakpoint *b, *c;
	uint32_t pc, address, native_index;

	pc = thread_get_pc(NULL);

	repeat = 1;
	is_native = 0;
	address = 0;
	native_index = 0;

	if (n == 1) {
		/* current address */
		if (mode == startup_mode || mode == terminated_mode) {
			fprintf(stderr, "No default breakpoint address now.\n");
			return 0;
		}

		address = pc;
		where = frame_get_method_name(frame);
	} else if (strspn(t[1], "0123456789") == strlen(t[1])) {
		/* address */
		address = atoi(t[1]);
		if (mdb_address_find_index(address) == -1) {
			fprintf(stderr, "Invalid address %" PRIu32 ".\n", address);
			return 0;
		}

		if ((where = symbol_table_find_label(symbol_table, address)) == NULL)
			where = "???";
	} else if ((native_index = native_method_array_get_index(native_method_array, t[1])) != -1) {
		/* native method name */
		is_native = 1;
		address = 0;
		where = native_method_array_get_name(native_method_array, native_index);
	} else if ((address = symbol_table_find_address(symbol_table, t[1])) != 0) {
		/* label name */
		where = t[1];
		if (mdb_address_find_index(address) == -1) {
			fprintf(stderr, "Invalid address %" PRIu32 ".\n", address);
			return 0;
		}
	} else {
		/* error */
		fprintf(stderr, "Method or symbol \"%s\" not defined.\n", t[1]);
		return 0;
	}

	if (is_native == 1) {
		if ((c = breakpoint_list_find_by_where(breakpoints, where)) != NULL)
			fprintf(stderr, "Note: breakpoint already set at %s.\n", where);
	} else {
		if ((c = breakpoint_list_find_by_address(breakpoints, address)) != NULL)
			fprintf(stderr, "Note: breakpoint already set at pc %" PRIu32 ".\n", address);
	}

	b = breakpoint_list_add(breakpoints, address, native_index, where);
	breakpoint_enable(b);

	if (is_native == 0) {
		fprintf(stderr, "Breakpoint %d at %" PRIu32 "\n", b->num, b->address);
	} else {
		fprintf(stderr, "Breakpoint %d at %s\n", b->num, b->where);
	}

	return 0;
}

int continue_function(char **t, int n) {
	repeat = 1;

	if (mode == startup_mode || mode == terminated_mode) {
		fprintf(stderr, "The program is not being run.\n");
		return 0;
	}

	fprintf(stderr, "Continuing.\n");

	mode = continue_mode;
	return 1;
}

int delete_function(char **t, int n) {
	int num, i;
	char buf[BUFFER_SIZE];
	struct breakpoint *b;

	repeat = 0;

	if (n == 1) {
		for (;;) {
			fprintf(stderr, "Delete all breakpoints? (y or n) ");

			fgets(buf, BUFFER_SIZE, stdin);
			buf[strlen(buf)-1] = '\0';

			if (strcmp(buf, "y") == 0) {
				breakpoint_list_clear(breakpoints);
				break;
			} else if (strcmp(buf, "n") == 0) {
				break;
			} else {
				fprintf(stderr, "Please answer y or n.\n");
			}
		}

		return 0;
	}

	for (i = 1; i < n; i++) {
		num = atoi(t[i]);

		if ((b = breakpoint_list_find_by_num(breakpoints, num)) == NULL) {
			fprintf(stderr, "No breakpoint number %d.\n", num);
			continue;
		}

		breakpoint_list_remove_by_num(breakpoints, num);
	}

	return 0;
}

int disable_function(char **t, int n) {
	int num, i;
	struct breakpoint *b;

	repeat = 1;

	for (i = 1; i < n; i++) {
		num = atoi(t[i]);

		if ((b = breakpoint_list_find_by_num(breakpoints, num)) == NULL) {
			fprintf(stderr, "No breakpoint number %d.\n", num);
			continue;
		}

		breakpoint_disable(b);
	}

	return 0;
}

int disassemble_function(char **t, int n) {
	int i, continue_prev;

	repeat = 1;

	if (n == 1 && frame != NULL && frame_get_method_name(frame) != 0) {
		/* list from current address in selected frame */
		start = frame_get_current_address(frame);
	} else if (n == 2) {
		/* list from given address/symbol */
		if (strspn(t[1], "0123456789") == strlen(t[1])) {
			start = atoi(t[1]);
		} else if ((start = symbol_table_find_address(symbol_table, t[1])) == 0) {
			fprintf(stderr, "Symbol \"%s\" not defined.\n", t[1]);
			return 0;
		}

		if (mdb_address_find_index(start) == -1) {
			fprintf(stderr, "Invalid address %" PRIu32 ".\n", start);
			return 0;
		}
	} else {
		/* print help text */
		return 0;
	}

	/* continue listing based on original start location */
	continue_prev = (start == old_start);
	old_start = start;

	/* print 10 instruction *around* start location,
	 * so back start location up at most 5 instructions
	 */
	start_index = mdb_address_find_index(start);
	start_index = ((start_index-5) < 0) ? 0 : start_index-5;
	start = addresses_value[start_index];

	/* ensure start address is valid */
	if ((start_index = mdb_address_find_index(start)) == -1) {
		fprintf(stderr, "Invalid address %" PRIu32 ".\n", start);
		return 0;
	}

	/* same start address, continue previous listing */
	if (continue_prev) {
		start_index = end_index+1;
		if (addresses[start_index] == 0) {
			fprintf(stderr, "Instruction number %d out of "
				"range; %s has %d instructions.\n", start_index+1,
				method_area_get_class_file(method_area), start_index);
			return 0;
		}
	}

	start = addresses_value[start_index];

	/* calculate end address */
	end_index = start_index+1;
	for (i = 0; i < DISASSEMBLE_LINES &&
		     addresses_value[end_index+1] != 0; i++) {
		end_index++;
	}
	end = addresses_value[end_index];

	mvm_disassemble_region(1, start, end);
	return 0;
}

int down_function(char **t, int n) {
	int num, i;
	struct frame *tmp;
	struct vm_stack *vm_stack;

	vm_stack = thread_get_vm_stack(NULL);

	repeat = 1;

	if (vm_stack_empty(vm_stack)) {
		fprintf(stderr, "No stack.\n");
		return 0;
	}

	switch (n) {
	case 1:
		if (current_frame_num == 0) {
			fprintf(stderr, "Bottom (i.e., innermost) frame selected; you cannot go down.\n");
			return 0;
		}

		num = 1;
		break;
	case 2:
		num = atoi(t[1]);
		if (num > current_frame_num)
			num = current_frame_num;
		break;
	default:
		/* print help text */
		return 0;
	}

	current_frame_num -= num;

	i = current_frame_num;
	tmp = vm_stack_peek(vm_stack);
	while (i-- > 0)
		tmp = frame_get_calling_frame(tmp);
	frame = tmp;
	mdb_update_frame();

	fprintf(stderr, "#%d  ", current_frame_num);
	mdb_print_location(1);
	return 0;
}

int enable_function(char **t, int n) {
	int num, i;
	struct breakpoint *b;

	repeat = 1;

	for (i = 1; i < n; i++) {
		num = atoi(t[i]);

		if ((b = breakpoint_list_find_by_num(breakpoints, num)) == NULL) {
			fprintf(stderr, "No breakpoint number %d.\n", num);
			continue;
		}

		breakpoint_enable(b);
	}

	return 0;
}

int finish_function(char **t, int n) {
	struct vm_stack *vm_stack;

	vm_stack = thread_get_vm_stack(NULL);
	
	repeat = 1;

	if (mode == startup_mode || mode == terminated_mode) {
		fprintf(stderr, "The program is not running.\n");
		return 0;
	}

	if (current_frame_num == vm_stack_size(vm_stack)-1) {
		fprintf(stderr, "\"finish\" not meaningful in the outermost frame.\n");
		return 0;
	}

	mode = finish_mode;
	finish_mode_stack_size = vm_stack_size(vm_stack)-current_frame_num;

	fprintf(stderr, "Run till exit from #%d  %s\n\n",
		current_frame_num, frame_get_method_name(frame));
	return 1;
}

int frame_function(char **t, int n) {
	int num;
	struct frame *tmp;
	struct vm_stack *vm_stack;

	vm_stack = thread_get_vm_stack(NULL);

	repeat = 1;

	if (vm_stack_empty(vm_stack)) {
		fprintf(stderr, "No stack.\n");
		return 0;
	}

	switch (n) {
	case 1:
		fprintf(stderr, "#%d  ", current_frame_num);
		mdb_print_location(1);
		return 0;
	case 2:
		num = atoi(t[1]);
		if (num < 0)
			num = 0;
		if (num >= vm_stack_size(vm_stack))
			num = vm_stack_size(vm_stack)-1;
		break;
	default:
		/* print help text */
		return 0;
	}

	current_frame_num = num;

	tmp = vm_stack_peek(vm_stack);
	while (num-- > 0) {
		tmp = frame_get_calling_frame(tmp);
	}
	frame = tmp;
	mdb_update_frame();

	fprintf(stderr, "#%d  ", current_frame_num);
	mdb_print_location(1);
	return 0;
}

int help_function(char **t, int n) {
	int i;

	repeat = 1;

	if (n > 1) {
		if (mdb_command_is_ambiguous(t[1], commands) != 0) {
			mdb_disambugiate_command(t[1], "", commands);
			return 0;
		} else if ((i = mdb_command_find_index(t[1], commands)) == -1) {
			fprintf(stderr, "Undefined command: \"%s\".  Try \"help\".\n", t[1]);
			return 0;
		}

		fprintf(stderr, "%s\n", commands[i].short_help);
		fprintf(stderr, "%s", commands[i].help);
	} else {
		fprintf(stderr, "List of commands:\n");
		fprintf(stderr, "\n");

		for (i = 0; commands[i].name != NULL; i++) {
			if (commands[i].alias == 0)
				fprintf(stderr, "%s -- %s\n", commands[i].name, commands[i].short_help);
		}

		fprintf(stderr, "\n");
		fprintf(stderr, "Type \"help\" followed by command name for full documentation.\n");
		fprintf(stderr, "Command name abbreviations are allowed if unambiguous.\n");
	}

	return 0;
}

int info_function(char **t, int n) {
	int i, index;
	char *command;
	uint32_t pc;
	struct thread *thread;
	struct vm_stack *vm_stack;

	thread = thread_get_current();

	pc = thread_get_pc(thread);
	vm_stack = thread_get_vm_stack(thread);

	repeat = 1;

	if (n != 2) {
		for (i = 0; info_commands[i].name != NULL; i++) {
			fprintf(stderr, "info %s -- %s\n", info_commands[i].name,
				info_commands[i].short_help);
		}

		return 0;
	}

	command = t[1];

	if (mdb_command_is_ambiguous(command, info_commands) != 0) {
		mdb_disambugiate_command(command, "info ", info_commands);
		return 0;
	} else if ((index = mdb_command_find_index(command, info_commands)) == -1) {
		fprintf(stderr, "Undefined info command: \"%s\".  Try \"help info\".\n", command);
		return 0;
	}

	command = info_commands[index].name;

	if (strcmp(command, "breakpoints") == 0) {
		breakpoint_list_dump(breakpoints);
	} else if (strcmp(command, "class-table") == 0) {
		class_table_dump(class_table);
	} else if (strcmp(command, "frame") == 0) {
		if (vm_stack_size(vm_stack) == 0 || frame == NULL) {
			fprintf(stderr, "No stack.\n");
			return 0;
		}

		frame_dump(frame, 0, 0);
	} else if (strcmp(command, "garbage-collector") == 0) {
		garbage_collector_dump(garbage_collector);
	} else if (strcmp(command, "heap") == 0) {
		heap_dump(heap);
	} else if (strcmp(command, "instruction-table") == 0) {
		instruction_table_dump(instruction_table);
	} else if (strcmp(command, "local-variable-array") == 0) {
		if (vm_stack_size(vm_stack) == 0 || local_variable_array == NULL) {
			fprintf(stderr, "No stack.\n");
			return 0;
		}

		local_variable_array_dump(local_variable_array);
	} else if (strcmp(command, "method-area") == 0) {
		method_area_dump(method_area);
	} else if (strcmp(command, "native-method-array") == 0) {
		native_method_array_dump(native_method_array);
	} else if (strcmp(command, "operand-stack") == 0) {
		if (vm_stack_size(vm_stack) == 0 || operand_stack == NULL) {
			fprintf(stderr, "No stack.\n");
			return 0;
		}

		operand_stack_dump(operand_stack);
	} else if (strcmp(command, "registers") == 0) {
		fprintf(stderr, "%-16s%" PRIu32 "\n", "pc", pc);
	} else if (strcmp(command, "symbol-table") == 0) {
		symbol_table_dump(symbol_table);
	} else if (strcmp(command, "threads") == 0) {
		mdb_dump_threads();
	} else if (strcmp(command, "vm-stack") == 0) {
		if (vm_stack_size(vm_stack) == 0) {
			fprintf(stderr, "No stack.\n");
			return 0;
		}

		vm_stack_dump(vm_stack, 0, 0);
	}

	return 0;
}

void mdb_dump_threads() {
	char *name;
	int i, ref;
	uint32_t pc;
	struct frame *frame;
	struct thread *thread;
	struct object *object;
	struct ref_set *threads;
	struct vm_stack *vm_stack;

	threads = ref_set_create();
	heap_populate_thread_set(heap, threads);
	i = ref_set_size(threads);

	ref_set_iterator_init(threads);

	while ((ref = ref_set_iterator_next(threads)) != 0) {
		object = heap_fetch_object(heap, ref);
		thread = object_get_thread(object);

		pc = thread_get_pc(thread);
		vm_stack = thread_get_vm_stack(thread);

		frame = vm_stack_peek(vm_stack);

		fprintf(stderr, "* %d %-14" PRIu32 " in ", i--, pc);

		if (frame == NULL) {
			fprintf(stderr, "??? ()\n");
		} else {
			name = frame_get_method_name(frame);
			frame_dump(frame, 0, 0);
		}
	}
}

int jump_function(char **t, int n) {
	uint32_t pc, address;

	pc = thread_get_pc(NULL);
	repeat = 1;

	if (mode == startup_mode || mode == terminated_mode) {
		fprintf(stderr, "The program is not being run.\n");
		return 0;
	}

	if (n == 1) {
		fprintf(stderr, "Argument required (starting address).\n");
		return 0;
	}

	if (strspn(t[1], "0123456789") == strlen(t[1])) {
		address = atoi(t[1]);
	} else if ((address = symbol_table_find_address(symbol_table, t[1])) == 0) {
		fprintf(stderr, "Symbol \"%s\" not defined.\n", t[1]);
		return 0;
	}

	if (mdb_address_find_index(address) == -1) {
		fprintf(stderr, "Invalid address %" PRIu32 ".\n", address);
		return 0;
	}

	thread_set_pc(NULL, address);
	fprintf(stderr, "Continuing at %" PRIu32 ".\n", address);

	return 1;
}

int nexti_function(char **t, int n) {
	struct vm_stack *vm_stack;

	vm_stack = thread_get_vm_stack(NULL);
	
	repeat = 1;

	if (mode == startup_mode || mode == terminated_mode) {
		fprintf(stderr, "The program is not being run.\n");
		return 0;
	}

	mode = nexti_mode;
	nexti_mode_stack_size = vm_stack_size(vm_stack);

	return 1;
}

int print_function(char **t, int n) {
	int32_t value;
	int ref, print_as;
	struct table *table;
	struct string *string;
	struct object *object;
	struct integer *integer;
	char *format, *predefined_name, *chars;

	repeat = 1;

	if (n == 1) {
		return 0;
	} else if (n == 2) {
		format = "/o";
		ref = atoi(t[1]);
	} else {
		format = t[1];
		ref = atoi(t[2]);
	}

	if ((object = heap_fetch_object(heap, ref)) == NULL) {
		fprintf(stderr, "No reference %d in current context.\n", ref);
		return 0;
	}

	print_as = object_type;
	if (strcmp(format, "/o") == 0) {
		print_as = object_type;
		predefined_name = "Object";
	} else if (strcmp(format, "/i") == 0) {
		print_as = integer_type;
		predefined_name = "Integer";
	} else if (strcmp(format, "/s") == 0) {
		print_as = string_type;
		predefined_name = "String";
	} else if (strcmp(format, "/t") == 0) {
		print_as = table_type;
		predefined_name = "Table";
	} else {
		fprintf(stderr, "Undefined output format \"%s\".\n",
			format);
		return 0;
	}

	if (print_as != object_type &&
	    object_get_predefined_type(object) != print_as) {
		fprintf(stderr, "Reference %d does not extend predefined type %s.\n",
			ref, predefined_name);
		return 0;
	}

	switch (print_as) {
	case object_type:
		object_dump(object, 1);
		break;
	case integer_type:
		integer = object_get_integer(object);
		value = integer_get_value(integer);
		fprintf(stderr, "%" PRId32 "", value);
		break;
	case string_type:
		string = object_get_string(object);
		chars = string_get_chars(string);
		fprintf(stderr, "\"%s\"", chars);
		break;
	case table_type:
		table = object_get_table(object);
		table_dump(table);
		break;
	}

	fprintf(stderr, "\n");
	return 0;
}

int run_function(char **t, int n) {
	int i, do_restart, resume;
	char buf[BUFFER_SIZE];

	repeat = 0;

	/* input/output redirection */
	for (i = 1; i < n; i++) {
		if (strcmp(t[i], "<") == 0 && i+1 < n) {
			if ((input = fopen(t[i+1], "r")) == NULL) {
				fprintf(stderr, "mvm: %s: %s\n", t[2], strerror(errno));
				exit(1);
			}
			i++;
		} else if (strcmp(t[i], ">") == 0 && i+1 < n) {
			if ((output = fopen(t[i+1], "w")) == NULL) {
				fprintf(stderr, "mvm: %s: %s\n", t[2], strerror(errno));
				exit(1);
			}
			i++;
		} else {
			fprintf(stderr, "syntax error near \"%s\".\n", t[i]);
			return 0;
		}
	}

	if (mode == startup_mode) {
		resume = 1;
		do_restart = 0;
	} else if (mode == terminated_mode) {
		resume = 1;
		do_restart = 1;
	} else {
		for (;;) {
			fprintf(stderr, "The program being debugged has been started already.\n");
			fprintf(stderr, "Start it from the beginning? (y or n) ");

			fgets(buf, BUFFER_SIZE, stdin);
			buf[strlen(buf)-1] = '\0';

			if (strcmp(buf, "y") == 0) {
				resume = 1;
				do_restart = 1;
				break;
			} else if (strcmp(buf, "n") == 0) {
				resume = 0;
				do_restart = 0;
				fprintf(stderr, "Program not restarted.\n");
				break;
			} else {
				fprintf(stderr, "Please answer y or n.\n");
			}
		}
	}

	if (do_restart != 0) {
		mdb_restart();
		restart = 1;
	}

	if (resume != 0) {
		fprintf(stderr, "Starting program: %s",
			method_area_get_class_file(method_area));
		for (i = 1; i < n; i++)
			fprintf(stderr, " %s", t[i]);
		fprintf(stderr, "\n");

	}

	mode = continue_mode;
	return resume;
}

int set_function(char **t, int n) {
	uint32_t address, value;

	repeat = 1;

	if (n != 3) {
		/* print help text */
		return 0;
	}

	if (strspn(t[1], "0123456789") == strlen(t[1])) {
		address = atoi(t[1]);
	} else if ((address = symbol_table_find_address(symbol_table, t[1])) == 0) {
		fprintf(stderr, "Symbol \"%s\" not defined.\n", t[1]);
		return 0;
	}

	value = atoi(t[2]);
	method_area_write(method_area, address, value);

	return 0;
}

int stepi_function(char **t, int n) {
	repeat = 1;

	if (mode == startup_mode || mode == terminated_mode) {
		fprintf(stderr, "The program is not being run.\n");
		return 0;
	}

	mode = stepi_mode;
	return 1;
}

int until_function(char **t, int n) {
	int index;
	uint32_t address;

	repeat = 1;

	if (mode == startup_mode || mode == terminated_mode) {
		fprintf(stderr, "The program is not running.\n");
		return 0;
	}

	if (n < 2) {
		index = mdb_address_find_index(frame_get_current_address(frame));
		if (index == -1 || addresses_value[index+1] == 0 ||
		    addresses_value[index+1] > frame_get_end_address(frame)) {
			fprintf(stderr, "No next instruction.\n");
			return 0;
		}

		until_address = address = addresses_value[index+1];
	} else {
		if (strspn(t[1], "0123456789") == strlen(t[1])) {
			address = atoi(t[1]);
		} else if ((address = symbol_table_find_address(symbol_table, t[1])) == 0) {
			fprintf(stderr, "Symbol \"%s\" not defined.\n", t[1]);
			return 0;
		}

		until_address = address;
	}

	if (mdb_address_find_index(until_address) == -1) {
		fprintf(stderr, "Invalid address %" PRIu32 ".\n", address);
		return 0;
	}

	mode = until_mode;
	return 1;
}

int up_function(char **t, int n) {
	int num;
	struct vm_stack *vm_stack;

	vm_stack = thread_get_vm_stack(NULL);

	repeat = 1;

	if (vm_stack_empty(vm_stack)) {
		fprintf(stderr, "No stack.\n");
		return 0;
	}

	switch (n) {
	case 1:
		if (current_frame_num == vm_stack_size(vm_stack)-1) {
			fprintf(stderr, "Initial frame selected; you cannot go up.\n");
			return 0;
		}

		num = 1;
		break;
	case 2:
		num = atoi(t[1]);
		if (num > vm_stack_size(vm_stack)-current_frame_num-1)
			num = vm_stack_size(vm_stack)-current_frame_num-1;
		break;
	default:
		/* print help text */
		return 0;
	}

	current_frame_num += num;

	while (num-- > 0)
		frame = frame_get_calling_frame(frame);
	mdb_update_frame();

	fprintf(stderr, "#%d  ", current_frame_num);
	mdb_print_location(1);
	return 0;
}

int quit_function(char **t, int n) {
	int quit;
	char buf[BUFFER_SIZE];

	repeat = 1;

	if (mode == startup_mode || mode == terminated_mode) {
		quit = 1;
	} else {
		for (;;) {
			fprintf(stderr, "The program is running.  Exit anyway? (y or n) ");

			fgets(buf, BUFFER_SIZE, stdin);
			buf[strlen(buf)-1] = '\0';

			if (strcmp(buf, "y") == 0) {
				quit = 1;
				break;
			} else if (strcmp(buf, "n") == 0) {
				quit = 0;
				fprintf(stderr, "Not confirmed.\n");
				break;
			} else {
				fprintf(stderr, "Please answer y or n.\n");
			}
		}
	}

	if (quit == 1)
		exit(0);

	return 0;
}

int x_function(char **t, int n) {
	char *buf, *format, *addr, *label;
	uint32_t address, value;

	repeat = 0;

	if (n < 2 || n > 3) {
		/* print help text */
		return 0;
	}

	format = "/x";

	if (n == 2) {
		addr = t[1];
	} else {
		format = t[1];
		addr = t[2];
	}

	if (strspn(addr, "0123456789") == strlen(addr)) {
		address = atoi(t[1]);
	} else if ((address = symbol_table_find_address(symbol_table, addr)) == 0) {
		fprintf(stderr, "Symbol \"%s\" not defined.\n", addr);
		return 0;
	}

	if (method_area_address_is_valid(method_area, address) == 0) {
		fprintf(stderr, "Invalid address %" PRIu32 "\n", address);
		return 0;
	}

	value = method_area_fetch(method_area, address);

	if ((label = symbol_table_find_label(symbol_table, address)) == NULL)
		fprintf(stderr, "%" PRIu32 ":    ", address);
	else
		fprintf(stderr, "%" PRIu32 " <%s>:    ", address, label);

	if (strcmp(format, "/o") == 0) {
		fprintf(stderr, "0%.21" PRIo32 "\n", value);
	} else if (strcmp(format, "/x") == 0) {
		fprintf(stderr, "0x%.8" PRIx32 "\n", value);
	} else if (strcmp(format, "/u") == 0) {
		fprintf(stderr, "%" PRIu32 "\n", value);
	} else if (strcmp(format, "/i") == 0) {
		mvm_disassemble_instruction(0, address);
	} else if (strcmp(format, "/c") == 0) {
		fprintf(stderr, "'%c'\n", (char)value);
	} else if (strcmp(format, "/s") == 0) {
		buf = mvm_disassemble_string_argument(address-sizeof(uint32_t));
		fprintf(stderr, "\"%s\"\n", buf);
		free(buf);
	} else {
		fprintf(stderr, "Undefined output format \"%s\".\n", format);
	}

	return 0;
}

/* completion */

char * mdb_completion_function(const char *k, int c) {
	char *str;
	static int off, len, commands_index, info_commands_index,
		symbols_index, classes_index, methods_index,
		native_methods_index, addresses_index, filenames_index;

	if (completion_type & NONE)
		return NULL;

	if (c == 0) {
		off = strspn(k, " ");
		len = strlen(k);

		commands_index = 0;
		info_commands_index = 0;
		symbols_index = 0;
		classes_index = 0;
		methods_index = 0;
		native_methods_index = 0;
		addresses_index = 0;
		filenames_index = 0;
	}

	str = NULL;

	/* commands */
	while ((COMMANDS & completion_type) &&
	       (str = commands[commands_index].name) != NULL) {
		commands_index++;
		if (strncmp(k+off, str, strlen(k+off)) == 0)
			return mvm_strdup(str);
	}

	/* info commands */
	while ((INFO_COMMANDS & completion_type) &&
	       (str = info_commands[info_commands_index].name) != NULL) {
		info_commands_index++;
		if (strncmp(k, str, strlen(k)) == 0)
			return mvm_strdup(str);
	}

	/* symbols */
	while ((SYMBOLS & completion_type) &&
	       (str = symbols[symbols_index]) != NULL) {
		symbols_index++;
		if (strncmp(k, str, len) == 0)
			return mvm_strdup(str);
	}

	/* methods */
	while ((METHODS & completion_type) &&
	       (str = methods[methods_index]) != NULL) {
		methods_index++;
		if (strncmp(k, str, len) == 0)
			return mvm_strdup(str);
	}

	/* native_methods */
	while ((NATIVE_METHODS & completion_type) &&
	       (str = native_methods[native_methods_index]) != NULL) {
		native_methods_index++;
		if (strncmp(k, str, len) == 0)
			return mvm_strdup(str);
	}

	/* addresses */
	while ((ADDRESSES & completion_type) &&
	       (str = addresses[addresses_index]) != NULL) {
		addresses_index++;
		if (strncmp(k, str, len) == 0)
			return mvm_strdup(str);
	}

	/* filenames */
	while ((FILENAMES & completion_type) &&
	       (str = rl_filename_completion_function(k, filenames_index)) != NULL) {
		filenames_index++;
		return str;
	}

	return NULL;
}

char ** mdb_completion(const char *l, int s, int e) {
	int n, index, i;
	char *line, **tokens, **matches;

	matches = NULL;

	if (s == 0 || strspn(rl_line_buffer, " ") == s) {
		completion_type = COMMANDS;
		matches = rl_completion_matches(l, mdb_completion_function);
	} else {
		line = mvm_strdup(rl_line_buffer);
		tokens = mdb_tokenize(line, &n);

		if (mdb_command_is_ambiguous(tokens[0], commands) == 0 &&
		    (index = mdb_command_find_index(tokens[0], commands)) != -1 &&
		    (n-1 < commands[index].max_args ||
		     (n-1 == commands[index].max_args && strcmp(l, "") != 0))) {
			completion_type = commands[index].completion_type;
		} else {
			completion_type = NONE;
		}

		if (completion_type & NONE)
			rl_attempted_completion_over = 1;

		matches = rl_completion_matches(l, mdb_completion_function);

		for (i = 0; i < n; i++)
			free(tokens[i]);
		free(tokens);
	}

	return matches;
}

void mdb_sigint_handler(int s) {
	struct vm_stack *vm_stack;

	vm_stack = thread_get_vm_stack(NULL);
	
	fprintf(stderr, "\nProgram interrupted.\n");
	frame = vm_stack_peek(vm_stack);
	mdb_update_frame();
	mdb_print_location(0);
	mdb_command_processor();
}

void mdb_sigint(void (*h)(int)) {
	struct sigaction action;

	action.sa_handler = h;
	action.sa_flags = SA_RESTART;

	if (sigaction(SIGINT, &action, NULL) != 0) {
		perror("mvm: sigaction");
		exit(1);
	}
}

int mdb_disambugiate_command(char *c, char *t, struct mdb_command a[]) {
	int i, j, num_found;

	num_found = mdb_command_num_matches(c, partial_type, a);

	fprintf(stderr, "Ambiguous %scommand \"%s\":", t, c);

	j = 0;
	for (i = 0; a[i].name != NULL; i++) {
		if (mdb_do_match(c, a[i].name, partial_type) == 0) {
			fprintf(stderr, " %s", a[i].name);
			if (j++ < num_found-1)
				fprintf(stderr, ",");
		}
	}

	fprintf(stderr, ".\n");
	return 0;
}

int mdb_do_match(char *c, char *d, enum match_type t) {
	int retval;

	retval = 1;

	if (t == complete_type)
		retval = strcmp(c, d);
	else if (t == partial_type)
		retval = strncmp(c, d, strlen(c));

	return retval;
}

int mdb_command_num_matches(char *c, enum match_type t, struct mdb_command a[]) {
	int i, num_found;

	num_found = 0;
	for (i = 0; a[i].name != NULL; i++) {
		if (mdb_do_match(c, a[i].name, t) == 0)
			num_found++;
	}

	return num_found;
}

int mdb_command_is_ambiguous(char *c, struct mdb_command a[]) {
	return mdb_command_num_matches(c, complete_type, a) != 1 &&
		mdb_command_num_matches(c, partial_type, a) >= 2;
}

int mdb_command_find_index(char *c, struct mdb_command a[]) {
	int n, len;

	len = strlen(c);

	for (n = 0; a[n].name != NULL; n++) {
		if (strncmp(c, a[n].name, len) == 0)
			break;
	}

	if (a[n].name == NULL)
		return -1;

	return n;
}

int mdb_address_find_index(uint32_t a) {
	int i;

	for (i = 0; addresses_value[i] != 0; i++) {
		if (addresses_value[i] == a)
			return i;
	}

	return -1;
}

char ** mdb_tokenize(char *l, int *n) {
	int num_tokens, i;
	char **tokens, *token, *line;

	/* determine num_tokens */
	line = mvm_strdup(l);

	num_tokens = 0;
	if (strtok(line, " ") != NULL) {
		num_tokens++;
		while (strtok(NULL, " ") != NULL)
			num_tokens++;
	}

	free(line);

	/* allocate array */
	if ((tokens = (char **)malloc(sizeof(char *)*num_tokens)) == NULL) {
		perror("mvm: malloc");
		exit(1);
	}

	/* strdup each token into array */
	line = mvm_strdup(l);

	token = strtok(line, " ");
	tokens[0] = mvm_strdup(token);

	for (i = 1; (token = strtok(NULL, " ")) != NULL; i++)
		tokens[i] = mvm_strdup(token);

	free(line);

	if (n != NULL)
		*n = num_tokens;

	return tokens;
}

int mdb_initialize() {
	mode = continue_mode;
	repeat = 1;
	last_line = NULL;
	nexti_mode_stack_size = 1;
	finish_mode_stack_size = 1;
	last_hook = enter_method_hook;

	old_start = start = end = 0;
	start_index = end_index = 0;

	if ((symbols = symbol_table_label_array(symbol_table)) == NULL)
		exit(1);
	if ((classes = class_table_classes_array(class_table)) == NULL)
		exit(1);
	if ((methods = class_table_methods_array(class_table)) == NULL)
		exit(1);
	if ((native_methods = native_method_array_methods_array(native_method_array)) == NULL)
		exit(1);
	if ((addresses = mvm_disassemble_addresses_array()) == NULL)
		exit(1);
	if ((addresses_value = mvm_disassemble_addresses_value_array()) == NULL)
		exit(1);

	if ((breakpoints = breakpoint_list_create()) == NULL)
		exit(1);

	rl_attempted_completion_function = mdb_completion;
	rl_basic_word_break_characters = " \t\n\"\\'`@=;|&{(";
	rl_ignore_completion_duplicates = 1;

	return 0;
}

int mdb_cleanup() {
	int i;

	if (last_line != NULL)
		free(last_line);

	if (symbols != NULL) {
		for (i = 0; symbols[i] != NULL; i++ )
			free(symbols[i]);
		free(symbols);
	}

	if (classes != NULL) {
		for (i = 0; classes[i] != NULL; i++ )
			free(classes[i]);
		free(classes);
	}

	if (methods != NULL) {
		for (i = 0; methods[i] != NULL; i++ )
			free(methods[i]);
		free(methods);
	}

	if (native_methods != NULL) {
		for (i = 0; native_methods[i] != NULL; i++ )
			free(native_methods[i]);
		free(native_methods);
	}

	if (addresses != NULL) {
		for (i = 0; addresses[i] != NULL; i++ )
			free(addresses[i]);
		free(addresses);
	}

	breakpoint_list_destroy(breakpoints);
	return 0;
}

int mdb_print_location(int d) {
	if (d != 0)
		frame_dump(frame, 0, 0);
	if (frame_get_start_address(frame) != NATIVE_ADDRESS)
		mvm_disassemble_instruction(1, frame_get_current_address(frame));

	return 0;
}

int mdb_restart() {
	mode = continue_mode;
	nexti_mode_stack_size = 1;
	finish_mode_stack_size = 1;
	old_start = start = end = 0;
	start_index = end_index = 0;
	breakpoint_list_clear_hits(breakpoints);
	restart = 1;
	return 0;
}

int mdb_update_frame() {
	if (frame == NULL) {
		where = NULL;
		operand_stack = NULL;
		local_variable_array = NULL;
	} else {
		where = frame_get_method_name(frame);
		operand_stack = frame_get_operand_stack(frame);
		local_variable_array = frame_get_local_variable_array(frame);
	}

	return 0;
}

int mdb_uninsert_enabled_breakpoints() {
	if (breakpoints == NULL)
		return 0;

	return breakpoint_list_uninsert_enabled(breakpoints);
}

int mdb_insert_enabled_breakpoints() {
	if (breakpoints == NULL)
		return 0;

	return breakpoint_list_insert_enabled(breakpoints);
}

uint32_t mdb_hook(enum mdb_hook h) {
	int enter_cli, hits;
	struct breakpoint *b;
	uint32_t pc, retval, opcode;
	struct vm_stack *vm_stack;
	struct thread *thread;

	thread = thread_get_current();

	pc = thread_get_pc(thread);
	vm_stack = thread_get_vm_stack(thread);

	last_hook = h;
	enter_cli = 0;
	b = NULL;
	hits = 0;

	opcode = method_area_fetch(method_area, pc);
	retval = (h == execute_instruction_hook) ? opcode : 0;

	/* get current frame information */
	frame = vm_stack_peek(vm_stack);
	mdb_update_frame();

	/* inserted breakpoints have opcode 0
	 * or occur at entry point of native method
	 */
	if ((h == execute_instruction_hook && opcode == 0) ||
	    (h == enter_method_hook && frame != NULL &&
	     frame_get_start_address(frame) == NATIVE_ADDRESS)) {
		/* mark and print all hit breakpoints */
		if ((hits = breakpoint_list_mark_hits(breakpoints, pc, where)) > 0) {
			breakpoint_list_print_hits(breakpoints);
			enter_cli = 1;
		}

		/* if address breakpoint, grab actual opcode to execute */
		if ((b = breakpoint_list_find_by_address(breakpoints, pc)) != NULL)
			retval = b->opcode;
	}

	/* should we enter the cli? */
	switch (h) {
	case startup_hook:
		mode = startup_mode;
		enter_cli = 1;
		break;
	case enter_method_hook:
		if (mode == stepi_mode) {
			mdb_print_location(1);
			enter_cli = 1;
		}
		break;
	case leave_method_hook:
		if (mode == nexti_mode &&
		    vm_stack_size(vm_stack) <= nexti_mode_stack_size) {
			if (vm_stack_size(vm_stack) == nexti_mode_stack_size)
				mdb_print_location(0);
			else
				mdb_print_location(1);
			enter_cli = 1;
		} else if (mode == finish_mode &&
			   vm_stack_size(vm_stack) < finish_mode_stack_size) {
			mdb_print_location(1);
			enter_cli = 1;
		}
		break;
	case execute_instruction_hook:
		if (mode == until_mode && pc == until_mode) {
			mdb_print_location(1);
			enter_cli = 1;
		}
		else if (mode == stepi_mode ||
		    (mode == nexti_mode &&
		     vm_stack_size(vm_stack) <= nexti_mode_stack_size)) {
			mdb_print_location(0);
			enter_cli = 1;
		}
		break;
	case halt_vm_hook:
		enter_cli = 1;
		break;
	case terminate_hook:
		if (main_block_return_value == 0) {
			fprintf(stderr, "\nProgram exited normally.\n");
		} else {
			fprintf(stderr, "\nProgram exited with code %.2d.\n",
				main_block_return_value);
		}

		mode = terminated_mode;
		enter_cli = 1;
		break;
	}

	/* clear any hit marks */
	breakpoint_list_clear_mark(breakpoints, bp_hit);

	if (enter_cli)
		mdb_command_processor();

	return retval;
}

int mdb_command_processor() {
	int i, resume, n, index;
	char *line, **tokens;

	/* ignore SIGINT */
	mdb_sigint(SIG_IGN);

	do {
		if ((line = readline("(mdb) ")) == NULL)
			break;

		if (strlen(line) > 0 && strspn(line, " ") == strlen(line)) {
			free(line);
			continue;
		} else if (strlen(line) == 0) {
			free(line);
			if (repeat == 0 || last_line == NULL)
				continue;
			line = last_line;
		}

		add_history(line);
		tokens = mdb_tokenize(line, &n);

		resume = 0;
		if (mdb_command_is_ambiguous(tokens[0], commands) != 0)
			mdb_disambugiate_command(tokens[0], "", commands);
		else if ((index = mdb_command_find_index(tokens[0], commands)) == -1)
			fprintf(stderr, "Undefined command: \"%s\".  Try \"help\".\n", line);
		else
			resume = (*commands[index].function)(tokens, n); /* execute command */

		for (i = 0; i < n; i++)
			free(tokens[i]);
		free(tokens);
		last_line = line;
	} while (resume == 0);

	/* catch SIGINT */
	mdb_sigint(mdb_sigint_handler);

	return 0;
}
