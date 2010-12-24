%{

// Niels Widger
// CS 712
// Time-stamp: <23 Dec 2010 at 20:52:52 by nwidger on macros.local>

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>

#define YYERROR_VERBOSE 1

// function declarations
extern int yyparse();
extern int get_current_source_line_number();
extern int yylex();
void yyerror(const char *s);

%}

%union {
        unsigned long value;
	float float_value;
	char *str;
};

%token <str> VARIABLE
%token <str> LABEL
%token <str> MAINBLOCKVARIABLE
%token <str> MAINBLOCKLABEL
%token <str> STRING_LITERAL
%token <value> INTEGER_LITERAL
%token <float_value> FLOAT_LITERAL

%token ACONST_NULL
%token ALOAD
%token ARETURN
%token ASTORE
%token CHECKCAST
%token DELETE
%token DUP
%token DUP_X1
%token GETFIELD
%token GOTO
%token IFEQ
%token IN
%token INVOKESPECIAL
%token INVOKENATIVE
%token INVOKEVIRTUAL
%token MONITORENTER
%token MONITOREXIT
%token NEW
%token NEWINT
%token NEWREAL
%token NEWSTR
%token OUT
%token POP
%token PUTFIELD
%token REFCMP
%token RETURN

%token BAD

%start ClassFile

%%

ClassFile
	: MainBlockDeclaration ClassTable MainBlockBody ClassBodies
	{
	}
	| MainBlockDeclaration ClassTable MainBlockBody
	{
	}
	;

MainBlockDeclaration
	: MAINBLOCKVARIABLE INTEGER_LITERAL
	{
	}
	;

ClassTable
	: INTEGER_LITERAL ClassNames ClassDeclarations
	{
	}
	;

ClassNames
	: ClassNames ClassName
	{
	}
	| ClassName
	{
	}
	;

ClassName
	: VARIABLE STRING_LITERAL
	{
	}
	;

ClassDeclarations
	: ClassDeclarations ClassDeclaration
	{
	}
	| ClassDeclaration
	{
	}
	;

ClassDeclaration
	: LABEL SuperClass Fields Methods
	{
	}
	;

SuperClass
	: VARIABLE
	{
	}
	| INTEGER_LITERAL
	{
	}
	;
	
Fields
	: INTEGER_LITERAL
	{
	}
	;

Methods
	: INTEGER_LITERAL MethodDeclarations
	{
	}
	;

MethodDeclarations
	: MethodDeclarations MethodDeclaration
	{
	}
	| MethodDeclaration
	{
	}
	;

MethodDeclaration
	: VARIABLE INTEGER_LITERAL STRING_LITERAL
	{
	}
	| INTEGER_LITERAL INTEGER_LITERAL STRING_LITERAL
	;
	
ClassBodies
	: ClassBodies ClassBody
	{
	}
	| ClassBody
	{
	}
	;

ClassBody
	: LABEL Instructions
	{
	}
	| LABEL
	{
	}
	;

MainBlockBody
	: MAINBLOCKLABEL Instructions
	{
	}
	| MAINBLOCKLABEL
	;

Instructions
	: Instructions Instruction
	{
	}
	| Instruction
	{
	}
	;

Instruction
	: ACONST_NULL
	{
	}
	| ALOAD INTEGER_LITERAL
	{
	}
	| ARETURN
	{
	}
	| ASTORE INTEGER_LITERAL
	{
	}
	| CHECKCAST VARIABLE
	{
	}
	| DELETE
	{
	}
	| DUP
	{
	}
	| DUP_X1
	{
	}
	| GETFIELD INTEGER_LITERAL
	{
	}
	| GOTO VARIABLE
	{
	}
	| IFEQ VARIABLE
	{
	}
	| IN
	{
	}
	| INVOKESPECIAL VARIABLE INTEGER_LITERAL INTEGER_LITERAL
	{
	}
	| INVOKENATIVE INTEGER_LITERAL
	{
	}
	| INVOKEVIRTUAL INTEGER_LITERAL INTEGER_LITERAL
	{
	}
	| MONITORENTER
	{
	}
	| MONITOREXIT
	{
	}
	| NEW VARIABLE
	{
	}
	| NEWINT INTEGER_LITERAL
	{
	}
	| NEWREAL FLOAT_LITERAL
	{
	}
	| NEWSTR STRING_LITERAL
	{
	}
	| OUT
	{
	}
	| POP
	{
	}
	| PUTFIELD INTEGER_LITERAL
	{
	}
	| REFCMP
	{
	}
	| RETURN
	{
	}
	;
	
%%

void yyerror(const char *s) {
        fprintf(stderr, "mas: %d: %s\n", get_current_source_line_number(), s);
}
