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
     NATIVE = 262,
     INTEGER_LITERAL = 263,
     NULL_LITERAL = 264,
     OPERATOR = 265,
     EQ_OP = 266,
     INSTANCEOF_OP = 267,
     MAIN = 268,
     CLASS = 269,
     EXTENDS = 270,
     THIS = 271,
     SUPER = 272,
     SYNCHRONIZED = 273,
     IF = 274,
     ELSE = 275,
     WHILE = 276,
     NEW = 277,
     RETURN = 278,
     OUT = 279,
     IN = 280,
     BREAK = 281,
     CONTINUE = 282,
     BAD = 283
   };
#endif
/* Tokens.  */
#define IDENTIFIER 258
#define STRING_LITERAL 259
#define NEWLINE 260
#define TAB 261
#define NATIVE 262
#define INTEGER_LITERAL 263
#define NULL_LITERAL 264
#define OPERATOR 265
#define EQ_OP 266
#define INSTANCEOF_OP 267
#define MAIN 268
#define CLASS 269
#define EXTENDS 270
#define THIS 271
#define SUPER 272
#define SYNCHRONIZED 273
#define IF 274
#define ELSE 275
#define WHILE 276
#define NEW 277
#define RETURN 278
#define OUT 279
#define IN 280
#define BREAK 281
#define CONTINUE 282
#define BAD 283




/* Copy the first part of user declarations.  */
#line 1 "parse.yy"


