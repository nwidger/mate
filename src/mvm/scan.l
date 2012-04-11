%{

/* Niels Widger
 * Time-stamp: <10 Apr 2012 at 21:35:01 by nwidger on macros.local>
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <constants.h>

#include "parse.h"

// local prototypes
extern int yyparse();

// scanner tracks the current line number
static int line = 1;
int getCurrentSourceLineNumber();

// following supports the stand-alone scanner program: lexdbg
#ifdef DEBUG
#define token(x) (displayToken((char *) # x),  x)
YYSTYPE yylval;
int displayToken (char *);

#else

#define token(x) x

#endif

%}

%option nounput

letter          [A-Za-z_]
digit		[0-9]
identifier      {letter}({letter}|{digit})*

%%

[/][/].*                 {
			   // do nothing: comments discarded
			 }

[ \t\f]                  {
                           // do nothing: whitespace discarded
                         }

[\r][\n]                 {
                           // just advance the line number then discard newline
                           line += 1;
			 }

[\n]                     {
                           // just advance the line number then discard newline
                           line += 1;
                         }

native[:]({digit})+      {
			   yylval.value = atoi(yytext+7);
			   return token(NATIVE);
			 }

class                    {
                           return token(CLASS);
                         }

operator		 {
                           yylval.str = strdup(yytext);
			   return token(OPERATOR);
			 }

extends                  {
                           return token(EXTENDS);
                         }

{identifier}             {
                           yylval.str = strdup(yytext);
                           return token(IDENTIFIER);
                         }

[{]                      {
                           return token('{');
                         }

[}]                      {
                           return token('}');
                         }

[(]                      {
                           return token('(');
                         }

[)]                      {
                           return token(')');
                         }

[;]                      {
                           return token(';');
                         }

[>][=]			 {
			   yylval.str = strdup(yytext);
			   return token(GE_OP);
			 }

[<][=]			 {
			   yylval.str = strdup(yytext);
			   return token(LE_OP);
			 }

[>]			 {
			   yylval.str = strdup(yytext);
			   return token('>');
			 }

[<]			 {
			   yylval.str = strdup(yytext);
			   return token('<');
			 }

[,]                      {
                           return token(',');
                         }

[+]			 {
			   yylval.str = strdup(yytext);
			   return token('+');
			 }

[-]			 {
			   yylval.str = strdup(yytext);
			   return token('-');
			 }

[*]			 {
			   yylval.str = strdup(yytext);
			   return token('*');
			 }

[/]			 {
			   yylval.str = strdup(yytext);
			   return token('/');
			 }

[!]			 {
			   yylval.str = strdup(yytext);
			   return token('!');
			 }

.                        {
                           return token(BAD);
                         }

%%

int yywrap(void) {
  return 1;
}

/* provide the AST routines access to the line number */
int get_current_source_line_number()
{
  return line;
}

/* following supports the stand-alone scanner program: lexdbg */
#ifdef DEBUG

int main()
{
  while (yylex());

  return 0;
}

int display_token(char *p)
{
  fprintf(stderr, "%s is %s\n", p, yytext);
  return 1;
}
#endif

