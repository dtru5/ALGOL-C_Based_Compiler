/*  Symbol Table --linked list
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
      -Added functionality to check_params function.
*/

    
#include <string.h>
#include "ast.h"
#include "symtable.h"

int GTEMP=0;  /* Global Temp counter */

// PRE:   Assume one up global variable GTEMP
// POST:  Returns string with the format _t%d and increments the global vairbal
// USAGE:   creates a variable name that is used to hold temporary, intermediate
//         values in the runtime enviroment

char * CreateTemp()
{    char hold[100];
     char *s;
     sprintf(hold,"_t%d",GTEMP++);
     s=strdup(hold);
     return (s);
}




/* Simple Insert into the symbol table with the size, type level that the name is being inserted into */

// PRE:  given elements for an item in the symbol table
//  POST:  Inserts an item in the symbol table list provided
//         the symbol does not exists at the level

struct SymbTab * Insert(char *name, enum DataTypes my_assigned_type, enum  SYMBOL_SUBTYPE subtype, int  level, int mysize, int offset )
{
  struct SymbTab * n;
    n=Search(name,level, 0);
    if(n!=NULL)
      {
      printf("\n\tThe name %s exists at level %d already in the symbol table\n\tDuplicate can.t be inserted",name, level);
      return (NULL);
      }
    else
    {
      struct SymbTab *p;
      p=malloc(sizeof(struct SymbTab));
      p->name=name;
      p->offset=offset;  /* assign the offset */
      p->level=level;  /* assign the level */
      p->mysize=mysize;  /* assign the size */
      p->Declared_Type=my_assigned_type;  /* assign the Type */
      p->SubType=subtype;  /* assign the Function  */
      p->next=NULL;

   //  Insert the record in the list ... 
      if(first==NULL)
      {
        first=p;
      }
      else
      {
        p->next=first;
        first=p;
      }
      return (p);
 
    }
     
  printf("\n\tLabel inserted\n");
}

/* print out a single symbol table entry -- for debugging */
//  PRE:  Ptr to a symtabl structore
//  POST:  output to the screen in human readable form
void PrintSym(struct SymbTab *s)
{
         printf("\t%s\t\t%d\t\t%d\t%d\n",s->name,s->offset, s->level,s->Declared_Type);

}


/*  General display to see what is our symbol table */

//  PRE:  depends on global variable first
//  POST:  Formatted output of the symbol table

void Display()
{
   int i;
   struct SymbTab *p;
   p=first;
   printf("\n----------------------------------------------------");
   printf("\n\t\t DISPLAY SYM TABLE\n");
   printf("\n\tLABEL\t\tOffset \t\tLEVEL \ttype\n");
      while (p!=NULL)
      {
         PrintSym(p);
         p=p->next;
      }
  printf("----------------------------------------------------\n\n\n");
}

/*  Search for a symbol name at level or below.  We have to do multiple passes into the symbol table because we have to find
   the name closest to us 


  If recur is non-zero, then we look through all of the levels, otherwise, only our level 
   We return a pointer to a SymbolTab structure so that we can use other functions/methods to get the attributes */

 


// PRE:   given a name and level and recure
//  POST:  returns NULL if not there, otherwise a PTR to the element in the table
// DETAIL:  search will stop at first level if recur set to 0
//          search will continue struct until level is 0 if recur is 1 
struct SymbTab * Search(char name[], int level, int recur)
{
   int i,flag=0;
   struct SymbTab *p;

  /* for each level, try to find our symbol */
   while (level >= 0)
    {
       p=first;
       while (p!=NULL)
        {
         if((strcmp(p->name,name)==0) && (p->level == level))
           return p;
         p=p->next;
        }
       if (recur == 0) return (NULL);   /* we did not find it at our level */
       level--; /* check the next level up */
    }


   return  NULL;  /* did not find it, return 0 */
}

/* Remove all enteries that have the indicated level
   We need to take care about updating first pointer into the linked list when we are deleting edge elements */

// PRE:  level
// POST:  removes all symbol table entrues with level equal to or higher

int Delete(int level)
{
    struct SymbTab *p,*f=NULL;  /* we follow with pointer f */
    int SIZE=0;
    p=first;

    
    
  /* cruise through the list */

    while (p != NULL)
      {
        /* do we match? */
        if (p->level>= level )
        { /* if it is the first in the list we have to update first, we know this by f being NULL */
           SIZE+=p->mysize;
           if ( f==NULL) first=p->next;
           else /* not the first element */
              {f->next=p->next;
              }
            p=p->next;
           
        }
        else
         {
               /* update follow pointer, move the p pointer */
                f=p;
                p=p->next;
          }

      }
    return(SIZE);
}

  // PRE:  No input
  // POST:  boolen if table has a PROTOTYPE in it
  int Has_Proto()
  {
    struct SymbTab *p = first;
    while (p != NULL)
      {
          if (p->SubType == SYM_FUNCTION_PROTO) return 1;
          p = p->next;
      }
    return 0;
}

  /*
  PRE: Two lists that represent formals and actuals
  POST: if they are the same length, each element is type consistent and 0 otherwise.
  */
int check_params(ASTnode *formal, ASTnode *actual) {

    //If both nodes have reached the end of their list at the same time, then return 1.
    if(formal == NULL && actual == NULL){
      return 1;
    }

    //If one or the other node is NULL while one is not NULL, then return 0 since not the same size.
    if(formal == NULL || actual == NULL){
      return 0;
    }

    //If the datatypes don't match, then return 0.
    if((formal->symbol->Declared_Type != actual->s1->datatype)){
      return 0;
    }

    // Recursively check the rest of the parameters
    return check_params(formal->s1, actual->s2);
}




