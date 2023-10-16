/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "lab6.y"

/*
    Name: Dominik Trujillo
    Date: 09/26/2023
    Lab: LAB 6 ALGOL Abstract Syntax Tree
    Purpose: The purpose of this lab is to enhance our existing parsing and syntax-checking capabilities by extending 
    the functionality of the YACC program. Building upon the knowledge gained in previous
    labs, the primary objective is to create an Abstract Syntax Tree (AST), which serves as an Intermediate Representation 
    (IR) data structure for the parsed input program. This AST will facilitate the execution of multiple passes over the 
    source code that is given.

    To achieve this, our primary task is to modify the YACC program so that it constructs 
    the AST during the shift/reduce processes. This involves adding semantic actions to each production rule, 
    enabling the creation of AST nodes, linking these nodes to represent the program's structure, and ensuring 
    that the relevant information is attached to the yylval companion stack. These AST nodes will be of different 
    types, providing us with valuable insights into the program's structure.

    Upon successful parsing and construction of the AST using the YACC program, the ultimate objective is to 
    have a main() program that prints out the AST. This printout should be designed to reflect the structure of the 
    input program, similar to the example provided in the lab instructions.

    Key tasks for this lab include:

    -Creating (in this case we'll be using and updating the given via the canvas page) a separate "ast.c" and "ast.h" file 
    to house the Abstract Syntax Tree code.

    Adding semantic actions to each production rule in the ALGOL-C submission from the previous 
    lab to ensure AST construction.

    Developing an AST printing routine to assist in debugging the semantic actions.

    Using "AST()" directives as presented in class to guide the development of the Abstract Syntax Tree.

    Documenting all major differences introduced in this lab compared to previous submissions, especially 
    any changes to production rules.

    Preparing to explain and discuss the code during potential in-person assessments.

    Ensuring that the YACC code remains consistent with the LAB 5 submission, without altering 
    non-terminal and terminal names.

    Limiting the use of pointers in the AST to "s1" and "s2" as pointers to other AST nodes, without 
    introducing additional pointers or alternative names.

    Adhering to the naming conventions, such as starting AST enumerated types with "A_" and token names from 
    LEX with "T_" prefixes, to avoid deductions.

    By successfully completing these tasks, we aim to extend our compiler construction capabilities, enabling the 
    creation and utilization of an Abstract Syntax Tree for improved program analysis and code generation. 
    This lab represents a crucial step in achieving our ultimate goal of generating assembly code and running it on a simulator.
*/


	/* begin specs */
#include <stdio.h>
#include <ctype.h>
#include "ast.h"

int yylex();
#define MAX 26
int regs[MAX];
int base, debugsw;
int oneup = 0;
extern int lineno;
extern ASTnode * program;

/*
This method catches any errors made from input
Parameters: a variable s
Pre-condition: If any errors are made
Post-condition: To print out the error
*/
void yyerror (s)  /* Called by yyparse on error */
     char *s;
{
  printf ("%s on line %d\n", s, lineno);
}

#line 150 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    T_NUM = 258,                   /* T_NUM  */
    T_ID = 259,                    /* T_ID  */
    T_STRING = 260,                /* T_STRING  */
    T_INT = 261,                   /* T_INT  */
    T_VOID = 262,                  /* T_VOID  */
    T_BOOLEAN = 263,               /* T_BOOLEAN  */
    T_BEGIN = 264,                 /* T_BEGIN  */
    T_END = 265,                   /* T_END  */
    T_RETURN = 266,                /* T_RETURN  */
    T_READ = 267,                  /* T_READ  */
    T_WRITE = 268,                 /* T_WRITE  */
    T_LE = 269,                    /* T_LE  */
    T_GE = 270,                    /* T_GE  */
    T_EQ = 271,                    /* T_EQ  */
    T_NE = 272,                    /* T_NE  */
    T_AND = 273,                   /* T_AND  */
    T_OR = 274,                    /* T_OR  */
    T_NOT = 275,                   /* T_NOT  */
    T_IF = 276,                    /* T_IF  */
    T_THEN = 277,                  /* T_THEN  */
    T_ELSE = 278,                  /* T_ELSE  */
    T_ENDIF = 279,                 /* T_ENDIF  */
    T_WHILE = 280,                 /* T_WHILE  */
    T_DO = 281,                    /* T_DO  */
    T_TRUE = 282,                  /* T_TRUE  */
    T_FALSE = 283,                 /* T_FALSE  */
    UMINUS = 284                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define T_NUM 258
#define T_ID 259
#define T_STRING 260
#define T_INT 261
#define T_VOID 262
#define T_BOOLEAN 263
#define T_BEGIN 264
#define T_END 265
#define T_RETURN 266
#define T_READ 267
#define T_WRITE 268
#define T_LE 269
#define T_GE 270
#define T_EQ 271
#define T_NE 272
#define T_AND 273
#define T_OR 274
#define T_NOT 275
#define T_IF 276
#define T_THEN 277
#define T_ELSE 278
#define T_ENDIF 279
#define T_WHILE 280
#define T_DO 281
#define T_TRUE 282
#define T_FALSE 283
#define UMINUS 284

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 85 "lab6.y"
 int num;
	 char * string; 
	 ASTnode * node; //Added ASTNode pointer called node into the union.
	 enum DataTypes datatype; //Added DataTypes into the union.
	 enum OPERATORS operator; //added operators to the union
	 

