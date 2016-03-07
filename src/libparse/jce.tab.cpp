/* A Bison parser, made by GNU Bison 2.5.1.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 1 "jce.y"

#include <iostream>
#include <memory>
#include <cassert>

using namespace std;

#include "parse/parse.h"
#define YYDEBUG 1
#define YYINITDEPTH 10000


/* Line 268 of yacc.c  */
#line 84 "jce.tab.cpp"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
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
     JCE_VOID = 258,
     JCE_STRUCT = 259,
     JCE_BOOL = 260,
     JCE_BYTE = 261,
     JCE_SHORT = 262,
     JCE_INT = 263,
     JCE_DOUBLE = 264,
     JCE_FLOAT = 265,
     JCE_LONG = 266,
     JCE_STRING = 267,
     JCE_VECTOR = 268,
     JCE_MAP = 269,
     JCE_NAMESPACE = 270,
     JCE_INTERFACE = 271,
     JCE_IDENTIFIER = 272,
     JCE_OUT = 273,
     JCE_OP = 274,
     JCE_KEY = 275,
     JCE_ROUTE_KEY = 276,
     JCE_REQUIRE = 277,
     JCE_OPTIONAL = 278,
     JCE_INTEGER_LITERAL = 279,
     JCE_FLOATING_POINT_LITERAL = 280,
     JCE_FALSE = 281,
     JCE_TRUE = 282,
     JCE_STRING_LITERAL = 283,
     JCE_SCOPE_DELIMITER = 284,
     JCE_CONST = 285,
     JCE_ENUM = 286,
     JCE_UNSIGNED = 287,
     BAD_CHAR = 288
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 345 of yacc.c  */
#line 167 "jce.tab.cpp"

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

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

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  75
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   558

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  135
/* YYNRULES -- Number of states.  */
#define YYNSTATES  195

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   288

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    41,    42,     2,    37,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    43,    34,
      44,    38,    45,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    39,     2,    40,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    35,     2,    36,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,    11,    12,    17,    19,    20,
      22,    24,    26,    28,    30,    32,    33,    39,    42,    45,
      49,    51,    53,    55,    59,    60,    61,    68,    69,    77,
      79,    83,    84,    90,    93,    96,   100,   104,   106,   107,
     109,   113,   116,   118,   120,   121,   123,   127,   130,   135,
     138,   143,   145,   147,   149,   151,   152,   158,   161,   164,
     167,   171,   173,   174,   176,   180,   186,   192,   196,   199,
     202,   205,   207,   209,   211,   213,   215,   217,   222,   225,
     231,   235,   240,   243,   245,   247,   251,   253,   257,   259,
     261,   264,   266,   269,   271,   274,   276,   278,   280,   282,
     284,   286,   288,   293,   297,   302,   305,   312,   316,   318,
     321,   325,   327,   329,   331,   333,   335,   337,   339,   341,
     343,   345,   347,   349,   351,   353,   355,   357,   359,   361,
     363,   365,   367,   369,   371,   373
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      47,     0,    -1,    48,    -1,    -1,    51,    49,    34,    48,
      -1,    -1,     1,    34,    50,    48,    -1,    51,    -1,    -1,
      57,    -1,    62,    -1,    73,    -1,    59,    -1,    52,    -1,
      80,    -1,    -1,    54,    53,    35,    55,    36,    -1,    31,
      17,    -1,    31,    87,    -1,    56,    37,    55,    -1,    56,
      -1,    17,    -1,    87,    -1,    17,    38,    79,    -1,    -1,
      -1,    15,    17,    58,    35,    48,    36,    -1,    -1,    20,
      39,    86,    37,    60,    61,    40,    -1,    17,    -1,    61,
      37,    17,    -1,    -1,    64,    63,    35,    65,    36,    -1,
      16,    17,    -1,    16,    87,    -1,    66,    34,    65,    -1,
       1,    34,    65,    -1,    66,    -1,    -1,    67,    -1,    68,
      70,    41,    -1,    69,    19,    -1,    82,    -1,     3,    -1,
      -1,    81,    -1,    70,    37,    81,    -1,    72,    81,    -1,
      70,    37,    72,    81,    -1,    71,    81,    -1,    70,    37,
      71,    81,    -1,    72,    -1,    71,    -1,    21,    -1,    18,
      -1,    -1,    75,    74,    35,    76,    36,    -1,     4,    17,
      -1,     4,    87,    -1,     4,     1,    -1,    77,    34,    76,
      -1,    77,    -1,    -1,    78,    -1,    24,    22,    81,    -1,
      24,    22,    81,    38,    79,    -1,    24,    23,    81,    38,
      79,    -1,    24,    23,    81,    -1,    22,    81,    -1,    23,
      81,    -1,    24,    81,    -1,    81,    -1,    24,    -1,    25,
      -1,    28,    -1,    26,    -1,    27,    -1,    30,    81,    38,
      79,    -1,    82,    17,    -1,    82,    17,    39,    24,    40,
      -1,    82,    42,    17,    -1,    82,    17,    43,    24,    -1,
      82,    87,    -1,    82,    -1,     1,    -1,    83,    43,    24,
      -1,    83,    -1,    83,    43,     1,    -1,     5,    -1,     6,
      -1,    32,     6,    -1,     7,    -1,    32,     7,    -1,     8,
      -1,    32,     8,    -1,    11,    -1,    10,    -1,     9,    -1,
      12,    -1,    84,    -1,    85,    -1,    86,    -1,    13,    44,
      82,    45,    -1,    13,    44,     1,    -1,    13,    44,    82,
       1,    -1,    13,     1,    -1,    14,    44,    82,    37,    82,
      45,    -1,    14,    44,     1,    -1,    17,    -1,    29,    17,
      -1,    86,    29,    17,    -1,     4,    -1,     3,    -1,     5,
      -1,     6,    -1,     7,    -1,     8,    -1,    10,    -1,     9,
      -1,    12,    -1,    13,    -1,    20,    -1,    14,    -1,    15,
      -1,    16,    -1,    18,    -1,    22,    -1,    23,    -1,    24,
      -1,    25,    -1,    26,    -1,    27,    -1,    28,    -1,    30,
      -1,    31,    -1,    32,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    50,    50,    57,    56,    61,    60,    65,    70,    77,
      81,    85,    89,    92,    96,   106,   105,   128,   141,   152,
     156,   164,   175,   180,   194,   202,   201,   235,   234,   253,
     266,   286,   285,   319,   323,   334,   337,   340,   345,   352,
     358,   375,   404,   405,   416,   418,   429,   440,   452,   464,
     476,   488,   492,   501,   512,   524,   523,   566,   570,   576,
     585,   589,   594,   603,   612,   630,   653,   676,   693,   697,
     701,   705,   714,   724,   734,   742,   750,   763,   781,   790,
     800,   810,   819,   824,   828,   837,   846,   850,   859,   863,
     867,   871,   875,   879,   883,   887,   891,   895,   899,   903,
     907,   911,   929,   933,   937,   941,   950,   954,   963,   966,
     972,   985,   988,   991,   994,   997,  1000,  1003,  1006,  1009,
    1012,  1015,  1018,  1021,  1024,  1027,  1030,  1033,  1036,  1039,
    1042,  1045,  1048,  1051,  1054,  1057
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "JCE_VOID", "JCE_STRUCT", "JCE_BOOL",
  "JCE_BYTE", "JCE_SHORT", "JCE_INT", "JCE_DOUBLE", "JCE_FLOAT",
  "JCE_LONG", "JCE_STRING", "JCE_VECTOR", "JCE_MAP", "JCE_NAMESPACE",
  "JCE_INTERFACE", "JCE_IDENTIFIER", "JCE_OUT", "JCE_OP", "JCE_KEY",
  "JCE_ROUTE_KEY", "JCE_REQUIRE", "JCE_OPTIONAL", "JCE_INTEGER_LITERAL",
  "JCE_FLOATING_POINT_LITERAL", "JCE_FALSE", "JCE_TRUE",
  "JCE_STRING_LITERAL", "JCE_SCOPE_DELIMITER", "JCE_CONST", "JCE_ENUM",
  "JCE_UNSIGNED", "BAD_CHAR", "';'", "'{'", "'}'", "','", "'='", "'['",
  "']'", "')'", "'*'", "':'", "'<'", "'>'", "$accept", "start",
  "definitions", "$@1", "$@2", "definition", "enum_def", "@3", "enum_id",
  "enumerator_list", "enumerator", "namespace_def", "@4", "key_def", "$@5",
  "key_members", "interface_def", "@6", "interface_id",
  "interface_exports", "interface_export", "operation",
  "operation_preamble", "return_type", "parameters", "routekey_qualifier",
  "out_qualifier", "struct_def", "@7", "struct_id", "struct_exports",
  "data_member", "struct_type_id", "const_initializer", "const_def",
  "type_id", "type", "type_no", "vector", "map", "scoped_name", "keyword", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,    59,   123,   125,    44,    61,    91,
      93,    41,    42,    58,    60,    62
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    46,    47,    49,    48,    50,    48,    48,    48,    51,
      51,    51,    51,    51,    51,    53,    52,    54,    54,    55,
      55,    56,    56,    56,    56,    58,    57,    60,    59,    61,
      61,    63,    62,    64,    64,    65,    65,    65,    65,    66,
      67,    68,    69,    69,    70,    70,    70,    70,    70,    70,
      70,    70,    70,    71,    72,    74,    73,    75,    75,    75,
      76,    76,    76,    77,    78,    78,    78,    78,    78,    78,
      78,    78,    79,    79,    79,    79,    79,    80,    81,    81,
      81,    81,    81,    81,    81,    82,    82,    82,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    84,    84,    84,    84,    85,    85,    86,    86,
      86,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     4,     0,     4,     1,     0,     1,
       1,     1,     1,     1,     1,     0,     5,     2,     2,     3,
       1,     1,     1,     3,     0,     0,     6,     0,     7,     1,
       3,     0,     5,     2,     2,     3,     3,     1,     0,     1,
       3,     2,     1,     1,     0,     1,     3,     2,     4,     2,
       4,     1,     1,     1,     1,     0,     5,     2,     2,     2,
       3,     1,     0,     1,     3,     5,     5,     3,     2,     2,
       2,     1,     1,     1,     1,     1,     1,     4,     2,     5,
       3,     4,     2,     1,     1,     3,     1,     3,     1,     1,
       2,     1,     2,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     4,     3,     4,     2,     6,     3,     1,     2,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     2,
       7,    13,    15,     9,    12,    10,    31,    11,    55,    14,
       5,    59,   112,   111,   113,   114,   115,   116,   118,   117,
     119,   120,   122,   123,   124,    57,   125,   121,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,    58,    25,
      33,    34,     0,    84,    88,    89,    91,    93,    97,    96,
      95,    98,     0,     0,   108,     0,     0,     0,    83,    86,
      99,   100,   101,    17,    18,     1,     0,     0,     0,     0,
       0,     0,     0,   105,     0,     0,   109,    90,    92,    94,
       0,    78,     0,    82,     0,     0,     0,    24,     0,     0,
       6,     0,    27,   103,     0,   107,     0,    72,    73,    75,
      76,    74,    77,     0,     0,    80,    87,    85,   110,     4,
      21,     0,    20,    22,     0,    43,     0,    37,    39,     0,
       0,    42,     0,     0,     0,     0,    61,    63,    71,     0,
       0,   104,   102,     0,     0,    81,     0,    16,    24,     0,
      32,     0,    54,    53,     0,     0,     0,    45,    41,    68,
      69,     0,     0,    70,    56,     0,    26,    29,     0,     0,
      79,    23,    19,    36,    35,     0,    40,    49,    47,    64,
      67,    60,     0,    28,   106,     0,     0,    46,     0,     0,
      30,    50,    48,    65,    66
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     8,     9,    76,    80,    10,    11,    77,    12,   121,
     122,    13,    81,    14,   140,   168,    15,    78,    16,   126,
     127,   128,   129,   130,   154,   155,   156,    17,    79,    18,
     135,   136,   137,   112,    19,   138,    68,    69,    70,    71,
      72,   123
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -140
static const yytype_int16 yypact[] =
{
     409,    -8,   265,    -7,   438,    -1,   351,   468,    42,  -140,
      10,  -140,  -140,  -140,  -140,  -140,  -140,  -140,  -140,  -140,
    -140,  -140,  -140,  -140,  -140,  -140,  -140,  -140,  -140,  -140,
    -140,  -140,  -140,  -140,  -140,  -140,  -140,  -140,  -140,  -140,
    -140,  -140,  -140,  -140,  -140,  -140,  -140,  -140,  -140,  -140,
    -140,  -140,   -11,  -140,  -140,  -140,  -140,  -140,  -140,  -140,
    -140,  -140,     8,     9,  -140,    28,    49,    20,   178,     4,
    -140,  -140,    32,  -140,  -140,  -140,    30,    27,    35,    47,
     100,    61,   -18,  -140,   365,   394,  -140,  -140,  -140,  -140,
     133,   -27,    48,  -140,    13,    67,   100,   498,   134,   216,
    -140,   231,  -140,  -140,     3,  -140,    62,  -140,  -140,  -140,
    -140,  -140,  -140,    78,    81,  -140,  -140,  -140,  -140,  -140,
      68,    72,    73,  -140,    77,  -140,    82,    79,  -140,    22,
      95,  -140,   351,   351,   293,    83,    88,  -140,  -140,    89,
     107,  -140,  -140,   526,    92,  -140,   133,  -140,   498,   134,
    -140,   134,  -140,  -140,   -24,    66,    80,  -140,  -140,  -140,
    -140,   351,   351,  -140,  -140,   216,  -140,  -140,   -16,    84,
    -140,  -140,  -140,  -140,  -140,   322,  -140,  -140,  -140,    96,
     114,  -140,   116,  -140,  -140,   351,   351,  -140,   133,   133,
    -140,  -140,  -140,  -140,  -140
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -140,  -140,   -55,  -140,  -140,  -140,  -140,  -140,  -140,   -10,
    -140,  -140,  -140,  -140,  -140,  -140,  -140,  -140,  -140,  -129,
    -140,  -140,  -140,  -140,  -140,   -22,   -21,  -140,  -140,  -140,
      -3,  -140,  -140,  -139,  -140,    -6,   -83,  -140,  -140,  -140,
     112,     1
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -63
static const yytype_int16 yytable[] =
{
      67,   104,   106,    48,   141,    51,    64,   171,    74,    83,
      49,    95,   113,   175,   116,   131,   114,   176,    65,   102,
     173,   182,   174,    53,   183,   100,    20,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,   117,    52,    64,
     152,   119,    75,   153,    -3,    86,   139,    94,   142,   193,
     194,    65,    84,    85,    66,    87,    88,    89,    90,   -44,
     169,    95,    97,   -44,    96,   115,   131,    53,   131,    93,
      98,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    53,    99,    64,   118,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    65,   101,    64,    66,   143,
      -8,     1,   144,   -52,     2,   145,   146,   -52,   147,    65,
     148,   149,    66,   151,   158,     3,     4,   -51,   150,   164,
       5,   -51,   165,   157,   167,   166,   159,   160,   163,   184,
       6,     7,   170,   190,   188,   124,    -8,   125,   172,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,   177,
     178,    64,   189,   185,   186,   179,   180,   107,   108,   109,
     110,   111,   181,    65,    82,     0,    66,     0,     0,   187,
     -38,     0,     0,     0,     0,     0,     0,     0,     0,   191,
     192,    22,    23,    24,    25,    26,    27,    28,    29,     0,
      30,    31,    32,    33,    34,    91,    36,     0,    37,     0,
      38,    39,    40,    41,    42,    43,    44,     0,    45,    46,
      47,     0,     0,     0,     0,     0,     0,    53,     0,     0,
      92,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,     0,     1,    64,     0,     2,     0,     0,   132,   133,
     134,     0,     0,     0,     0,    65,     3,     4,    66,     0,
       0,     5,   -62,     0,     0,     0,     0,     0,     0,     0,
       0,     6,     7,     0,     0,     0,    21,    -8,    22,    23,
      24,    25,    26,    27,    28,    29,     0,    30,    31,    32,
      33,    34,    35,    36,     0,    37,     0,    38,    39,    40,
      41,    42,    43,    44,    53,    45,    46,    47,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,     0,     0,
      64,     0,     0,     0,     0,   161,   162,     0,     0,     0,
       0,     0,    65,    53,     0,    66,     0,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,     0,     0,    64,
     152,     0,     0,   153,     0,     0,     0,     0,     0,     0,
       0,    65,    53,     0,    66,     0,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,   103,     0,    64,     0,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      65,     0,    64,    66,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    65,   105,     0,    66,     0,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -8,
       1,    64,     0,     2,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     3,     4,    66,     0,     0,     5,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     6,
       7,    22,    23,    24,    25,    26,    27,    28,    29,     0,
      30,    31,    32,    33,    34,    50,    36,     0,    37,     0,
      38,    39,    40,    41,    42,    43,    44,     0,    45,    46,
      47,    22,    23,    24,    25,    26,    27,    28,    29,     0,
      30,    31,    32,    33,    34,    73,    36,     0,    37,     0,
      38,    39,    40,    41,    42,    43,    44,     0,    45,    46,
      47,    22,    23,    24,    25,    26,    27,    28,    29,     0,
      30,    31,    32,    33,    34,   120,    36,     0,    37,     0,
      38,    39,    40,    41,    42,    43,    44,     0,    45,    46,
      47,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,     0,     0,    64,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,     0,    66
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-140))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       6,    84,    85,     2,     1,     4,    17,   146,     7,     1,
      17,    29,    39,    37,     1,    98,    43,    41,    29,    37,
     149,    37,   151,     1,    40,    80,    34,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    24,    39,    17,
      18,    96,     0,    21,    34,    17,   101,    43,    45,   188,
     189,    29,    44,    44,    32,     6,     7,     8,    38,    37,
     143,    29,    35,    41,    34,    17,   149,     1,   151,    68,
      35,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,     1,    35,    17,    17,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    29,    35,    17,    32,    37,
       0,     1,    24,    37,     4,    24,    38,    41,    36,    29,
      37,    34,    32,    34,    19,    15,    16,    37,    36,    36,
      20,    41,    34,   129,    17,    36,   132,   133,   134,    45,
      30,    31,    40,    17,    38,     1,    36,     3,   148,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,   155,
     156,    17,    38,   175,   175,   161,   162,    24,    25,    26,
      27,    28,   165,    29,    52,    -1,    32,    -1,    -1,   175,
      36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,
     186,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      12,    13,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    -1,    30,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,    -1,
      42,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    -1,     1,    17,    -1,     4,    -1,    -1,    22,    23,
      24,    -1,    -1,    -1,    -1,    29,    15,    16,    32,    -1,
      -1,    20,    36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    31,    -1,    -1,    -1,     1,    36,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    12,    13,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,     1,    30,    31,    32,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    22,    23,    -1,    -1,    -1,
      -1,    -1,    29,     1,    -1,    32,    -1,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    -1,    -1,    17,
      18,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    29,     1,    -1,    32,    -1,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,     1,    -1,    17,    -1,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      29,    -1,    17,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    29,     1,    -1,    32,    -1,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,     0,
       1,    17,    -1,     4,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    29,    15,    16,    32,    -1,    -1,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      31,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      12,    13,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    -1,    30,    31,
      32,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      12,    13,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    -1,    30,    31,
      32,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      12,    13,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    -1,    30,    31,
      32,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    32
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     4,    15,    16,    20,    30,    31,    47,    48,
      51,    52,    54,    57,    59,    62,    64,    73,    75,    80,
      34,     1,     3,     4,     5,     6,     7,     8,     9,    10,
      12,    13,    14,    15,    16,    17,    18,    20,    22,    23,
      24,    25,    26,    27,    28,    30,    31,    32,    87,    17,
      17,    87,    39,     1,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    17,    29,    32,    81,    82,    83,
      84,    85,    86,    17,    87,     0,    49,    53,    63,    74,
      50,    58,    86,     1,    44,    44,    17,     6,     7,     8,
      38,    17,    42,    87,    43,    29,    34,    35,    35,    35,
      48,    35,    37,     1,    82,     1,    82,    24,    25,    26,
      27,    28,    79,    39,    43,    17,     1,    24,    17,    48,
      17,    55,    56,    87,     1,     3,    65,    66,    67,    68,
      69,    82,    22,    23,    24,    76,    77,    78,    81,    48,
      60,     1,    45,    37,    24,    24,    38,    36,    37,    34,
      36,    34,    18,    21,    70,    71,    72,    81,    19,    81,
      81,    22,    23,    81,    36,    34,    36,    17,    61,    82,
      40,    79,    55,    65,    65,    37,    41,    81,    81,    81,
      81,    76,    37,    40,    45,    71,    72,    81,    38,    38,
      17,    81,    81,    79,    79
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
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
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


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
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
		  Type, Value); \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
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
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
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
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

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

