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
        -Added check_params() declaration.
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