#line 269 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_T_NUM = 3,                      /* T_NUM  */
  YYSYMBOL_T_ID = 4,                       /* T_ID  */
  YYSYMBOL_T_STRING = 5,                   /* T_STRING  */
  YYSYMBOL_T_INT = 6,                      /* T_INT  */
  YYSYMBOL_T_VOID = 7,                     /* T_VOID  */
  YYSYMBOL_T_BOOLEAN = 8,                  /* T_BOOLEAN  */
  YYSYMBOL_T_BEGIN = 9,                    /* T_BEGIN  */
  YYSYMBOL_T_END = 10,                     /* T_END  */
  YYSYMBOL_T_RETURN = 11,                  /* T_RETURN  */
  YYSYMBOL_T_READ = 12,                    /* T_READ  */
  YYSYMBOL_T_WRITE = 13,                   /* T_WRITE  */
  YYSYMBOL_T_LE = 14,                      /* T_LE  */
  YYSYMBOL_T_GE = 15,                      /* T_GE  */
  YYSYMBOL_T_EQ = 16,                      /* T_EQ  */
  YYSYMBOL_T_NE = 17,                      /* T_NE  */
  YYSYMBOL_T_AND = 18,                     /* T_AND  */
  YYSYMBOL_T_OR = 19,                      /* T_OR  */
  YYSYMBOL_T_NOT = 20,                     /* T_NOT  */
  YYSYMBOL_T_IF = 21,                      /* T_IF  */
  YYSYMBOL_T_THEN = 22,                    /* T_THEN  */
  YYSYMBOL_T_ELSE = 23,                    /* T_ELSE  */
  YYSYMBOL_T_ENDIF = 24,                   /* T_ENDIF  */
  YYSYMBOL_T_WHILE = 25,                   /* T_WHILE  */
  YYSYMBOL_T_DO = 26,                      /* T_DO  */
  YYSYMBOL_T_TRUE = 27,                    /* T_TRUE  */
  YYSYMBOL_T_FALSE = 28,                   /* T_FALSE  */
  YYSYMBOL_29_ = 29,                       /* '|'  */
  YYSYMBOL_30_ = 30,                       /* '&'  */
  YYSYMBOL_31_ = 31,                       /* '+'  */
  YYSYMBOL_32_ = 32,                       /* '-'  */
  YYSYMBOL_33_ = 33,                       /* '*'  */
  YYSYMBOL_34_ = 34,                       /* '/'  */
  YYSYMBOL_35_ = 35,                       /* '%'  */
  YYSYMBOL_UMINUS = 36,                    /* UMINUS  */
  YYSYMBOL_37_ = 37,                       /* ';'  */
  YYSYMBOL_38_ = 38,                       /* '['  */
  YYSYMBOL_39_ = 39,                       /* ']'  */
  YYSYMBOL_40_ = 40,                       /* ','  */
  YYSYMBOL_41_ = 41,                       /* '('  */
  YYSYMBOL_42_ = 42,                       /* ')'  */
  YYSYMBOL_43_ = 43,                       /* '='  */
  YYSYMBOL_44_ = 44,                       /* '<'  */
  YYSYMBOL_45_ = 45,                       /* '>'  */
  YYSYMBOL_YYACCEPT = 46,                  /* $accept  */
  YYSYMBOL_Program = 47,                   /* Program  */
  YYSYMBOL_Declarationlist = 48,           /* Declarationlist  */
  YYSYMBOL_Declaration = 49,               /* Declaration  */
  YYSYMBOL_VarDeclaration = 50,            /* VarDeclaration  */
  YYSYMBOL_VarList = 51,                   /* VarList  */
  YYSYMBOL_TypeSpecifier = 52,             /* TypeSpecifier  */
  YYSYMBOL_FunDeclaration = 53,            /* FunDeclaration  */
  YYSYMBOL_Params = 54,                    /* Params  */
  YYSYMBOL_ParamList = 55,                 /* ParamList  */
  YYSYMBOL_Param = 56,                     /* Param  */
  YYSYMBOL_CompoundStmt = 57,              /* CompoundStmt  */
  YYSYMBOL_LocalDeclarations = 58,         /* LocalDeclarations  */
  YYSYMBOL_StatementList = 59,             /* StatementList  */
  YYSYMBOL_Statement = 60,                 /* Statement  */
  YYSYMBOL_ExpressionStmt = 61,            /* ExpressionStmt  */
  YYSYMBOL_SelectionStmt = 62,             /* SelectionStmt  */
  YYSYMBOL_IterationStmt = 63,             /* IterationStmt  */
  YYSYMBOL_ReturnStmt = 64,                /* ReturnStmt  */
  YYSYMBOL_ReadStmt = 65,                  /* ReadStmt  */
  YYSYMBOL_WriteStmt = 66,                 /* WriteStmt  */
  YYSYMBOL_AssignmentStmt = 67,            /* AssignmentStmt  */
  YYSYMBOL_Expression = 68,                /* Expression  */
  YYSYMBOL_Var = 69,                       /* Var  */
  YYSYMBOL_SimpleExpression = 70,          /* SimpleExpression  */
  YYSYMBOL_Relop = 71,                     /* Relop  */
  YYSYMBOL_AdditiveExpression = 72,        /* AdditiveExpression  */
  YYSYMBOL_Addop = 73,                     /* Addop  */
  YYSYMBOL_Term = 74,                      /* Term  */
  YYSYMBOL_Multop = 75,                    /* Multop  */
  YYSYMBOL_Factor = 76,                    /* Factor  */
  YYSYMBOL_Call = 77,                      /* Call  */
  YYSYMBOL_Args = 78,                      /* Args  */
  YYSYMBOL_ArgList = 79                    /* ArgList  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
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
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  10
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   132

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  34
/* YYNRULES -- Number of rules.  */
#define YYNRULES  78
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  128

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   284


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    35,    30,     2,
      41,    42,    33,    31,    40,    32,     2,    34,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    37,
      44,    43,    45,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    38,     2,    39,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    29,     2,     2,     2,     2,     2,
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
      25,    26,    27,    28,    36
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   115,   115,   121,   126,   134,   135,   138,   151,   156,
     162,   168,   177,   178,   179,   182,   192,   197,   204,   208,
     215,   221,   230,   239,   242,   250,   253,   261,   262,   263,
     264,   265,   266,   267,   268,   271,   275,   282,   290,   302,
     310,   314,   321,   328,   333,   340,   348,   351,   356,   364,
     365,   374,   375,   376,   377,   378,   379,   382,   386,   395,
     396,   399,   400,   409,   410,   411,   412,   415,   419,   424,
     425,   426,   430,   434,   441,   450,   453,   460,   465
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "T_NUM", "T_ID",
  "T_STRING", "T_INT", "T_VOID", "T_BOOLEAN", "T_BEGIN", "T_END",
  "T_RETURN", "T_READ", "T_WRITE", "T_LE", "T_GE", "T_EQ", "T_NE", "T_AND",
  "T_OR", "T_NOT", "T_IF", "T_THEN", "T_ELSE", "T_ENDIF", "T_WHILE",
  "T_DO", "T_TRUE", "T_FALSE", "'|'", "'&'", "'+'", "'-'", "'*'", "'/'",
  "'%'", "UMINUS", "';'", "'['", "']'", "','", "'('", "')'", "'='", "'<'",
  "'>'", "$accept", "Program", "Declarationlist", "Declaration",
  "VarDeclaration", "VarList", "TypeSpecifier", "FunDeclaration", "Params",
  "ParamList", "Param", "CompoundStmt", "LocalDeclarations",
  "StatementList", "Statement", "ExpressionStmt", "SelectionStmt",
  "IterationStmt", "ReturnStmt", "ReadStmt", "WriteStmt", "AssignmentStmt",
  "Expression", "Var", "SimpleExpression", "Relop", "AdditiveExpression",
  "Addop", "Term", "Multop", "Factor", "Call", "Args", "ArgList", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-46)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-17)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      84,   -46,   -46,   -46,     3,   -46,    84,   -46,    24,   -46,
     -46,   -46,   -16,   -24,    29,    49,    93,   -46,    18,   -26,
     -46,     0,    57,    44,   -46,    40,    65,    66,    97,    84,
      49,    68,    84,   -46,   -46,   -46,   -46,    84,    49,     6,
     -46,   -46,    55,    17,   104,    35,    32,    32,    32,   -46,
     -46,   -46,    32,   -46,    99,     6,   -46,   -46,   -46,   -46,
     -46,   -46,   -46,    73,    69,    34,    38,    64,   -46,   -46,
      32,    32,   -46,    74,   -46,    75,    77,    78,    80,   -46,
      94,    92,    81,   -46,   -46,   -46,    32,   -46,   -46,   -46,
     -46,   -46,   -46,    32,   -46,   -46,    32,   -46,   -46,   -46,
     -46,    32,    82,    85,    86,   -46,   -46,   -46,   -46,   -46,
       6,     6,   -46,    50,    38,    64,   -46,   -46,    32,   -46,
      79,   -46,   -46,   -46,     6,   -46,    95,   -46
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    12,    13,    14,     0,     2,     3,     5,     0,     6,
       1,     4,     8,     0,     0,     0,     0,     7,     0,     8,
      10,    13,     0,     0,    17,    18,     9,    20,     0,     0,
       0,     0,    23,    15,    19,    11,    21,    23,     0,    25,
      24,    68,    47,     0,     0,     0,     0,     0,     0,    71,
      72,    36,     0,    28,     0,    25,    27,    29,    30,    32,
      33,    34,    31,     0,    69,    46,    49,    57,    61,    70,
       0,    75,    40,     0,    69,    47,     0,     0,     0,    73,
       0,     0,     0,    22,    26,    35,     0,    51,    54,    55,
      56,    52,    53,     0,    59,    60,     0,    65,    66,    63,
      64,     0,     0,    77,     0,    76,    41,    42,    44,    43,
       0,     0,    67,     0,    50,    58,    62,    48,     0,    74,
       0,    39,    45,    78,     0,    37,     0,    38
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -46,   -46,   114,   -46,     9,    -7,    52,   -46,   -46,    98,
     -46,    96,    89,    67,   -36,   -46,   -46,   -46,   -46,   -46,
     -46,   -46,   -41,   -39,    43,   -46,    37,   -46,    36,   -46,
     -45,   -46,   -46,    13
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     4,     5,     6,     7,    13,     8,     9,    23,    24,
      25,    53,    39,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    74,    65,    93,    66,    96,    67,   101,
      68,    69,   104,   105
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      64,    79,    73,    10,    78,    76,    80,    81,    20,    41,
      42,    82,    14,    17,    15,    32,    64,    43,    44,    45,
      41,    42,    14,    35,    15,    16,    46,    47,    12,   102,
     103,    48,    18,    49,    50,    41,    42,    46,    41,    42,
      77,    37,   -16,    51,    49,    50,    37,    52,    87,    88,
      89,    90,    46,    19,    72,    46,   116,    26,    52,    49,
      50,    27,    49,    50,    87,    88,    89,    90,    22,    94,
      95,    64,    64,    52,   120,   121,    52,   103,    91,    92,
      29,    22,    97,    98,    38,    64,    28,   122,   126,    38,
       1,     2,     3,    70,    91,    92,    71,    99,   100,     1,
      21,     3,   124,   125,    31,    30,    32,    36,    75,    83,
      85,   106,    86,    70,   107,   108,   110,   109,   111,   127,
      11,   117,    84,   112,    33,   118,    40,    34,   119,   113,
     114,   123,   115
};

