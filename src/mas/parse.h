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
     ACONST_NULL = 264,
     ALOAD = 265,
     ARETURN = 266,
     ASTORE = 267,
     CHECKCAST = 268,
     DELETE = 269,
     DUP = 270,
     DUP_X1 = 271,
     GETFIELD = 272,
     GOTO = 273,
     IFEQ = 274,
     IN = 275,
     INVOKESPECIAL = 276,
     INVOKENATIVE = 277,
     INVOKEVIRTUAL = 278,
     MONITORENTER = 279,
     MONITOREXIT = 280,
     NEW = 281,
     NEWINT = 282,
     NEWSTR = 283,
     OUT = 284,
     POP = 285,
     PUTFIELD = 286,
     REFCMP = 287,
     RETURN = 288,
     BAD = 289
   };
#endif
/* Tokens.  */
#define VARIABLE 258
#define LABEL 259
#define MAINBLOCKVARIABLE 260
#define MAINBLOCKLABEL 261
#define STRING_LITERAL 262
#define INTEGER_LITERAL 263
#define ACONST_NULL 264
#define ALOAD 265
#define ARETURN 266
#define ASTORE 267
#define CHECKCAST 268
#define DELETE 269
#define DUP 270
#define DUP_X1 271
#define GETFIELD 272
#define GOTO 273
#define IFEQ 274
#define IN 275
#define INVOKESPECIAL 276
#define INVOKENATIVE 277
#define INVOKEVIRTUAL 278
#define MONITORENTER 279
#define MONITOREXIT 280
#define NEW 281
#define NEWINT 282
#define NEWSTR 283
#define OUT 284
#define POP 285
#define PUTFIELD 286
#define REFCMP 287
#define RETURN 288
#define BAD 289




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 23 "parse.y"
{
        unsigned long value;
	char *str;
}
/* Line 1489 of yacc.c.  */
#line 122 "parse.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

