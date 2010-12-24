%{

/* Niels Widger
 * Time-stamp: <23 Dec 2010 at 21:58:37 by nwidger on macros.local>
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

/* local prototypes */
static unsigned long a2int(char *);
static float a2float(char *tptr);

/* need to use global string pool from main.C */
extern int yyparse();

/* scanner tracks the current line number */
int line = 1;
int get_current_source_line_number();

/* following supports the stand-alone scanner program: lexdbg */
#ifdef DEBUG
#define token(x) (display_token((char *) # x),  x)
YYSTYPE yylval;
int display_token (char *);

#else

#define token(x) x

#endif

%}

%option noinput
%option nounput

letter          [A-Za-z_]
misc		[+*/!$-<>\[\]]
digit		[0-9]
label		{letter}({letter}|{digit}|{misc})*

%%

[#].*	                 {
			   /* do nothing: comments discarded */
			 }
			 
[ \t\f]                  {
                           /* do nothing: whitespace discarded */
                         }

[\r][\n]                 {
                           /* just advance the line number then discard newline */
                           line += 1;
			 }
			 
[\n]                     {
                           /* just advance the line number then discard newline */
                           line += 1;
                         }

^mainBlock[:]		 {
			   yytext[strlen(yytext)-1] = '\0';
			   return token(MAINBLOCKLABEL);
			 }

\$mainBlock		 {
			   yytext++;
			   return token(MAINBLOCKVARIABLE);
			 }
			 
aconst_null		 {
			   return token(ACONST_NULL);
			 }
			 
aload			 {
			   return token(ALOAD);
			 }
			 
areturn			 {
			   return token(ARETURN);
			 }
			 
astore			 {
			   return token(ASTORE);
			 }
			 
checkcast		 {
			   return token(CHECKCAST);
			 }
			 
delete			 {
			   return token(DELETE);
			 }
			 
dup			 {
			   return token(DUP);
			 }

dup_x1			 {
			   return token(DUP_X1);
			 }

getfield		 {
			   return token(GETFIELD);
			 }
			 
goto			 {
			   return token(GOTO);
			 }
			 
ifeq			 {
			   return token(IFEQ);
			 }
			 
in			 {
			   return token(IN);
			 }
			 
invokespecial		 {
			   return token(INVOKESPECIAL);
			 }
			 
invokenative		 {
			   return token(INVOKENATIVE);
			 }
			 
invokevirtual		 {
			   return token(INVOKEVIRTUAL);
			 }

monitorenter		 {
			   return token(MONITORENTER);
			 }

monitorexit		 {
			   return token(MONITOREXIT);
			 }

new			 {
			   return token(NEW);
			 }

newint			 {
			   return token(NEWINT);
			 }

newreal			 {
			   return token(NEWREAL);
			 }

newstr			 {
			   return token(NEWSTR);
			 }
			 
out			 {
			   return token(OUT);
			 }

pop      		 {
			   return token(POP);
			 }
			 
putfield		 {
			   return token(PUTFIELD);
			 }

refcmp			 {
			   return token(REFCMP);
			 }
			 
return			 {
			   return token(RETURN);
			 }

["]([^"])*["]            {
			   yytext[strlen(yytext)-1] = '\0';
			   yytext++;
			   return token(STRING_LITERAL);
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

\${label}		 {
			   yytext++;
			   return token(VARIABLE);
			 }

^{label}[:]		 {
			   yytext[strlen(yytext)-1] = '\0';
			   return token(LABEL);
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
    fprintf(stderr, "mas: integer literal too large\n");
    return 0;
  }

  // check now if value will fit in uint32_t
  uint32_t_tmp = uint64_t_tmp;
  uint64_t_tmp2 = uint32_t_tmp;
  if (uint32_t_tmp != uint64_t_tmp2)
  {
    fprintf(stderr, "mas: integer literal too large\n");
    return 0;
  }

  if (uint32_t_tmp < INTEGER_MIN_INTEGER) {
    fprintf(stderr, "mas: integer literal too small\n");
    return 0;
  }

  if (uint32_t_tmp > INTEGER_MAX_INTEGER) {
    fprintf(stderr, "mas: integer literal too large\n");
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
      fprintf(stderr, "mas: float literal out of range\n");
    }

    if (errno == EINVAL)
    {
      fprintf(stderr, "mas: float literal is invalid\n");
    }
    
    return 0;
  }

  return float_tmp;
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