static const yytype_int8 yycheck[] =
{
      39,    46,    43,     0,    45,    44,    47,    48,    15,     3,
       4,    52,    38,    37,    40,     9,    55,    11,    12,    13,
       3,     4,    38,    30,    40,    41,    20,    21,     4,    70,
      71,    25,     3,    27,    28,     3,     4,    20,     3,     4,
       5,    32,    42,    37,    27,    28,    37,    41,    14,    15,
      16,    17,    20,     4,    37,    20,   101,    39,    41,    27,
      28,     4,    27,    28,    14,    15,    16,    17,    16,    31,
      32,   110,   111,    41,   110,   111,    41,   118,    44,    45,
      40,    29,    18,    19,    32,   124,    42,    37,   124,    37,
       6,     7,     8,    38,    44,    45,    41,    33,    34,     6,
       7,     8,    23,    24,    38,    40,     9,    39,     4,    10,
      37,    37,    43,    38,    37,    37,    22,    37,    26,    24,
       6,    39,    55,    42,    28,    40,    37,    29,    42,    86,
      93,   118,    96
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     6,     7,     8,    47,    48,    49,    50,    52,    53,
       0,    48,     4,    51,    38,    40,    41,    37,     3,     4,
      51,     7,    52,    54,    55,    56,    39,     4,    42,    40,
      40,    38,     9,    57,    55,    51,    39,    50,    52,    58,
      58,     3,     4,    11,    12,    13,    20,    21,    25,    27,
      28,    37,    41,    57,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    72,    74,    76,    77,
      38,    41,    37,    68,    69,     4,    69,     5,    68,    76,
      68,    68,    68,    10,    59,    37,    43,    14,    15,    16,
      17,    44,    45,    71,    31,    32,    73,    18,    19,    33,
      34,    75,    68,    68,    78,    79,    37,    37,    37,    37,
      22,    26,    42,    70,    72,    74,    76,    39,    40,    42,
      60,    60,    37,    79,    23,    24,    60,    24
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    46,    47,    48,    48,    49,    49,    50,    51,    51,
      51,    51,    52,    52,    52,    53,    54,    54,    55,    55,
      56,    56,    57,    58,    58,    59,    59,    60,    60,    60,
      60,    60,    60,    60,    60,    61,    61,    62,    62,    63,
      64,    64,    65,    66,    66,    67,    68,    69,    69,    70,
      70,    71,    71,    71,    71,    71,    71,    72,    72,    73,
      73,    74,    74,    75,    75,    75,    75,    76,    76,    76,
      76,    76,    76,    76,    77,    78,    78,    79,    79
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     1,     3,     1,     4,
       3,     6,     1,     1,     1,     6,     1,     1,     1,     3,
       2,     4,     4,     0,     2,     0,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     1,     5,     7,     4,
       2,     3,     3,     3,     3,     4,     1,     1,     4,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     3,     1,
       1,     1,     3,     1,     1,     1,     1,     3,     1,     1,
       1,     1,     1,     2,     4,     0,     1,     1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* Program: Declarationlist  */
#line 116 "lab6.y"
                    {
						program = (yyvsp[0].node);
					}
#line 1418 "y.tab.c"
    break;

  case 3: /* Declarationlist: Declaration  */
#line 122 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_DEC_LIST); //Set the companion value to be a node with type A_DEC_LIST
						(yyval.node)->s1 = (yyvsp[0].node); //Set s1 of the node to be the Declaration value
					}
