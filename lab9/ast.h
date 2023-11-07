/*   Abstract syntax tree code


 Header file   
 Shaun Cooper January 2022

*/

/*
    Name: Dominik Trujillo
    Date: 10/29/2023
    Lab: Lab: LAB 7 ALGOL -- add symbol table and type checking
    Purpose: The objective of this lab is to enhance a compiler's functionality by incorporating a symbol table, semantic actions, 
	and rudimentary type checking. The key goals and requirements of this lab include:

	Symbol Table Management: To facilitate proper scope management and variable tracking, the compiler will maintain a static 
	scope counter referred to as "level." When entering a compound statement, the level is incremented, and upon exiting, it is
	decremented. During the exit, all symbols defined at that level are removed from the symbol table. Offsets are adjusted 
	accordingly to account for the memory allocated to the removed symbols. New variables must be inserted into the symbol table 
	when declared, including information about their level and size. The offset for each variable is assigned based on its type, 
	with scalars assigned 1, arrays their respective sizes, and functions using registers for return values instead of memory.

	Type Checking: A fundamental type checking mechanism is implemented to prevent illegal operations, such as combining INT and 
	VOID types in expressions. Variables must be used in the correct context, distinguishing between scalars, arrays, and functions. 
	Type compatibility is enforced for assignment statements and expressions to ensure the integrity of the generated code.

	Symbol Table Requirements: The YACC file is modified to incorporate the symbol table, enabling the compiler to manage variable
	declarations and usages efficiently. Upon the completion of a compound statement, all variables defined within that context are 
	displayed, and subsequently, they are removed from the symbol table. The verification process ensures that variable names are both
	defined and used correctly. Type inheritance is implemented for expressions. Furthermore, the symbol table maintains the level for
	each entry, with level 0 reserved for global variables and functions, level 1 for function parameters and the primary compound 
	statement of the function. The level is incremented as nested compound statements are encountered and decremented when exiting these nested scopes.

	In summary, this lab's primary objective is to equip the compiler with the capability to manage
	symbol tables effectively, perform rudimentary type checking, and handle scope management. These enhancements aim to 
	improve the compiler's code generation process, making it more robust and reliable.

   Changes made:
      -Added A_UNKNOWN to datatypes to handle VarList when the type is not immediately set in YACC.
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
