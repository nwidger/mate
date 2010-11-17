// Niels Widger
// Time-stamp: <22 Sep 2010 at 21:19:18 by nwidger on macros.local>

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
using namespace std;

#include "ClassTable.h"
#include "LabelStack.h"
#include "LocalVariableStack.h"
#include "Node.h"
#include "Seq.h"
#include "StringPool.h"
#include "TypeModule.h"

// forward declarations
extern int yyparse();
void usage();

#ifdef YYDEBUG
extern int yydebug;
#endif

// globals
StringPool *stringPool;
TypeModule *types;
LocalVariableStack *localVariableStack;
LabelStack *labelStack;
Seq *ast;

int labelCounter;
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

void usage() {
	fprintf(stderr, "usage: mc [-h] [-before] [-after] [-classes]\n");
	fprintf(stderr, "Reads maTe source code from stdin and writes maTe assembler to stdout.\n");
	fprintf(stderr, "  -h        Display this help message and exit.\n");
	fprintf(stderr, "  -before   Dump the AST before semantic analysis.\n");
	fprintf(stderr, "  -after    Dump the AST after semantic analysis.\n");
	fprintf(stderr, "  -classes  Dump the class table after semantic analysis.\n");
}

int main(int argc, char *argv[]) {
#ifdef YYDEBUG
	yydebug = 1;
#endif
	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0) {
			usage();
			return 0;
		}
	}

	stringPool = new StringPool();
	types = new TypeModule();
	localVariableStack = new LocalVariableStack(0);
	labelStack = new LabelStack(whileLabel, 0);

	labelCounter = 0;

	if (yyparse() != 0) {
		delete ast;
		delete labelStack;
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
	delete localVariableStack;
	delete types;
	delete stringPool;

	return (err == true) ? 1 : 0;
}
