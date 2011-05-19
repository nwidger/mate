
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 1



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "parse.yy"


// Niels Widger
// CS 712
// Time-stamp: <12 May 2011 at 19:25:47 by nwidger on macros.local>

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

extern Seq *ast;
extern TypeModule *types;



/* Line 189 of yacc.c  */
#line 106 "parse.cc"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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

/* Line 214 of yacc.c  */
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



/* Line 214 of yacc.c  */
#line 245 "parse.cc"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
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


/* Line 264 of yacc.c  */
#line 270 "parse.cc"

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
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
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
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
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
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  12
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   742

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
       2,     2,     2,    22,     2,     2,     2,     2,     2,     2,
      43,    44,    25,    19,    48,    21,    50,    23,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    47,
      24,    49,    20,     2,     2,     2,     2,     2,     2,     2,
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
      15,    16,    17,    18,    26,    27,    28,    29,    30,    31,
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
      -1,    54,    53,    54,    -1,   114,    26,    43,    44,    75,
      -1,    54,    55,    -1,    55,    -1,    27,   121,    56,    -1,
      27,   121,    28,   116,    56,    -1,    45,    57,    46,    -1,
      45,    46,    -1,    57,    58,    -1,    58,    -1,    59,    -1,
      67,    -1,    60,    -1,    61,    -1,    64,    -1,   114,    82,
      47,    -1,     7,   114,    62,    47,    -1,   114,    62,    63,
      -1,   121,    71,    -1,    75,    -1,     7,   114,    65,    47,
      -1,   114,    65,    66,    -1,    11,   124,    43,    73,    44,
      -1,    11,   125,    43,    73,    44,    -1,    11,   123,    43,
      44,    -1,    11,   125,    43,    44,    -1,    75,    -1,     7,
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
     117,    -1,   104,    17,   103,    -1,   104,    18,   103,    -1,
     104,    -1,   104,    12,   105,    -1,   104,    13,   105,    -1,
     105,    -1,   105,    14,   116,    -1,   106,    -1,   106,    24,
     107,    -1,   106,    20,   107,    -1,   106,    15,   107,    -1,
     106,    16,   107,    -1,   107,    -1,   107,    19,   108,    -1,
     107,    21,   108,    -1,   108,    -1,   108,    25,   109,    -1,
     108,    23,   109,    -1,   109,    -1,    21,   109,    -1,    22,
     109,    -1,   110,    -1,    96,   110,    -1,   111,    -1,   121,
      -1,    96,    -1,    29,    -1,   117,    -1,   118,    -1,   113,
      -1,   122,    -1,   112,    -1,    39,    -1,    36,   116,   119,
      -1,   115,    -1,   116,    -1,   121,    -1,   111,    50,   121,
      -1,    30,    50,   121,    -1,   121,   119,    -1,   111,    50,
     121,   119,    -1,    30,    50,   121,   119,    -1,    43,   120,
      44,    -1,    43,    44,    -1,   120,    48,    99,    -1,    99,
      -1,     3,    -1,     8,    -1,     9,    -1,    10,    -1,     4,
      -1,    22,    -1,    19,    -1,    25,    -1,    23,    -1,    20,
      -1,    24,    -1,    15,    -1,    16,    -1,    21,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   207,   207,   212,   217,   223,   231,   243,   247,   254,
     259,   268,   272,   279,   283,   290,   294,   301,   305,   309,
     316,   324,   332,   342,   349,   356,   364,   374,   378,   382,
     386,   393,   400,   406,   414,   422,   426,   430,   438,   442,
     449,   453,   460,   464,   471,   479,   487,   491,   498,   505,
     509,   516,   520,   527,   531,   535,   539,   543,   548,   552,
     556,   560,   564,   568,   572,   576,   583,   590,   598,   602,
     609,   617,   625,   633,   652,   658,   664,   669,   675,   681,
     687,   695,   699,   706,   713,   720,   727,   734,   742,   749,
     753,   760,   767,   771,   778,   785,   789,   796,   800,   804,
     811,   815,   820,   827,   831,   838,   842,   846,   850,   854,
     861,   865,   869,   876,   880,   884,   891,   896,   900,   907,
     911,   918,   922,   926,   930,   934,   938,   942,   946,   953,
     960,   968,   975,   982,   989,   993,  1000,  1004,  1008,  1015,
    1019,  1026,  1030,  1037,  1044,  1048,  1052,  1056,  1063,  1070,
    1074,  1078,  1082,  1086,  1090,  1094,  1101
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "STRING_LITERAL",
  "NEWLINE", "TAB", "NATIVE", "INTEGER_LITERAL", "FLOAT_LITERAL",
  "NULL_LITERAL", "OPERATOR", "EQ_OP", "NE_OP", "INSTANCEOF_OP", "GE_OP",
  "LE_OP", "BOOL_AND", "BOOL_OR", "'+'", "'>'", "'-'", "'!'", "'/'", "'<'",
  "'*'", "MAIN", "CLASS", "EXTENDS", "THIS", "SUPER", "SYNCHRONIZED", "IF",
  "ELSE", "FOR", "WHILE", "NEW", "RETURN", "OUT", "IN", "BREAK",
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
     265,   266,   267,   268,   269,   270,   271,   272,   273,    43,
      62,    45,    33,    47,    60,    42,   274,   275,   276,   277,
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
      24,  -137,    43,    10,    12,    24,  -137,    34,  -137,  -137,
    -137,    27,  -137,    12,    12,  -137,    22,    43,    42,  -137,
      12,    39,    41,    43,  -137,    50,  -137,  -137,  -137,  -137,
    -137,  -137,    44,    25,    55,    59,  -137,    62,    25,  -137,
    -137,   278,  -137,  -137,   138,    59,    59,    96,  -137,    55,
      17,  -137,   323,  -137,  -137,    76,    78,    55,  -137,  -137,
    -137,  -137,    71,   -34,    77,    85,    87,    85,    43,   638,
     691,  -137,    88,    90,   691,  -137,  -137,   368,  -137,  -137,
     413,  -137,  -137,  -137,   118,  -137,  -137,  -137,  -137,  -137,
    -137,  -137,  -137,  -137,  -137,  -137,   120,  -137,   127,   129,
    -137,  -137,    43,   139,   141,    32,  -137,  -137,  -137,  -137,
    -137,  -137,  -137,  -137,  -137,  -137,   150,   152,   161,  -137,
    -137,  -137,  -137,  -137,    43,  -137,  -137,    73,  -137,    43,
    -137,   155,  -137,  -137,   654,   160,    43,   164,   691,   593,
     699,   593,    71,   691,   691,  -137,   699,   169,  -137,  -137,
    -137,   157,   203,    95,    57,    72,  -137,  -137,   129,  -137,
      36,   175,  -137,  -137,   180,  -137,   458,  -137,  -137,  -137,
    -137,   691,    43,   179,  -137,  -137,   184,    43,    61,  -137,
    -137,    43,  -137,  -137,  -137,  -137,   146,  -137,    71,  -137,
     187,   199,   186,  -137,  -137,  -137,  -137,  -137,    71,  -137,
    -137,  -137,   691,   691,   691,   691,    43,   691,   691,   691,
     691,   691,   691,   691,   691,  -137,  -137,  -137,  -137,    71,
    -137,   190,  -137,   191,  -137,  -137,   691,  -137,   192,   593,
     691,   203,   203,  -137,  -137,  -137,    57,    57,    57,    57,
      72,    72,  -137,  -137,  -137,  -137,  -137,  -137,   503,  -137,
     193,  -137,  -137,   548,   699,  -137,   194,  -137,   593,  -137
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -137,  -137,   234,    67,    53,   219,  -137,   217,  -137,  -137,
    -137,   205,  -137,  -137,   206,  -137,  -137,   222,  -137,  -137,
     212,  -137,   -65,  -137,   110,   -73,   -63,  -137,  -126,  -137,
    -137,   145,   125,  -137,  -137,  -137,  -137,  -137,  -137,  -137,
    -137,  -137,  -137,  -137,  -137,   -33,  -137,  -136,   -44,    79,
      -7,  -137,    -4,  -137,    -5,  -137,   -58,    -9,  -122,   105,
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
      10,    99,    11,    22,   193,    10,     7,   168,    94,   134,
      12,     7,    99,   191,   166,   194,   136,    10,    34,    94,
       1,   196,   199,    34,    33,    34,   161,     1,     1,    38,
     164,    33,   139,    49,   141,  -133,    44,    99,    57,     2,
      99,   105,   135,   137,    94,     1,     1,    94,   104,    23,
      10,     2,   105,     1,   142,    17,   129,    23,    15,   104,
      16,   126,   149,   149,     1,    21,    15,   149,    10,   160,
     160,    13,    18,    15,   160,   134,   211,   105,   212,   134,
     105,    20,  -121,    35,   104,   -95,    18,   104,    24,    41,
     185,   242,   243,   168,   190,   213,    39,   214,    50,    99,
      99,    99,   174,   249,    52,   222,    94,    94,    94,    54,
     207,   208,   221,   223,   134,   209,   224,   180,   257,   210,
     138,   181,   195,   132,   174,   133,    99,   149,    74,   183,
     140,   149,   259,    94,   160,   162,   188,   163,   160,   105,
     160,   105,    43,   123,   124,    53,   104,   104,   104,   236,
     237,   238,   239,   107,   108,   120,   122,   109,   110,   111,
     112,   113,   114,   115,   149,   169,   105,   170,   227,   202,
     203,   160,   219,   104,   204,   205,   171,    10,    10,   172,
     168,    10,   247,   129,   129,   253,   251,   129,   -96,    99,
     225,  -125,   235,   176,   226,   177,    94,   231,   232,   244,
     233,   234,   240,   241,   178,   136,    10,   187,    99,   197,
     197,   189,   197,    99,    99,    94,   201,   206,    99,   149,
      94,    94,   215,   149,   216,    94,   160,   124,   220,   105,
     160,   228,   229,   230,   245,   246,   104,   248,   258,    14,
     254,    36,    40,    55,    56,    37,    51,   173,   105,   179,
     218,   200,     0,   105,   160,   104,     0,     0,   105,     0,
     104,   104,     0,     0,     0,   104,     0,     0,   197,   197,
     197,   197,     0,   197,   197,   197,   197,   197,   197,   197,
     197,     1,    58,     0,     0,     0,    59,    60,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,    63,    64,
      65,     0,    66,    67,    68,    69,    70,    71,    72,    73,
       0,    74,     0,    52,    75,    76,     1,    58,     0,     0,
       0,    59,    60,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   130,   131,    64,    65,     0,    66,    67,    68,
      69,    70,    71,    72,    73,     0,    74,     0,    52,    75,
      76,     1,    58,     0,     0,     0,    59,    60,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   130,   131,    64,
      65,     0,    66,    67,    68,    69,    70,    71,    72,    73,
       0,    74,     0,    52,   165,    76,     1,    58,     0,     0,
       0,    59,    60,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   130,   131,    64,    65,     0,    66,    67,    68,
      69,    70,    71,    72,    73,     0,    74,     0,    52,   167,
      76,     1,    58,     0,     0,     0,    59,    60,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   130,   131,    64,
      65,     0,    66,    67,    68,    69,    70,    71,    72,    73,
       0,    74,     0,    52,   217,    76,     1,    58,     0,     0,
       0,    59,    60,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   130,   131,    64,    65,     0,    66,    67,    68,
      69,    70,    71,    72,    73,     0,    74,     0,    52,   252,
      76,     1,    58,     0,     0,     0,    59,    60,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   130,   131,    64,
      65,     0,    66,    67,    68,    69,    70,    71,    72,    73,
       0,    74,     0,    52,   255,    76,     1,    58,     0,     0,
       0,    59,    60,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   130,   131,    64,    65,     0,    66,    67,    68,
      69,    70,    71,    72,    73,     0,    74,     0,    52,     0,
      76,     1,    58,     0,     0,     0,    59,    60,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     1,    58,   143,
     144,     0,    59,    60,    61,     0,     0,   130,   131,     0,
       0,     0,     0,     0,    68,   143,   144,    71,     0,     0,
       0,    74,     0,   130,   131,   145,     0,     0,     0,     0,
      68,     0,     0,    71,     1,    58,     0,    74,   184,    59,
      60,    61,     1,    58,     0,     0,     0,    59,    60,    61,
       0,     0,   143,   144,     0,     0,     0,     0,     0,     0,
     130,   131,     0,     0,     0,     0,     0,    68,   130,   131,
      71,     0,     0,     0,    74,    68,     0,     0,    71,     0,
       0,     0,    74
};