#line 1427 "y.tab.c"
    break;

  case 4: /* Declarationlist: Declaration Declarationlist  */
#line 127 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_DEC_LIST); //Set the companion value to be a node with type A_DEC_LIST;
						(yyval.node)->s1 = (yyvsp[-1].node); //Set the s1 branch to be a Declaration
						(yyval.node)->s2 = (yyvsp[0].node); //Set the s2 branch to be a Declarationlist
					}
#line 1437 "y.tab.c"
    break;

  case 5: /* Declaration: VarDeclaration  */
#line 134 "lab6.y"
                                         {(yyval.node) = (yyvsp[0].node);}
#line 1443 "y.tab.c"
    break;

  case 6: /* Declaration: FunDeclaration  */
#line 135 "lab6.y"
                                                 {(yyval.node) = (yyvsp[0].node);}
#line 1449 "y.tab.c"
    break;

  case 7: /* VarDeclaration: TypeSpecifier VarList ';'  */
#line 139 "lab6.y"
                                        {
						(yyval.node) = (yyvsp[-1].node);
						ASTnode *p;
						p = (yyvsp[-1].node);
						while(p != NULL){
							p->datatype = (yyvsp[-2].datatype);
							p = p->s1;
							}
					}
#line 1463 "y.tab.c"
    break;

  case 8: /* VarList: T_ID  */