// Niels Widger
// CS 712
// Time-stamp: <16 Nov 2010 at 20:36:17 by nwidger on macros.local>

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

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 33 "parse.yy"
{
	Seq *seq;

        unsigned long value;
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
/* Line 193 of yacc.c.  */
#line 213 "parse.cc"
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
#line 238 "parse.cc"

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
# if defined YYENABLE_NLS && YYENABLE_NLS
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
#define YYLAST   500

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  44
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  70
/* YYNRULES -- Number of rules.  */
#define YYNRULES  138
/* YYNRULES -- Number of states.  */
#define YYNSTATES  230

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   283

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    16,     2,     2,     2,     2,     2,     2,
      36,    37,    19,    13,    41,    15,    43,    17,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    40,
      18,    42,    14,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    38,     2,    39,     2,     2,     2,     2,
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
       5,     6,     7,     8,     9,    10,    11,    12,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35
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
     168,   170,   176,   178,   180,   182,   184,   186,   188,   190,
     192,   194,   196,   198,   200,   203,   206,   210,   212,   214,
     218,   224,   228,   231,   235,   239,   242,   245,   247,   251,
     254,   256,   258,   260,   262,   264,   268,   270,   272,   276,
     278,   282,   284,   288,   292,   294,   298,   302,   304,   308,
     312,   314,   317,   320,   322,   325,   327,   329,   331,   333,
     335,   337,   339,   341,   343,   345,   349,   351,   353,   355,
     359,   363,   366,   371,   376,   380,   383,   387,   389,   391,
     393,   395,   397,   399,   401,   403,   405,   407,   409
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      45,     0,    -1,    46,    -1,    46,    47,    -1,    47,    46,
      -1,    47,    46,    47,    -1,   102,    20,    36,    37,    68,
      -1,    47,    48,    -1,    48,    -1,    21,   109,    49,    -1,
      21,   109,    22,   104,    49,    -1,    38,    50,    39,    -1,
      38,    39,    -1,    50,    51,    -1,    51,    -1,    52,    -1,
      60,    -1,    53,    -1,    54,    -1,    57,    -1,   102,    75,
      40,    -1,     7,   102,    55,    40,    -1,   102,    55,    56,
      -1,   109,    64,    -1,    68,    -1,     7,   102,    58,    40,
      -1,   102,    58,    59,    -1,    10,   112,    36,    66,    37,
      -1,    10,   113,    36,    66,    37,    -1,    10,   111,    36,
      37,    -1,    10,   113,    36,    37,    -1,    68,    -1,     7,
      61,    40,    -1,    61,    62,    -1,   109,    64,    -1,    38,
      63,    70,    39,    -1,    38,    63,    39,    -1,    68,    -1,
      23,   107,    40,    -1,    24,   107,    40,    -1,    36,    65,
      37,    -1,    36,    37,    -1,    65,    41,    66,    -1,    66,
      -1,   102,    67,    -1,   109,    -1,    38,    70,    39,    -1,
      38,    39,    -1,    72,    -1,    70,    69,    -1,    69,    -1,
      25,    36,    88,    37,    68,    -1,    68,    -1,    71,    -1,
      84,    -1,    73,    -1,    86,    -1,    77,    -1,    78,    -1,
      79,    -1,    80,    -1,    81,    -1,    82,    -1,    83,    -1,
      74,    40,    -1,   102,    75,    -1,    75,    41,    76,    -1,
      76,    -1,   109,    -1,    26,    85,    72,    -1,    26,    85,
      72,    27,    72,    -1,    28,    85,    72,    -1,    30,    40,
      -1,    30,    88,    40,    -1,    31,    88,    40,    -1,    33,
      40,    -1,    34,    40,    -1,    40,    -1,    36,    88,    37,
      -1,    87,    40,    -1,    90,    -1,   106,    -1,    89,    -1,
      90,    -1,    92,    -1,    91,    42,    89,    -1,   109,    -1,
     105,    -1,    92,    11,    93,    -1,    93,    -1,    93,    12,
     104,    -1,    94,    -1,    94,    18,    95,    -1,    94,    14,
      95,    -1,    95,    -1,    95,    13,    96,    -1,    95,    15,
      96,    -1,    96,    -1,    96,    19,    97,    -1,    96,    17,
      97,    -1,    97,    -1,    15,    97,    -1,    16,    97,    -1,
      98,    -1,    85,    98,    -1,    99,    -1,   109,    -1,    85,
      -1,    23,    -1,   105,    -1,   106,    -1,   101,    -1,   110,
      -1,   100,    -1,    32,    -1,    29,   104,   107,    -1,   103,
      -1,   104,    -1,   109,    -1,    99,    43,   109,    -1,    24,
      43,   109,    -1,   109,   107,    -1,    99,    43,   109,   107,
      -1,    24,    43,   109,   107,    -1,    36,   108,    37,    -1,
      36,    37,    -1,   108,    41,    88,    -1,    88,    -1,     3,
      -1,     8,    -1,     9,    -1,     4,    -1,    16,    -1,    13,
      -1,    19,    -1,    17,    -1,    14,    -1,    18,    -1,    15,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   194,   194,   199,   204,   210,   218,   230,   234,   241,
     246,   255,   259,   266,   270,   277,   281,   288,   292,   296,
     303,   311,   319,   329,   336,   343,   351,   361,   365,   369,
     373,   380,   387,   393,   401,   409,   413,   417,   425,   429,
     436,   440,   447,   451,   458,   466,   474,   478,   485,   492,
     496,   503,   510,   514,   518,   522,   526,   531,   535,   539,
     543,   547,   551,   555,   562,   569,   577,   581,   588,   596,
     604,   612,   620,   624,   631,   638,   645,   652,   659,   667,
     674,   678,   685,   692,   696,   703,   710,   714,   721,   725,
     732,   736,   743,   747,   751,   758,   762,   766,   773,   777,
     781,   788,   793,   797,   804,   808,   815,   819,   823,   827,
     831,   835,   839,   843,   850,   857,   865,   872,   879,   886,
     890,   897,   901,   905,   912,   916,   923,   927,   934,   941,
     945,   949,   956,   963,   967,   971,   975,   979,   986
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "STRING_LITERAL",
  "NEWLINE", "TAB", "NATIVE", "INTEGER_LITERAL", "NULL_LITERAL",
  "OPERATOR", "EQ_OP", "INSTANCEOF_OP", "'+'", "'>'", "'-'", "'!'", "'/'",
  "'<'", "'*'", "MAIN", "CLASS", "EXTENDS", "THIS", "SUPER",
  "SYNCHRONIZED", "IF", "ELSE", "WHILE", "NEW", "RETURN", "OUT", "IN",
  "BREAK", "CONTINUE", "BAD", "'('", "')'", "'{'", "'}'", "';'", "','",
  "'='", "'.'", "$accept", "CompilationUnit", "MainFunctionDeclaration",
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
  "IfThenStatement", "IfThenElseStatement", "WhileStatement",
  "ReturnStatement", "OutputStatement", "BreakStatement",
  "ContinueStatement", "EmptyStatement", "ParenExpression",
  "ExpressionStatement", "StatementExpression", "Expression",
  "AssignmentExpression", "Assignment", "LeftHandSide",
  "EqualityExpression", "InstanceOfExpression", "RelationalExpression",
  "AdditiveExpression", "MultiplicativeExpression", "UnaryExpression",
  "CastExpression", "Primary", "InputExpression",
  "ClassInstanceCreationExpression", "Type", "ReferenceType", "ClassType",
  "FieldAccess", "MethodInvocation", "Arguments", "ArgumentList",
  "Identifier", "Literal", "UnaryClassOperator", "BinaryClassOperator",
  "MinusClassOperator", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,    43,    62,    45,    33,    47,    60,    42,
     268,   269,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,   283,    40,    41,   123,   125,
      59,    44,    61,    46
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    44,    45,    45,    45,    45,    46,    47,    47,    48,
      48,    49,    49,    50,    50,    51,    51,    52,    52,    52,
      53,    54,    54,    55,    56,    57,    57,    58,    58,    58,
      58,    59,    60,    60,    61,    62,    62,    62,    63,    63,
      64,    64,    65,    65,    66,    67,    68,    68,    69,    70,
      70,    71,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    73,    74,    75,    75,    76,    77,
      78,    79,    80,    80,    81,    82,    83,    84,    85,    86,
      87,    87,    88,    89,    89,    90,    91,    91,    92,    92,
      93,    93,    94,    94,    94,    95,    95,    95,    96,    96,
      96,    97,    97,    97,    98,    98,    99,    99,    99,    99,
      99,    99,    99,    99,   100,   101,   102,   103,   104,   105,
     105,   106,   106,   106,   107,   107,   108,   108,   109,   110,
     110,   110,   111,   112,   112,   112,   112,   112,   113
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     2,     3,     5,     2,     1,     3,
       5,     3,     2,     2,     1,     1,     1,     1,     1,     1,
       3,     4,     3,     2,     1,     4,     3,     5,     5,     4,
       4,     1,     3,     2,     2,     4,     3,     1,     3,     3,
       3,     2,     3,     1,     2,     1,     3,     2,     1,     2,
       1,     5,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     3,     1,     1,     3,
       5,     3,     2,     3,     3,     2,     2,     1,     3,     2,
       1,     1,     1,     1,     1,     3,     1,     1,     3,     1,
       3,     1,     3,     3,     1,     3,     3,     1,     3,     3,
       1,     2,     2,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     1,     1,     3,
       3,     2,     4,     4,     3,     2,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,   128,     0,     0,     2,     0,     8,     0,   116,   117,
     118,     0,     1,     3,     4,     7,     0,     0,     0,     9,
       5,     0,     0,     0,    12,     0,    14,    15,    17,    18,
      19,    16,     0,     0,   118,     0,    10,     0,     0,    11,
      13,     0,    33,    37,     0,     0,     0,     0,    67,    68,
       0,    34,     0,     6,    32,     0,     0,     0,   131,   129,
     130,   108,     0,     0,     0,     0,     0,     0,     0,   114,
       0,     0,     0,    47,    77,     0,    52,    50,     0,    53,
      48,    55,     0,    57,    58,    59,    60,    61,    62,    63,
      54,   107,    56,     0,    80,     0,     0,   113,   111,     0,
     109,    81,    86,   112,   133,   136,   138,   132,   135,   137,
     134,     0,     0,     0,    22,    24,    26,    31,    20,     0,
      23,    41,     0,    43,     0,   108,     0,    21,    25,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    72,
     107,     0,    82,    83,    84,    89,    91,    94,    97,   100,
     103,   105,   110,   106,     0,    75,    76,     0,    36,     0,
      46,    49,    64,    79,     0,     0,    65,    68,   121,     0,
       0,     0,    66,    40,     0,    44,    45,   125,   127,     0,
      38,   120,    39,     0,    69,    71,   115,   101,   109,   106,
     102,   104,    73,     0,     0,     0,     0,     0,     0,     0,
       0,    74,    78,    35,    85,   119,    29,     0,    30,     0,
      42,   124,     0,   123,     0,     0,    88,    90,    93,    92,
      95,    96,    99,    98,   122,    27,    28,   126,    51,    70
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    19,    25,    26,    27,    28,
      29,    45,   114,    30,    46,   116,    31,    32,    42,    75,
     120,   122,   123,   175,    76,    77,    78,    79,    80,    81,
      82,    47,    48,    83,    84,    85,    86,    87,    88,    89,
      90,   140,    92,    93,   141,   142,    94,    95,   144,   145,
     146,   147,   148,   149,   150,   151,    97,    98,    99,     8,
       9,   100,   152,   168,   179,   189,   103,   111,   112,   113
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -108
static const yytype_int16 yypact[] =
{
       9,  -108,    13,    34,    16,     9,  -108,    45,  -108,  -108,
    -108,    -2,  -108,    16,    16,  -108,    60,    13,    44,  -108,
      16,    53,    68,    13,  -108,    66,  -108,  -108,  -108,  -108,
    -108,  -108,    73,    54,    79,    87,  -108,    80,    54,  -108,
    -108,   196,  -108,  -108,   162,    87,    87,    41,  -108,    79,
       3,  -108,   234,  -108,  -108,    88,    92,    79,  -108,  -108,
    -108,   103,    57,   105,   107,   107,    13,   424,   462,  -108,
     112,   117,   462,  -108,  -108,   272,  -108,  -108,   310,  -108,
    -108,  -108,   118,  -108,  -108,  -108,  -108,  -108,  -108,  -108,
    -108,  -108,  -108,   120,  -108,   124,   125,  -108,  -108,    13,
     127,   129,    19,  -108,  -108,  -108,  -108,  -108,  -108,  -108,
    -108,   131,   137,   149,  -108,  -108,  -108,  -108,  -108,    13,
    -108,  -108,    75,  -108,    13,  -108,   143,  -108,  -108,   426,
     147,    13,   148,   462,   386,   386,   103,   462,   462,  -108,
     464,   150,  -108,  -108,   178,   184,    95,    82,   121,  -108,
    -108,   125,  -108,    56,   157,  -108,  -108,   164,  -108,   348,
    -108,  -108,  -108,  -108,   462,    13,   161,  -108,  -108,   166,
      13,    26,  -108,  -108,    13,  -108,  -108,  -108,  -108,    86,
    -108,   103,  -108,   169,   180,  -108,  -108,  -108,  -108,   103,
    -108,  -108,  -108,   462,    13,   462,   462,   462,   462,   462,
     462,  -108,  -108,  -108,  -108,   103,  -108,   171,  -108,   172,
    -108,  -108,   462,  -108,    87,   386,   184,  -108,    82,    82,
     121,   121,  -108,  -108,  -108,  -108,  -108,  -108,  -108,  -108
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -108,  -108,   205,    72,    74,   189,  -108,   188,  -108,  -108,
    -108,   176,  -108,  -108,   185,  -108,  -108,   193,  -108,  -108,
     197,  -108,   -67,  -108,   -31,   -75,   158,  -108,  -107,  -108,
    -108,   119,   122,  -108,  -108,  -108,  -108,  -108,  -108,  -108,
    -108,   -33,  -108,  -108,   -59,    76,   -19,  -108,  -108,    46,
    -108,   -40,   -35,   -78,   104,   -17,  -108,  -108,    21,  -108,
     -10,   -49,     2,   -51,  -108,     0,  -108,  -108,  -108,  -108
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -119
static const yytype_int16 yytable[] =
{
      10,    43,    11,   161,    53,    10,     1,    22,    91,   154,
     130,   132,     1,   157,   115,   117,     1,    10,    34,    91,
      17,     7,  -118,    34,    96,    34,     7,   184,   185,     1,
       2,   134,   135,    49,    12,    96,    18,     2,    57,    33,
     121,   102,    91,   101,    38,    91,    33,     1,   143,   143,
      10,    23,   102,   143,   101,   129,   136,     1,    96,   187,
     190,    96,  -106,   208,    44,    16,    10,   153,   153,     1,
     178,   124,   153,    23,   183,   102,    13,   101,   102,    15,
     101,   118,   119,    24,   161,   186,    20,    15,   188,   188,
      35,   188,   129,   129,    15,   197,    21,   198,   -86,   167,
     131,    91,    91,   207,   209,    39,    18,   210,   229,   195,
     143,    41,   173,   196,   143,    50,   174,    96,    96,   167,
      54,   222,   223,   211,   176,    52,    91,   212,   127,   153,
     213,   181,   128,   153,   102,   102,   101,   101,   199,   129,
     200,   133,    96,    72,   188,   143,   188,   188,   188,   188,
     188,   188,   155,   227,   224,   218,   219,   156,   162,   102,
     163,   101,   220,   221,   153,   205,   164,   169,   165,   -87,
      10,    10,  -110,   170,    10,   104,   105,   106,   107,   108,
     109,   110,    91,   228,   217,   171,   131,   180,   182,   193,
     192,   124,   124,   143,    10,   124,   194,   201,    96,     1,
      58,   202,   119,   206,    59,    60,   214,   215,   225,   226,
      14,    36,   153,    40,    55,   102,    37,   101,   166,    61,
      62,    63,    64,    56,    65,    66,    67,    68,    69,    70,
      71,    51,    72,   159,    52,    73,    74,     1,    58,   216,
     204,   172,    59,    60,   191,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   125,   126,    63,
      64,     0,    65,    66,    67,    68,    69,    70,    71,     0,
      72,     0,    52,    73,    74,     1,    58,     0,     0,     0,
      59,    60,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   125,   126,    63,    64,     0,
      65,    66,    67,    68,    69,    70,    71,     0,    72,     0,
      52,   158,    74,     1,    58,     0,     0,     0,    59,    60,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   125,   126,    63,    64,     0,    65,    66,
      67,    68,    69,    70,    71,     0,    72,     0,    52,   160,
      74,     1,    58,     0,     0,     0,    59,    60,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   125,   126,    63,    64,     0,    65,    66,    67,    68,
      69,    70,    71,     0,    72,     0,    52,   203,    74,     1,
      58,     0,     0,     0,    59,    60,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   125,
     126,    63,    64,     0,    65,    66,    67,    68,    69,    70,
      71,     0,    72,     0,    52,     0,    74,     1,    58,     1,
      58,     0,    59,    60,    59,    60,     0,     0,     0,   137,
     138,   137,   138,     0,     0,     0,     0,   125,   126,   125,
     126,     0,     0,    66,     0,    66,    69,     0,    69,     0,
      72,     0,    72,   177,   139,     1,    58,     1,    58,     0,
      59,    60,    59,    60,     0,     0,     0,   137,   138,     0,
       0,     0,     0,     0,     0,   125,   126,   125,   126,     0,
       0,    66,     0,    66,    69,     0,    69,     0,    72,     0,
      72
};

static const yytype_int16 yycheck[] =
{
       0,    32,     2,    78,    35,     5,     3,    17,    41,    68,
      61,    62,     3,    72,    45,    46,     3,    17,    18,    52,
      22,     0,     3,    23,    41,    25,     5,   134,   135,     3,
      21,    64,    65,    33,     0,    52,    38,    21,    38,    18,
      37,    41,    75,    41,    23,    78,    25,     3,    67,    68,
      50,     7,    52,    72,    52,    36,    66,     3,    75,   137,
     138,    78,    43,    37,    10,    20,    66,    67,    68,     3,
     129,    50,    72,     7,   133,    75,     4,    75,    78,     5,
      78,    40,    41,    39,   159,   136,    14,    13,   137,   138,
      37,   140,    36,    36,    20,    13,    36,    15,    42,    99,
      43,   134,   135,   170,   171,    39,    38,   174,   215,    14,
     129,    38,    37,    18,   133,    36,    41,   134,   135,   119,
      40,   199,   200,    37,   124,    38,   159,    41,    40,   129,
     181,   131,    40,   133,   134,   135,   134,   135,    17,    36,
      19,    36,   159,    36,   193,   164,   195,   196,   197,   198,
     199,   200,    40,   212,   205,   195,   196,    40,    40,   159,
      40,   159,   197,   198,   164,   165,    42,    36,    43,    42,
     170,   171,    43,    36,   174,    13,    14,    15,    16,    17,
      18,    19,   215,   214,   194,    36,    43,    40,    40,    11,
      40,   170,   171,   212,   194,   174,    12,    40,   215,     3,
       4,    37,    41,    37,     8,     9,    37,    27,    37,    37,
       5,    22,   212,    25,    38,   215,    23,   215,    99,    23,
      24,    25,    26,    38,    28,    29,    30,    31,    32,    33,
      34,    34,    36,    75,    38,    39,    40,     3,     4,   193,
     164,   119,     8,     9,   140,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    24,    25,
      26,    -1,    28,    29,    30,    31,    32,    33,    34,    -1,
      36,    -1,    38,    39,    40,     3,     4,    -1,    -1,    -1,
       8,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    24,    25,    26,    -1,
      28,    29,    30,    31,    32,    33,    34,    -1,    36,    -1,
      38,    39,    40,     3,     4,    -1,    -1,    -1,     8,     9,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    24,    25,    26,    -1,    28,    29,
      30,    31,    32,    33,    34,    -1,    36,    -1,    38,    39,
      40,     3,     4,    -1,    -1,    -1,     8,     9,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    24,    25,    26,    -1,    28,    29,    30,    31,
      32,    33,    34,    -1,    36,    -1,    38,    39,    40,     3,
       4,    -1,    -1,    -1,     8,     9,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      24,    25,    26,    -1,    28,    29,    30,    31,    32,    33,
      34,    -1,    36,    -1,    38,    -1,    40,     3,     4,     3,
       4,    -1,     8,     9,     8,     9,    -1,    -1,    -1,    15,
      16,    15,    16,    -1,    -1,    -1,    -1,    23,    24,    23,
      24,    -1,    -1,    29,    -1,    29,    32,    -1,    32,    -1,
      36,    -1,    36,    37,    40,     3,     4,     3,     4,    -1,
       8,     9,     8,     9,    -1,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    24,    23,    24,    -1,
      -1,    29,    -1,    29,    32,    -1,    32,    -1,    36,    -1,
      36
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    21,    45,    46,    47,    48,   102,   103,   104,
     109,   109,     0,    47,    46,    48,    20,    22,    38,    49,
      47,    36,   104,     7,    39,    50,    51,    52,    53,    54,
      57,    60,    61,   102,   109,    37,    49,    61,   102,    39,
      51,    38,    62,    68,    10,    55,    58,    75,    76,   109,
      36,    64,    38,    68,    40,    55,    58,   109,     4,     8,
       9,    23,    24,    25,    26,    28,    29,    30,    31,    32,
      33,    34,    36,    39,    40,    63,    68,    69,    70,    71,
      72,    73,    74,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    90,    91,    99,   100,   101,   102,
     105,   106,   109,   110,    13,    14,    15,    16,    17,    18,
      19,   111,   112,   113,    56,    68,    59,    68,    40,    41,
      64,    37,    65,    66,   102,    23,    24,    40,    40,    36,
     107,    43,   107,    36,    85,    85,   104,    15,    16,    40,
      85,    88,    89,    90,    92,    93,    94,    95,    96,    97,
      98,    99,   106,   109,    88,    40,    40,    88,    39,    70,
      39,    69,    40,    40,    42,    43,    75,   109,   107,    36,
      36,    36,    76,    37,    41,    67,   109,    37,    88,   108,
      40,   109,    40,    88,    72,    72,   107,    97,   105,   109,
      97,    98,    40,    11,    12,    14,    18,    13,    15,    17,
      19,    40,    37,    39,    89,   109,    37,    66,    37,    66,
      66,    37,    41,   107,    37,    27,    93,   104,    95,    95,
      96,    96,    97,    97,   107,    37,    37,    88,    68,    72
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
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
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
#if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
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
#line 195 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, (yyvsp[(1) - (1)].mainBlock));
	  ast = (yyval.seq);
	}
    break;

  case 3:
#line 200 "parse.yy"
    {
	  (yyval.seq) = (yyvsp[(2) - (2)].seq)->add((yyvsp[(1) - (2)].mainBlock));
	  ast = (yyval.seq);
	}
    break;

  case 4:
#line 205 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, (yyvsp[(2) - (2)].mainBlock));
	  (yyval.seq)->add((yyvsp[(1) - (2)].seq));
	  ast = (yyval.seq);
	}
    break;

  case 5:
#line 211 "parse.yy"
    {
	  (yyval.seq) = (yyvsp[(3) - (3)].seq)->add((yyvsp[(2) - (3)].mainBlock))->add((yyvsp[(1) - (3)].seq));
	  ast = (yyval.seq);
	}
    break;

  case 6:
#line 219 "parse.yy"
    {
	  if (*((yyvsp[(1) - (5)].type)->getName()) != "Integer") {
	     cerr << "mc: " << (yyloc).first_line << ": parse error\n";
	     return 1;
	  }

	  (yyval.mainBlock) = new MainBlockNode((yyvsp[(5) - (5)].blockStmnt));
	}
    break;

  case 7:
#line 231 "parse.yy"
    {
	  (yyval.seq) = new Seq((yyvsp[(1) - (2)].seq), (yyvsp[(2) - (2)].classDec));
	}
    break;

  case 8:
#line 235 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, (yyvsp[(1) - (1)].classDec));
	}
    break;

  case 9:
#line 242 "parse.yy"
    {
	  (yyval.classDec) = new ClassDecNode((yyvsp[(2) - (3)].str), (yyvsp[(3) - (3)].seq));
	  (yyval.classDec)->setLineNumber((yyloc).first_line);
	}
    break;

  case 10:
#line 247 "parse.yy"
    {
	  (yyval.classDec) = new ClassDecNode((yyvsp[(2) - (5)].str), ((ClassType *)(yyvsp[(4) - (5)].type))->getName(), (yyvsp[(5) - (5)].seq));
	  delete (yyvsp[(4) - (5)].type);
	  (yyval.classDec)->setLineNumber((yyloc).first_line);
	}
    break;

  case 11:
#line 256 "parse.yy"
    {
	  (yyval.seq) = (yyvsp[(2) - (3)].seq);
	}
    break;

  case 12:
#line 260 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, 0);
	}
    break;

  case 13:
#line 267 "parse.yy"
    {
	  (yyval.seq) = new Seq((yyvsp[(1) - (2)].seq), (yyvsp[(2) - (2)].node));
	}
    break;

  case 14:
#line 271 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, (yyvsp[(1) - (1)].node));
	}
    break;

  case 15:
#line 278 "parse.yy"
    {
	  (yyval.node) = (yyvsp[(1) - (1)].node);
	}
    break;

  case 16:
#line 282 "parse.yy"
    {
	  (yyval.node) = (yyvsp[(1) - (1)].classConst);
	}
    break;

  case 17:
#line 289 "parse.yy"
    {
	  (yyval.node) = (yyvsp[(1) - (1)].classFieldDec);
	}
    break;

  case 18:
#line 293 "parse.yy"
    {
	  (yyval.node) = (yyvsp[(1) - (1)].classMeth);
	}
    break;

  case 19:
#line 297 "parse.yy"
    {
	  (yyval.node) = (yyvsp[(1) - (1)].classMeth);
	}
    break;

  case 20:
#line 304 "parse.yy"
    {
	  (yyval.classFieldDec) = new ClassFieldDecNode((yyvsp[(1) - (3)].type), (yyvsp[(2) - (3)].seq));
	  (yyval.classFieldDec)->setLineNumber((yyloc).first_line);
	}
    break;

  case 21:
#line 312 "parse.yy"
    {
	  (yyvsp[(3) - (4)].classMeth)->setNativeIndex((yyvsp[(1) - (4)].value));
	  (yyvsp[(3) - (4)].classMeth)->setType((yyvsp[(2) - (4)].type));
	  (yyvsp[(3) - (4)].classMeth)->setBody(new BlockStatementNode());
	  (yyval.classMeth) = (yyvsp[(3) - (4)].classMeth);
	  (yyval.classMeth)->setLineNumber((yyloc).first_line);
	}
    break;

  case 22:
#line 320 "parse.yy"
    {
	  (yyvsp[(2) - (3)].classMeth)->setType((yyvsp[(1) - (3)].type));
	  (yyvsp[(2) - (3)].classMeth)->setBody((yyvsp[(3) - (3)].blockStmnt));
	  (yyval.classMeth) = (yyvsp[(2) - (3)].classMeth);
	  (yyval.classMeth)->setLineNumber((yyloc).first_line);
	}
    break;

  case 23:
#line 330 "parse.yy"
    {
	  (yyval.classMeth) = new ClassMethodNode((yyvsp[(1) - (2)].str), (yyvsp[(2) - (2)].seq));
	}
    break;

  case 24:
#line 337 "parse.yy"
    {
	  (yyval.blockStmnt) = (yyvsp[(1) - (1)].blockStmnt);
	}
    break;

  case 25:
#line 344 "parse.yy"
    {
	  (yyvsp[(3) - (4)].classMeth)->setNativeIndex((yyvsp[(1) - (4)].value));
	  (yyvsp[(3) - (4)].classMeth)->setType((yyvsp[(2) - (4)].type));
	  (yyvsp[(3) - (4)].classMeth)->setBody(new BlockStatementNode());
	  (yyval.classMeth) = (yyvsp[(3) - (4)].classMeth);
	  (yyval.classMeth)->setLineNumber((yyloc).first_line);
	}
    break;

  case 26:
#line 352 "parse.yy"
    {
	  (yyvsp[(2) - (3)].classMeth)->setType((yyvsp[(1) - (3)].type));
	  (yyvsp[(2) - (3)].classMeth)->setBody((yyvsp[(3) - (3)].blockStmnt));
	  (yyval.classMeth) = (yyvsp[(2) - (3)].classMeth);
	  (yyval.classMeth)->setLineNumber((yyloc).first_line);
	}
    break;

  case 27:
#line 362 "parse.yy"
    {
	  (yyval.classMeth) = new ClassMethodNode((yyvsp[(2) - (5)].str), new Seq(0, (yyvsp[(4) - (5)].varDec)));
	}
    break;

  case 28:
#line 366 "parse.yy"
    {
	  (yyval.classMeth) = new ClassMethodNode((yyvsp[(2) - (5)].str), new Seq(0, (yyvsp[(4) - (5)].varDec)));
	}
    break;

  case 29:
#line 370 "parse.yy"
    {
	  (yyval.classMeth) = new ClassMethodNode((yyvsp[(2) - (4)].str), new Seq(0, 0));
	}
    break;

  case 30:
#line 374 "parse.yy"
    {
	  (yyval.classMeth) = new ClassMethodNode((yyvsp[(2) - (4)].str), new Seq(0, 0));
	}
    break;

  case 31:
#line 381 "parse.yy"
    {
	  (yyval.blockStmnt) = (yyvsp[(1) - (1)].blockStmnt);
	}
    break;

  case 32:
#line 388 "parse.yy"
    {
	  (yyvsp[(2) - (3)].classConst)->setNativeIndex((yyvsp[(1) - (3)].value));	
	  (yyvsp[(2) - (3)].classConst)->setBody(new BlockStatementNode());
	  (yyval.classConst) = (yyvsp[(2) - (3)].classConst);
	}
    break;

  case 33:
#line 394 "parse.yy"
    {
	  (yyvsp[(1) - (2)].classConst)->setBody((yyvsp[(2) - (2)].blockStmnt));
	  (yyval.classConst) = (yyvsp[(1) - (2)].classConst);
	}
    break;

  case 34:
#line 402 "parse.yy"
    {
	  (yyval.classConst) = new ClassConstructorNode((yyvsp[(1) - (2)].str), (yyvsp[(2) - (2)].seq));
	  (yyval.classConst)->setLineNumber((yyloc).first_line);
	}
    break;

  case 35:
#line 410 "parse.yy"
    {
	  (yyval.blockStmnt) = new BlockStatementNode((yyvsp[(3) - (4)].seq)->add((yyvsp[(2) - (4)].stmnt)));
	}
    break;

  case 36:
#line 414 "parse.yy"
    {
	  (yyval.blockStmnt) = new BlockStatementNode(new Seq(0, (yyvsp[(2) - (3)].stmnt)));
	}
    break;

  case 37:
#line 418 "parse.yy"
    {
	  (yyvsp[(1) - (1)].blockStmnt)->add(new ConstructorInvocationStatementNode(new SuperExpressionNode(), new Seq(0, 0)));
	  (yyval.blockStmnt) = (yyvsp[(1) - (1)].blockStmnt);
	}
    break;

  case 38:
#line 426 "parse.yy"
    {
	  (yyval.stmnt) = new ConstructorInvocationStatementNode(new ThisExpressionNode(), (yyvsp[(2) - (3)].seq));
	}
    break;

  case 39:
#line 430 "parse.yy"
    {
	  (yyval.stmnt) = new ConstructorInvocationStatementNode(new SuperExpressionNode(), (yyvsp[(2) - (3)].seq));
	}
    break;

  case 40:
#line 437 "parse.yy"
    {
	  (yyval.seq) = (yyvsp[(2) - (3)].seq);
	}
    break;

  case 41:
#line 441 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, 0);
	}
    break;

  case 42:
#line 448 "parse.yy"
    {
	  (yyval.seq) = new Seq((yyvsp[(1) - (3)].seq), (yyvsp[(3) - (3)].varDec));
	}
    break;

  case 43:
#line 452 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, (yyvsp[(1) - (1)].varDec));
	}
    break;

  case 44:
#line 459 "parse.yy"
    {
	  (yyval.varDec) = new VarDecNode((yyvsp[(1) - (2)].type), new Seq(0, (yyvsp[(2) - (2)].varNode)));
	  (yyval.varDec)->setLineNumber((yyloc).first_line);
	}
    break;

  case 45:
#line 467 "parse.yy"
    {
	  (yyval.varNode) = new VarNode((yyvsp[(1) - (1)].str));
	  (yyval.varNode)->setLineNumber((yyloc).first_line);
	}
    break;

  case 46:
#line 475 "parse.yy"
    {
	  (yyval.blockStmnt) = new BlockStatementNode((yyvsp[(2) - (3)].seq));
	}
    break;

  case 47:
#line 479 "parse.yy"
    {
	  (yyval.blockStmnt) = new BlockStatementNode(new Seq(0, 0));
	}
    break;

  case 48:
#line 486 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 49:
#line 493 "parse.yy"
    {
	  (yyval.seq) = new Seq((yyvsp[(1) - (2)].seq), (yyvsp[(2) - (2)].stmnt));
	}
    break;

  case 50:
#line 497 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, (yyvsp[(1) - (1)].stmnt));
	}
    break;

  case 51:
#line 504 "parse.yy"
    {
	  (yyval.stmnt) = new SynchronizedStatementNode((yyvsp[(3) - (5)].expr), (yyvsp[(5) - (5)].blockStmnt));
	}
    break;

  case 52:
#line 511 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].blockStmnt);
	}
    break;

  case 53:
#line 515 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 54:
#line 519 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 55:
#line 523 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 56:
#line 527 "parse.yy"
    {
	  (yyvsp[(1) - (1)].expr)->setIsStatement(true);
	  (yyval.stmnt) = new ExpressionStatementNode((yyvsp[(1) - (1)].expr));
	}
    break;

  case 57:
#line 532 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 58:
#line 536 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 59:
#line 540 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 60:
#line 544 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 61:
#line 548 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 62:
#line 552 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 63:
#line 556 "parse.yy"
    {
	  (yyval.stmnt) = (yyvsp[(1) - (1)].stmnt);
	}
    break;

  case 64:
#line 563 "parse.yy"
    {
	  (yyval.stmnt) = new LocalVarDecStatementNode((yyvsp[(1) - (2)].localVarDec));
	}
    break;

  case 65:
#line 570 "parse.yy"
    {
	  (yyval.localVarDec) = new LocalVarDecNode((yyvsp[(1) - (2)].type), (yyvsp[(2) - (2)].seq));
	  (yyval.localVarDec)->setLineNumber((yyloc).first_line);
	}
    break;

  case 66:
#line 578 "parse.yy"
    {
	  (yyval.seq) = new Seq((yyvsp[(1) - (3)].seq), (yyvsp[(3) - (3)].varNode));
	}
    break;

  case 67:
#line 582 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, (yyvsp[(1) - (1)].varNode));
	}
    break;

  case 68:
#line 589 "parse.yy"
    {
	  (yyval.varNode) = new VarNode((yyvsp[(1) - (1)].str));
	  (yyval.varNode)->setLineNumber((yyloc).first_line);
	}
    break;

  case 69:
#line 597 "parse.yy"
    {
	  (yyval.stmnt) = new IfThenStatementNode((yyvsp[(2) - (3)].expr), (yyvsp[(3) - (3)].stmnt));
	  (yyval.stmnt)->setLineNumber((yyloc).first_line);
	}
    break;

  case 70:
#line 605 "parse.yy"
    {
	  (yyval.stmnt) = new IfThenElseStatementNode((yyvsp[(2) - (5)].expr), (yyvsp[(3) - (5)].stmnt), (yyvsp[(5) - (5)].stmnt));
	  (yyval.stmnt)->setLineNumber((yyloc).first_line);
	}
    break;

  case 71:
#line 613 "parse.yy"
    {
	  (yyval.stmnt) = new WhileStatementNode((yyvsp[(2) - (3)].expr), (yyvsp[(3) - (3)].stmnt));
	  (yyval.stmnt)->setLineNumber((yyloc).first_line);
	}
    break;

  case 72:
#line 621 "parse.yy"
    {
	  (yyval.stmnt) = new ReturnStatementNode();
	}
    break;

  case 73:
#line 625 "parse.yy"
    {
	  (yyval.stmnt) = new ReturnStatementNode((yyvsp[(2) - (3)].expr));
	}
    break;

  case 74:
#line 632 "parse.yy"
    {
	  (yyval.stmnt) = new OutStatementNode((yyvsp[(2) - (3)].expr));
	}
    break;

  case 75:
#line 639 "parse.yy"
    {
	  (yyval.stmnt) = new BreakStatementNode();
	}
    break;

  case 76:
#line 646 "parse.yy"
    {
	  (yyval.stmnt) = new ContinueStatementNode();
	}
    break;

  case 77:
#line 653 "parse.yy"
    {
	  (yyval.stmnt) = new StatementNode();
	}
    break;

  case 78:
#line 660 "parse.yy"
    {
	  (yyvsp[(2) - (3)].expr)->setIsParenthesized(true);
	  (yyval.expr) = (yyvsp[(2) - (3)].expr);
	}
    break;

  case 79:
#line 668 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (2)].expr);
	}
    break;

  case 80:
#line 675 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 81:
#line 679 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 82:
#line 686 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 83:
#line 693 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 84:
#line 697 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 85:
#line 704 "parse.yy"
    {
	  (yyval.expr) = new AssignmentOperatorNode((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
	}
    break;

  case 86:
#line 711 "parse.yy"
    {
	  (yyval.expr) = new DerefNode(new VarNode((yyvsp[(1) - (1)].str)));
	}
    break;

  case 87:
#line 715 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 88:
#line 722 "parse.yy"
    {
	  (yyval.expr) = new EqualityOperatorNode((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
	}
    break;

  case 89:
#line 726 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 90:
#line 733 "parse.yy"
    {
	  (yyval.expr) = new InstanceOfExpressionNode((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].type));
	}
    break;

  case 91:
#line 737 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 92:
#line 744 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].str), new Seq(0, (yyvsp[(3) - (3)].expr)));
	}
    break;

  case 93:
#line 748 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].str), new Seq(0, (yyvsp[(3) - (3)].expr)));
	}
    break;

  case 94:
#line 752 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 95:
#line 759 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].str), new Seq(0, (yyvsp[(3) - (3)].expr)));
	}
    break;

  case 96:
#line 763 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].str), new Seq(0, (yyvsp[(3) - (3)].expr)));
	}
    break;

  case 97:
#line 767 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 98:
#line 774 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].str), new Seq(0, (yyvsp[(3) - (3)].expr)));
	}
    break;

  case 99:
#line 778 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].str), new Seq(0, (yyvsp[(3) - (3)].expr)));
	}
    break;

  case 100:
#line 782 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 101:
#line 789 "parse.yy"
    {
	  (yyvsp[(2) - (2)].expr)->setParentIsNegate(true);
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(2) - (2)].expr), (yyvsp[(1) - (2)].str), new Seq(0, 0));
	}
    break;

  case 102:
#line 794 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(2) - (2)].expr), (yyvsp[(1) - (2)].str), new Seq(0, 0));
	}
    break;

  case 103:
#line 798 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 104:
#line 805 "parse.yy"
    {
	  (yyval.expr) = new CastExpressionNode((yyvsp[(1) - (2)].expr), (yyvsp[(2) - (2)].expr));
	}
    break;

  case 105:
#line 809 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 106:
#line 816 "parse.yy"
    {
	  (yyval.expr) = new DerefNode(new VarNode((yyvsp[(1) - (1)].str)));
	}
    break;

  case 107:
#line 820 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 108:
#line 824 "parse.yy"
    {
	  (yyval.expr) = new ThisExpressionNode();
	}
    break;

  case 109:
#line 828 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 110:
#line 832 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 111:
#line 836 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 112:
#line 840 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 113:
#line 844 "parse.yy"
    {
	  (yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 114:
#line 851 "parse.yy"
    {
	  (yyval.expr) = new InExpressionNode();
	}
    break;

  case 115:
#line 858 "parse.yy"
    {
	  (yyval.expr) = new ClassInstanceCreationExpressionNode((yyvsp[(2) - (3)].type)->getName(), (yyvsp[(3) - (3)].seq));
	  delete (yyvsp[(2) - (3)].type);
	}
    break;

  case 116:
#line 866 "parse.yy"
    {
	  (yyval.type) = (yyvsp[(1) - (1)].type);
	}
    break;

  case 117:
#line 873 "parse.yy"
    {
	  (yyval.type) = (yyvsp[(1) - (1)].type);
	}
    break;

  case 118:
#line 880 "parse.yy"
    {
	  (yyval.type) = new ClassType((yyvsp[(1) - (1)].str));
	}
    break;

  case 119:
#line 887 "parse.yy"
    {
	  (yyval.expr) = new DerefNode(new FieldAccessOperatorNode((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].str)));
	}
    break;

  case 120:
#line 891 "parse.yy"
    {
	  (yyval.expr) = new DerefNode(new FieldAccessOperatorNode(new SuperExpressionNode(), (yyvsp[(3) - (3)].str)));
	}
    break;

  case 121:
#line 898 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode(new ThisExpressionNode(), (yyvsp[(1) - (2)].str), (yyvsp[(2) - (2)].seq));
	}
    break;

  case 122:
#line 902 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode((yyvsp[(1) - (4)].expr), (yyvsp[(3) - (4)].str), (yyvsp[(4) - (4)].seq));
	}
    break;

  case 123:
#line 906 "parse.yy"
    {
	  (yyval.expr) = new MethodInvocationExpressionNode(new SuperExpressionNode(), (yyvsp[(3) - (4)].str), (yyvsp[(4) - (4)].seq));
	}
    break;

  case 124:
#line 913 "parse.yy"
    {
	  (yyval.seq) = (yyvsp[(2) - (3)].seq);
	}
    break;

  case 125:
#line 917 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, 0);
	}
    break;

  case 126:
#line 924 "parse.yy"
    {
	  (yyval.seq) = new Seq((yyvsp[(1) - (3)].seq), (yyvsp[(3) - (3)].expr));
	}
    break;

  case 127:
#line 928 "parse.yy"
    {
	  (yyval.seq) = new Seq(0, (yyvsp[(1) - (1)].expr));
	}
    break;

  case 128:
#line 935 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);
	}
    break;

  case 129:
#line 942 "parse.yy"
    {
	  (yyval.expr) = new IntegerLiteralNode((yyvsp[(1) - (1)].value));
	}
    break;

  case 130:
#line 946 "parse.yy"
    {
	  (yyval.expr) = new NullLiteralNode();
	}
    break;

  case 131:
#line 950 "parse.yy"
    {
	  (yyval.expr) = new StringLiteralNode((yyvsp[(1) - (1)].str));
	}
    break;

  case 132:
#line 957 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);
	}
    break;

  case 133:
#line 964 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);
	}
    break;

  case 134:
#line 968 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);	
	}
    break;

  case 135:
#line 972 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);	
	}
    break;

  case 136:
#line 976 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);	
	}
    break;

  case 137:
#line 980 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);	
	}
    break;

  case 138:
#line 987 "parse.yy"
    {
	  (yyval.str) = (yyvsp[(1) - (1)].str);
	}
    break;


/* Line 1267 of yacc.c.  */
#line 2749 "parse.cc"
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


#line 992 "parse.yy"


void yyerror(const char *s) {
        cerr << "mc: " << getCurrentSourceLineNumber() << ": " << s << '\n';
}

