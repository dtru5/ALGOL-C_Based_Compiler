/*   Abstract syntax tree code

     This code is used to define an AST node, 
    routine for printing out the AST
    defining an enumerated nodetype so we can figure out what we need to
    do with this.  The ENUM is basically going to be every non-terminal
    and terminal in our language.

    Shaun Cooper February 2023

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
#include<stdlib.h>  
#include<malloc.h>
#include "ast.h" 
#include "symtable.h"

//Originally, ASTnode *program was in ast.h, but moved to ast.c due to MINGW compilation errors
//and was instructed that this difference wasn't significant.
ASTnode *program; // pointer to the tree

/* uses malloc to create an ASTnode and passes back the heap address of the newley created node */
// PRE: Given a valid ASTtype variable.
// POST: Return a new node.
ASTnode *ASTCreateNode(enum ASTtype mytype)
{
    ASTnode *p;
    if (mydebug) fprintf(stderr,"Creating AST Node \n");
    p=(ASTnode *)malloc(sizeof(ASTnode));
    p->nodetype=mytype;
    p->s1=NULL;
    p->s2=NULL;
    p->value=0;
    p->name = NULL;
    p->symbol = NULL;
    return(p);
}

/*  Helper function to print tabbing */

// PRE: Given a positive integer of howmany.
//  POST: Prints out that many of spaces.
void PT(int howmany)
{
	 for(int i = 0; i < howmany; i++){ //Added a for loop that will print out the spaces to 0 to the given howmany
        printf(" ");
     }
}

// PRE:  a Data Type
// POST:  a character string for that type to print nicely -- caller does final output

char * DataTypeToString(enum DataTypes mydatatype){
    switch (mydatatype) {
            //Adding case for A_INTTYPE
            case A_INTTYPE: return ("int");
                            break;
            case A_BOOLEANTYPE: return ("boolean");
                            break;
           case A_VOIDTYPE:  return ("void");
                             break;
           default: printf("Unknown type in DataTypeToString\n");
                     exit(1);
      } //of switch
}// of DataTypeToString()


