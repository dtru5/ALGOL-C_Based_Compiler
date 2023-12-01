/*   Abstract syntax tree code


 Header file   
 Shaun Cooper January 2022

*/

/*
Name: Dominik Trujillo
Date: 11/4/2023
LAB9 ALGOL Create MIPS code from you AST
Purpose: The purpose of this lab is to implement a MIPS code generator for ALGOL structures using an Abstract Syntax Tree (AST).
The primary objectives include creating MIPS code for ALGOL constructs with jumps, labels, and MIPS directives, 
incrementally developing MIPS code for the ALGOL language, and gaining an understanding of activation records. 
The lab emphasizes updating the main program to accept command-line arguments, copying the AST printing routine to 
create an "emit" section for generating MIPS code, and implementing functions to handle standard header elements for 
MIPS to define arrays, scalars, and strings. Additional goals involve creating simplified code for basic functionalities
such as printing "hello world," handling integers, and managing variable input/output. The lab also focuses on updating 
helper functions for evaluating expressions, making assignments work correctly, enhancing the system to handle If/while 
constructs, and extending support for arrays The complexity increases gradually, allowing students
to tackle one functional unit at a time. The lab encourages documentation for each function and major construct in the 
emit.c file to facilitate understanding and evaluation. Overall, the lab aims to provide students with hands-on experience in 
MIPS code generation while reinforcing their understanding of ALGOL language construct; and ultimately, this lab aims to 
demonstrate our complete works from LEX, YACC, using ASTs, and emiting the corresponding MIPS code to have a relatively completed 
compiler.
*/

#include<stdio.h>
#include<malloc.h>

#ifndef AST_H
#define AST_H
extern int mydebug;

/* define the enumerated types for the AST.  THis is used to tell us what 
sort of production rule we came across */

enum ASTtype {
   A_VARDEC,
   A_FUNDEC, //Added A_FUNDEC
   A_DEC_LIST, //Added A_DEC_LIST
   A_COMPOUND, //Added A_COMPOUND
   A_STATEMENTLIST, //Added A_STATEMENTLIST
   A_WRITE, //Added A_WRITE
   A_READ, //Added A_READ
   A_VAR, //Added A_VAR
   A_FUNCTIONDEC, //Added A_FUNCTIONDEC
   A_IDENT, //Didn't need to use 
   A_NUM, //Added A_NUM
   A_EXPR, //Added A_EXPR
   A_SELECTIONSTMT, //Added A_SELECTIONSTMT
   A_ASSIGNMENTSTMT, //Added A_ASSIGNMENTSTMT
   A_ITERATIONSTMT, //Added A_ITERATIONSTMT
   A_IF,//Added A_IF
   A_CALL, //Added A_CALL
   A_ARGLIST, //Added A_ARGLIST
   A_PARAMS, //Added A_PARAMS
   A_TRUE, //Added A_TRUE
   A_FALSE, //Added A_FALSE
   A_RETURNSTMT, //Added A_RETURNSTMT
   A_PARAM, //Added A_PARAM
   A_EXPRESSIONSTMT //Added A_EXPRESSIONSTMT
 
};

enum DataTypes {
   A_UNKNOWN, //Added A_UNKNOWN to help us with labeling types that aren't yet defined.
   A_INTTYPE,
   A_VOIDTYPE,
   A_BOOLEANTYPE
};

enum OPERATORS {
   //Added operators for expressions
   A_PLUS,
   A_MINUS,
   A_TIMES,
   A_LE,
   A_LESSTHAN,
   A_GREATERTHAN,
   A_GE,
   A_EQ,
   A_NE,
   A_DIVIDES,
   A_AND,
   A_OR,
   A_NOT
 
};

/* define a type AST node which will hold pointers to AST structs that will
   allow us to represent the parsed code 
*/
typedef struct ASTnodetype
{
     enum ASTtype nodetype;
     enum OPERATORS operator;
     enum DataTypes datatype;
     char * name;
     char * label;
     int value;
     struct ASTnodetype *s1,*s2 ; /* used for holding IF and WHILE components -- not very descriptive */
     struct SymbTab *symbol;
} ASTnode;


/* uses malloc to create an ASTnode and passes back the heap address of the newley created node */
ASTnode *ASTCreateNode(enum ASTtype mytype);

void PT(int howmany);
//moving ASTnode *program; to c file

/*  Print out the abstract syntax tree */
void ASTprint(int level,ASTnode *p);

#endif // of AST_H
