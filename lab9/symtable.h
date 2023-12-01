/*  Symbol Table --linked list  headers
    Used for Compilers class

    Modified Spring 2015 to allow for name to pointed to by symtable, instead of copied, since the name is copied
    into the heap

    Modified to have levels.  A level 0 means global variable, other levels means in range of the function.  We
    start out our offsets at 0 (from stack pointer) for level 1,,,when we enter a functional declaration.
    We increment offset each time we insert a new variable.  A variable is considered to be valid if it is found in
    the symbol table at our level or lesser level.  If at 0, then it is global.  

    We return a pointer to the symbol table when a variable matches our creteria.

    We add a routine to remove variables at our level and above.
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


#ifndef _SYMTAB 
#define _SYMTAB

#include "ast.h"

enum  SYMBOL_SUBTYPE
{
   SYM_SCALAR,
   SYM_FUNCTION,
   SYM_FUNCTION_PROTO,
   SYM_FUNCTION_PRE,
   SYM_ARRAY
};

void Display();
int Delete();


struct SymbTab
{
     char *name;
     int offset; /* from activation record boundary */
     int mysize;  /* number of words this item is 1 or more */
     int level;  /* the level where we found the variable */
     enum DataTypes Declared_Type;  /* the type of the symbol */
     enum SYMBOL_SUBTYPE SubType;  /* the subtype of the symbol */
     ASTnode * fparms; /* pointer to parameters of the function in the AST */

     struct SymbTab *next;
};


struct SymbTab * Insert(char *name, enum DataTypes my_assigned_type , enum  SYMBOL_SUBTYPE sub_type, int  level, int mysize, int offset);

struct SymbTab * Search(char name[], int level, int recur );

int check_params(ASTnode *formal, ASTnode *actual);

static struct SymbTab *first=NULL;   /* global pointers into the symbol table */

char * CreateTemp();


int Has_Proto();

#endif

