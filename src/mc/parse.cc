/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 1



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




/* Copy the first part of user declarations.  */
#line 1 "parse.yy"


// Niels Widger
// CS 712
// Time-stamp: <30 Sep 2012 at 15:34:00 by nwidger on macros.local>

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <string>
#include <iostream>
using namespace std;

#define YYERROR_VERBOSE 1

#include "TypeModule.h"
#include "Type.h"
#include "Seq.h"
#include "Node.h"

// function declarations
extern int yyparse();
extern int getCurrentSourceLineNumber();
extern int yylex();
void yyerror(const char *s);

extern int nativeIndex;
extern Seq *ast;
extern TypeModule *types;



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

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
/* Line 187 of yacc.c.  */
#line 230 "parse.cc"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

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


/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 255 "parse.cc"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
    YYLTYPE yyls;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  12
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   647

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  75
/* YYNRULES -- Number of rules.  */
#define YYNRULES  156
/* YYNRULES -- Number of states.  */
#define YYNSTATES  260

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   290

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    21,     2,     2,     2,     2,     2,     2,
      43,    44,    24,    18,    48,    20,    50,    22,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    47,
      23,    49,    19,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    45,     2,    46,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    11,    15,    21,    24,    26,
      30,    36,    40,    43,    46,    48,    50,    52,    54,    56,
      58,    62,    67,    71,    74,    76,    81,    85,    91,    97,
     102,   107,   109,   113,   116,   119,   124,   128,   130,   134,
     138,   142,   145,   149,   151,   154,   156,   160,   163,   165,
     168,   170,   178,   185,   187,   189,   191,   193,   195,   197,
     199,   201,   203,   205,   207,   209,   211,   214,   217,   221,
     223,   225,   229,   235,   245,   247,   248,   250,   251,   253,
     254,   258,   261,   265,   269,   272,   275,   277,   281,   284,
     286,   288,   290,   292,   294,   298,   300,   302,   306,   310,
     312,   316,   320,   322,   326,   328,   332,   336,   340,   344,
     346,   350,   354,   356,   360,   364,   366,   369,   372,   374,
     377,   379,   381,   383,   385,   387,   389,   391,   393,   395,
     397,   401,   403,   405,   407,   411,   415,   418,   423,   428,
     432,   435,   439,   441,   443,   445,   447,   449,   451,   453,
     455,   457,   459,   461,   463,   465,   467
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      52,     0,    -1,    53,    -1,    53,    54,    -1,    54,    53,
      -1,    54,    53,    54,    -1,   114,    25,    43,    44,    75,
      -1,    54,    55,    -1,    55,    -1,    26,   121,    56,    -1,
      26,   121,    27,   116,    56,    -1,    45,    57,    46,    -1,
      45,    46,    -1,    57,    58,    -1,    58,    -1,    59,    -1,
      67,    -1,    60,    -1,    61,    -1,    64,    -1,   114,    82,
      47,    -1,    28,   114,    62,    47,    -1,   114,    62,    63,
      -1,   121,    71,    -1,    75,    -1,    28,   114,    65,    47,
      -1,   114,    65,    66,    -1,    10,   124,    43,    73,    44,
      -1,    10,   125,    43,    73,    44,    -1,    10,   123,    43,
      44,    -1,    10,   125,    43,    44,    -1,    75,    -1,    28,
      68,    47,    -1,    68,    69,    -1,   121,    71,    -1,    45,
      70,    77,    46,    -1,    45,    70,    46,    -1,    75,    -1,
      29,   119,    47,    -1,    30,   119,    47,    -1,    43,    72,
      44,    -1,    43,    44,    -1,    72,    48,    73,    -1,    73,
      -1,   114,    74,    -1,   121,    -1,    45,    77,    46,    -1,
      45,    46,    -1,    79,    -1,    77,    76,    -1,    76,    -1,
      31,    43,    99,    44,    45,    77,    46,    -1,    31,    43,
      99,    44,    45,    46,    -1,    75,    -1,    78,    -1,    95,
      -1,    80,    -1,    97,    -1,    84,    -1,    85,    -1,    90,
      -1,    86,    -1,    91,    -1,    92,    -1,    93,    -1,    94,
      -1,    81,    47,    -1,   114,    82,    -1,    82,    48,    83,
      -1,    83,    -1,   121,    -1,    32,    96,    79,    -1,    32,
      96,    79,    33,    79,    -1,    34,    43,    87,    47,    88,
      47,    89,    44,    79,    -1,    98,    -1,    -1,    99,    -1,
      -1,    98,    -1,    -1,    35,    96,    79,    -1,    37,    47,
      -1,    37,    99,    47,    -1,    38,    99,    47,    -1,    40,
      47,    -1,    41,    47,    -1,    47,    -1,    43,    99,    44,
      -1,    98,    47,    -1,   101,    -1,   118,    -1,   100,    -1,
     101,    -1,   103,    -1,   102,    49,   100,    -1,   121,    -1,
     117,    -1,   104,    16,   103,    -1,   104,    17,   103,    -1,
     104,    -1,   104,    11,   105,    -1,   104,    12,   105,    -1,
     105,    -1,   105,    13,   116,    -1,   106,    -1,   106,    23,
     107,    -1,   106,    19,   107,    -1,   106,    14,   107,    -1,
     106,    15,   107,    -1,   107,    -1,   107,    18,   108,    -1,
     107,    20,   108,    -1,   108,    -1,   108,    24,   109,    -1,
     108,    22,   109,    -1,   109,    -1,    20,   109,    -1,    21,
     109,    -1,   110,    -1,    96,   110,    -1,   111,    -1,   121,
      -1,    96,    -1,    29,    -1,   117,    -1,   118,    -1,   113,
      -1,   122,    -1,   112,    -1,    39,    -1,    36,   116,   119,
      -1,   115,    -1,   116,    -1,   121,    -1,   111,    50,   121,
      -1,    30,    50,   121,    -1,   121,   119,    -1,   111,    50,
     121,   119,    -1,    30,    50,   121,   119,    -1,    43,   120,
      44,    -1,    43,    44,    -1,   120,    48,    99,    -1,    99,
      -1,     3,    -1,     7,    -1,     8,    -1,     9,    -1,     4,
      -1,    21,    -1,    18,    -1,    24,    -1,    22,    -1,    19,
      -1,    23,    -1,    14,    -1,    15,    -1,    20,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   209,   209,   214,   219,   225,   233,   245,   249,   256,
     261,   270,   274,   281,   285,   292,   296,   303,   307,   311,
     318,   326,   334,   344,   351,   358,   366,   376,   380,   384,
     388,   395,   402,   408,   416,   424,   428,   432,   440,   444,
     451,   455,   462,   466,   473,   481,   489,   493,   500,   507,
     511,   518,   522,   529,   533,   537,   541,   545,   550,   554,
     558,   562,   566,   570,   574,   578,   585,   592,   600,   604,
     611,   619,   627,   635,   643,   649,   655,   660,   666,   672,
     678,   686,   690,   697,   704,   711,   718,   725,   733,   740,
     744,   751,   758,   762,   769,   776,   780,   787,   791,   795,
     802,   806,   811,   818,   822,   829,   833,   837,   841,   845,
     852,   856,   860,   867,   871,   875,   882,   887,   891,   898,
     902,   909,   913,   917,   921,   925,   929,   933,   937,   944,
     951,   959,   966,   973,   980,   984,   991,   995,   999,  1006,
    1010,  1017,  1021,  1028,  1035,  1039,  1043,  1047,  1054,  1061,
    1065,  1069,  1073,  1077,  1081,  1085,  1092
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "STRING_LITERAL",
  "NEWLINE", "TAB", "INTEGER_LITERAL", "FLOAT_LITERAL", "NULL_LITERAL",
  "OPERATOR", "EQ_OP", "NE_OP", "INSTANCEOF_OP", "GE_OP", "LE_OP",
  "BOOL_AND", "BOOL_OR", "'+'", "'>'", "'-'", "'!'", "'/'", "'<'", "'*'",
  "MAIN", "CLASS", "EXTENDS", "NATIVE", "THIS", "SUPER", "SYNCHRONIZED",
  "IF", "ELSE", "FOR", "WHILE", "NEW", "RETURN", "OUT", "IN", "BREAK",
  "CONTINUE", "BAD", "'('", "')'", "'{'", "'}'", "';'", "','", "'='",
  "'.'", "$accept", "CompilationUnit", "MainFunctionDeclaration",
  "ClassDeclarations", "ClassDeclaration", "ClassBody",
  "ClassBodyDeclarations", "ClassBodyDeclaration",
  "ClassMemberDeclaration", "FieldDeclaration", "MethodDeclaration",
  "MethodDeclarator", "MethodBody", "OperatorDeclaration",
  "OperatorDeclarator", "OperatorBody", "ConstructorDeclaration",
  "ConstructorDeclarator", "ConstructorBody", "ConstructorInvocation",
  "FormalParameters", "FormalParameterList", "FormalParameter",
  "VariableDeclaratorID", "Block", "BlockStatement", "BlockStatements",
  "SynchronizedStatement", "Statement", "VariableDeclarationStatement",
  "VariableDeclaration", "VariableDeclarators", "VariableDeclarator",
  "IfThenStatement", "IfThenElseStatement", "ForStatement", "ForInit",
  "ForExpression", "ForUpdate", "WhileStatement", "ReturnStatement",
  "OutputStatement", "BreakStatement", "ContinueStatement",
  "EmptyStatement", "ParenExpression", "ExpressionStatement",
  "StatementExpression", "Expression", "AssignmentExpression",
  "Assignment", "LeftHandSide", "BooleanExpression", "EqualityExpression",
  "InstanceOfExpression", "RelationalExpression", "AdditiveExpression",
  "MultiplicativeExpression", "UnaryExpression", "CastExpression",
  "Primary", "InputExpression", "ClassInstanceCreationExpression", "Type",
  "ReferenceType", "ClassType", "FieldAccess", "MethodInvocation",
  "Arguments", "ArgumentList", "Identifier", "Literal",
  "UnaryClassOperator", "BinaryClassOperator", "MinusClassOperator", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,    43,    62,
      45,    33,    47,    60,    42,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   290,    40,    41,   123,   125,    59,    44,    61,
      46
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    51,    52,    52,    52,    52,    53,    54,    54,    55,
      55,    56,    56,    57,    57,    58,    58,    59,    59,    59,
      60,    61,    61,    62,    63,    64,    64,    65,    65,    65,
      65,    66,    67,    67,    68,    69,    69,    69,    70,    70,
      71,    71,    72,    72,    73,    74,    75,    75,    76,    77,
      77,    78,    78,    79,    79,    79,    79,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    80,    81,    82,    82,
      83,    84,    85,    86,    87,    87,    88,    88,    89,    89,
      90,    91,    91,    92,    93,    94,    95,    96,    97,    98,
      98,    99,   100,   100,   101,   102,   102,   103,   103,   103,
     104,   104,   104,   105,   105,   106,   106,   106,   106,   106,
     107,   107,   107,   108,   108,   108,   109,   109,   109,   110,
     110,   111,   111,   111,   111,   111,   111,   111,   111,   112,
     113,   114,   115,   116,   117,   117,   118,   118,   118,   119,
     119,   120,   120,   121,   122,   122,   122,   122,   123,   124,
     124,   124,   124,   124,   124,   124,   125
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     2,     3,     5,     2,     1,     3,
       5,     3,     2,     2,     1,     1,     1,     1,     1,     1,
       3,     4,     3,     2,     1,     4,     3,     5,     5,     4,
       4,     1,     3,     2,     2,     4,     3,     1,     3,     3,
       3,     2,     3,     1,     2,     1,     3,     2,     1,     2,
       1,     7,     6,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     3,     1,
       1,     3,     5,     9,     1,     0,     1,     0,     1,     0,
       3,     2,     3,     3,     2,     2,     1,     3,     2,     1,
       1,     1,     1,     1,     3,     1,     1,     3,     3,     1,
       3,     3,     1,     3,     1,     3,     3,     3,     3,     1,
       3,     3,     1,     3,     3,     1,     2,     2,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     1,     1,     3,     3,     2,     4,     4,     3,
       2,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,   143,     0,     0,     2,     0,     8,     0,   131,   132,
     133,     0,     1,     3,     4,     7,     0,     0,     0,     9,
       5,     0,     0,     0,    12,     0,    14,    15,    17,    18,
      19,    16,     0,     0,   133,     0,    10,     0,     0,    11,
      13,     0,    33,    37,     0,     0,     0,     0,    69,    70,
       0,    34,     0,     6,    32,     0,     0,     0,   147,   144,
     145,   146,   123,     0,     0,     0,     0,     0,     0,     0,
       0,   129,     0,     0,     0,    47,    86,     0,    53,    50,
       0,    54,    48,    56,     0,    58,    59,    61,    60,    62,
      63,    64,    65,    55,   122,    57,     0,    89,     0,     0,
     128,   126,     0,   124,    90,    95,   127,   154,   155,   149,
     152,   156,   148,   151,   153,   150,     0,     0,     0,    22,
      24,    26,    31,    20,     0,    23,    41,     0,    43,     0,
     123,     0,    21,    25,     0,     0,     0,     0,     0,     0,
      75,     0,     0,     0,     0,    81,   122,     0,    91,    92,
      93,    99,   102,   104,   109,   112,   115,   118,   120,   125,
     121,     0,    84,    85,     0,    36,     0,    46,    49,    66,
      88,     0,     0,    67,    70,   136,     0,     0,     0,    68,
      40,     0,    44,    45,   140,   142,     0,    38,   135,    39,
       0,    71,     0,    74,    80,   130,   116,   124,   121,   117,
     119,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    87,    35,    94,   134,
      29,     0,    30,     0,    42,   139,     0,   138,     0,     0,
      77,   100,   101,    97,    98,   103,   107,   108,   106,   105,
     110,   111,   114,   113,   137,    27,    28,   141,     0,    72,
       0,    76,    52,     0,    79,    51,     0,    78,     0,    73
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    19,    25,    26,    27,    28,
      29,    45,   119,    30,    46,   121,    31,    32,    42,    77,
     125,   127,   128,   182,    78,    79,    80,    81,    82,    83,
      84,    47,    48,    85,    86,    87,   192,   250,   256,    88,
      89,    90,    91,    92,    93,   146,    95,    96,   147,   148,
      97,    98,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   100,   101,   102,     8,     9,   103,   159,   175,   186,
     198,   106,   116,   117,   118
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -137
static const yytype_int16 yypact[] =
{
      46,  -137,    17,    10,    50,    46,  -137,    26,  -137,  -137,
    -137,   -18,  -137,    50,    50,  -137,    38,    17,    25,  -137,
      50,    44,    80,    17,  -137,    33,  -137,  -137,  -137,  -137,
    -137,  -137,    92,    36,   122,   129,  -137,    88,    36,  -137,
    -137,   278,  -137,  -137,   139,   129,   129,    96,  -137,   122,
      13,  -137,   297,  -137,  -137,   120,   128,   122,  -137,  -137,
    -137,  -137,   133,    35,   136,   145,   147,   145,    17,   534,
     596,  -137,   144,   146,   596,  -137,  -137,   342,  -137,  -137,
     361,  -137,  -137,  -137,   148,  -137,  -137,  -137,  -137,  -137,
    -137,  -137,  -137,  -137,  -137,  -137,   150,  -137,   149,   151,
    -137,  -137,    17,   153,   154,    32,  -137,  -137,  -137,  -137,
    -137,  -137,  -137,  -137,  -137,  -137,   157,   160,   162,  -137,
    -137,  -137,  -137,  -137,    17,  -137,  -137,    16,  -137,    17,
    -137,   161,  -137,  -137,   558,   169,    17,   170,   596,   489,
     604,   489,   133,   596,   596,  -137,   604,   175,  -137,  -137,
    -137,    99,   181,    90,    94,   106,  -137,  -137,   151,  -137,
      40,   177,  -137,  -137,   163,  -137,   406,  -137,  -137,  -137,
    -137,   596,    17,   179,  -137,  -137,   184,    17,    42,  -137,
    -137,    17,  -137,  -137,  -137,  -137,    73,  -137,   133,  -137,
     187,   199,   186,  -137,  -137,  -137,  -137,  -137,   133,  -137,
    -137,  -137,   596,   596,   596,   596,    17,   596,   596,   596,
     596,   596,   596,   596,   596,  -137,  -137,  -137,  -137,   133,
    -137,   190,  -137,   191,  -137,  -137,   596,  -137,   192,   489,
     596,   181,   181,  -137,  -137,  -137,    94,    94,    94,    94,
     106,   106,  -137,  -137,  -137,  -137,  -137,  -137,   425,  -137,
     193,  -137,  -137,   470,   604,  -137,   194,  -137,   489,  -137
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -137,  -137,   234,    51,    53,   219,  -137,   217,  -137,  -137,
    -137,   205,  -137,  -137,   206,  -137,  -137,   222,  -137,  -137,
     212,  -137,   -58,  -137,   110,   -73,   -63,  -137,  -126,  -137,
    -137,   155,   123,  -137,  -137,  -137,  -137,  -137,  -137,  -137,
    -137,  -137,  -137,  -137,  -137,   -33,  -137,  -136,   -44,    78,
      -7,  -137,   -55,  -137,   -51,  -137,  -112,   -42,  -122,   104,
     -40,  -137,  -137,     6,  -137,   -14,    66,     7,   -20,  -137,
       0,  -137,  -137,  -137,  -137
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -134
static const yytype_int16 yytable[] =
{
      10,    99,    11,    22,   193,    10,     7,   168,    94,    17,
      12,     7,    99,   191,   166,   194,     1,    10,    34,    94,
       1,   196,   199,    34,    33,    34,   161,    18,     1,    38,
     164,    33,   139,    49,   141,  -133,     1,    99,    57,     1,
      99,   105,   135,   137,    94,     1,    44,    94,   104,     1,
      10,    16,   105,    23,   142,    13,   129,   126,    15,   104,
     180,    23,   149,   149,   181,    20,    15,   149,    10,   160,
     160,    24,     2,    15,   160,   134,     2,   105,   134,    39,
     105,    21,  -121,   134,   104,   136,   222,   104,    35,   -95,
     185,   242,   243,   168,   190,   236,   237,   238,   239,    99,
      99,    99,   174,   249,   207,   208,    94,    94,    94,   209,
     202,   203,   211,   210,   212,   204,   205,   225,   257,   221,
     223,   226,   195,   224,   174,    18,    99,   149,   213,   183,
     214,   149,   259,    94,   160,    54,   188,    41,   160,   105,
     160,   105,    43,   123,   124,    53,   104,   104,   104,   233,
     234,   231,   232,   107,   108,   120,   122,   109,   110,   111,
     112,   113,   114,   115,   149,    50,   105,   132,   227,   240,
     241,   160,   219,   104,    52,   133,   134,    10,    10,   138,
     168,    10,   247,   129,   129,   253,   251,   129,    74,    99,
     140,   162,   235,   163,   206,   169,    94,   170,   171,   244,
     176,   172,   -96,   177,  -125,   178,    10,   216,    99,   197,
     197,   136,   197,    99,    99,    94,   187,   189,    99,   149,
      94,    94,   201,   149,   215,    94,   160,   124,   220,   105,
     160,   228,   229,   230,   245,   246,   104,   248,   258,    14,
     254,    36,    40,    55,    56,    37,    51,   179,   105,   218,
     200,     0,     0,   105,   160,   104,     0,   173,   105,     0,
     104,   104,     0,     0,     0,   104,     0,     0,   197,   197,
     197,   197,     0,   197,   197,   197,   197,   197,   197,   197,
     197,     1,    58,     0,     0,    59,    60,    61,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       1,    58,     0,     0,    59,    60,    61,    62,    63,    64,
      65,     0,    66,    67,    68,    69,    70,    71,    72,    73,
       0,    74,     0,    52,    75,    76,   130,   131,    64,    65,
       0,    66,    67,    68,    69,    70,    71,    72,    73,     0,
      74,     0,    52,    75,    76,     1,    58,     0,     0,    59,
      60,    61,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     1,    58,     0,     0,    59,    60,
      61,   130,   131,    64,    65,     0,    66,    67,    68,    69,
      70,    71,    72,    73,     0,    74,     0,    52,   165,    76,
     130,   131,    64,    65,     0,    66,    67,    68,    69,    70,
      71,    72,    73,     0,    74,     0,    52,   167,    76,     1,
      58,     0,     0,    59,    60,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     1,    58,
       0,     0,    59,    60,    61,   130,   131,    64,    65,     0,
      66,    67,    68,    69,    70,    71,    72,    73,     0,    74,
       0,    52,   217,    76,   130,   131,    64,    65,     0,    66,
      67,    68,    69,    70,    71,    72,    73,     0,    74,     0,
      52,   252,    76,     1,    58,     0,     0,    59,    60,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     1,    58,     0,     0,    59,    60,    61,   130,
     131,    64,    65,     0,    66,    67,    68,    69,    70,    71,
      72,    73,     0,    74,     0,    52,   255,    76,   130,   131,
      64,    65,     0,    66,    67,    68,    69,    70,    71,    72,
      73,     0,    74,     0,    52,     0,    76,     1,    58,     0,
       0,    59,    60,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   143,   144,     0,     0,     0,     0,
       0,     1,    58,   130,   131,    59,    60,    61,     0,     0,
      68,     0,     0,    71,     0,     0,     0,    74,   143,   144,
       0,   145,     0,     0,     0,     0,     0,   130,   131,     0,
       0,     0,     0,     0,    68,     0,     0,    71,     0,     1,
      58,    74,   184,    59,    60,    61,     0,     1,    58,     0,
       0,    59,    60,    61,     0,     0,   143,   144,     0,     0,
       0,     0,     0,     0,     0,   130,   131,     0,     0,     0,
       0,     0,    68,   130,   131,    71,     0,     0,     0,    74,
      68,     0,     0,    71,     0,     0,     0,    74
};

static const yytype_int16 yycheck[] =
{
       0,    41,     2,    17,   140,     5,     0,    80,    41,    27,
       0,     5,    52,   139,    77,   141,     3,    17,    18,    52,
       3,   143,   144,    23,    18,    25,    70,    45,     3,    23,
      74,    25,    65,    33,    67,     3,     3,    77,    38,     3,
      80,    41,    62,    63,    77,     3,    10,    80,    41,     3,
      50,    25,    52,    28,    68,     4,    50,    44,     5,    52,
      44,    28,    69,    70,    48,    14,    13,    74,    68,    69,
      70,    46,    26,    20,    74,    43,    26,    77,    43,    46,
      80,    43,    50,    43,    77,    50,    44,    80,    44,    49,
     134,   213,   214,   166,   138,   207,   208,   209,   210,   139,
     140,   141,   102,   229,    14,    15,   139,   140,   141,    19,
      11,    12,    18,    23,    20,    16,    17,    44,   254,   177,
     178,    48,   142,   181,   124,    45,   166,   134,    22,   129,
      24,   138,   258,   166,   134,    47,   136,    45,   138,   139,
     140,   141,    32,    47,    48,    35,   139,   140,   141,   204,
     205,   202,   203,    14,    15,    45,    46,    18,    19,    20,
      21,    22,    23,    24,   171,    43,   166,    47,   188,   211,
     212,   171,   172,   166,    45,    47,    43,   177,   178,    43,
     253,   181,   226,   177,   178,   248,   230,   181,    43,   229,
      43,    47,   206,    47,    13,    47,   229,    47,    49,   219,
      43,    50,    49,    43,    50,    43,   206,    44,   248,   143,
     144,    50,   146,   253,   254,   248,    47,    47,   258,   226,
     253,   254,    47,   230,    47,   258,   226,    48,    44,   229,
     230,    44,    33,    47,    44,    44,   229,    45,    44,     5,
      47,    22,    25,    38,    38,    23,    34,   124,   248,   171,
     146,    -1,    -1,   253,   254,   248,    -1,   102,   258,    -1,
     253,   254,    -1,    -1,    -1,   258,    -1,    -1,   202,   203,
     204,   205,    -1,   207,   208,   209,   210,   211,   212,   213,
     214,     3,     4,    -1,    -1,     7,     8,     9,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,    -1,    -1,     7,     8,     9,    29,    30,    31,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    43,    -1,    45,    46,    47,    29,    30,    31,    32,
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      43,    -1,    45,    46,    47,     3,     4,    -1,    -1,     7,
       8,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,    -1,    -1,     7,     8,
       9,    29,    30,    31,    32,    -1,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    43,    -1,    45,    46,    47,
      29,    30,    31,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    43,    -1,    45,    46,    47,     3,
       4,    -1,    -1,     7,     8,     9,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
      -1,    -1,     7,     8,     9,    29,    30,    31,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    43,
      -1,    45,    46,    47,    29,    30,    31,    32,    -1,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    43,    -1,
      45,    46,    47,     3,     4,    -1,    -1,     7,     8,     9,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,    -1,    -1,     7,     8,     9,    29,
      30,    31,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    43,    -1,    45,    46,    47,    29,    30,
      31,    32,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    43,    -1,    45,    -1,    47,     3,     4,    -1,
      -1,     7,     8,     9,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,     3,     4,    29,    30,     7,     8,     9,    -1,    -1,
      36,    -1,    -1,    39,    -1,    -1,    -1,    43,    20,    21,
      -1,    47,    -1,    -1,    -1,    -1,    -1,    29,    30,    -1,
      -1,    -1,    -1,    -1,    36,    -1,    -1,    39,    -1,     3,
       4,    43,    44,     7,     8,     9,    -1,     3,     4,    -1,
      -1,     7,     8,     9,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    29,    30,    -1,    -1,    -1,
      -1,    -1,    36,    29,    30,    39,    -1,    -1,    -1,    43,
      36,    -1,    -1,    39,    -1,    -1,    -1,    43
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    26,    52,    53,    54,    55,   114,   115,   116,
     121,   121,     0,    54,    53,    55,    25,    27,    45,    56,
      54,    43,   116,    28,    46,    57,    58,    59,    60,    61,
      64,    67,    68,   114,   121,    44,    56,    68,   114,    46,
      58,    45,    69,    75,    10,    62,    65,    82,    83,   121,
      43,    71,    45,    75,    47,    62,    65,   121,     4,     7,
       8,     9,    29,    30,    31,    32,    34,    35,    36,    37,
      38,    39,    40,    41,    43,    46,    47,    70,    75,    76,
      77,    78,    79,    80,    81,    84,    85,    86,    90,    91,
      92,    93,    94,    95,    96,    97,    98,   101,   102,   111,
     112,   113,   114,   117,   118,   121,   122,    14,    15,    18,
      19,    20,    21,    22,    23,    24,   123,   124,   125,    63,
      75,    66,    75,    47,    48,    71,    44,    72,    73,   114,
      29,    30,    47,    47,    43,   119,    50,   119,    43,    96,
      43,    96,   116,    20,    21,    47,    96,    99,   100,   101,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   118,
     121,    99,    47,    47,    99,    46,    77,    46,    76,    47,
      47,    49,    50,    82,   121,   119,    43,    43,    43,    83,
      44,    48,    74,   121,    44,    99,   120,    47,   121,    47,
      99,    79,    87,    98,    79,   119,   109,   117,   121,   109,
     110,    47,    11,    12,    16,    17,    13,    14,    15,    19,
      23,    18,    20,    22,    24,    47,    44,    46,   100,   121,
      44,    73,    44,    73,    73,    44,    48,   119,    44,    33,
      47,   105,   105,   103,   103,   116,   107,   107,   107,   107,
     108,   108,   109,   109,   119,    44,    44,    99,    45,    79,
      88,    99,    46,    77,    47,    46,    89,    98,    44,    79
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;
/* Location data for the look-ahead symbol.  */
YYLTYPE yylloc;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;

  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[2];

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;
#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 0;
#endif

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
	YYSTACK_RELOCATE (yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 210 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, (yyvsp[(1) - (1)].mainBlock));
	  ast = (yyval.seq);
	}
    break;

  case 3:
#line 215 "parse.yy"
    {
	  (yyval.seq) = (yyvsp[(2) - (2)].seq)->add((yyvsp[(1) - (2)].mainBlock));
	  ast = (yyval.seq);
	}
    break;

  case 4:
#line 220 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, (yyvsp[(2) - (2)].mainBlock));
	  (yyval.seq)->add((yyvsp[(1) - (2)].seq));
	  ast = (yyval.seq);
	}
    break;

  case 5:
#line 226 "parse.yy"
    {
	  (yyval.seq) = (yyvsp[(3) - (3)].seq)->add((yyvsp[(2) - (3)].mainBlock))->add((yyvsp[(1) - (3)].seq));
	  ast = (yyval.seq);
	}
    break;

  case 6:
#line 234 "parse.yy"
    {
	  if (*((yyvsp[(1) - (5)].type)->getName()) != "Integer") {
	     cerr << "mc: " << (yyloc).first_line << ": parse error\n";
	     return 1;
	  }

	  (yyval.mainBlock) = new MainBlockNode((yyvsp[(5) - (5)].blockStmnt));
	}
    break;

  case 7:
#line 246 "parse.yy"
    {
	  (yyval.seq) = new Seq((yyvsp[(1) - (2)].seq), (yyvsp[(2) - (2)].classDec));
	}
    break;

  case 8:
#line 250 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, (yyvsp[(1) - (1)].classDec));
	}
    break;

  case 9:
#line 257 "parse.yy"
    {
	  (yyval.classDec) = new ClassDecNode((yyvsp[(2) - (3)].str), (yyvsp[(3) - (3)].seq));
	  (yyval.classDec)->setLineNumber((yyloc).first_line);
	}
    break;

  case 10:
#line 262 "parse.yy"
    {
	  (yyval.classDec) = new ClassDecNode((yyvsp[(2) - (5)].str), ((ClassType *)(yyvsp[(4) - (5)].type))->getName(), (yyvsp[(5) - (5)].seq));
	  delete (yyvsp[(4) - (5)].type);
	  (yyval.classDec)->setLineNumber((yyloc).first_line);
	}
    break;

  case 11:
#line 271 "parse.yy"
    {
	  (yyval.seq) = (yyvsp[(2) - (3)].seq);
	}
    break;

  case 12:
#line 275 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, 0);
	}
    break;

  case 13:
#line 282 "parse.yy"
    {
	  (yyval.seq) = new Seq((yyvsp[(1) - (2)].seq), (yyvsp[(2) - (2)].node));
	}
    break;

  case 14:
#line 286 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, (yyvsp[(1) - (1)].node));
	}
    break;

  case 15:
#line 293 "parse.yy"
    {
	  (yyval.node) = (yyvsp[(1) - (1)].node);
	}
    break;

  case 16:
#line 297 "parse.yy"
    {
	  (yyval.node) = (yyvsp[(1) - (1)].classConst);
	}
    break;

  case 17:
#line 304 "parse.yy"
    {
	  (yyval.node) = (yyvsp[(1) - (1)].classFieldDec);
	}
    break;

  case 18:
#line 308 "parse.yy"
    {
	  (yyval.node) = (yyvsp[(1) - (1)].classMeth);
	}
    break;

  case 19:
#line 312 "parse.yy"
    {
	  (yyval.node) = (yyvsp[(1) - (1)].classMeth);
	}
    break;

  case 20:
#line 319 "parse.yy"
    {
	  (yyval.classFieldDec) = new ClassFieldDecNode((yyvsp[(1) - (3)].type), (yyvsp[(2) - (3)].seq));
	  (yyval.classFieldDec)->setLineNumber((yyloc).first_line);
	}
    break;

  case 21:
#line 327 "parse.yy"
    {
	  (yyvsp[(3) - (4)].classMeth)->setNativeIndex(nativeIndex++);
	  (yyvsp[(3) - (4)].classMeth)->setType((yyvsp[(2) - (4)].type));
	  (yyvsp[(3) - (4)].classMeth)->setBody(new BlockStatementNode());
	  (yyval.classMeth) = (yyvsp[(3) - (4)].classMeth);
	  (yyval.classMeth)->setLineNumber((yyloc).first_line);
	}
    break;

  case 22:
#line 335 "parse.yy"
    {
	  (yyvsp[(2) - (3)].classMeth)->setType((yyvsp[(1) - (3)].type));
	  (yyvsp[(2) - (3)].classMeth)->setBody((yyvsp[(3) - (3)].blockStmnt));
	  (yyval.classMeth) = (yyvsp[(2) - (3)].classMeth);
	  (yyval.classMeth)->setLineNumber((yyloc).first_line);
	}
    break;

  case 23:
#line 345 "parse.yy"
    {
	  (yyval.classMeth) = new ClassMethodNode((yyvsp[(1) - (2)].str), (yyvsp[(2) - (2)].seq));
	}
    break;

  case 24:
#line 352 "parse.yy"
    {
	  (yyval.blockStmnt) = (yyvsp[(1) - (1)].blockStmnt);
	}
    break;

  case 25:
#line 359 "parse.yy"
    {
	  (yyvsp[(3) - (4)].classMeth)->setNativeIndex(nativeIndex++);
	  (yyvsp[(3) - (4)].classMeth)->setType((yyvsp[(2) - (4)].type));
	  (yyvsp[(3) - (4)].classMeth)->setBody(new BlockStatementNode());
	  (yyval.classMeth) = (yyvsp[(3) - (4)].classMeth);
	  (yyval.classMeth)->setLineNumber((yyloc).first_line);
	}
    break;

  case 26:
#line 367 "parse.yy"
    {
	  (yyvsp[(2) - (3)].classMeth)->setType((yyvsp[(1) - (3)].type));
	  (yyvsp[(2) - (3)].classMeth)->setBody((yyvsp[(3) - (3)].blockStmnt));
	  (yyval.classMeth) = (yyvsp[(2) - (3)].classMeth);
	  (yyval.classMeth)->setLineNumber((yyloc).first_line);
	}
    break;

  case 27:
#line 377 "parse.yy"
    {
	  (yyval.classMeth) = new ClassMethodNode((yyvsp[(2) - (5)].str), new Seq(0, (yyvsp[(4) - (5)].varDec)));
	}
    break;

  case 28:
#line 381 "parse.yy"
    {
	  (yyval.classMeth) = new ClassMethodNode((yyvsp[(2) - (5)].str), new Seq(0, (yyvsp[(4) - (5)].varDec)));
	}
    break;

  case 29:
#line 385 "parse.yy"
    {
	  (yyval.classMeth) = new ClassMethodNode((yyvsp[(2) - (4)].str), new Seq(0, 0));
	}
    break;

  case 30:
#line 389 "parse.yy"
    {
	  (yyval.classMeth) = new ClassMethodNode((yyvsp[(2) - (4)].str), new Seq(0, 0));
	}
    break;

  case 31:
#line 396 "parse.yy"
    {
	  (yyval.blockStmnt) = (yyvsp[(1) - (1)].blockStmnt);
	}
    break;

  case 32:
#line 403 "parse.yy"
    {
	  (yyvsp[(2) - (3)].classConst)->setNativeIndex(nativeIndex++);
	  (yyvsp[(2) - (3)].classConst)->setBody(new BlockStatementNode());
	  (yyval.classConst) = (yyvsp[(2) - (3)].classConst);
	}
    break;

  case 33:
#line 409 "parse.yy"
    {
	  (yyvsp[(1) - (2)].classConst)->setBody((yyvsp[(2) - (2)].blockStmnt));
	  (yyval.classConst) = (yyvsp[(1) - (2)].classConst);
	}
    break;

  case 34:
#line 417 "parse.yy"
    {
	  (yyval.classConst) = new ClassConstructorNode((yyvsp[(1) - (2)].str), (yyvsp[(2) - (2)].seq));
	  (yyval.classConst)->setLineNumber((yyloc).first_line);
	}
    break;

  case 35:
#line 425 "parse.yy"
    {
	  (yyval.blockStmnt) = new BlockStatementNode((yyvsp[(3) - (4)].seq)->add((yyvsp[(2) - (4)].stmnt)));
	}
    break;

  case 36:
#line 429 "parse.yy"
    {
	  (yyval.blockStmnt) = new BlockStatementNode(new Seq(0, (yyvsp[(2) - (3)].stmnt)));
	}
    break;

  case 37:
#line 433 "parse.yy"
    {
	  (yyvsp[(1) - (1)].blockStmnt)->add(new ConstructorInvocationStatementNode(new SuperExpressionNode(), new Seq(0, 0)));
	  (yyval.blockStmnt) = (yyvsp[(1) - (1)].blockStmnt);
	}
    break;

  case 38:
#line 441 "parse.yy"
    {
	  (yyval.stmnt) = new ConstructorInvocationStatementNode(new ThisExpressionNode(), (yyvsp[(2) - (3)].seq));
	}
    break;

  case 39:
#line 445 "parse.yy"
    {
	  (yyval.stmnt) = new ConstructorInvocationStatementNode(new SuperExpressionNode(), (yyvsp[(2) - (3)].seq));
	}
    break;

  case 40:
#line 452 "parse.yy"
    {
	  (yyval.seq) = (yyvsp[(2) - (3)].seq);
	}
    break;

  case 41:
#line 456 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, 0);
	}
    break;

  case 42:
#line 463 "parse.yy"
    {
	  (yyval.seq) = new Seq((yyvsp[(1) - (3)].seq), (yyvsp[(3) - (3)].varDec));
	}
    break;

  case 43:
#line 467 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, (yyvsp[(1) - (1)].varDec));
	}
    break;

  case 44:
#line 474 "parse.yy"
    {
	  (yyval.varDec) = new VarDecNode((yyvsp[(1) - (2)].type), new Seq(0, (yyvsp[(2) - (2)].varNode)));
	  (yyval.varDec)->setLineNumber((yyloc).first_line);
	}
    break;

  case 45:
#line 482 "parse.yy"
    {
	  (yyval.varNode) = new VarNode((yyvsp[(1) - (1)].str));
	  (yyval.varNode)->setLineNumber((yyloc).first_line);
	}
    break;

  case 46:
#line 490 "parse.yy"
    {
	  (yyval.blockStmnt) = new BlockStatementNode((yyvsp[(2) - (3)].seq));
	}
    break;

  case 47:
