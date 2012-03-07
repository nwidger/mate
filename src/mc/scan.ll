%{

// Niels Widger
// Time-stamp: <06 Mar 2012 at 19:01:20 by nwidger on macros.local>

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <inttypes.h>
#include <stdint.h>
#include <string>
#include <iostream>
using namespace std;

#include <constants.h>

#include "Node.h"
#include "StringPool.h"
#include "parse.h"

// local prototypes
static unsigned long a2int(char *);
static float a2float(char *tptr);
static string * stashIdentifier(char *);
static string * stashStringLiteral(char *);

// need to use global string pool from main.C
extern StringPool * stringPool;
extern bool err;
extern int yyparse();
extern int predefinedLines;

// scanner tracks the current line number
static int sourceLineNumber = 1;
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
                           sourceLineNumber += 1;
			   yylloc.first_line = sourceLineNumber;			   
			 }
			 
[\n]                     {
                           // just advance the line number then discard newline
                           sourceLineNumber += 1;
			   yylloc.first_line = sourceLineNumber;
                         }
			 
native[:]({digit})+      {
			   yylval.value = a2int(yytext+7);
			   return token(NATIVE);
			 }

(([0-9])+)?\.([0-9])+ |
([0-9])+\.(([0-9])+)? |
(([0-9])+)?\.([0-9])+([eE][-+]?([0-9])+)? |
([0-9])+\.?[eE][-+]?([0-9])+ {
                           yylval.float_value = a2float(yytext);
                           return token(FLOAT_LITERAL);
                         }
			 
({digit})+               {
                           yylval.value = a2int(yytext);
                           return token(INTEGER_LITERAL);
                         }

synchronized             {
                           return token(SYNCHRONIZED);
                         }

null                     {
                           return token(NULL_LITERAL);
                         }

main                     {
                           return token(MAIN);
                         }

class                    {
                           return token(CLASS);
                         }

operator		 {
			   return token(OPERATOR);
			 }

extends                  {
                           return token(EXTENDS);
                         }

this                     {
                           return token(THIS);
                         }

super                    {
                           return token(SUPER);
                         }

if                       {
                           return token(IF);
                         }

else                     {
                           return token(ELSE);
                         }

for                      {
                           return token(FOR);
                         }
			 
while                    {
                           return token(WHILE);
                         }

instanceof               {
                           return token(INSTANCEOF_OP);
                         }
			 
newline             |
tab                 |
["]([^"\t\r\n])*["]      {
			   yylval.str = stashStringLiteral(yytext);
			   return token(STRING_LITERAL);
			 }
			 
new                      {
                           return token(NEW);
                         }

return                   {
                           return token(RETURN);
                         }

out                      {
                           return token(OUT);
                         }

break                    {
                           return token(BREAK);
                         }

continue                 {
                           return token(CONTINUE);
                         }			 			 
			 
in			 {
			   return token(IN);
			 }

{identifier}             {
                           yylval.str = stashIdentifier(yytext);
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

[&][&]                   {
                           return token(BOOL_AND);
                         }

[|][|]                   {
                           return token(BOOL_OR);
                         }
			 
[=][=]                   {
                           return token(EQ_OP);
                         }

[!][=]                   {
			   yylval.str = stringPool->getOpString("!");
                           return token(NE_OP);
                         }
			 
[>][=]			 {
			   yylval.str = stringPool->getOpString(yytext);
			   return token(GE_OP);
			 }

[<][=]			 {
			   yylval.str = stringPool->getOpString(yytext);
			   return token(LE_OP);
			 }			 
			 
[>]			 {
			   yylval.str = stringPool->getOpString(yytext);
			   return token('>');
			 }

[<]			 {
			   yylval.str = stringPool->getOpString(yytext);
			   return token('<');
			 }			 
			 
[=]			 {
			   return token('=');
			 }

[~]                      {
                           return token('~');
                         }

[,]                      {
                           return token(',');
                         }

[+]			 {
			   yylval.str = stringPool->getOpString(yytext);
			   return token('+');
			 }

[-]			 {
			   yylval.str = stringPool->getOpString(yytext);
			   return token('-');
			 }			 

[*]			 {
			   yylval.str = stringPool->getOpString(yytext);
			   return token('*');
			 }

[/]			 {
			   yylval.str = stringPool->getOpString(yytext);
			   return token('/');
			 }

[!]			 {
			   yylval.str = stringPool->getOpString(yytext);
			   return token('!');
			 }

\.			 {
			   return token('.');
			 }			 			 			 
			 
.                        {
                           return token(BAD);
                         }

%%

int yywrap(void) {
  return 1;
}


/*
 * Convert from ascii hex to an integer.
 */
static unsigned long a2int(char *tptr)
{
  uint64_t uint64_t_tmp;
  uint32_t uint32_t_tmp;
  uint64_t uint64_t_tmp2;  

  // errno used to detect overflow of long long
  errno = 0;
  uint64_t_tmp = strtoull(tptr, NULL, 0);
  if (errno)
  {
    cerr << sourceLineNumber << ": integer literal too large\n";
    err = true;
    return 0;
  }

  // check now if value will fit in uint32_t
  uint32_t_tmp = uint64_t_tmp;
  uint64_t_tmp2 = uint32_t_tmp;
  if (uint32_t_tmp != uint64_t_tmp2)
  {
    cerr << sourceLineNumber << ": integer literal too large\n";
    err = true;
    return 0;
  }

  if (uint32_t_tmp < INTEGER_MIN_INTEGER) {
    cerr << sourceLineNumber << ": integer literal too small\n";
    err = true;
    return 0;
  }

  if (uint32_t_tmp > INTEGER_MAX_INTEGER) {
    cerr << sourceLineNumber << ": integer literal too large\n";
    err = true;
    return 0;
  }

  return uint32_t_tmp;
}

/*
 * Convert from ascii hex to a float.
 */
static float a2float(char *tptr)
{
  float float_tmp;

  // errno used to detect overflow/underflow of float
  errno = 0;
  float_tmp = strtof(tptr, NULL);
  if (errno)
  {
    if (errno == ERANGE)
    {
      cerr << sourceLineNumber << ": float literal out of range\n";
    }

    if (errno == EINVAL)
    {
      cerr << sourceLineNumber << ": float literal is invalid\n";
    }
    
    return 0;
  }

  return float_tmp;
}

// identifier is now in yytext, which will soon be re-used
// so allocate a safe place for the string in the string pool
static string * stashIdentifier(char *str)
{
  string *tmp;
  string s = string(str);

  tmp = stringPool->newIdentifier(s);

  return tmp;
}

// string lit is now in yytext, which will soon be re-used
// so allocate a safe place for the string in the string pool
static string * stashStringLiteral(char *str)
{
  return stringPool->newStringLiteral(str);
}

// provide the AST routines access to the line number
int getCurrentSourceLineNumber()
{
  return sourceLineNumber - predefinedLines;
}

// following supports the stand-alone scanner program: lexdbg
#ifdef DEBUG
StringPool* stringPool;

YYLTYPE yylloc;

int main()
{
  stringPool = new StringPool();

  while (yylex());

  delete stringPool;

  return 0;
}

int displayToken(char *p)
{
  cout << p << " is " << yytext << '\n';
  return 1;
}
#endif