static const yytype_int16 yycheck[] =
{
       0,    41,     2,    17,   140,     5,     0,    80,    41,    43,
       0,     5,    52,   139,    77,   141,    50,    17,    18,    52,
       3,   143,   144,    23,    18,    25,    70,     3,     3,    23,
      74,    25,    65,    33,    67,     3,    11,    77,    38,    27,
      80,    41,    62,    63,    77,     3,     3,    80,    41,     7,
      50,    27,    52,     3,    68,    28,    50,     7,     5,    52,
      26,    44,    69,    70,     3,    43,    13,    74,    68,    69,
      70,     4,    45,    20,    74,    43,    19,    77,    21,    43,
      80,    14,    50,    44,    77,    49,    45,    80,    46,    45,
     134,   213,   214,   166,   138,    23,    46,    25,    43,   139,
     140,   141,   102,   229,    45,    44,   139,   140,   141,    47,
      15,    16,   177,   178,    43,    20,   181,    44,   254,    24,
      43,    48,   142,    47,   124,    47,   166,   134,    43,   129,
      43,   138,   258,   166,   134,    47,   136,    47,   138,   139,
     140,   141,    32,    47,    48,    35,   139,   140,   141,   207,
     208,   209,   210,    15,    16,    45,    46,    19,    20,    21,
      22,    23,    24,    25,   171,    47,   166,    47,   188,    12,
      13,   171,   172,   166,    17,    18,    49,   177,   178,    50,
     253,   181,   226,   177,   178,   248,   230,   181,    49,   229,
      44,    50,   206,    43,    48,    43,   229,   202,   203,   219,
     204,   205,   211,   212,    43,    50,   206,    47,   248,   143,
     144,    47,   146,   253,   254,   248,    47,    14,   258,   226,
     253,   254,    47,   230,    44,   258,   226,    48,    44,   229,
     230,    44,    33,    47,    44,    44,   229,    45,    44,     5,
      47,    22,    25,    38,    38,    23,    34,   102,   248,   124,
     171,   146,    -1,   253,   254,   248,    -1,    -1,   258,    -1,
     253,   254,    -1,    -1,    -1,   258,    -1,    -1,   202,   203,
     204,   205,    -1,   207,   208,   209,   210,   211,   212,   213,
     214,     3,     4,    -1,    -1,    -1,     8,     9,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    30,    31,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    43,    -1,    45,    46,    47,     3,     4,    -1,    -1,
      -1,     8,     9,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    30,    31,    32,    -1,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    43,    -1,    45,    46,
      47,     3,     4,    -1,    -1,    -1,     8,     9,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    30,    31,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    43,    -1,    45,    46,    47,     3,     4,    -1,    -1,
      -1,     8,     9,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    30,    31,    32,    -1,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    43,    -1,    45,    46,
      47,     3,     4,    -1,    -1,    -1,     8,     9,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    30,    31,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    43,    -1,    45,    46,    47,     3,     4,    -1,    -1,
      -1,     8,     9,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    30,    31,    32,    -1,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    43,    -1,    45,    46,
      47,     3,     4,    -1,    -1,    -1,     8,     9,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    30,    31,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    43,    -1,    45,    46,    47,     3,     4,    -1,    -1,
      -1,     8,     9,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    30,    31,    32,    -1,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    43,    -1,    45,    -1,
      47,     3,     4,    -1,    -1,    -1,     8,     9,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,    21,
      22,    -1,     8,     9,    10,    -1,    -1,    29,    30,    -1,
      -1,    -1,    -1,    -1,    36,    21,    22,    39,    -1,    -1,
      -1,    43,    -1,    29,    30,    47,    -1,    -1,    -1,    -1,
      36,    -1,    -1,    39,     3,     4,    -1,    43,    44,     8,
       9,    10,     3,     4,    -1,    -1,    -1,     8,     9,    10,
      -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      29,    30,    -1,    -1,    -1,    -1,    -1,    36,    29,    30,
      39,    -1,    -1,    -1,    43,    36,    -1,    -1,    39,    -1,
      -1,    -1,    43
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    27,    52,    53,    54,    55,   114,   115,   116,
     121,   121,     0,    54,    53,    55,    26,    28,    45,    56,
      54,    43,   116,     7,    46,    57,    58,    59,    60,    61,
      64,    67,    68,   114,   121,    44,    56,    68,   114,    46,
      58,    45,    69,    75,    11,    62,    65,    82,    83,   121,
      43,    71,    45,    75,    47,    62,    65,   121,     4,     8,
       9,    10,    29,    30,    31,    32,    34,    35,    36,    37,
      38,    39,    40,    41,    43,    46,    47,    70,    75,    76,
      77,    78,    79,    80,    81,    84,    85,    86,    90,    91,
      92,    93,    94,    95,    96,    97,    98,   101,   102,   111,
     112,   113,   114,   117,   118,   121,   122,    15,    16,    19,
      20,    21,    22,    23,    24,    25,   123,   124,   125,    63,
      75,    66,    75,    47,    48,    71,    44,    72,    73,   114,
      29,    30,    47,    47,    43,   119,    50,   119,    43,    96,
      43,    96,   116,    21,    22,    47,    96,    99,   100,   101,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   118,
     121,    99,    47,    47,    99,    46,    77,    46,    76,    47,
      47,    49,    50,    82,   121,   119,    43,    43,    43,    83,
      44,    48,    74,   121,    44,    99,   120,    47,   121,    47,
      99,    79,    87,    98,    79,   119,   109,   117,   121,   109,
     110,    47,    12,    13,    17,    18,    14,    15,    16,    20,
      24,    19,    21,    23,    25,    47,    44,    46,   100,   121,
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
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
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
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       );
      YYFPRINTF (stderr, "\n");
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


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

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
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[2];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

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
  yylloc.first_column = yylloc.last_column = 1;
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
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
	YYSTACK_RELOCATE (yyls_alloc, yyls);
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

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
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

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
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