#line 494 "parse.yy"
    {
	  (yyval.blockStmnt) = new BlockStatementNode(new Seq(0, 0));
	}
    break;

  case 48:
#line 501 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 49:
#line 508 "parse.yy"
    {
	  (yyval.seq) = new Seq((yyvsp[(1) - (2)].seq), (yyvsp[(2) - (2)].stmnt));
	}
    break;

  case 50:
#line 512 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, (yyvsp[(1) - (1)].stmnt));
	}
    break;

  case 51:
#line 519 "parse.yy"
    {
	  (yyval.stmnt) = new SynchronizedStatementNode((yyvsp[(3) - (7)].expr), (yyvsp[(6) - (7)].seq));
	}
    break;

  case 52:
#line 523 "parse.yy"
    {
	  (yyval.stmnt) = new SynchronizedStatementNode((yyvsp[(3) - (6)].expr), new Seq(0, 0));
	}
    break;

  case 53:
#line 530 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].blockStmnt);
	}
    break;

  case 54:
#line 534 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 55:
#line 538 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 56:
#line 542 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 57:
#line 546 "parse.yy"
    {
	  (yyvsp[(1) - (1)].expr)->setIsStatement(true);
	  (yyval.stmnt) = new ExpressionStatementNode((yyvsp[(1) - (1)].expr));
	}
    break;

  case 58:
#line 551 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 59:
#line 555 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 60:
#line 559 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 61:
#line 563 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 62:
#line 567 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 63:
#line 571 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 64:
#line 575 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 65:
#line 579 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 66:
#line 586 "parse.yy"
    {
	  (yyval.stmnt) = new LocalVarDecStatementNode((yyvsp[(1) - (2)].localVarDec));
	}
    break;

  case 67:
#line 593 "parse.yy"
    {
	  (yyval.localVarDec) = new LocalVarDecNode((yyvsp[(1) - (2)].type), (yyvsp[(2) - (2)].seq));
	  (yyval.localVarDec)->setLineNumber((yyloc).first_line);
	}
    break;

  case 68:
#line 601 "parse.yy"
    {
	  (yyval.seq) = new Seq((yyvsp[(1) - (3)].seq), (yyvsp[(3) - (3)].varNode));
	}
    break;

  case 69:
#line 605 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, (yyvsp[(1) - (1)].varNode));
	}
    break;

  case 70:
#line 612 "parse.yy"
    {
	  (yyval.varNode) = new VarNode((yyvsp[(1) - (1)].str));
	  (yyval.varNode)->setLineNumber((yyloc).first_line);
	}
    break;

  case 71:
#line 620 "parse.yy"
    {
	  (yyval.stmnt) = new IfThenStatementNode((yyvsp[(2) - (3)].expr), (yyvsp[(3) - (3)].stmnt));
	  (yyval.stmnt)->setLineNumber((yyloc).first_line);
	}
    break;

  case 72:
#line 628 "parse.yy"
    {
	  (yyval.stmnt) = new IfThenElseStatementNode((yyvsp[(2) - (5)].expr), (yyvsp[(3) - (5)].stmnt), (yyvsp[(5) - (5)].stmnt));
	  (yyval.stmnt)->setLineNumber((yyloc).first_line);
	}
    break;

  case 73:
#line 636 "parse.yy"
    {
	  (yyval.stmnt) = new ForStatementNode((yyvsp[(3) - (9)].expr), (yyvsp[(5) - (9)].expr), (yyvsp[(7) - (9)].expr), (yyvsp[(9) - (9)].stmnt));
	  (yyval.stmnt)->setLineNumber((yyloc).first_line);
	}
    break;

  case 74:
#line 644 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	  ((ExpressionNode *)(yyvsp[(1) - (1)].expr))->setIsStatement(true);
	}
    break;

  case 75:
#line 649 "parse.yy"
    {
          (yyval.expr) = new ExpressionNode();
	}
    break;

  case 76:
#line 656 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 77:
#line 660 "parse.yy"
    {
          (yyval.expr) = new IntegerLiteralNode(1);
	}
    break;

  case 78:
#line 667 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	  ((ExpressionNode *)(yyvsp[(1) - (1)].expr))->setIsStatement(true);
	}
    break;

  case 79:
#line 672 "parse.yy"
    {
          (yyval.expr) = new ExpressionNode();
	}
    break;

  case 80:
#line 679 "parse.yy"
    {
	  (yyval.stmnt) = new WhileStatementNode((yyvsp[(2) - (3)].expr), (yyvsp[(3) - (3)].stmnt));
	  (yyval.stmnt)->setLineNumber((yyloc).first_line);
	}
    break;

  case 81:
#line 687 "parse.yy"
    {
	  (yyval.stmnt) = new ReturnStatementNode();
	}
    break;

  case 82:
#line 691 "parse.yy"
    {
	  (yyval.stmnt) = new ReturnStatementNode((yyvsp[(2) - (3)].expr));
	}
    break;

  case 83:
#line 698 "parse.yy"
    {
	  (yyval.stmnt) = new OutStatementNode((yyvsp[(2) - (3)].expr));
	}
    break;

  case 84:
#line 705 "parse.yy"
    {
	  (yyval.stmnt) = new BreakStatementNode();
	}
    break;

  case 85:
#line 712 "parse.yy"
    {
	  (yyval.stmnt) = new ContinueStatementNode();
	}
    break;

  case 86:
#line 719 "parse.yy"
    {
	  (yyval.stmnt) = new StatementNode();
	}
    break;

  case 87:
#line 726 "parse.yy"
    {
	  (yyvsp[(2) - (3)].expr)->setIsParenthesized(true);
	  (yyval.expr) = (yyvsp[(2) - (3)].expr);
	}
    break;

  case 88:
#line 734 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (2)].expr);
	}
    break;

  case 89:
#line 741 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 90:
#line 745 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 91:
#line 752 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 92:
#line 759 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 93:
#line 763 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 94:
#line 770 "parse.yy"
    {
	  (yyval.expr) = new AssignmentOperatorNode((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
	}
    break;

  case 95:
#line 777 "parse.yy"
    {
	  (yyval.expr) = new DerefNode(new VarNode((yyvsp[(1) - (1)].str)));
	}
    break;

  case 96:
#line 781 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 97:
#line 788 "parse.yy"
    {
	  (yyval.expr) = new BooleanAndOperatorNode((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
	}
    break;

  case 98:
#line 792 "parse.yy"
    {
	  (yyval.expr) = new BooleanOrOperatorNode((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
	}
    break;

  case 99:
#line 796 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 100:
#line 803 "parse.yy"
    {
	  (yyval.expr) = new EqualityOperatorNode((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
	}
    break;

  case 101:
#line 807 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode(new EqualityOperatorNode((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)),
	           (yyvsp[(2) - (3)].str), new Seq(0, 0));
	}
    break;

  case 102:
#line 812 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 103:
#line 819 "parse.yy"
    {
	  (yyval.expr) = new InstanceOfExpressionNode((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].type));
	}
    break;

  case 104:
#line 823 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 105:
#line 830 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].str), new Seq(0, (yyvsp[(3) - (3)].expr)));
	}
    break;

  case 106:
#line 834 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].str), new Seq(0, (yyvsp[(3) - (3)].expr)));
	}
    break;

  case 107:
#line 838 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].str), new Seq(0, (yyvsp[(3) - (3)].expr)));
	}
    break;

  case 108:
#line 842 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].str), new Seq(0, (yyvsp[(3) - (3)].expr)));
	}
    break;

  case 109:
#line 846 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 110:
#line 853 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].str), new Seq(0, (yyvsp[(3) - (3)].expr)));
	}
    break;

  case 111:
#line 857 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].str), new Seq(0, (yyvsp[(3) - (3)].expr)));
	}
    break;

  case 112:
#line 861 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 113:
#line 868 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].str), new Seq(0, (yyvsp[(3) - (3)].expr)));
	}
    break;

  case 114:
#line 872 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].str), new Seq(0, (yyvsp[(3) - (3)].expr)));
	}
    break;

  case 115:
#line 876 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 116:
#line 883 "parse.yy"
    {
	  (yyvsp[(2) - (2)].expr)->setParentIsNegate(true);
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(2) - (2)].expr), (yyvsp[(1) - (2)].str), new Seq(0, 0));
	}
    break;

  case 117:
#line 888 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(2) - (2)].expr), (yyvsp[(1) - (2)].str), new Seq(0, 0));
	}
    break;

  case 118:
#line 892 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 119:
#line 899 "parse.yy"
    {
	  (yyval.expr) = new CastExpressionNode((yyvsp[(1) - (2)].expr), (yyvsp[(2) - (2)].expr));
	}
    break;

  case 120:
#line 903 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 121:
#line 910 "parse.yy"
    {
	  (yyval.expr) = new DerefNode(new VarNode((yyvsp[(1) - (1)].str)));
	}
    break;

  case 122:
#line 914 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 123:
#line 918 "parse.yy"
    {
	  (yyval.expr) = new ThisExpressionNode();
	}
    break;

  case 124:
#line 922 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 125:
#line 926 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 126:
#line 930 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 127:
#line 934 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 128:
#line 938 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 129:
#line 945 "parse.yy"
    {
	  (yyval.expr) = new InExpressionNode();
	}
    break;

  case 130:
#line 952 "parse.yy"
    {
	  (yyval.expr) = new ClassInstanceCreationExpressionNode((yyvsp[(2) - (3)].type)->getName(), (yyvsp[(3) - (3)].seq));
	  delete (yyvsp[(2) - (3)].type);
	}
    break;

  case 131:
#line 960 "parse.yy"
    {
	  (yyval.type) = (yyvsp[(1) - (1)].type);
	}
    break;

  case 132:
#line 967 "parse.yy"
    {
	  (yyval.type) = (yyvsp[(1) - (1)].type);
	}
    break;

  case 133:
#line 974 "parse.yy"
    {
	  (yyval.type) = new ClassType((yyvsp[(1) - (1)].str));
	}
    break;

  case 134:
#line 981 "parse.yy"
    {
	  (yyval.expr) = new DerefNode(new FieldAccessOperatorNode((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].str)));
	}
    break;

  case 135:
#line 985 "parse.yy"
    {
	  (yyval.expr) = new DerefNode(new FieldAccessOperatorNode(new SuperExpressionNode(), (yyvsp[(3) - (3)].str)));
	}
    break;

  case 136:
#line 992 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode(new ThisExpressionNode(), (yyvsp[(1) - (2)].str), (yyvsp[(2) - (2)].seq));
	}
    break;

  case 137:
#line 996 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(1) - (4)].expr), (yyvsp[(3) - (4)].str), (yyvsp[(4) - (4)].seq));
	}
    break;

  case 138:
#line 1000 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode(new SuperExpressionNode(), (yyvsp[(3) - (4)].str), (yyvsp[(4) - (4)].seq));
	}
    break;

  case 139:
#line 1007 "parse.yy"
    {
	  (yyval.seq) = (yyvsp[(2) - (3)].seq);
	}
    break;

  case 140:
#line 1011 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, 0);
	}
    break;

  case 141:
#line 1018 "parse.yy"
    {
	  (yyval.seq) = new Seq((yyvsp[(1) - (3)].seq), (yyvsp[(3) - (3)].expr));
	}
    break;

  case 142:
#line 1022 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, (yyvsp[(1) - (1)].expr));
	}
    break;

  case 143:
#line 1029 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);
	}
    break;

  case 144:
#line 1036 "parse.yy"
    {
	  (yyval.expr) = new IntegerLiteralNode((yyvsp[(1) - (1)].value));
	}
    break;

  case 145:
#line 1040 "parse.yy"
    {
	  (yyval.expr) = new RealLiteralNode((yyvsp[(1) - (1)].float_value));
	}
    break;

  case 146:
#line 1044 "parse.yy"
    {
	  (yyval.expr) = new NullLiteralNode();
	}
    break;

  case 147:
#line 1048 "parse.yy"
    {
	  (yyval.expr) = new StringLiteralNode((yyvsp[(1) - (1)].str));
	}
    break;

  case 148:
#line 1055 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);
	}
    break;

  case 149:
#line 1062 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);
	}
    break;

  case 150:
#line 1066 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);
	}
    break;

  case 151:
#line 1070 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);
	}
    break;

  case 152:
#line 1074 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);
	}
    break;

  case 153:
#line 1078 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);
	}
    break;

  case 154:
#line 1082 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);
	}
    break;

  case 155:
#line 1086 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);
	}
    break;

  case 156:
#line 1093 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);
	}
    break;


/* Line 1267 of yacc.c.  */
#line 2951 "parse.cc"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }

  yyerror_range[0] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, &yylloc);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[0] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the look-ahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 1098 "parse.yy"


void yyerror(const char *s) {
        cerr << "mc: " << getCurrentSourceLineNumber() << ": " << s << '\n';
}

