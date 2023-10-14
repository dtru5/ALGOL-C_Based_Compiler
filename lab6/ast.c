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
    Purpose: 
*/

#include<stdio.h>
#include<stdlib.h>  
#include<malloc.h>
#include "ast.h" 

/*MAKE SURE TO READ THIS AND TO DO.*/
//Originally in ast.h but moved to c file due to compliation errors
//Make sure to move back when submitting.
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
    return(p);
}

/*  Helper function to print tabbing */

// PRE: Given a positive integer of howmany.
//  POST: Prints out that many of spaces.
void PT(int howmany)
{
	 for(int i = 0; i < howmany; i++){
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


/*  Print out the abstract syntax tree */
void ASTprint(int level,ASTnode *p)
{
   int i;
   if (p == NULL ) return;

    // when here p is not NULL
   switch (p->nodetype) {
        case A_DEC_LIST :
                        ASTprint(level, p->s1);
                        ASTprint(level, p->s2);
                        break;

        case A_VARDEC : PT(level); 
                        printf("Variable ");
                        printf("%s ", DataTypeToString(p->datatype));
                        printf("%s",p->name);
                        if (p->value > 0)
                           printf("[%d]",p->value);
                        printf("\n");
		                ASTprint(level,p->s1); 
                        if(p->s2 != NULL){
                            ASTprint(level, p->s2);
                        }
                        break;

        case A_FUNDEC : 
                        PT(level); 
                        printf("Function ");
                        printf("%s ", DataTypeToString(p->datatype));
                        printf("%s",p->name);
                        printf("\n");
		                ASTprint(level + 1,p->s1); //Parameters
                        ASTprint(level + 1,p->s2); //Compound Statement
                        break;

        case A_COMPOUND : 
                        PT(level);
                        printf("Begin\n"); 
                        ASTprint(level + 1,p->s1); //Local Decs
                        ASTprint(level + 1,p->s2); //Statement list
                        PT(level);
                        printf("End\n");
                        break;

        case A_STATEMENTLIST : 
                        ASTprint(level,p->s1); //Local Decs
                        ASTprint(level,p->s2); //Statement list
                        break;

        case A_READ :   PT(level);
                        printf("Read \n");
                        ASTprint(level + 1, p->s1);
                        break;

        case A_VAR :    PT(level);
                        printf("VAR with name %s\n", p->name);
                        if(p->s1 != NULL){
                            PT(level+1);
                            printf("[\n");
                            ASTprint(level + 2, p->s1);
                            PT(level+1);
                            printf("]\n");
                        }
                        break;

        case A_WRITE :  PT(level);
                        printf("Write \n");
                        if(p->name != NULL){
                            PT(level+1);
                            printf("STRING: %s\n", p->name);
                        }
                        else{
                            ASTprint(level + 1, p->s1);
                        }
                        break;

        case A_ASSIGNMENTSTMT : 
                        PT(level);
                        printf("ASSIGNMENT\n");
                        PT(level+1);
                        printf("LEFT HAND SIDE\n");
                        ASTprint(level+2, p->s1);
                        PT(level+1);
                        printf("RIGHT HAND SIDE\n");
                        ASTprint(level + 2, p->s2);
                        break;

        case A_IF :     PT(level);
                        printf("IF Statement\n");
                        PT(level + 1);
                        printf("CONDITION\n");
                        ASTprint(level + 2, p->s1);
                        PT(level + 1);
                        printf("IF BODY\n");
                        ASTprint(level + 2, p->s2->s1);
                        if(p->s2->s2 != NULL){
                            PT(level+1);
                            printf("ELSE\n");
                            ASTprint(level+2, p->s2->s2);
                        }
                        break;
                        
        case A_ITERATIONSTMT : 
                        PT(level);
                        printf("WHILE\n");
                        PT(level + 1);
                        printf("CONDITION\n");
                        ASTprint(level + 2, p->s1);
                        PT(level + 1);
                        printf("WHILE BODY\n");
                        ASTprint(level + 2, p->s2);
                        break;

        case A_EXPR :   PT(level);
                        printf("Expression operator: ");
                        switch (p->operator) {
                            case A_PLUS:
                                printf("+\n");
                                break;

                            case A_MINUS:
                                printf("-\n");
                                break;

                            case A_TIMES:
                                printf("*\n");
                                break;

                            case A_LE:
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

                            default:
                                printf("unknown type in A_EXPR in ASTprint%d\n",p->operator);
                                printf("Exiting ASTprint immediately\n");
                                exit(1);
                        }
                        ASTprint(level + 1, p->s1);
                        ASTprint(level + 1, p->s2);
                        break;

        case A_NUM : PT(level);
                     printf("NUM with value %d\n", p->value);
                     break;

        case A_CALL : PT(level);
                    printf("CALL %s\n", p->name);
                    PT(level + 1);
                    printf("(\n");
                    ASTprint(level + 2, p->s1);

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
