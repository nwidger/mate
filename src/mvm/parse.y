%{

/* Niels Widger
 * Time-stamp: <19 Apr 2012 at 19:44:49 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "native_methods.h"

#define YYERROR_VERBOSE 1

// function declarations
extern int yyparse();
extern int get_current_source_line_number();
extern int yylex();
void yyerror(const char *s);

extern int max_native_index;
extern struct native_class *native_classes;

%}

%union {
        int value;
	char *str;

	struct native_class *nclass;
	struct native_method *nmethod;
};

%error-verbose

%token <str> IDENTIFIER
%token <value> NATIVE
%token <str> OPERATOR

%token <str> GE_OP
%token <str> LE_OP
%token <str> '+'
%token <str> '>'
%token <str> '-'
%token <str> '!'
%token <str> '/'
%token <str> '<'
%token <str> '*'

%token CLASS EXTENDS

%token BAD

%type <nclass> CompilationUnit
%type <nclass> ClassDeclarations
%type <nclass> ClassDeclaration

%type <nmethod> ClassBodyDeclarations
%type <nmethod> ClassBodyDeclaration

%type <str> OperatorDeclarator

%type <str> FormalParameters
%type <str> FormalParameterList

%type <str> UnaryClassOperator
%type <str> BinaryClassOperator
%type <str> MinusClassOperator

%start CompilationUnit

%%

CompilationUnit
        : ClassDeclarations
	{
	  native_classes = $$;
	}
	;

ClassDeclarations
	: ClassDeclaration ClassDeclarations
	{
	  $$ = $1;
	  native_class_set_next($$, $2);
	}
	| ClassDeclaration
	{
	  $$ = $1;
	  native_class_set_next($$, NULL);
	}
	;

ClassDeclaration
	: CLASS IDENTIFIER '{' ClassBodyDeclarations '}'
	{
	  $$ = native_class_create($2, $4, NULL);
	}
	| CLASS IDENTIFIER '{' '}'
	{
          $$ = native_class_create($2, NULL, NULL);
	  free($2);
	}
	| CLASS IDENTIFIER EXTENDS IDENTIFIER '{' ClassBodyDeclarations '}'
	{
	  $$ = native_class_create($2, $6, NULL);
	  free($4);
	}
	| CLASS IDENTIFIER EXTENDS IDENTIFIER '{' '}'
	{
	  $$ = native_class_create($2, NULL, NULL);
	  free($4);
	}
	;

ClassBodyDeclarations
	: ClassBodyDeclaration ClassBodyDeclarations
	{
	  $$ = $1;
	  native_method_set_next($$, $2);
	}
	| ClassBodyDeclaration
	{
	  $$ = $1;
	  native_method_set_next($$, NULL);
	}
	;

ClassBodyDeclaration
        : NATIVE IDENTIFIER FormalParameters ';'
	{
          /* constructor */
	  int len;
	  char *name;

	  len = strlen($2)+strlen("_constructor");

	  if (strcmp($3, "") != 0)
	    len += strlen("$")+strlen($3);

          name = malloc(sizeof(char)*(len+2));
	  name[0] = '\0';

	  strcat(name, $2);
	  strcat(name, "_constructor");

	  if (strcmp($3, "") != 0) {
            strcat(name, "$");	
            strcat(name, $3);
          }

	  $$ = native_method_create($1, name, NULL);

	  if ($1 > max_native_index)
	    max_native_index = $1;

	  free($2);
	  free($3);
	}
        | NATIVE IDENTIFIER IDENTIFIER FormalParameters ';'
	{
          /* method */
	  int len;
	  char *name;

	  len = strlen($3);

	  if (strcmp($4, "") != 0)
	    len += strlen("$")+strlen($4);
	    
	  name = malloc(sizeof(char)*(len+2));
	  name[0] = '\0';

	  strcat(name, $3);

	  if (strcmp($4, "") != 0) {
            strcat(name, "$");	
            strcat(name, $4);
          }

	  $$ = native_method_create($1, name, NULL);

	  if ($1 > max_native_index)
	    max_native_index = $1;

	  free($2);
	  free($3);
	  free($4);
	}
	| NATIVE IDENTIFIER OperatorDeclarator ';'
	{
          /* operator */
	  int len;
	  char *name;

	  len = strlen($3);
	  
	  name = malloc(sizeof(char)*(len+2));
	  name[0] = '\0';

	  strcat(name, $3);

  	  $$ = native_method_create($1, name, NULL);

	  if ($1 > max_native_index)
	    max_native_index = $1;

	  free($2);
	  free($3);
	}
	;

FormalParameters
	: '(' FormalParameterList ')'
	{
	  $$ = $2;
	}
	| '(' ')'
	{
	  $$ = strdup("");
	}
	;

FormalParameterList
	: FormalParameterList ',' IDENTIFIER IDENTIFIER
	{
	  $$ = malloc(sizeof(char)*(strlen($1)+strlen("$")+strlen($3)+2));

	  $$[0] = '\0';

	  strcat($$, $1);
	  strcat($$, "$");
	  strcat($$, $3);

	  free($1);
	  free($3);
	  free($4);
	}
	| IDENTIFIER IDENTIFIER
	{ 
	  $$ = $1;

	  free($2);
	}
	;

OperatorDeclarator
	: OPERATOR BinaryClassOperator '(' IDENTIFIER IDENTIFIER ')'
	{
	  $$ = malloc(sizeof(char)*(strlen($1)+strlen($2)+strlen("$")+strlen($4)+2));

	  $$[0] = '\0';

	  strcat($$, $1);
	  strcat($$, $2);
	  strcat($$, "$");
	  strcat($$, $4);

	  free($1);
	  free($2);
	  free($4);
	  free($5);	  
	}
	| OPERATOR MinusClassOperator '(' IDENTIFIER IDENTIFIER ')'
	{
	  $$ = malloc(sizeof(char)*(strlen($1)+strlen($2)+strlen("$")+strlen($4)+2));

	  $$[0] = '\0';

	  strcat($$, $1);
	  strcat($$, $2);
	  strcat($$, "$");
	  strcat($$, $4);

	  free($1);
	  free($2);
	  free($4);
	  free($5);	  
	}
	| OPERATOR UnaryClassOperator '(' ')'
	{
	  $$ = malloc(sizeof(char)*(strlen($1)+strlen($2)+2));

	  $$[0] = '\0';

	  strcat($$, $1);
	  strcat($$, $2);

	  free($1);
	  free($2);
	}
	| OPERATOR MinusClassOperator '(' ')'
	{
	  $$ = malloc(sizeof(char)*(strlen($1)+strlen($2)+2));

	  $$[0] = '\0';

	  strcat($$, $1);
	  strcat($$, $2);

	  free($1);
	  free($2);
	}
	;

UnaryClassOperator
	: '!'
	{
	  $$ = $1;
	}
	;

BinaryClassOperator
	: '+'
	{
	  $$ = $1;
	}
	| '*'
	{
	  $$ = $1;
	}
	| '/'
	{
	  $$ = $1;
	}
	| '>'
	{
	  $$ = $1;
	}
	| '<'
	{
	  $$ = $1;
	}
	| GE_OP
	{
	  $$ = $1;
	}
	| LE_OP
	{
	  $$ = $1;
	}
	;

MinusClassOperator
	: '-'
	{
	  $$ = $1;
	}
	;

%%


void yyerror(const char *s) {
        fprintf(stderr, "mvm: %d: %s\n", get_current_source_line_number(), s);
}
