// Niels Widger
// Time-stamp: <06 Mar 2012 at 19:41:03 by nwidger on macros.local>

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
using namespace std;

#include "ClassTable.h"
#include "LabelStack.h"
#include "LocalVariableStack.h"
#include "MonitorStack.h"
#include "Node.h"
#include "Seq.h"
#include "StringPool.h"
#include "TypeModule.h"

// forward declarations
extern int yyparse();
extern FILE *yyin;
int getInput(char *predefinedFile);
void usage();

#ifdef YYDEBUG
extern int yydebug;
#endif

// globals
StringPool *stringPool;
TypeModule *types;
LocalVariableStack *localVariableStack;
LabelStack *labelStack;
MonitorStack *monitorStack;
Seq *ast;

int labelCounter;
int predefinedLines;
int synchronizedCounter;
const char *predefinedFileDefault = DATA_PATH "/predefined_classes.m";
const char *whileLabel = "W.";
const char *label = "L.";

string mainEntryLabel = string("mainBlock");
string mainExitLabel = string("mainBlock$.exit");

string objectName = string("Object");

bool analyzingMainBlock = false;
bool analyzingClassDec = false;
bool analyzingClassConst = false;
bool analyzingClassMeth = false;
bool analyzingLocalVarDec = false;

MainBlockNode * mainBlockAnalyzing = 0;
ClassMethodNode * methodAnalyzing = 0;
ClassConstructorNode * constructorAnalyzing = 0;

bool err = false;

int getInput(char *predefinedFile) {
	int n;
	uint8_t c;
	FILE *tmp, *predefined;

	predefinedLines = 0;

	if ((predefined = fopen(predefinedFile, "r")) == NULL) {
		fprintf(stderr, "mc: %s: %s\n", predefinedFile, strerror(errno));
		return 1;
	}

	if ((tmp = tmpfile()) == NULL) {
		perror("mc: tmpfile");
		return 1;
	}

	while ((n = fread(&c, sizeof(c), 1, predefined)) > 0) {
		if (c == '\n') predefinedLines++;

		if (fwrite(&c, sizeof(c), 1, tmp) < 0) {
			perror("mc: fwrite");
			return 1;
		}
	}

	while ((n = fread(&c, sizeof(c), 1, stdin)) > 0) {
		if (fwrite(&c, sizeof(c), 1, tmp) < 0) {
			perror("mc: fwrite");
			return 1;
		}
	}

	if (n < 0) {
		perror("mc: fread");
		return 1;
	}

	if (fflush(yyin) == EOF) {
		perror("mc: fflush");
		return 1;
	}

	if (fseek(tmp, 0, SEEK_SET) < 0) {
		perror("mc: fseek");
		return 1;
	}

	yyin = tmp;

	return 0;
}

void usage() {
	fprintf(stderr, "usage: mc [-h] [-before] [-after] [-classes]\n");
	fprintf(stderr, "Reads maTe source code from stdin and writes maTe assembler to stdout.\n");
	fprintf(stderr, "  -h        Display this help message and exit.\n");
	fprintf(stderr, "  -p FILE   Specify alternate location of predefined classes file\n");
	fprintf(stderr, "            Defaults to \"%s\".\n",
		predefinedFileDefault);
	fprintf(stderr, "  -before   Dump the AST before semantic analysis.\n");
	fprintf(stderr, "  -after    Dump the AST after semantic analysis.\n");
	fprintf(stderr, "  -classes  Dump the class table after semantic analysis.\n");
}

int main(int argc, char *argv[]) {
	char *predefinedFile;

	predefinedFile = (char *)predefinedFileDefault;
	
#ifdef YYDEBUG
	yydebug = 1;
#endif
	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0) {
			usage();
			return 0;
		} else if (strcmp(argv[i], "-p") == 0) {
			if (i+1 >= argc) {
				cerr << "mc: No filename argument for -p switch\n";
				usage();
				return 0;
			}
			
			predefinedFile = argv[++i];
		}
	}

	stringPool = new StringPool();
	types = new TypeModule();
	localVariableStack = new LocalVariableStack(0);
	labelStack = new LabelStack(whileLabel, 0);
	monitorStack = new MonitorStack();

	labelCounter = 0;
	synchronizedCounter = 0;

	if (getInput(predefinedFile) != 0 || yyparse() != 0) {
		delete ast;
		delete labelStack;
		delete monitorStack;
		delete localVariableStack;
		delete types;
		delete stringPool;
		
		return 1;
	}

	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-before") == 0) {
			cerr << '\n';
			ast->dump();
		}
	}
	
	types->classTable()->analyze(ast);
	ast = ast->analyze(0);

	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-after") == 0) {
			cerr << '\n';
			ast->dump();
		}		
		if (strcmp(argv[i], "-classes") == 0) {
			cerr << '\n';
			types->classTable()->dump();
		}
	}

	if (err == false)
		ast->encode();

	delete ast;
	delete labelStack;
	delete monitorStack;
	delete localVariableStack;
	delete types;
	delete stringPool;

	return (err == true) ? 1 : 0;
}
