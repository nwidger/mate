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
     IDENTIFIER = 258,
     STRING_LITERAL = 259,
     NEWLINE = 260,
     TAB = 261,
     INTEGER_LITERAL = 262,
     FLOAT_LITERAL = 263,
     NULL_LITERAL = 264,
     OPERATOR = 265,
     EQ_OP = 266,
     NE_OP = 267,
     INSTANCEOF_OP = 268,
     GE_OP = 269,
     LE_OP = 270,
     BOOL_AND = 271,
     BOOL_OR = 272,
     MAIN = 273,
     CLASS = 274,
     EXTENDS = 275,
     NATIVE = 276,
     THIS = 277,
     SUPER = 278,
     SYNCHRONIZED = 279,
     IF = 280,
     ELSE = 281,
     FOR = 282,
     WHILE = 283,
     NEW = 284,
     RETURN = 285,
     OUT = 286,
     IN = 287,
     BREAK = 288,
     CONTINUE = 289,
     BAD = 290
   };
#endif
/* Tokens.  */
#define IDENTIFIER 258
#define STRING_LITERAL 259
#define NEWLINE 260
#define TAB 261
#define INTEGER_LITERAL 262
#define FLOAT_LITERAL 263
#define NULL_LITERAL 264
#define OPERATOR 265
#define EQ_OP 266
#define NE_OP 267
#define INSTANCEOF_OP 268
#define GE_OP 269
#define LE_OP 270
#define BOOL_AND 271
#define BOOL_OR 272
#define MAIN 273
#define CLASS 274
#define EXTENDS 275
#define NATIVE 276
#define THIS 277
#define SUPER 278
#define SYNCHRONIZED 279
#define IF 280
#define ELSE 281
#define FOR 282
#define WHILE 283
#define NEW 284
#define RETURN 285
#define OUT 286
#define IN 287
#define BREAK 288
#define CONTINUE 289
#define BAD 290




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 34 "parse.yy"
{
	Seq *seq;

        unsigned long value;
        float float_value;

	string *str;
	char c;

	Type *type;

	Node *node;

	LocalVarDecNode *localVarDec;
	VarDecNode *varDec;
	VarNode *varNode;

	MainBlockNode *mainBlock;

	ClassDecNode *classDec;
	ClassMethodNode *classMeth;
	ClassConstructorNode *classConst;
	ClassFieldDecNode *classFieldDec;
	ClassFieldNode *classField;

	ExpressionNode *expr;
	StatementNode *stmnt;

	BlockStatementNode *blockStmnt;
}
/* Line 1489 of yacc.c.  */
#line 150 "parse.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYLTYPE yylloc;
