/*   Abstract syntax tree code


 Header file   
 Shaun Cooper January 2022

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
   A_WRITE,
   A_READ, //Added A_READ
   A_VAR, //Added A_VAR
   A_FUNCTIONDEC,
   A_IDENT,
   A_NUM,
   A_EXPR,
   A_SELECTIONSTMT,
   A_ASSIGNMENTSTMT,
   A_ITERATIONSTMT, //Added A_ITERATIONSTMT
   A_IF
 
	   //missing
};

enum DataTypes {
   A_INTTYPE,
   A_VOIDTYPE,
   A_BOOLEANTYPE
};

enum OPERATORS {
   A_PLUS,
   A_MINUS,
   A_TIMES,
   A_LE,
   A_LESSTHAN,
   A_GREATERTHAN,
   A_GE,
   A_EQ,
   A_NE
 
	   //missing
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
     ///.. missing
     struct ASTnodetype *s1,*s2 ; /* used for holding IF and WHILE components -- not very descriptive */
} ASTnode;


/* uses malloc to create an ASTnode and passes back the heap address of the newley created node */
ASTnode *ASTCreateNode(enum ASTtype mytype);

void PT(int howmany);
//moving ASTnode *program; to c file

/*  Print out the abstract syntax tree */
void ASTprint(int level,ASTnode *p);

#endif // of AST_H