#line 152 "lab6.y"
                                                {
						(yyval.node) = ASTCreateNode(A_VARDEC); //Create a new A_VARDEC node
						(yyval.node)->name = (yyvsp[0].string); //Set the name with the given ID
						}
#line 1472 "y.tab.c"
    break;

  case 9: /* VarList: T_ID '[' T_NUM ']'  */
#line 157 "lab6.y"
                                                {
						(yyval.node) = ASTCreateNode(A_VARDEC); //Create a new A_VARDEC node
						(yyval.node)->name = (yyvsp[-3].string); //Set the name to be the ID
						(yyval.node)->value = (yyvsp[-1].num); //Set the value with the given NUM
						}
#line 1482 "y.tab.c"
    break;

  case 10: /* VarList: T_ID ',' VarList  */
#line 163 "lab6.y"
                                                {
						(yyval.node) = ASTCreateNode(A_VARDEC); //Create a new A_VARDEC node
						(yyval.node)->name = (yyvsp[-2].string); //Set the name with ID
						(yyval.node)->s1 = (yyvsp[0].node); //Set the s1 branch to be another Varlist
						}
#line 1492 "y.tab.c"
    break;

  case 11: /* VarList: T_ID '[' T_NUM ']' ',' VarList  */
#line 169 "lab6.y"
                                                {
						(yyval.node) = ASTCreateNode(A_VARDEC); //Create a new A_VARDEC node
						(yyval.node)->name = (yyvsp[-5].string); //Set the name with the ID
						(yyval.node)->value = (yyvsp[-3].num); //Set the value with the NUM
						(yyval.node)->s1 = (yyvsp[0].node); //Set the s1 branch to be another Varlist.
						}
#line 1503 "y.tab.c"
    break;

  case 12: /* TypeSpecifier: T_INT  */
#line 177 "lab6.y"
                                {(yyval.datatype) = A_INTTYPE;}
#line 1509 "y.tab.c"
    break;

  case 13: /* TypeSpecifier: T_VOID  */
#line 178 "lab6.y"
                                                 {(yyval.datatype) = A_VOIDTYPE;}
#line 1515 "y.tab.c"
    break;

  case 14: /* TypeSpecifier: T_BOOLEAN  */
#line 179 "lab6.y"
                                                    {(yyval.datatype) = A_BOOLEANTYPE;}
#line 1521 "y.tab.c"
    break;

  case 15: /* FunDeclaration: TypeSpecifier T_ID '(' Params ')' CompoundStmt  */
#line 183 "lab6.y"
                                                {
						(yyval.node) = ASTCreateNode(A_FUNDEC);
						(yyval.node)->name = (yyvsp[-4].string); //Setting the name with the ID
						(yyval.node)->datatype = (yyvsp[-5].datatype); //Setting the datatype with the given TypeSpecifier
						(yyval.node)->s1 = (yyvsp[-2].node); //FIX ME Setting the s1 branch to be the Params
						(yyval.node)->s2 = (yyvsp[0].node); //Setting s2 branch to be the Compound statement
						}
