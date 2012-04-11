%{

/* Niels Widger
 * Time-stamp: <10 Apr 2012 at 22:12:40 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define YYERROR_VERBOSE 1

// function declarations
extern int yyparse();
extern int get_current_source_line_number();
extern int yylex();
void yyerror(const char *s);

struct native_class {
        char *name;
	char *extends;
	struct native_method *methods;
	
	struct native_class *next;
};

struct native_method {
        unsigned long index;
	char *name;
	struct native_method *next;
};

struct native_class * native_class_create() {
        struct native_class *c;

	if ((c = (struct native_class *)malloc(sizeof(struct native_class))) == NULL) {
	        perror("mvm: malloc");
		return NULL;
	}

	return c;
}

struct native_method * native_method_create() {
        struct native_method *c;

	if ((c = (struct native_method *)malloc(sizeof(struct native_method))) == NULL) {
	        perror("mvm: malloc");
		return NULL;
	}

	return c;
}

%}

%union {
        unsigned long value;
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

%start ClassDeclarations

%%

ClassDeclarations
	: ClassDeclarations ClassDeclaration
	{
	  $$ = $1;
	}
	| ClassDeclaration
	{
	  $$ = $1;
	}
	;

ClassDeclaration
	: CLASS IDENTIFIER '{' ClassBodyDeclarations '}'
	{
	  $$ = native_class_create();

	  $$->name = $2;
	  $$->extends = NULL;
	  
	  $$->methods = $4;

	  free($2);
	}
	| CLASS IDENTIFIER '{' '}'
	{
	  $$ = native_class_create();

	  $$->name = $2;
	  $$->extends = NULL;
	  
	  $$->methods = NULL;

	  free($2);
	}
	| CLASS IDENTIFIER EXTENDS IDENTIFIER '{' ClassBodyDeclarations '}'
	{
	  $$ = native_class_create();

	  $$->name = $2;
	  $$->extends = $4;
	  
	  $$->methods = $6;

	  free($2);
	  free($4);
	}
	| CLASS IDENTIFIER EXTENDS IDENTIFIER '{' '}'
	{
	  $$ = native_class_create();

	  $$->name = $2;
	  $$->extends = $4;
	  
	  $$->methods = NULL;

	  free($2);
	  free($4);
	}
	;

ClassBodyDeclarations
	: ClassBodyDeclarations ClassBodyDeclaration
	{
	  $$ = $1;
	  $$->next = $2;
	}
	| ClassBodyDeclaration
	{
	  $$ = $1;
	  $$->next = NULL;
	}
	;

ClassBodyDeclaration
        : NATIVE IDENTIFIER FormalParameters ';'
	{
          /* constructor */
	  int len;

	  $$ = native_method_create();

	  $$->index = $1;

	  len = strlen($2)+strlen("_constructor");

	  if (strcmp($3, "") != 0)
	    len += strlen("$")+strlen($3);

          $$->name = malloc(sizeof(char)*len);
	  $$->name[0] = '\0';

	  strcat($$->name, $2);
	  strcat($$->name, "_constructor");

	  if (strcmp($3, "") != 0) {
            strcat($$->name, "$");	
            strcat($$->name, $3);
          }

	  free($2);
	  free($3);
	}
        | NATIVE IDENTIFIER IDENTIFIER FormalParameters ';'
	{
          /* method */
	  int len;

	  $$ = native_method_create();

	  $$->index = $1;

	  len = strlen($2)+strlen("$")+strlen($3);

	  if (strcmp($4, "") != 0)
	    len += strlen("$")+strlen($4);
	    
	  $$->name = malloc(sizeof(char)*len);
	  $$->name[0] = '\0';

	  strcat($$->name, $2);
	  strcat($$->name, "$");
	  strcat($$->name, $3);

	  if (strcmp($4, "") != 0) {
            strcat($$->name, "$");	
            strcat($$->name, $4);
          }

	  free($2);
	  free($3);
	  free($4);
	}
	| NATIVE IDENTIFIER OperatorDeclarator ';'
	{
          /* operator */
	  int len;
	  
	  $$ = native_method_create();

	  $$->index = $1;

	  len = strlen($2)+strlen("$")+strlen($3);
	  
	  $$->name = malloc(sizeof(char)*len);
	  $$->name[0] = '\0';

	  strcat($$->name, $2);
	  strcat($$->name, "$");
	  strcat($$->name, $3);

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
	  $$ = malloc(sizeof(char)*(strlen($1)+strlen("$")+strlen($3)));

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
	  $$ = malloc(sizeof(char)*(strlen($1)+strlen($2)+strlen("$")+strlen($4)));

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
	  $$ = malloc(sizeof(char)*(strlen($1)+strlen($2)+strlen("$")+strlen($4)));

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
	  $$ = malloc(sizeof(char)*(strlen($1)+strlen($2)));

	  $$[0] = '\0';

	  strcat($$, $1);
	  strcat($$, $2);

	  free($1);
	  free($2);
	}
	| OPERATOR MinusClassOperator '(' ')'
	{
	  $$ = malloc(sizeof(char)*(strlen($1)+strlen($2)));

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