/* Line 1455 of yacc.c  */
#line 208 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, (yyvsp[(1) - (1)].mainBlock));
	  ast = (yyval.seq);
	}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 213 "parse.yy"
    {
	  (yyval.seq) = (yyvsp[(2) - (2)].seq)->add((yyvsp[(1) - (2)].mainBlock));
	  ast = (yyval.seq);
	}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 218 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, (yyvsp[(2) - (2)].mainBlock));
	  (yyval.seq)->add((yyvsp[(1) - (2)].seq));
	  ast = (yyval.seq);
	}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 224 "parse.yy"
    {
	  (yyval.seq) = (yyvsp[(3) - (3)].seq)->add((yyvsp[(2) - (3)].mainBlock))->add((yyvsp[(1) - (3)].seq));
	  ast = (yyval.seq);
	}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 232 "parse.yy"
    {
	  if (*((yyvsp[(1) - (5)].type)->getName()) != "Integer") {
	     cerr << "mc: " << (yyloc).first_line << ": parse error\n";
	     return 1;
	  }

	  (yyval.mainBlock) = new MainBlockNode((yyvsp[(5) - (5)].blockStmnt));
	}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 244 "parse.yy"
    {
	  (yyval.seq) = new Seq((yyvsp[(1) - (2)].seq), (yyvsp[(2) - (2)].classDec));
	}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 248 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, (yyvsp[(1) - (1)].classDec));
	}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 255 "parse.yy"
    {
	  (yyval.classDec) = new ClassDecNode((yyvsp[(2) - (3)].str), (yyvsp[(3) - (3)].seq));
	  (yyval.classDec)->setLineNumber((yyloc).first_line);
	}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 260 "parse.yy"
    {
	  (yyval.classDec) = new ClassDecNode((yyvsp[(2) - (5)].str), ((ClassType *)(yyvsp[(4) - (5)].type))->getName(), (yyvsp[(5) - (5)].seq));
	  delete (yyvsp[(4) - (5)].type);
	  (yyval.classDec)->setLineNumber((yyloc).first_line);
	}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 269 "parse.yy"
    {
	  (yyval.seq) = (yyvsp[(2) - (3)].seq);
	}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 273 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, 0);
	}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 280 "parse.yy"
    {
	  (yyval.seq) = new Seq((yyvsp[(1) - (2)].seq), (yyvsp[(2) - (2)].node));
	}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 284 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, (yyvsp[(1) - (1)].node));
	}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 291 "parse.yy"
    {
	  (yyval.node) = (yyvsp[(1) - (1)].node);
	}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 295 "parse.yy"
    {
	  (yyval.node) = (yyvsp[(1) - (1)].classConst);
	}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 302 "parse.yy"
    {
	  (yyval.node) = (yyvsp[(1) - (1)].classFieldDec);
	}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 306 "parse.yy"
    {
	  (yyval.node) = (yyvsp[(1) - (1)].classMeth);
	}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 310 "parse.yy"
    {
	  (yyval.node) = (yyvsp[(1) - (1)].classMeth);
	}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 317 "parse.yy"
    {
	  (yyval.classFieldDec) = new ClassFieldDecNode((yyvsp[(1) - (3)].type), (yyvsp[(2) - (3)].seq));
	  (yyval.classFieldDec)->setLineNumber((yyloc).first_line);
	}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 325 "parse.yy"
    {
	  (yyvsp[(3) - (4)].classMeth)->setNativeIndex((yyvsp[(1) - (4)].value));
	  (yyvsp[(3) - (4)].classMeth)->setType((yyvsp[(2) - (4)].type));
	  (yyvsp[(3) - (4)].classMeth)->setBody(new BlockStatementNode());
	  (yyval.classMeth) = (yyvsp[(3) - (4)].classMeth);
	  (yyval.classMeth)->setLineNumber((yyloc).first_line);
	}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 333 "parse.yy"
    {
	  (yyvsp[(2) - (3)].classMeth)->setType((yyvsp[(1) - (3)].type));
	  (yyvsp[(2) - (3)].classMeth)->setBody((yyvsp[(3) - (3)].blockStmnt));
	  (yyval.classMeth) = (yyvsp[(2) - (3)].classMeth);
	  (yyval.classMeth)->setLineNumber((yyloc).first_line);
	}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 343 "parse.yy"
    {
	  (yyval.classMeth) = new ClassMethodNode((yyvsp[(1) - (2)].str), (yyvsp[(2) - (2)].seq));
	}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 350 "parse.yy"
    {
	  (yyval.blockStmnt) = (yyvsp[(1) - (1)].blockStmnt);
	}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 357 "parse.yy"
    {
	  (yyvsp[(3) - (4)].classMeth)->setNativeIndex((yyvsp[(1) - (4)].value));
	  (yyvsp[(3) - (4)].classMeth)->setType((yyvsp[(2) - (4)].type));
	  (yyvsp[(3) - (4)].classMeth)->setBody(new BlockStatementNode());
	  (yyval.classMeth) = (yyvsp[(3) - (4)].classMeth);
	  (yyval.classMeth)->setLineNumber((yyloc).first_line);
	}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 365 "parse.yy"
    {
	  (yyvsp[(2) - (3)].classMeth)->setType((yyvsp[(1) - (3)].type));
	  (yyvsp[(2) - (3)].classMeth)->setBody((yyvsp[(3) - (3)].blockStmnt));
	  (yyval.classMeth) = (yyvsp[(2) - (3)].classMeth);
	  (yyval.classMeth)->setLineNumber((yyloc).first_line);
	}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 375 "parse.yy"
    {
	  (yyval.classMeth) = new ClassMethodNode((yyvsp[(2) - (5)].str), new Seq(0, (yyvsp[(4) - (5)].varDec)));
	}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 379 "parse.yy"
    {
	  (yyval.classMeth) = new ClassMethodNode((yyvsp[(2) - (5)].str), new Seq(0, (yyvsp[(4) - (5)].varDec)));
	}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 383 "parse.yy"
    {
	  (yyval.classMeth) = new ClassMethodNode((yyvsp[(2) - (4)].str), new Seq(0, 0));
	}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 387 "parse.yy"
    {
	  (yyval.classMeth) = new ClassMethodNode((yyvsp[(2) - (4)].str), new Seq(0, 0));
	}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 394 "parse.yy"
    {
	  (yyval.blockStmnt) = (yyvsp[(1) - (1)].blockStmnt);
	}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 401 "parse.yy"
    {
	  (yyvsp[(2) - (3)].classConst)->setNativeIndex((yyvsp[(1) - (3)].value));
	  (yyvsp[(2) - (3)].classConst)->setBody(new BlockStatementNode());
	  (yyval.classConst) = (yyvsp[(2) - (3)].classConst);
	}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 407 "parse.yy"
    {
	  (yyvsp[(1) - (2)].classConst)->setBody((yyvsp[(2) - (2)].blockStmnt));
	  (yyval.classConst) = (yyvsp[(1) - (2)].classConst);
	}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 415 "parse.yy"
    {
	  (yyval.classConst) = new ClassConstructorNode((yyvsp[(1) - (2)].str), (yyvsp[(2) - (2)].seq));
	  (yyval.classConst)->setLineNumber((yyloc).first_line);
	}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 423 "parse.yy"
    {
	  (yyval.blockStmnt) = new BlockStatementNode((yyvsp[(3) - (4)].seq)->add((yyvsp[(2) - (4)].stmnt)));
	}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 427 "parse.yy"
    {
	  (yyval.blockStmnt) = new BlockStatementNode(new Seq(0, (yyvsp[(2) - (3)].stmnt)));
	}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 431 "parse.yy"
    {
	  (yyvsp[(1) - (1)].blockStmnt)->add(new ConstructorInvocationStatementNode(new SuperExpressionNode(), new Seq(0, 0)));
	  (yyval.blockStmnt) = (yyvsp[(1) - (1)].blockStmnt);
	}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 439 "parse.yy"
    {
	  (yyval.stmnt) = new ConstructorInvocationStatementNode(new ThisExpressionNode(), (yyvsp[(2) - (3)].seq));
	}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 443 "parse.yy"
    {
	  (yyval.stmnt) = new ConstructorInvocationStatementNode(new SuperExpressionNode(), (yyvsp[(2) - (3)].seq));
	}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 450 "parse.yy"
    {
	  (yyval.seq) = (yyvsp[(2) - (3)].seq);
	}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 454 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, 0);
	}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 461 "parse.yy"
    {
	  (yyval.seq) = new Seq((yyvsp[(1) - (3)].seq), (yyvsp[(3) - (3)].varDec));
	}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 465 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, (yyvsp[(1) - (1)].varDec));
	}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 472 "parse.yy"
    {
	  (yyval.varDec) = new VarDecNode((yyvsp[(1) - (2)].type), new Seq(0, (yyvsp[(2) - (2)].varNode)));
	  (yyval.varDec)->setLineNumber((yyloc).first_line);
	}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 480 "parse.yy"
    {
	  (yyval.varNode) = new VarNode((yyvsp[(1) - (1)].str));
	  (yyval.varNode)->setLineNumber((yyloc).first_line);
	}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 488 "parse.yy"
    {
	  (yyval.blockStmnt) = new BlockStatementNode((yyvsp[(2) - (3)].seq));
	}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 492 "parse.yy"
    {
	  (yyval.blockStmnt) = new BlockStatementNode(new Seq(0, 0));
	}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 499 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 506 "parse.yy"
    {
	  (yyval.seq) = new Seq((yyvsp[(1) - (2)].seq), (yyvsp[(2) - (2)].stmnt));
	}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 510 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, (yyvsp[(1) - (1)].stmnt));
	}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 517 "parse.yy"
    {
	  (yyval.stmnt) = new SynchronizedStatementNode((yyvsp[(3) - (7)].expr), (yyvsp[(6) - (7)].seq));
	}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 521 "parse.yy"
    {
	  (yyval.stmnt) = new SynchronizedStatementNode((yyvsp[(3) - (6)].expr), new Seq(0, 0));
	}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 528 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].blockStmnt);
	}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 532 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 536 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 540 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 544 "parse.yy"
    {
	  (yyvsp[(1) - (1)].expr)->setIsStatement(true);
	  (yyval.stmnt) = new ExpressionStatementNode((yyvsp[(1) - (1)].expr));
	}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 549 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 553 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 557 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 561 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 565 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 569 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 573 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 577 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 584 "parse.yy"
    {
	  (yyval.stmnt) = new LocalVarDecStatementNode((yyvsp[(1) - (2)].localVarDec));
	}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 591 "parse.yy"
    {
	  (yyval.localVarDec) = new LocalVarDecNode((yyvsp[(1) - (2)].type), (yyvsp[(2) - (2)].seq));
	  (yyval.localVarDec)->setLineNumber((yyloc).first_line);
	}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 599 "parse.yy"
    {
	  (yyval.seq) = new Seq((yyvsp[(1) - (3)].seq), (yyvsp[(3) - (3)].varNode));
	}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 603 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, (yyvsp[(1) - (1)].varNode));
	}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 610 "parse.yy"
    {
	  (yyval.varNode) = new VarNode((yyvsp[(1) - (1)].str));
	  (yyval.varNode)->setLineNumber((yyloc).first_line);
	}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 618 "parse.yy"
    {
	  (yyval.stmnt) = new IfThenStatementNode((yyvsp[(2) - (3)].expr), (yyvsp[(3) - (3)].stmnt));
	  (yyval.stmnt)->setLineNumber((yyloc).first_line);
	}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 626 "parse.yy"
    {
	  (yyval.stmnt) = new IfThenElseStatementNode((yyvsp[(2) - (5)].expr), (yyvsp[(3) - (5)].stmnt), (yyvsp[(5) - (5)].stmnt));
	  (yyval.stmnt)->setLineNumber((yyloc).first_line);
	}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 634 "parse.yy"
    {
	  BlockStatementNode *retval, *body;

	  body = new BlockStatementNode();
	  body->add((yyvsp[(7) - (9)].node));
	  body->add((yyvsp[(9) - (9)].stmnt));

	  retval = new BlockStatementNode();
	  retval->add(new WhileStatementNode((yyvsp[(5) - (9)].expr), body));
	  retval->add((yyvsp[(3) - (9)].node));

	  retval->setLineNumber((yyloc).first_line);

	  (yyval.stmnt) = retval;
	}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 653 "parse.yy"
    {
	  (yyval.node) = (yyvsp[(1) - (1)].expr);
	  ((ExpressionNode *)(yyvsp[(1) - (1)].expr))->setIsStatement(true);
	}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 658 "parse.yy"
    {
          (yyval.node) = new BlockStatementNode(new Seq(0, 0));
	}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 665 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 669 "parse.yy"
    {
          (yyval.expr) = new IntegerLiteralNode(1);
	}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 676 "parse.yy"
    {
	  (yyval.node) = (yyvsp[(1) - (1)].expr);
	  ((ExpressionNode *)(yyvsp[(1) - (1)].expr))->setIsStatement(true);
	}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 681 "parse.yy"
    {
          (yyval.node) = new BlockStatementNode(new Seq(0, 0));
	}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 688 "parse.yy"
    {
	  (yyval.stmnt) = new WhileStatementNode((yyvsp[(2) - (3)].expr), (yyvsp[(3) - (3)].stmnt));
	  (yyval.stmnt)->setLineNumber((yyloc).first_line);
	}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 696 "parse.yy"
    {
	  (yyval.stmnt) = new ReturnStatementNode();
	}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 700 "parse.yy"
    {
	  (yyval.stmnt) = new ReturnStatementNode((yyvsp[(2) - (3)].expr));
	}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 707 "parse.yy"
    {
	  (yyval.stmnt) = new OutStatementNode((yyvsp[(2) - (3)].expr));
	}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 714 "parse.yy"
    {
	  (yyval.stmnt) = new BreakStatementNode();
	}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 721 "parse.yy"
    {
	  (yyval.stmnt) = new ContinueStatementNode();
	}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 728 "parse.yy"
    {
	  (yyval.stmnt) = new StatementNode();
	}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 735 "parse.yy"
    {
	  (yyvsp[(2) - (3)].expr)->setIsParenthesized(true);
	  (yyval.expr) = (yyvsp[(2) - (3)].expr);
	}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 743 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (2)].expr);
	}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 750 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 754 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 761 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 768 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 772 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 779 "parse.yy"
    {
	  (yyval.expr) = new AssignmentOperatorNode((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
	}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 786 "parse.yy"
    {
	  (yyval.expr) = new DerefNode(new VarNode((yyvsp[(1) - (1)].str)));
	}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 790 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 797 "parse.yy"
    {
	  (yyval.expr) = new BooleanAndOperatorNode((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
	}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 801 "parse.yy"
    {
	  (yyval.expr) = new BooleanOrOperatorNode((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
	}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 805 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 812 "parse.yy"
    {
	  (yyval.expr) = new EqualityOperatorNode((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
	}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 816 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode(new EqualityOperatorNode((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)),
	           (yyvsp[(2) - (3)].str), new Seq(0, 0));
	}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 821 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 828 "parse.yy"
    {
	  (yyval.expr) = new InstanceOfExpressionNode((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].type));
	}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 832 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 839 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].str), new Seq(0, (yyvsp[(3) - (3)].expr)));
	}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 843 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].str), new Seq(0, (yyvsp[(3) - (3)].expr)));
	}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 847 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].str), new Seq(0, (yyvsp[(3) - (3)].expr)));
	}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 851 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].str), new Seq(0, (yyvsp[(3) - (3)].expr)));
	}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 855 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 862 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].str), new Seq(0, (yyvsp[(3) - (3)].expr)));
	}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 866 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].str), new Seq(0, (yyvsp[(3) - (3)].expr)));
	}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 870 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 877 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].str), new Seq(0, (yyvsp[(3) - (3)].expr)));
	}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 881 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].str), new Seq(0, (yyvsp[(3) - (3)].expr)));
	}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 885 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 892 "parse.yy"
    {
	  (yyvsp[(2) - (2)].expr)->setParentIsNegate(true);
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(2) - (2)].expr), (yyvsp[(1) - (2)].str), new Seq(0, 0));
	}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 897 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(2) - (2)].expr), (yyvsp[(1) - (2)].str), new Seq(0, 0));
	}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 901 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 908 "parse.yy"
    {
	  (yyval.expr) = new CastExpressionNode((yyvsp[(1) - (2)].expr), (yyvsp[(2) - (2)].expr));
	}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 912 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 919 "parse.yy"
    {
	  (yyval.expr) = new DerefNode(new VarNode((yyvsp[(1) - (1)].str)));
	}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 923 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 927 "parse.yy"
    {
	  (yyval.expr) = new ThisExpressionNode();
	}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 931 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 935 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 939 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 943 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 947 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 954 "parse.yy"
    {
	  (yyval.expr) = new InExpressionNode();
	}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 961 "parse.yy"
    {
	  (yyval.expr) = new ClassInstanceCreationExpressionNode((yyvsp[(2) - (3)].type)->getName(), (yyvsp[(3) - (3)].seq));
	  delete (yyvsp[(2) - (3)].type);
	}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 969 "parse.yy"
    {
	  (yyval.type) = (yyvsp[(1) - (1)].type);
	}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 976 "parse.yy"
    {
	  (yyval.type) = (yyvsp[(1) - (1)].type);
	}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 983 "parse.yy"
    {
	  (yyval.type) = new ClassType((yyvsp[(1) - (1)].str));
	}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 990 "parse.yy"
    {
	  (yyval.expr) = new DerefNode(new FieldAccessOperatorNode((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].str)));
	}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 994 "parse.yy"
    {
	  (yyval.expr) = new DerefNode(new FieldAccessOperatorNode(new SuperExpressionNode(), (yyvsp[(3) - (3)].str)));
	}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 1001 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode(new ThisExpressionNode(), (yyvsp[(1) - (2)].str), (yyvsp[(2) - (2)].seq));
	}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 1005 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(1) - (4)].expr), (yyvsp[(3) - (4)].str), (yyvsp[(4) - (4)].seq));
	}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 1009 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode(new SuperExpressionNode(), (yyvsp[(3) - (4)].str), (yyvsp[(4) - (4)].seq));
	}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 1016 "parse.yy"
    {
	  (yyval.seq) = (yyvsp[(2) - (3)].seq);
	}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 1020 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, 0);
	}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 1027 "parse.yy"
    {
	  (yyval.seq) = new Seq((yyvsp[(1) - (3)].seq), (yyvsp[(3) - (3)].expr));
	}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 1031 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, (yyvsp[(1) - (1)].expr));
	}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 1038 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);
	}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 1045 "parse.yy"
    {
	  (yyval.expr) = new IntegerLiteralNode((yyvsp[(1) - (1)].value));
	}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 1049 "parse.yy"
    {
	  (yyval.expr) = new RealLiteralNode((yyvsp[(1) - (1)].float_value));
	}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 1053 "parse.yy"
    {
	  (yyval.expr) = new NullLiteralNode();
	}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 1057 "parse.yy"
    {
	  (yyval.expr) = new StringLiteralNode((yyvsp[(1) - (1)].str));
	}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 1064 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);
	}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 1071 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);
	}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 1075 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);
	}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 1079 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);
	}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 1083 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);
	}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 1087 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);
	}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 1091 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);
	}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 1095 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);
	}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 1102 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);
	}
    break;



/* Line 1455 of yacc.c  */
#line 3318 "parse.cc"
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
      /* If just tried and failed to reuse lookahead token after an
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

  /* Else will try to reuse lookahead token after shifting the error
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

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
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

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
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



/* Line 1675 of yacc.c  */
#line 1107 "parse.yy"


void yyerror(const char *s) {
        cerr << "mc: " << getCurrentSourceLineNumber() << ": " << s << '\n';
}

