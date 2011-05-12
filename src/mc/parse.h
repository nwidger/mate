
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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
     NATIVE = 262,
     INTEGER_LITERAL = 263,
     FLOAT_LITERAL = 264,
     NULL_LITERAL = 265,
     OPERATOR = 266,
     EQ_OP = 267,
     NE_OP = 268,
     INSTANCEOF_OP = 269,
     GE_OP = 270,
     LE_OP = 271,
     BOOL_AND = 272,
     BOOL_OR = 273,
     MAIN = 274,
     CLASS = 275,
     EXTENDS = 276,
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
#define NATIVE 262
#define INTEGER_LITERAL 263
#define FLOAT_LITERAL 264
#define NULL_LITERAL 265
#define OPERATOR 266
#define EQ_OP 267
#define NE_OP 268
#define INSTANCEOF_OP 269
#define GE_OP 270
#define LE_OP 271
#define BOOL_AND 272
#define BOOL_OR 273
#define MAIN 274
#define CLASS 275
#define EXTENDS 276
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
{

/* Line 1676 of yacc.c  */
#line 33 "parse.yy"

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



/* Line 1676 of yacc.c  */
#line 155 "parse.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
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

