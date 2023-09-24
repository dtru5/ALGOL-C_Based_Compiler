/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

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
#line 48 "lab5.y"
 int num;
	 char * string; 

#line 129 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