/*  Print out the abstract syntax tree 
Pre: Given valid inputs of level and p
Post: Prints out the corresponding node or an error if not found.
*/
void ASTprint(int level,ASTnode *p)
{
   int i;
   if (p == NULL ) return;

    // when here p is not NULL
   switch (p->nodetype) {
        case A_DEC_LIST : //Added A_DEC_LIST
                        ASTprint(level, p->s1); //Print out s1 which is a Declaration
                        ASTprint(level, p->s2); //Print out s2 which is another DeclarationList
                        break; //break out

        case A_VARDEC : PT(level); //Added A_VARDEC, print out the level with function PT and the current level given. 
                        printf("Variable "); //Print out the variable with its given datatype and name.
                        printf("%s ", DataTypeToString(p->symbol->Declared_Type)); 
                        printf("%s",p->name);
                        if (p->value > 0){ //If the p value is greater than zero, then it is an array
                           printf("[%d]",p->value);
                           printf(" with offset %d", p->symbol->offset);
                        }
                        else{
                            printf(" with offset %d", p->symbol->offset);
                        }
                        printf("\n");
		                ASTprint(level,p->s1); //Print out the s1 from p
                        ASTprint(level, p->s2); 
                        break;

        case A_FUNDEC : //Added A_FUNDEC
                        PT(level); //Print tab
                        printf("Function "); //Print out the function's datatype and name
                        printf("%s ", DataTypeToString(p->datatype));
                        printf("%s",p->name);
                        printf(" offset %d", p->symbol->offset);
                        if(p->s1->name == NULL){ //If the name is void, then print out VOID for the param
                        /*
                        This works because in the YACC code, if the node type is A_PARAMS, then the name will but null,
                        but if it's not T_VOID, then an A_PARAM vode will be created and has a name.
                        */
                            printf("(VOID)\n");
                        }
                        else{ //Else, print out the () and within that print out s1.
                            printf("\n");
                            PT(level + 1);
                            printf("(\n");
                            ASTprint(level + 2, p->s1);
                            PT(level + 1);
                            printf(")\n");
                        }
                        ASTprint(level + 1,p->s2); //Compound Statement
                        break;

        case A_PARAM :  PT(level); //Added A_PARAM
                        printf("parameter ");
                        printf("%s ", DataTypeToString(p->datatype));
                        printf("%s", p->name);
                        if(p->value == -1){
                            printf("[]");
                        }
                        printf(" with offset %d\n", p->symbol->offset);
                        ASTprint(level, p->s1); //Print the s1 branch
                        break;

        case A_COMPOUND : //Added A_COMPOUND
                        PT(level);
                        printf("Begin\n"); //Print out the Begin of the compound
                        ASTprint(level + 1,p->s1); //Local Decs
                        ASTprint(level + 1,p->s2); //Statement list
                        PT(level);
                        printf("End\n"); //Print out the End of the compound
                        break;

        case A_STATEMENTLIST : //Added A_STATEMENTLIST
                        ASTprint(level,p->s1); //Local Decs
                        ASTprint(level,p->s2); //Statement list
                        break;

        case A_READ :   PT(level); //Added A_READ
                        printf("Read \n"); //Print out Read
                        ASTprint(level + 1, p->s1); //Print out the expression contained in s1
                        break;

        case A_VAR :    PT(level); //Added A_VAR
                        printf("VAR with name %s\n", p->name); //Print out the Var's name
                        if(p->s1 != NULL){ //If s1 is not empty, then print out s1
                            PT(level+1);
                            printf("[\n");
                            ASTprint(level + 2, p->s1);
                            PT(level+1);
                            printf("]\n");
                        }
                        break;

        case A_WRITE :  PT(level); //Added A_WRITE
                        printf("Write \n");
                        if(p->name != NULL){ //*If the name isn't NULL, then print out the string that is given
                            PT(level+1);
                            printf("STRING: %s\n", p->name);
                        }
                        else{ //Else, there isn't a string to write and it is an expression to write out
                            ASTprint(level + 1, p->s1);
                        }
                        break;

        case A_ASSIGNMENTSTMT : //Added A_ASSIGNMENTSTMT
                        PT(level);
                        printf("ASSIGNMENT\n"); //Print out ASSIGNMENT
                        PT(level+1);
                        printf("LEFT HAND SIDE\n"); //Print out LEFT HAND SIDE
                        ASTprint(level+2, p->s1); //Print out s1
                        PT(level+1);
                        printf("RIGHT HAND SIDE\n"); //Print out RIGHT HAND SIDE 
                        ASTprint(level + 2, p->s2); //Print out s2 
                        break;

        case A_IF :     PT(level); //Added A_IF
                        printf("IF Statement\n"); //Print out IF Statement
                        PT(level + 1); 
                        printf("CONDITION\n"); //Print out CONDITION
                        ASTprint(level + 2, p->s1); //Print out the s1 branch which is the expression
                        PT(level + 1);
                        printf("IF BODY\n"); //Print out IF BODY
                        ASTprint(level + 2, p->s2->s1); //Print out the expression that is contained in the THEN token
                        if(p->s2->s2 != NULL){ //If it is an else if statement, then print out that else expression
                            PT(level+1);
                            printf("ELSE\n");
                            ASTprint(level+2, p->s2->s2);
                        }
                        break;
                        
        case A_ITERATIONSTMT : //Added A_ITERATIONSTMT
                        PT(level);
                        printf("WHILE\n"); //Print out WHILE
                        PT(level + 1);
                        printf("CONDITION\n"); //Print out CONDITION
                        ASTprint(level + 2, p->s1); //Print out s1, which is the expression
                        PT(level + 1);
                        printf("WHILE BODY\n"); //Print out WHILE BODY, followed by the statement of the iterationstmt
                        ASTprint(level + 2, p->s2);
                        break;

        case A_EXPRESSIONSTMT : 
                        PT(level);
                        printf("Expression statement\n");
                        if(p->s1 != NULL){
                            ASTprint(level + 1, p->s1);
                        }
                        else{
                            PT(level+1);
                            printf("Expression statement is NULL\n");
                        }
                        break;

        case A_EXPR :   PT(level); //Added A_EXPR
                        printf("Expression operator: "); //Print out the Expression operator
                        switch (p->operator) { //Switch statement to handle the returned datatypes
                            case A_PLUS: //For case A_PLUS, print +
                                printf("+\n");
                                break;

                            case A_MINUS: //For case A_MINUS print -
                                printf("-\n");
                                break;

                            case A_TIMES: //For case A_TIMES print *
                                printf("*\n");
                                break;

                            case A_LE: //For case A_LE print <=
                                printf("<=\n");
                                break;

                            case A_LESSTHAN:
                                printf("<\n");
                                break;

                            case A_GREATERTHAN:
                                printf(">\n");
                                break;

                            case A_GE:
                                printf(">=\n");
                                break;

                            case A_EQ:
                                printf("==\n");
                                break;

                            case A_NE:
                                printf("!=\n");
                                break;

                            case A_AND:
                                printf("AND\n");
                                break;

                            case A_DIVIDES:
                                printf("/\n");
                                break;

                            case A_OR:
                                printf("OR\n");
                                break;

                            case A_NOT:
                                printf("NOT\n");
                                break;

                            default:
                                printf("unknown type in A_EXPR in ASTprint%d\n",p->operator);
                                printf("Exiting ASTprint immediately\n");
                                exit(1);
                        }

                        ASTprint(level + 1, p->s1);
                        ASTprint(level + 1, p->s2);
                        break;

        case A_NUM :    PT(level);
                        printf("NUM with value %d\n", p->value);
                        break;

        case A_CALL :   PT(level);
                        printf("CALL %s\n", p->name);
                        PT(level + 1);
                        printf("(\n");
                        ASTprint(level+2, p->s1);
                        PT(level + 1);
                        printf(")\n");
                        break;

        case A_ARGLIST :
                        PT(level);
                        printf("CALL ARGUMENT\n");
                        ASTprint(level+1, p->s1);
                        if(p->s2 != NULL){
                            ASTprint(level, p->s2);
                        }
                        break;

        case A_TRUE :   PT(level);
                        printf("TRUE\n");
                        break;

        case A_FALSE :  PT(level);
                        printf("False\n");
                        break;

        case A_RETURNSTMT:
                        PT(level);
                        printf("Return\n");
                        if(p->s1 != NULL){
                            ASTprint(level+1, p->s1);
                        }
                        break;

        default: printf("unknown type in ASTprint%d\n",p->nodetype);
                 printf("Exiting ASTprint immediately\n");
                 exit(1);


       } // of switch
} // of ASTprint



/* dummy main program so I can compile for syntax error independently   
main()
{
}
/* */