#line 1533 "y.tab.c"
    break;

  case 16: /* Params: T_VOID  */
#line 193 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_PARAMS);
						
					}
#line 1542 "y.tab.c"
    break;

  case 17: /* Params: ParamList  */
#line 198 "lab6.y"
                                        {
						(yyval.node) = (yyvsp[0].node);
					}
#line 1550 "y.tab.c"
    break;

  case 18: /* ParamList: Param  */
#line 205 "lab6.y"
                                        {
						(yyval.node) = (yyvsp[0].node);
					}
#line 1558 "y.tab.c"
    break;

  case 19: /* ParamList: Param ',' ParamList  */
#line 209 "lab6.y"
                                        {
						(yyval.node) = (yyvsp[-2].node);
						(yyval.node)->s1 = (yyvsp[0].node);
					}
#line 1567 "y.tab.c"
    break;

  case 20: /* Param: TypeSpecifier T_ID  */
#line 216 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_PARAM);
						(yyval.node)->datatype = (yyvsp[-1].datatype);
						(yyval.node)->name = (yyvsp[0].string);
					}
#line 1577 "y.tab.c"
    break;

  case 21: /* Param: TypeSpecifier T_ID '[' ']'  */
#line 222 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_PARAM);
						(yyval.node)->datatype = (yyvsp[-3].datatype);
						(yyval.node)->name = (yyvsp[-2].string);
						(yyval.node)->value = -1;
					}
#line 1588 "y.tab.c"
    break;

  case 22: /* CompoundStmt: T_BEGIN LocalDeclarations StatementList T_END  */
#line 231 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_COMPOUND);
						(yyval.node)->s1 = (yyvsp[-2].node);
						(yyval.node)->s2 = (yyvsp[-1].node);
					}
#line 1598 "y.tab.c"
    break;

  case 23: /* LocalDeclarations: %empty  */
#line 239 "lab6.y"
                                        {
						(yyval.node) = NULL;
					}
#line 1606 "y.tab.c"
    break;

  case 24: /* LocalDeclarations: VarDeclaration LocalDeclarations  */
#line 243 "lab6.y"
                                        {
						(yyval.node) = (yyvsp[-1].node);
						(yyval.node)->s2 = (yyvsp[0].node);
					}
#line 1615 "y.tab.c"
    break;

  case 25: /* StatementList: %empty  */
#line 250 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_STATEMENTLIST);
					}
#line 1623 "y.tab.c"
    break;

  case 26: /* StatementList: Statement StatementList  */
#line 254 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_STATEMENTLIST);
						(yyval.node)->s1 = (yyvsp[-1].node);
						(yyval.node)->s2 = (yyvsp[0].node);
					}
#line 1633 "y.tab.c"
    break;

  case 27: /* Statement: ExpressionStmt  */
#line 261 "lab6.y"
                                                {(yyval.node) = (yyvsp[0].node);}
#line 1639 "y.tab.c"
    break;

  case 28: /* Statement: CompoundStmt  */
#line 262 "lab6.y"
                                                      {(yyval.node) = (yyvsp[0].node);}
#line 1645 "y.tab.c"
    break;

  case 29: /* Statement: SelectionStmt  */
#line 263 "lab6.y"
                                                       {(yyval.node) = (yyvsp[0].node);}
#line 1651 "y.tab.c"
    break;

  case 30: /* Statement: IterationStmt  */
#line 264 "lab6.y"
                                                       {(yyval.node) = (yyvsp[0].node);}
#line 1657 "y.tab.c"
    break;

  case 31: /* Statement: AssignmentStmt  */
#line 265 "lab6.y"
                                                        {(yyval.node) = (yyvsp[0].node);}
#line 1663 "y.tab.c"
    break;

  case 32: /* Statement: ReturnStmt  */
#line 266 "lab6.y"
                                                    {(yyval.node) = (yyvsp[0].node);}
#line 1669 "y.tab.c"
    break;

  case 33: /* Statement: ReadStmt  */
#line 267 "lab6.y"
                                                  {(yyval.node) = (yyvsp[0].node);}
#line 1675 "y.tab.c"
    break;

  case 34: /* Statement: WriteStmt  */
#line 268 "lab6.y"
                                                   {(yyval.node) = (yyvsp[0].node);}
#line 1681 "y.tab.c"
    break;

  case 35: /* ExpressionStmt: Expression ';'  */
#line 272 "lab6.y"
                                        {
						(yyval.node) = (yyvsp[-1].node);
					}
#line 1689 "y.tab.c"
    break;

  case 36: /* ExpressionStmt: ';'  */
#line 276 "lab6.y"
                                        {
						(yyval.node) = NULL;
					}
#line 1697 "y.tab.c"
    break;

  case 37: /* SelectionStmt: T_IF Expression T_THEN Statement T_ENDIF  */
