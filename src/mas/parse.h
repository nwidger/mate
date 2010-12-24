/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     VARIABLE = 258,
     LABEL = 259,
     MAINBLOCKVARIABLE = 260,
     MAINBLOCKLABEL = 261,
     STRING_LITERAL = 262,
     INTEGER_LITERAL = 263,
     FLOAT_LITERAL = 264,
     ACONST_NULL = 265,
     ALOAD = 266,
     ARETURN = 267,
     ASTORE = 268,
     CHECKCAST = 269,
     DELETE = 270,
     DUP = 271,
     DUP_X1 = 272,
     GETFIELD = 273,
     GOTO = 274,
     IFEQ = 275,
     IN = 276,
     INVOKESPECIAL = 277,
     INVOKENATIVE = 278,
     INVOKEVIRTUAL = 279,
     MONITORENTER = 280,
     MONITOREXIT = 281,
     NEW = 282,
     NEWINT = 283,
     NEWREAL = 284,
     NEWSTR = 285,
     OUT = 286,
     POP = 287,
     PUTFIELD = 288,
     REFCMP = 289,
     RETURN = 290,
     BAD = 291
   };
#endif
/* Tokens.  */
#define VARIABLE 258
#define LABEL 259
#define MAINBLOCKVARIABLE 260
#define MAINBLOCKLABEL 261
#define STRING_LITERAL 262
#define INTEGER_LITERAL 263
#define FLOAT_LITERAL 264
#define ACONST_NULL 265
#define ALOAD 266
#define ARETURN 267
#define ASTORE 268
#define CHECKCAST 269
#define DELETE 270
#define DUP 271
#define DUP_X1 272
#define GETFIELD 273
#define GOTO 274
#define IFEQ 275
#define IN 276
#define INVOKESPECIAL 277
#define INVOKENATIVE 278
#define INVOKEVIRTUAL 279
#define MONITORENTER 280
#define MONITOREXIT 281
#define NEW 282
#define NEWINT 283
#define NEWREAL 284
#define NEWSTR 285
#define OUT 286
#define POP 287
#define PUTFIELD 288
#define REFCMP 289
#define RETURN 290
#define BAD 291




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 23 "parse.y"
{
        unsigned long value;
	float float_value;
	char *str;
}
/* Line 1529 of yacc.c.  */
#line 127 "parse.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

