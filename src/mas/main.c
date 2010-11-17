/* Niels Widger
 * Time-stamp: <29 Mar 2010 at 23:03:29 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "parse.h"
#include "passes.h"

#define OPTARGS ":sh"

/* extern declarations */
extern int line;
extern FILE *yyin;
extern int yylex();
extern char *yytext;
extern int yyparse();
#ifdef YYDEBUG
extern int yydebug;
#endif

/* forward declarations */
void usage();
void cleanup();
void get_input();
void verify_input();
void rewind_input();
int first_pass();
int second_pass();

/** prints usage text to stderr. */

void usage() {
	fprintf(stderr, "usage: mas [-h] [-s]\n");
	fprintf(stderr, "Reads maTe assembler from stdin and writes a maTe class file to stdout.\n");
	fprintf(stderr, "  -h  Print this help message and exit.\n");
	fprintf(stderr, "  -s  Dump the symbol table and exit.\n");
}

/** reads stdin to a temporary file to allow multiple passes.
 * After reading to temporary file, the stream is rewound.
 */

void get_input() {
	int n;
	uint8_t c;

	if ((yyin = tmpfile()) == NULL) {
		perror("mas: tmpfile");
		cleanup();
		exit(1);
	}

	while ((n = fread(&c, sizeof(c), 1, stdin)) > 0) {
		if (fwrite(&c, sizeof(c), 1, yyin) < 0) {
			perror("mas: fwrite");
			cleanup();
			exit(1);
		}
	}

	if (n < 0) {
		perror("mas: fread");
		cleanup();
		exit(1);
	}

	if (fflush(yyin) == EOF) {
		perror("mas: fflush");
		cleanup();
		exit(1);
	}

	rewind_input();
}

/** Verifies that the input is syntactically correct.
 */

void verify_input() {
	if (yyparse() != 0)
		exit(1);
}

/** Rewinds the input for the next pass.
 */

void rewind_input() {
	if (fseek(yyin, 0, SEEK_SET) < 0) {
		perror("mas: fseek");
		cleanup();
		exit(1);
	}

	line = 1;
}

/** calculates each symbol's address.
 *
 * @return: 0 on success, non-zero on failure
 */

int first_pass() {
	int token, err;
	
	err = 0;
	while ((token = yylex()) != 0) {
		switch (token) {
		case BAD:
			/* should never happen */
			fprintf(stderr, "mas: %d: parse error\n", line);
			err = 1;
			break;
		case MAINBLOCKLABEL:
		case LABEL:
			if (first_pass_process_label(line, yytext) != 0) err = 1;
			break;
		case INTEGER_LITERAL:
			if (first_pass_process_integer(line, yytext) != 0) err = 1;
			break;
		case STRING_LITERAL:
			if (first_pass_process_string(line, yytext) != 0) err = 1;
			break;
		case MAINBLOCKVARIABLE:
		case VARIABLE:
			if (first_pass_process_symbol(line, yytext) != 0) err = 1;
			break;
		default:
			if (first_pass_process_instruction(line, yytext) != 0) err = 1;
			break;
		}

		if (err == 1)
			return 1;
	}

	return 0;
}

/* writes binary class file.
 *
 * @return: 0 on success, non-zero on failure
 */

int second_pass() {
	int token, err;
	
	err = 0;
	while ((token = yylex()) != 0) {
		switch (token) {
		case BAD:
			/* should never happen */
			fprintf(stderr, "mas: %d: parse error\n", line);
			err = 1;
			break;
		case MAINBLOCKLABEL:
		case LABEL:
			/* do nothing */
			break;
		case INTEGER_LITERAL:
			if (second_pass_process_integer(line, yytext) != 0) err = 1;
			break;
		case STRING_LITERAL:
			if (second_pass_process_string(line, yytext) != 0) err = 1;
			break;
		case MAINBLOCKVARIABLE:
		case VARIABLE:
			if (second_pass_process_symbol(line, yytext) != 0) err = 1;
			break;
		default:
			if (second_pass_process_instruction(line, yytext) != 0) err = 1;
			break;
		}

		if (err == 1)
			return 1;
	}

	return 0;
}

/* closes temporary file.
 */

void cleanup() {
	if (yyin != NULL)
		fclose(yyin);
}

#ifndef YYDEBUG
int main(int argc, char *argv[]) {
	int c, dump_table;

	dump_table = 0;
	while ((c = getopt(argc, argv, OPTARGS)) != -1) {
		switch (c) {
		case 'h':
			usage();
			return 0;
		case 's':
			dump_table = 1;
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
	
	/* dump input to temporary file */
	get_input();

	/* verify correctness of input file */
	verify_input();

	/* rewind input file */
	rewind_input();
	
	/* first pass:
	 * verify correctness of input file
	 * add labels to symbol table
	 */
	if (first_pass_init() != 0 ||
	    first_pass() != 0 ||
	    first_pass_cleanup() != 0) {
		fprintf(stderr, "mas: error in first pass!\n");
		cleanup();		
		return 1;
	}

	/* dump symbol table */
	if (dump_table == 1) {
		dump_symbol_table();
		cleanup();
		return 0;
	}

	/* rewind input file */
	rewind_input();

	/* second pass:
	 * output binary file
	 * substitute instruction names for opcodes
	 * substitute symbols for addresses
	 * convert integer/string literals to their binary equivalents
	 */
	if (second_pass_init() != 0 ||
	    second_pass() != 0 ||
	    second_pass_cleanup() != 0) {
		fprintf(stderr, "mas: error in second pass!\n");
		cleanup();
		return 1;
	}

	/* close open files, destroy tables */
	cleanup();
	return 0;
}
#else
int main(int argc, char *argv[]) {
	yydebug = 1;

	if (yyparse() != 0)
		return 1;

	return 0;
}
#endif