#line 283 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_IF);
						(yyval.node)->s1 = (yyvsp[-3].node); //Set s1 branch of Selection to be the Expression
						ASTnode *p = ASTCreateNode(A_IF); //Create a new node A_IF that will hold the if statements
						p->s1 = (yyvsp[-1].node); //Set the A_IF s1 branch with the statement, other will be NULL since it's not an else
						(yyval.node)->s2=p; //Set Selection statement's s2 to be the A_IF we created.
					}
#line 1709 "y.tab.c"
    break;

  case 38: /* SelectionStmt: T_IF Expression T_THEN Statement T_ELSE Statement T_ENDIF  */
#line 291 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_IF);
						(yyval.node)->s1 = (yyvsp[-5].node); //Set s1 branch of Selection to be the Expression
						ASTnode *p = ASTCreateNode(A_IF); //Create a new node A_IF that will hold the if statements
						p->s1 = (yyvsp[-3].node); //Set the A_IF s1 branch with the then statement
						p->s2 = (yyvsp[-1].node); //Set the A_IF s2 branch with the else statment
						(yyval.node)->s2=p; //Set Selection statement's s2 to be the A_IF we created.

					}
#line 1723 "y.tab.c"
    break;

  case 39: /* IterationStmt: T_WHILE Expression T_DO Statement  */
#line 303 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_ITERATIONSTMT);
						(yyval.node)->s1 = (yyvsp[-2].node);
						(yyval.node)->s2 = (yyvsp[0].node);
					}
#line 1733 "y.tab.c"
    break;

  case 40: /* ReturnStmt: T_RETURN ';'  */
#line 311 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_RETURNSTMT);
					}
#line 1741 "y.tab.c"
    break;

  case 41: /* ReturnStmt: T_RETURN Expression ';'  */
#line 315 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_RETURNSTMT);
						(yyval.node)->s1 = (yyvsp[-1].node);
					}
#line 1750 "y.tab.c"
    break;

  case 42: /* ReadStmt: T_READ Var ';'  */
#line 322 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_READ);
						(yyval.node)->s1 = (yyvsp[-1].node);
					}
#line 1759 "y.tab.c"
    break;

  case 43: /* WriteStmt: T_WRITE Expression ';'  */
#line 329 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_WRITE);
						(yyval.node)->s1 = (yyvsp[-1].node);
					}
#line 1768 "y.tab.c"
    break;

  case 44: /* WriteStmt: T_WRITE T_STRING ';'  */
#line 334 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_WRITE);
						(yyval.node)->name = (yyvsp[-1].string);
					}
#line 1777 "y.tab.c"
    break;

  case 45: /* AssignmentStmt: Var '=' SimpleExpression ';'  */
#line 341 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_ASSIGNMENTSTMT);
						(yyval.node)->s1 = (yyvsp[-3].node);
						(yyval.node)->s2 = (yyvsp[-1].node);
					}
#line 1787 "y.tab.c"
    break;

  case 46: /* Expression: SimpleExpression  */
#line 348 "lab6.y"
                                                   {(yyval.node) = (yyvsp[0].node);}
#line 1793 "y.tab.c"
    break;

  case 47: /* Var: T_ID  */
#line 352 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_VAR);
						(yyval.node)->name = (yyvsp[0].string);
					}
#line 1802 "y.tab.c"
    break;

  case 48: /* Var: T_ID '[' Expression ']'  */
#line 357 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_VAR);
						(yyval.node)->name = (yyvsp[-3].string);
						(yyval.node)->s1 = (yyvsp[-1].node);
					}
#line 1812 "y.tab.c"
    break;

  case 49: /* SimpleExpression: AdditiveExpression  */
#line 364 "lab6.y"
                                             {(yyval.node) = (yyvsp[0].node);}
#line 1818 "y.tab.c"
    break;

  case 50: /* SimpleExpression: SimpleExpression Relop AdditiveExpression  */
#line 366 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_EXPR);
						(yyval.node)->s1 = (yyvsp[-2].node);
						(yyval.node)->s2 = (yyvsp[0].node);
						(yyval.node)->operator = (yyvsp[-1].operator);
					}
#line 1829 "y.tab.c"
    break;

  case 51: /* Relop: T_LE  */
#line 374 "lab6.y"
                                       {(yyval.operator) = A_LE;}
#line 1835 "y.tab.c"
    break;

  case 52: /* Relop: '<'  */
#line 375 "lab6.y"
                                              {(yyval.operator) = A_LESSTHAN;}
#line 1841 "y.tab.c"
    break;

  case 53: /* Relop: '>'  */
#line 376 "lab6.y"
                                              {(yyval.operator) = A_GREATERTHAN;}
#line 1847 "y.tab.c"
    break;

  case 54: /* Relop: T_GE  */
#line 377 "lab6.y"
                                               {(yyval.operator) = A_GE;}
#line 1853 "y.tab.c"
    break;

  case 55: /* Relop: T_EQ  */
#line 378 "lab6.y"
                                               {(yyval.operator) = A_EQ;}
#line 1859 "y.tab.c"
    break;

  case 56: /* Relop: T_NE  */
#line 379 "lab6.y"
                                               {(yyval.operator) = A_NE;}