/* Number of syntax errors so far.  */
int yynerrs;


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
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
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

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

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
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

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
  if (yypact_value_is_default (yyn))
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
      if (yytable_value_is_error (yyn))
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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:

/* Line 1810 of yacc.c  */
#line 57 "jce.y"
    {
}
    break;

  case 5:

/* Line 1810 of yacc.c  */
#line 61 "jce.y"
    {
    yyerrok;
}
    break;

  case 7:

/* Line 1810 of yacc.c  */
#line 66 "jce.y"
    {
    g_parse->error("`;' missing after definition");
}
    break;

  case 8:

/* Line 1810 of yacc.c  */
#line 70 "jce.y"
    {
}
    break;

  case 9:

/* Line 1810 of yacc.c  */
#line 78 "jce.y"
    {
    assert((yyvsp[(1) - (1)]) == 0 || NamespacePtr::dynamicCast((yyvsp[(1) - (1)])));
}
    break;

  case 10:

/* Line 1810 of yacc.c  */
#line 82 "jce.y"
    {
    assert((yyvsp[(1) - (1)]) == 0 || InterfacePtr::dynamicCast((yyvsp[(1) - (1)])));
}
    break;

  case 11:

/* Line 1810 of yacc.c  */
#line 86 "jce.y"
    {
    assert((yyvsp[(1) - (1)]) == 0 || StructPtr::dynamicCast((yyvsp[(1) - (1)])));
}
    break;

  case 12:

/* Line 1810 of yacc.c  */
#line 90 "jce.y"
    {
}
    break;

  case 13:

/* Line 1810 of yacc.c  */
#line 93 "jce.y"
    {
    assert((yyvsp[(1) - (1)]) == 0 || EnumPtr::dynamicCast((yyvsp[(1) - (1)])));
}
    break;

  case 14:

/* Line 1810 of yacc.c  */
#line 97 "jce.y"
    {
    assert((yyvsp[(1) - (1)]) == 0 || ConstPtr::dynamicCast((yyvsp[(1) - (1)])));
}
    break;

  case 15:

/* Line 1810 of yacc.c  */
#line 106 "jce.y"
    {
    (yyval) = (yyvsp[(1) - (1)]);
}
    break;

  case 16:

/* Line 1810 of yacc.c  */
#line 110 "jce.y"
    {
    if((yyvsp[(3) - (5)]))
    {
        g_parse->popContainer();
        (yyval) = (yyvsp[(3) - (5)]);
    }
    else
    {
        (yyval) = 0;
    }

    (yyval) = (yyvsp[(2) - (5)]);
}
    break;

  case 17:

/* Line 1810 of yacc.c  */
#line 129 "jce.y"
    {
    NamespacePtr c = NamespacePtr::dynamicCast(g_parse->currentContainer());
    if(!c)
    {
        g_parse->error("enum must define in namespace");
    }
    StringTokPtr ident  = StringTokPtr::dynamicCast((yyvsp[(2) - (2)]));
    EnumPtr e = c->createEnum(ident->v);
    g_parse->pushContainer(e);

    (yyval) = e;
}
    break;

  case 18:

/* Line 1810 of yacc.c  */
#line 142 "jce.y"
    {
    StringTokPtr ident = StringTokPtr::dynamicCast((yyvsp[(2) - (2)]));
    g_parse->error("keyword `" + ident->v + "' cannot be used as enumeration name");
    (yyval) = (yyvsp[(2) - (2)]);
}
    break;

  case 19:

/* Line 1810 of yacc.c  */
#line 153 "jce.y"
    {
    (yyval) = (yyvsp[(2) - (3)]);
}
    break;

  case 20:

/* Line 1810 of yacc.c  */
#line 157 "jce.y"
    {
}
    break;

  case 21:

/* Line 1810 of yacc.c  */
#line 165 "jce.y"
    {
    TypePtr type        = TypePtr::dynamicCast(g_parse->createBuiltin(Builtin::KindLong));
    StringTokPtr ident  = StringTokPtr::dynamicCast((yyvsp[(1) - (1)]));
    TypeIdPtr tPtr      = new TypeId(type, ident->v);
    tPtr->disableDefault();
    EnumPtr e = EnumPtr::dynamicCast(g_parse->currentContainer());
    assert(e);
    e->addMember(tPtr);
    (yyval) = e;
}
    break;

  case 22:

/* Line 1810 of yacc.c  */
#line 176 "jce.y"
    {
    StringTokPtr ident = StringTokPtr::dynamicCast((yyvsp[(1) - (1)]));
    g_parse->error("keyword `" + ident->v + "' cannot be used as enumerator");
}
    break;

  case 23:

/* Line 1810 of yacc.c  */
#line 181 "jce.y"
    {
    TypePtr type        = TypePtr::dynamicCast(g_parse->createBuiltin(Builtin::KindLong));
    StringTokPtr ident  = StringTokPtr::dynamicCast((yyvsp[(1) - (3)]));
    TypeIdPtr tPtr      = new TypeId(type, ident->v);
    ConstTokPtr sPtr    = ConstTokPtr::dynamicCast((yyvsp[(3) - (3)]));
    g_parse->checkConstValue(tPtr, sPtr->t);
    tPtr->setDefault(sPtr->v);
    EnumPtr e = EnumPtr::dynamicCast(g_parse->currentContainer());
    assert(e);
    e->addMember(tPtr);
    (yyval) = e;
}
    break;

  case 24:

/* Line 1810 of yacc.c  */
#line 194 "jce.y"
    {
}
    break;

  case 25:

/* Line 1810 of yacc.c  */
#line 202 "jce.y"
    {
    StringTokPtr ident  = StringTokPtr::dynamicCast((yyvsp[(2) - (2)]));
    ContainerPtr c      = g_parse->currentContainer();
    NamespacePtr n      = c->createNamespace(ident->v);
    if(n)
    {
        g_parse->pushContainer(n);
        (yyval) = GrammarBasePtr::dynamicCast(n);
    }
    else
    {
        (yyval) = 0;
    }
}
    break;

  case 26:

/* Line 1810 of yacc.c  */
#line 217 "jce.y"
    {
    if((yyvsp[(3) - (6)]))
    {
        g_parse->popContainer();
        (yyval) = (yyvsp[(3) - (6)]);
    }
    else
    {
        (yyval) = 0;
    }
}
    break;

  case 27:

/* Line 1810 of yacc.c  */
#line 235 "jce.y"
    {
    StringTokPtr ident = StringTokPtr::dynamicCast((yyvsp[(3) - (4)]));
    StructPtr sp = StructPtr::dynamicCast(g_parse->findUserType(ident->v));
    if(!sp)
    {
        g_parse->error("struct '" + ident->v + "' undefined!");
    }

    g_parse->setKeyStruct(sp);
}
    break;

  case 28:

/* Line 1810 of yacc.c  */
#line 246 "jce.y"
    {
}
    break;

  case 29:

/* Line 1810 of yacc.c  */
#line 254 "jce.y"
    {
    StringTokPtr ident = StringTokPtr::dynamicCast((yyvsp[(1) - (1)]));
    StructPtr np = g_parse->getKeyStruct();
    if(np)
    {
        np->addKey(ident->v);
    }
    else
    {
        (yyval) = 0;
    }
}
    break;

  case 30:

/* Line 1810 of yacc.c  */
#line 267 "jce.y"
    {
    StringTokPtr ident = StringTokPtr::dynamicCast((yyvsp[(3) - (3)]));
    StructPtr np = g_parse->getKeyStruct();
    if(np)
    {
        np->addKey(ident->v);
    }
    else
    {
        (yyval) = 0;
    }   
}
    break;

  case 31:

/* Line 1810 of yacc.c  */
#line 286 "jce.y"
    {
    StringTokPtr ident = StringTokPtr::dynamicCast((yyvsp[(1) - (1)]));

    NamespacePtr c = NamespacePtr::dynamicCast(g_parse->currentContainer());

    InterfacePtr cl = c->createInterface(ident->v);
    if(cl)
    {
        g_parse->pushContainer(cl);
        (yyval) = GrammarBasePtr::dynamicCast(cl);
    }
    else
    {
        (yyval) = 0;
    }
}
    break;

  case 32:

/* Line 1810 of yacc.c  */
#line 303 "jce.y"
    {
    if((yyvsp[(2) - (5)]))
    {
       g_parse->popContainer();
       (yyval) = GrammarBasePtr::dynamicCast((yyvsp[(2) - (5)]));
    }
    else
    {
       (yyval) = 0;
    }
}
    break;

  case 33:

/* Line 1810 of yacc.c  */
#line 320 "jce.y"
    {
    (yyval) = (yyvsp[(2) - (2)]);
}
    break;

  case 34:

/* Line 1810 of yacc.c  */
#line 324 "jce.y"
    {
    StringTokPtr ident = StringTokPtr::dynamicCast((yyvsp[(2) - (2)]));
    g_parse->error("keyword `" + ident->v + "' cannot be used as interface name");
    (yyval) = (yyvsp[(2) - (2)]);
}
    break;

  case 35:

/* Line 1810 of yacc.c  */
#line 335 "jce.y"
    {
}
    break;

  case 36:

/* Line 1810 of yacc.c  */
#line 338 "jce.y"
    {
}
    break;

  case 37:

/* Line 1810 of yacc.c  */
#line 341 "jce.y"
    {
    g_parse->error("`;' missing after definition");
}
    break;

  case 38:

/* Line 1810 of yacc.c  */
#line 345 "jce.y"
    {
}
    break;

  case 40:

/* Line 1810 of yacc.c  */
#line 359 "jce.y"
    {
    if((yyvsp[(1) - (3)]))
    {
        g_parse->popContainer();
        (yyval) = GrammarBasePtr::dynamicCast((yyvsp[(1) - (3)]));
    }
    else
    {
        (yyval) = 0;
    }
}
    break;

  case 41:

/* Line 1810 of yacc.c  */
#line 376 "jce.y"
    {
    TypePtr returnType = TypePtr::dynamicCast((yyvsp[(1) - (2)]));
    StringTokPtr ident = StringTokPtr::dynamicCast((yyvsp[(2) - (2)]));
    string name        = ident->v;
    InterfacePtr cl    = InterfacePtr::dynamicCast(g_parse->currentContainer());
    if(cl)
    {
         OperationPtr op = cl->createOperation(name, returnType);
         if(op)
         {
             g_parse->pushContainer(op);
             (yyval) = GrammarBasePtr::dynamicCast(op);
         }
         else
         {
             (yyval) = 0;
         }
    }
    else
    {
        (yyval) = 0;
    }
}
    break;

  case 43:

/* Line 1810 of yacc.c  */
#line 406 "jce.y"
    {
    (yyval) = 0;
}
    break;

  case 44:

/* Line 1810 of yacc.c  */
#line 416 "jce.y"
    {
}
    break;

  case 45:

/* Line 1810 of yacc.c  */
#line 419 "jce.y"
    {
    TypeIdPtr  tsp         = TypeIdPtr::dynamicCast((yyvsp[(1) - (1)]));

    OperationPtr op = OperationPtr::dynamicCast(g_parse->currentContainer());
    assert(op);
    if(op)
    {
        op->createParamDecl(tsp, false, false);
    }
}
    break;

  case 46:

/* Line 1810 of yacc.c  */
#line 430 "jce.y"
    {
    TypeIdPtr  tsp         = TypeIdPtr::dynamicCast((yyvsp[(3) - (3)]));

    OperationPtr op = OperationPtr::dynamicCast(g_parse->currentContainer());
    assert(op);
    if(op)
    {
        op->createParamDecl(tsp, false, false);
    }
}
    break;

  case 47:

/* Line 1810 of yacc.c  */
#line 441 "jce.y"
    {
    BoolTokPtr isOutParam  = BoolTokPtr::dynamicCast((yyvsp[(1) - (2)]));
    TypeIdPtr  tsp         = TypeIdPtr::dynamicCast((yyvsp[(2) - (2)]));

    OperationPtr op = OperationPtr::dynamicCast(g_parse->currentContainer());
    assert(op);
    if(op)
    {
        op->createParamDecl(tsp, isOutParam->v, false);
    }
}
    break;

  case 48:

/* Line 1810 of yacc.c  */
#line 453 "jce.y"
    {
    BoolTokPtr isOutParam  = BoolTokPtr::dynamicCast((yyvsp[(3) - (4)]));
    TypeIdPtr  tsp         = TypeIdPtr::dynamicCast((yyvsp[(4) - (4)]));

    OperationPtr op = OperationPtr::dynamicCast(g_parse->currentContainer());
    assert(op);
    if(op)
    {
        op->createParamDecl(tsp, isOutParam->v, false);
    }
}
    break;

  case 49:

/* Line 1810 of yacc.c  */
#line 465 "jce.y"
    {
    BoolTokPtr isRouteKeyParam  = BoolTokPtr::dynamicCast((yyvsp[(1) - (2)]));
    TypeIdPtr  tsp              = TypeIdPtr::dynamicCast((yyvsp[(2) - (2)]));

    OperationPtr op = OperationPtr::dynamicCast(g_parse->currentContainer());
    assert(op);
    if(op)
    {
         op->createParamDecl(tsp, false, isRouteKeyParam->v);
    }
}
    break;

  case 50:

/* Line 1810 of yacc.c  */
#line 477 "jce.y"
    {
    BoolTokPtr isRouteKeyParam = BoolTokPtr::dynamicCast((yyvsp[(3) - (4)]));
    TypeIdPtr  tsp             = TypeIdPtr::dynamicCast((yyvsp[(4) - (4)]));

    OperationPtr op = OperationPtr::dynamicCast(g_parse->currentContainer());
    assert(op);
    if(op)
    {
         op->createParamDecl(tsp, false, isRouteKeyParam->v);
    }
}
    break;

  case 51:

/* Line 1810 of yacc.c  */
#line 489 "jce.y"
    {
    g_parse->error("'out' must be defined with a type");
}
    break;

  case 52:

/* Line 1810 of yacc.c  */
#line 493 "jce.y"
    {
    g_parse->error("'routekey' must be defined with a type");
}
    break;

  case 53:

/* Line 1810 of yacc.c  */
#line 502 "jce.y"
    {
    BoolTokPtr routekey = new BoolTok;
    routekey->v = true;
    (yyval) = GrammarBasePtr::dynamicCast(routekey);
}
    break;

  case 54:

/* Line 1810 of yacc.c  */
#line 513 "jce.y"
    {
    BoolTokPtr out = new BoolTok;
    out->v = true;
    (yyval) = GrammarBasePtr::dynamicCast(out);
}
    break;

  case 55:

/* Line 1810 of yacc.c  */
#line 524 "jce.y"
    {
    StringTokPtr ident = StringTokPtr::dynamicCast((yyvsp[(1) - (1)]));
    NamespacePtr np = NamespacePtr::dynamicCast(g_parse->currentContainer());
    if(np)
    {
         StructPtr sp = np->createStruct(ident->v);
         if(sp)
         {
             g_parse->pushContainer(sp);
             (yyval) = GrammarBasePtr::dynamicCast(sp);
         }
         else
         {
             (yyval) = 0;
         }
    }
    else
    {
       g_parse->error("struct '" + ident->v + "' must definition in namespace");
    }
}
    break;

  case 56:

/* Line 1810 of yacc.c  */
#line 546 "jce.y"
    {
    if((yyvsp[(2) - (5)]))
    {
        g_parse->popContainer();
    }
    (yyval) = (yyvsp[(2) - (5)]);

    //不能有空结构
    StructPtr st = StructPtr::dynamicCast((yyval));
    assert(st);
    if(st->getAllMemberPtr().size() == 0)
    {
        g_parse->error("struct `" + st->getSid() + "' must have at least one member");
    }
}
    break;

  case 57:

/* Line 1810 of yacc.c  */
#line 567 "jce.y"
    {
    (yyval) = (yyvsp[(2) - (2)]);
}
    break;

  case 58:

/* Line 1810 of yacc.c  */
#line 571 "jce.y"
    {
    StringTokPtr ident = StringTokPtr::dynamicCast((yyvsp[(2) - (2)]));

    g_parse->error("keyword `" + ident->v + "' cannot be used as struct name");
}
    break;

  case 59:

/* Line 1810 of yacc.c  */
#line 577 "jce.y"
    {
    g_parse->error("abstract declarator '<anonymous struct>' used as declaration");
}
    break;

  case 60:

/* Line 1810 of yacc.c  */
#line 586 "jce.y"
    {

}
    break;

  case 61:

/* Line 1810 of yacc.c  */
#line 590 "jce.y"
    {
   g_parse->error("';' missing after definition");
}
    break;

  case 62:

/* Line 1810 of yacc.c  */
#line 594 "jce.y"
    {
}
    break;

  case 63:

/* Line 1810 of yacc.c  */
#line 604 "jce.y"
    {
    (yyval) = GrammarBasePtr::dynamicCast((yyvsp[(1) - (1)]));
}
    break;

  case 64:

/* Line 1810 of yacc.c  */
#line 613 "jce.y"
    {
    StructPtr np = StructPtr::dynamicCast(g_parse->currentContainer());
    if(np)
    {
        IntegerTokPtr iPtr = IntegerTokPtr::dynamicCast((yyvsp[(1) - (3)]));
        g_parse->checkTag(iPtr->v);

        TypeIdPtr tPtr  = TypeIdPtr::dynamicCast((yyvsp[(3) - (3)]));
        tPtr->setRequire(iPtr->v);
        np->addTypeId(tPtr);
        (yyval) = GrammarBasePtr::dynamicCast((yyvsp[(3) - (3)]));
    }
    else
    {
        (yyval) = 0;
    }
}
    break;

  case 65:

/* Line 1810 of yacc.c  */
#line 631 "jce.y"
    {
    StructPtr np = StructPtr::dynamicCast(g_parse->currentContainer());
    if(np)
    {
        IntegerTokPtr iPtr = IntegerTokPtr::dynamicCast((yyvsp[(1) - (5)]));
        g_parse->checkTag(iPtr->v);

        //判断类型和数值类型是否一致
        TypeIdPtr tPtr   = TypeIdPtr::dynamicCast((yyvsp[(3) - (5)]));
        ConstTokPtr sPtr = ConstTokPtr::dynamicCast((yyvsp[(5) - (5)]));
        g_parse->checkConstValue(tPtr, sPtr->t);

        tPtr->setRequire(iPtr->v);
        tPtr->setDefault(sPtr->v);
        np->addTypeId(tPtr);
        (yyval) = GrammarBasePtr::dynamicCast((yyvsp[(3) - (5)]));
    }
    else
    {
        (yyval) = 0;
    }
}
    break;

  case 66:

/* Line 1810 of yacc.c  */
#line 654 "jce.y"
    {
    StructPtr np = StructPtr::dynamicCast(g_parse->currentContainer());
    if(np)
    {
        IntegerTokPtr iPtr = IntegerTokPtr::dynamicCast((yyvsp[(1) - (5)]));
        g_parse->checkTag(iPtr->v);

        //判断类型和数值类型是否一致
        TypeIdPtr tPtr   = TypeIdPtr::dynamicCast((yyvsp[(3) - (5)]));
        ConstTokPtr sPtr = ConstTokPtr::dynamicCast((yyvsp[(5) - (5)]));
        g_parse->checkConstValue(tPtr, sPtr->t);

        tPtr->setOptional(iPtr->v);
        tPtr->setDefault(sPtr->v);
        np->addTypeId(tPtr);
        (yyval) = GrammarBasePtr::dynamicCast((yyvsp[(3) - (5)]));
    }
    else
    {
        (yyval) = 0;
    }
}
    break;

  case 67:

/* Line 1810 of yacc.c  */
#line 677 "jce.y"
    {
    StructPtr np = StructPtr::dynamicCast(g_parse->currentContainer());
    if(np)
    {
        IntegerTokPtr iPtr = IntegerTokPtr::dynamicCast((yyvsp[(1) - (3)]));
        g_parse->checkTag(iPtr->v);
        TypeIdPtr tPtr = TypeIdPtr::dynamicCast((yyvsp[(3) - (3)]));
        tPtr->setOptional(iPtr->v);
        np->addTypeId(tPtr);
        (yyval) = GrammarBasePtr::dynamicCast((yyvsp[(3) - (3)]));
    }
    else
    {
        (yyval) = 0;
    }
}
    break;

  case 68:

/* Line 1810 of yacc.c  */
#line 694 "jce.y"
    {
    g_parse->error("struct member need 'tag'");
}
    break;

  case 69:

/* Line 1810 of yacc.c  */
#line 698 "jce.y"
    {
    g_parse->error("struct member need 'tag'");
}
    break;

  case 70:

/* Line 1810 of yacc.c  */
#line 702 "jce.y"
    {
    g_parse->error("struct member need 'require' or 'optional'");
}
    break;

  case 71:

/* Line 1810 of yacc.c  */
#line 706 "jce.y"
    {
    g_parse->error("struct member need 'tag' or 'require' or 'optional'");
}
    break;

  case 72:

/* Line 1810 of yacc.c  */
#line 715 "jce.y"
    {
    IntegerTokPtr intVal = IntegerTokPtr::dynamicCast((yyvsp[(1) - (1)]));
    ostringstream sstr;
    sstr << intVal->v;
    ConstTokPtr c = new ConstTok();
    c->t = ConstTok::VALUE;
    c->v = sstr.str();
    (yyval) = c;
}
    break;

  case 73:

/* Line 1810 of yacc.c  */
#line 725 "jce.y"
    {
    FloatTokPtr floatVal = FloatTokPtr::dynamicCast((yyvsp[(1) - (1)]));
    ostringstream sstr;
    sstr << floatVal->v;
    ConstTokPtr c = new ConstTok();
    c->t = ConstTok::VALUE;
    c->v = sstr.str();
    (yyval) = c;
}
    break;

  case 74:

/* Line 1810 of yacc.c  */
#line 735 "jce.y"
    {
    StringTokPtr ident = StringTokPtr::dynamicCast((yyvsp[(1) - (1)]));
    ConstTokPtr c = new ConstTok();
    c->t = ConstTok::STRING;
    c->v = ident->v;
    (yyval) = c;
}
    break;

  case 75:

/* Line 1810 of yacc.c  */
#line 743 "jce.y"
    {
    StringTokPtr ident = StringTokPtr::dynamicCast((yyvsp[(1) - (1)]));
    ConstTokPtr c = new ConstTok();
    c->t = ConstTok::BOOL;
    c->v = ident->v;
    (yyval) = c;
}
    break;

  case 76:

/* Line 1810 of yacc.c  */
#line 751 "jce.y"
    {
    StringTokPtr ident = StringTokPtr::dynamicCast((yyvsp[(1) - (1)]));
    ConstTokPtr c = new ConstTok();
    c->t = ConstTok::BOOL;
    c->v = ident->v;
    (yyval) = c;
}
    break;

  case 77:

/* Line 1810 of yacc.c  */
#line 764 "jce.y"
    {
    NamespacePtr np = NamespacePtr::dynamicCast(g_parse->currentContainer());
    if(!np)
    {
        g_parse->error("const type must define in namespace");
    }

    TypeIdPtr t   = TypeIdPtr::dynamicCast((yyvsp[(2) - (4)]));
    ConstTokPtr c = ConstTokPtr::dynamicCast((yyvsp[(4) - (4)]));
    ConstPtr cPtr = np->createConst(t, c);
    (yyval) = cPtr;
}
    break;

  case 78:

/* Line 1810 of yacc.c  */
#line 782 "jce.y"
    {
    TypePtr type = TypePtr::dynamicCast((yyvsp[(1) - (2)]));
    StringTokPtr ident = StringTokPtr::dynamicCast((yyvsp[(2) - (2)]));

    TypeIdPtr typeIdPtr = new TypeId(type, ident->v);

    (yyval) = GrammarBasePtr::dynamicCast(typeIdPtr);
}
    break;

  case 79:

/* Line 1810 of yacc.c  */
#line 791 "jce.y"
    {
    TypePtr type = g_parse->createVector(TypePtr::dynamicCast((yyvsp[(1) - (5)])));
    IntegerTokPtr iPtrSize = IntegerTokPtr::dynamicCast((yyvsp[(4) - (5)]));
    g_parse->checkArrayVaid(type,iPtrSize->v);
    type->setArray(iPtrSize->v);
    StringTokPtr ident = StringTokPtr::dynamicCast((yyvsp[(2) - (5)]));
    TypeIdPtr typeIdPtr = new TypeId(type, ident->v);
    (yyval) = GrammarBasePtr::dynamicCast(typeIdPtr);
}
    break;

  case 80:

/* Line 1810 of yacc.c  */
#line 801 "jce.y"
    {
    TypePtr type = g_parse->createVector(TypePtr::dynamicCast((yyvsp[(1) - (3)])));
    //IntegerTokPtr iPtrSize = IntegerTokPtr::dynamicCast($4);
    g_parse->checkPointerVaid(type);
    type->setPointer(true);
    StringTokPtr ident = StringTokPtr::dynamicCast((yyvsp[(3) - (3)]));
    TypeIdPtr typeIdPtr = new TypeId(type, ident->v);
    (yyval) = GrammarBasePtr::dynamicCast(typeIdPtr);
}
    break;

  case 81:

/* Line 1810 of yacc.c  */
#line 811 "jce.y"
    {
    TypePtr type = TypePtr::dynamicCast((yyvsp[(1) - (4)]));
    StringTokPtr ident = StringTokPtr::dynamicCast((yyvsp[(2) - (4)]));
    TypeIdPtr typeIdPtr = new TypeId(type, ident->v);
    IntegerTokPtr iPtrSize = IntegerTokPtr::dynamicCast((yyvsp[(3) - (4)]));
    g_parse->checkArrayVaid(type,iPtrSize->v);
    (yyval) = GrammarBasePtr::dynamicCast(typeIdPtr);
}
    break;

  case 82:

/* Line 1810 of yacc.c  */
#line 820 "jce.y"
    {
    StringTokPtr ident = StringTokPtr::dynamicCast((yyvsp[(2) - (2)]));
    g_parse->error("keyword `" + ident->v + "' cannot be used as data member name");
}
    break;

  case 83:

/* Line 1810 of yacc.c  */
#line 825 "jce.y"
    {
    g_parse->error("missing data member name");
}
    break;

  case 84:

/* Line 1810 of yacc.c  */
#line 829 "jce.y"
    {
    g_parse->error("unkown type");
}
    break;

  case 85:

/* Line 1810 of yacc.c  */
#line 838 "jce.y"
    {

    TypePtr type = TypePtr::dynamicCast((yyvsp[(1) - (3)]));
    IntegerTokPtr iPtrSize = IntegerTokPtr::dynamicCast((yyvsp[(3) - (3)]));
    g_parse->checkArrayVaid(type,iPtrSize->v);
    type->setArray(iPtrSize->v);
    (yyval) = type;
}
    break;

  case 86:

/* Line 1810 of yacc.c  */
#line 847 "jce.y"
    {
    (yyval) = (yyvsp[(1) - (1)]);
}
    break;

  case 87:

/* Line 1810 of yacc.c  */
#line 851 "jce.y"
    {
   g_parse->error("array missing size");
}
    break;

  case 88:

/* Line 1810 of yacc.c  */
#line 860 "jce.y"
    {
    (yyval) = g_parse->createBuiltin(Builtin::KindBool);
}
    break;

  case 89:

/* Line 1810 of yacc.c  */
#line 864 "jce.y"
    {
    (yyval) = g_parse->createBuiltin(Builtin::KindByte);
}
    break;

  case 90:

/* Line 1810 of yacc.c  */
#line 868 "jce.y"
    {
    (yyval) = g_parse->createBuiltin(Builtin::KindShort,true);
}
    break;

  case 91:

/* Line 1810 of yacc.c  */
#line 872 "jce.y"
    {
    (yyval) = g_parse->createBuiltin(Builtin::KindShort);
}
    break;

  case 92:

/* Line 1810 of yacc.c  */
#line 876 "jce.y"
    {
    (yyval) = g_parse->createBuiltin(Builtin::KindInt,true);
}
    break;

  case 93:

/* Line 1810 of yacc.c  */
#line 880 "jce.y"
    {
    (yyval) = g_parse->createBuiltin(Builtin::KindInt);
}
    break;

  case 94:

/* Line 1810 of yacc.c  */
#line 884 "jce.y"
    {
    (yyval) = g_parse->createBuiltin(Builtin::KindLong,true);
}
    break;

  case 95:

/* Line 1810 of yacc.c  */
#line 888 "jce.y"
    {
    (yyval) = g_parse->createBuiltin(Builtin::KindLong);
}
    break;

  case 96:

/* Line 1810 of yacc.c  */
#line 892 "jce.y"
    {
    (yyval) = g_parse->createBuiltin(Builtin::KindFloat);
}
    break;

  case 97:

/* Line 1810 of yacc.c  */
#line 896 "jce.y"
    {
    (yyval) = g_parse->createBuiltin(Builtin::KindDouble);
}
    break;

  case 98:

/* Line 1810 of yacc.c  */
#line 900 "jce.y"
    {
    (yyval) = g_parse->createBuiltin(Builtin::KindString);
}
    break;

  case 99:

/* Line 1810 of yacc.c  */
#line 904 "jce.y"
    {
   (yyval) = GrammarBasePtr::dynamicCast((yyvsp[(1) - (1)]));
}
    break;

  case 100:

/* Line 1810 of yacc.c  */
#line 908 "jce.y"
    {
   (yyval) = GrammarBasePtr::dynamicCast((yyvsp[(1) - (1)]));
}
    break;

  case 101:

/* Line 1810 of yacc.c  */
#line 912 "jce.y"
    {
    StringTokPtr ident = StringTokPtr::dynamicCast((yyvsp[(1) - (1)]));
    TypePtr sp = g_parse->findUserType(ident->v);
    if(sp)
    {
        (yyval) = GrammarBasePtr::dynamicCast(sp);
    }
    else
    {
        g_parse->error("'" + ident->v + "' undefined!");
    }
}
    break;

  case 102:

/* Line 1810 of yacc.c  */
#line 930 "jce.y"
    {
   (yyval) = GrammarBasePtr::dynamicCast(g_parse->createVector(TypePtr::dynamicCast((yyvsp[(3) - (4)]))));
}
    break;

  case 103:

/* Line 1810 of yacc.c  */
#line 934 "jce.y"
    {
   g_parse->error("vector error");
}
    break;

  case 104:

/* Line 1810 of yacc.c  */
#line 938 "jce.y"
    {
   g_parse->error("vector missing '>'");
}
    break;

  case 105:

/* Line 1810 of yacc.c  */
#line 942 "jce.y"
    {
   g_parse->error("vector missing type");
}
    break;

  case 106:

/* Line 1810 of yacc.c  */
#line 951 "jce.y"
    {
   (yyval) = GrammarBasePtr::dynamicCast(g_parse->createMap(TypePtr::dynamicCast((yyvsp[(3) - (6)])), TypePtr::dynamicCast((yyvsp[(5) - (6)]))));
}
    break;

  case 107:

/* Line 1810 of yacc.c  */
#line 955 "jce.y"
    {
   g_parse->error("map error");
}
    break;

  case 108:

/* Line 1810 of yacc.c  */
#line 964 "jce.y"
    {
}
    break;

  case 109:

/* Line 1810 of yacc.c  */
#line 967 "jce.y"
    {
    StringTokPtr ident = StringTokPtr::dynamicCast((yyvsp[(2) - (2)]));
    ident->v = "::" + ident->v;
    (yyval) = GrammarBasePtr::dynamicCast(ident);
}
    break;

  case 110:

/* Line 1810 of yacc.c  */
#line 973 "jce.y"
    {
    StringTokPtr scoped = StringTokPtr::dynamicCast((yyvsp[(1) - (3)]));
    StringTokPtr ident  = StringTokPtr::dynamicCast((yyvsp[(3) - (3)]));
    scoped->v += "::";
    scoped->v += ident->v;
    (yyval) = GrammarBasePtr::dynamicCast(scoped);
}
    break;

  case 111:

/* Line 1810 of yacc.c  */
#line 986 "jce.y"
    {
}
    break;

  case 112:

/* Line 1810 of yacc.c  */
#line 989 "jce.y"
    {
}
    break;

  case 113:

/* Line 1810 of yacc.c  */
#line 992 "jce.y"
    {
}
    break;

  case 114:

/* Line 1810 of yacc.c  */
#line 995 "jce.y"
    {
}
    break;

  case 115:

/* Line 1810 of yacc.c  */
#line 998 "jce.y"
    {
}
    break;

  case 116:

/* Line 1810 of yacc.c  */
#line 1001 "jce.y"
    {
}
    break;

  case 117:

/* Line 1810 of yacc.c  */
#line 1004 "jce.y"
    {
}
    break;

  case 118:

/* Line 1810 of yacc.c  */
#line 1007 "jce.y"
    {
}
    break;

  case 119:

/* Line 1810 of yacc.c  */
#line 1010 "jce.y"
    {
}
    break;

  case 120:

/* Line 1810 of yacc.c  */
#line 1013 "jce.y"
    {
}
    break;

  case 121:

/* Line 1810 of yacc.c  */
#line 1016 "jce.y"
    {
}
    break;

  case 122:

/* Line 1810 of yacc.c  */
#line 1019 "jce.y"
    {
}
    break;

  case 123:

/* Line 1810 of yacc.c  */
#line 1022 "jce.y"
    {
}
    break;

  case 124:

/* Line 1810 of yacc.c  */
#line 1025 "jce.y"
    {
}
    break;

  case 125:

/* Line 1810 of yacc.c  */
#line 1028 "jce.y"
    {
}
    break;

  case 126:

/* Line 1810 of yacc.c  */
#line 1031 "jce.y"
    {
}
    break;

  case 127:

/* Line 1810 of yacc.c  */
#line 1034 "jce.y"
    {
}
    break;

  case 128:

/* Line 1810 of yacc.c  */
#line 1037 "jce.y"
    {
}
    break;

  case 129:

/* Line 1810 of yacc.c  */
#line 1040 "jce.y"
    {
}
    break;

  case 130:

/* Line 1810 of yacc.c  */
#line 1043 "jce.y"
    {
}
    break;

  case 131:

/* Line 1810 of yacc.c  */
#line 1046 "jce.y"
    {
}
    break;

  case 132:

/* Line 1810 of yacc.c  */
#line 1049 "jce.y"
    {
}
    break;

  case 133:

/* Line 1810 of yacc.c  */
#line 1052 "jce.y"
    {
}
    break;

  case 134:

/* Line 1810 of yacc.c  */
#line 1055 "jce.y"
    {
}
    break;

  case 135:

/* Line 1810 of yacc.c  */
#line 1058 "jce.y"
    {
}
    break;



/* Line 1810 of yacc.c  */
#line 3146 "jce.tab.cpp"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

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
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



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
		      yytoken, &yylval);
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
      if (!yypact_value_is_default (yyn))
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


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


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

#if !defined yyoverflow || YYERROR_VERBOSE
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
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
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



/* Line 2071 of yacc.c  */
#line 1062 "jce.y"




