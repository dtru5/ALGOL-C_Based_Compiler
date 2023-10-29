/*   Abstract syntax tree code


 Header file   
 Shaun Cooper January 2022

*/

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