#line 1865 "y.tab.c"
    break;

  case 57: /* AdditiveExpression: Term  */
#line 383 "lab6.y"
                                        {
						(yyval.node) = (yyvsp[0].node);
					}
#line 1873 "y.tab.c"
    break;

  case 58: /* AdditiveExpression: AdditiveExpression Addop Term  */
#line 387 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_EXPR);
						(yyval.node)->s1 = (yyvsp[-2].node);
						(yyval.node)->s2 = (yyvsp[0].node);
						(yyval.node)->operator = (yyvsp[-1].operator);
					}
#line 1884 "y.tab.c"
    break;

  case 59: /* Addop: '+'  */
#line 395 "lab6.y"
                                      {(yyval.operator) = A_PLUS;}
#line 1890 "y.tab.c"
    break;

  case 60: /* Addop: '-'  */
#line 396 "lab6.y"
                                              {(yyval.operator) = A_MINUS;}
#line 1896 "y.tab.c"
    break;

  case 61: /* Term: Factor  */
#line 399 "lab6.y"
                                         {(yyval.node) = (yyvsp[0].node);}
#line 1902 "y.tab.c"
    break;

  case 62: /* Term: Term Multop Factor  */
#line 401 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_EXPR);
						(yyval.node)->s1 = (yyvsp[-2].node);
						(yyval.node)->s2 = (yyvsp[0].node);
						(yyval.node)->operator = (yyvsp[-1].operator);
					}
#line 1913 "y.tab.c"
    break;

  case 63: /* Multop: '*'  */
#line 409 "lab6.y"
                                      {(yyval.operator) = A_TIMES;}
#line 1919 "y.tab.c"
    break;

  case 64: /* Multop: '/'  */
#line 410 "lab6.y"
                                              {(yyval.operator) = A_DIVIDES;}
#line 1925 "y.tab.c"
    break;

  case 65: /* Multop: T_AND  */
#line 411 "lab6.y"
                                                {(yyval.operator) = A_AND;}
#line 1931 "y.tab.c"
    break;

  case 66: /* Multop: T_OR  */
#line 412 "lab6.y"
                                               {(yyval.operator) = A_OR;}
#line 1937 "y.tab.c"
    break;

  case 67: /* Factor: '(' Expression ')'  */
#line 416 "lab6.y"
                                        {
						(yyval.node) = (yyvsp[-1].node);
					}
#line 1945 "y.tab.c"
    break;

  case 68: /* Factor: T_NUM  */
#line 420 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_NUM);
						(yyval.node)->value = (yyvsp[0].num);
					}
#line 1954 "y.tab.c"
    break;

  case 69: /* Factor: Var  */
#line 424 "lab6.y"
                                              {(yyval.node) = (yyvsp[0].node);}
#line 1960 "y.tab.c"
    break;

  case 70: /* Factor: Call  */
#line 425 "lab6.y"
                                               {(yyval.node) = (yyvsp[0].node);}
#line 1966 "y.tab.c"
    break;

  case 71: /* Factor: T_TRUE  */
#line 427 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_TRUE);
					}
#line 1974 "y.tab.c"
    break;

  case 72: /* Factor: T_FALSE  */
#line 431 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_FALSE); 
					}
#line 1982 "y.tab.c"
    break;

  case 73: /* Factor: T_NOT Factor  */
#line 435 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_NOT);
						(yyval.node)->s1 = (yyvsp[0].node);
					}
#line 1991 "y.tab.c"
    break;

  case 74: /* Call: T_ID '(' Args ')'  */
#line 442 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_CALL);
						(yyval.node)->s1 = (yyvsp[-1].node);
						(yyval.node)->name = (yyvsp[-3].string);
					}
#line 2001 "y.tab.c"
    break;

  case 75: /* Args: %empty  */
#line 450 "lab6.y"
                                        {
						(yyval.node) = NULL;
					}
#line 2009 "y.tab.c"
    break;

  case 76: /* Args: ArgList  */
#line 454 "lab6.y"
                                        {
						(yyval.node) = (yyvsp[0].node);
					}
#line 2017 "y.tab.c"
    break;

  case 77: /* ArgList: Expression  */
#line 461 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_ARGLIST);
						(yyval.node)->s1 = (yyvsp[0].node);
					}
#line 2026 "y.tab.c"
    break;

  case 78: /* ArgList: Expression ',' ArgList  */
#line 466 "lab6.y"
                                        {
						(yyval.node) = ASTCreateNode(A_ARGLIST);
						(yyval.node)->s1 = (yyvsp[-2].node);
						(yyval.node)->s2 = (yyvsp[0].node);
					}
#line 2036 "y.tab.c"
    break;


#line 2040 "y.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 474 "lab6.y"
	/* end of rules, start of program */

/*
Preconditions: Parser has been properly initialized and configured with proper tokens.
Postconditions: yyparse is called and main exits.
*/
int main(){ 
	yyparse();
	ASTprint(0, program);	
} //End of main
