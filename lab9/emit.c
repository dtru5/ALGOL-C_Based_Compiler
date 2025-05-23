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

#include <stdlib.h>
#include <string.h>
#include "emit.h"
#include "symtable.h"
#include "ast.h"

#define WSIZE 4 //Defining the wsize to be a constant 4

int GLABEL = 0; //For global labels 

int ARGMAX = 0; //For max arguments

//PRE: PTR to ASTnode
//POST: All MIPS code directly and through helper functions.
void EMIT(ASTnode * p, FILE * fp){
    fprintf(fp, "# Compilers MIPS code Fall 2023 \n\n");
    fprintf(fp, ".data\n\n");
    EMIT_STRINGS(p, fp);
    fprintf(fp, "\n.align 2 \n\n");
    EMIT_GLOBALS(p, fp);
    fprintf(fp, "\n.text \n\n");
    fprintf(fp, ".globl main \n\n");
    emit_ast(p, fp);
}

// PRE: PTR to ASTnode
// POST: Generates MIPS code for global variable declarations in the .data section.
void EMIT_GLOBALS(ASTnode * p, FILE * fp){
    if(p == NULL){
        return;
    }

    if(p->symbol != NULL && p->nodetype == A_VARDEC && p->symbol->level == 0){
        int space = WSIZE * p->value;
        fprintf(fp, "%s:\t\t.space %d\n", p->name, space);
    }

    EMIT_GLOBALS(p->s1, fp);
    EMIT_GLOBALS(p->s2, fp);
}

// PRE: PTR to ASTnode
// POST: Generates MIPS code for string declarations in the .data section.
void EMIT_STRINGS(ASTnode * p, FILE * fp){
    if(p == NULL) return;

    if(p->nodetype == A_WRITE && p->name != NULL){
        p->label = CreateLabel();
        fprintf(fp,"%s: .asciiz\t%s\n", p->label, p->name);
    }

    EMIT_STRINGS(p->s1, fp);
    EMIT_STRINGS(p->s2, fp);
}

// PRE: None
// POST: Generates a unique label for MIPS assembly code and returns that label.
char* CreateLabel() {    
    char hold[100];
    char * label;
    sprintf(hold, "_L%d", GLABEL++);
    label = strdup(hold);
    return label;
}

//PRE: Possible label, command, comment.
//POST: Formatted output to the label
void emit(FILE * fp, char * label, char * command, char * comment){
    if(strcmp("", comment) == 0){
        if (strcmp("", label) == 0) fprintf(fp, "\t%s\t\t\n", command);
        else fprintf(fp, "%s:\t%s\t\t\n", label, command);
    }
    else{
        if (strcmp("",label) == 0) fprintf(fp, "\t%s\t\t# %s\n", command, comment);
        else fprintf(fp, "%s:\t%s\t\t# %s\n", label, command, comment);
    }
} //End of emit

//PRE: PTR to ASTnode
//POST: Main driver for walking our AST tree to produce MIPS code in the file.
void emit_ast(ASTnode * p, FILE * fp){
    if (p == NULL) return;

    switch (p->nodetype){

        case A_DEC_LIST: emit_ast(p->s1, fp);
                         emit_ast(p->s2, fp);
                         break;

        case A_VARDEC:   emit_ast(p->s1, fp);
                         emit_ast(p->s2, fp);
                         break;

        case A_FUNDEC:   emit_function_dec(p, fp);
                         break;

        case A_PARAM:   emit_params(p, fp);
                         break;

        case A_COMPOUND: emit_ast(p->s2,fp); 
                         break;

        case A_STATEMENTLIST: emit_ast(p->s1, fp);
                         emit_ast(p->s2, fp);
                         break;

        case A_WRITE:   emit_write(p, fp);
                         break;

        case A_READ:    emit_read(p, fp);
                        break;

        case A_ASSIGNMENTSTMT:
                        emit_assignment(p, fp);
                        break;

        case A_IF:
                        emit_if(p, fp);
                        break;

        case A_ITERATIONSTMT: 
                        emit_iterationstmt(p, fp);
                        break;

        case A_RETURNSTMT:
                        emit_return(p, fp);
                        break;

        case A_EXPRESSIONSTMT:
                        if(p->s1 != NULL) emit_expr(p->s1, fp);
                        break;

        default: printf("emit_ast unknown nodetype %d\n", p->nodetype);
                 printf("Exiting program: FIX ME\n");
                 exit(1);
    }
}//End of emit_ast

//PRE: PTR to a A_FUNDEC node
//POST: MIPS code for the function, using emit_ast as a helper
void emit_function_dec(ASTnode * p, FILE * fp){
    char s[100];

    //Printing out the function label
    emit(fp, p->name, "", "Start of function");
    fprintf(fp,"\n");
    sprintf(s,"subu $a0, $sp, %d",p->symbol->offset*WSIZE);
    emit(fp, "", s, "adjust the stack for function setup");
	emit(fp, "", "sw $sp, ($a0)","remember old SP");
	emit(fp, "", "sw $ra, 4($a0)", "remember current Return address");
	emit(fp, "", "move $sp, $a0", "adjust the stack pointer");
    fprintf(fp,"\n\n");

    if(p->s1 != NULL) emit_ast(p->s1, fp); //If the param type isn't void (NULL), emit params.

    emit_ast(p->s2, fp); //Calls for compound statement.

    fprintf(fp,"\n");

    emit(fp, "", "li $a0, 0", "RETURN has no specified value set to 0");
    emit(fp, "", "lw $ra 4($sp)", "restore old environment RA");
	emit(fp, "", "lw $sp ($sp)", "Return from function store SP");

    //Printing out the ending code to exit from main.
    if(strcmp(p->name, "main") == 0){
        emit(fp, "", "li $v0, 10", "Exit from Main we are done");
	    emit(fp, "", "syscall", "EXIT everything");
    }
    else{
        emit(fp, "", "jr $ra", "return to the caller");
    }
}

    // PRE: PTR to A_RETURNSTMT
    // POST: Generates MIPS code for returning from a function, including restoring the old environment.
    void emit_return(ASTnode * p, FILE * fp){
        if(p->s1 != NULL)  emit_expr(p->s1, fp);
        emit(fp, "", "lw $ra 4($sp)", "restore old environment RA");
        emit(fp, "", "lw $sp ($sp)", "Return from function store SP");
        emit(fp, "", "jr $ra", "return to the caller");
    }

    // PRE: PTR to A_PARAM
    // POST: Generates MIPS code for setting up parameters on the stack.
    void emit_params(ASTnode * p, FILE * fp){
        char s[100];
        int i = 0;
        sprintf(s, "sw $t%d, %d($sp)", i, p->symbol->offset * WSIZE);
        emit(fp, "", s, "Load temp variable int formal paramter");

        while(p->s1 != NULL){
            i++;
            sprintf(s, "sw $t%d, %d($sp)", i, p->s1->symbol->offset * WSIZE);
            emit(fp, "", s, "Load temp variable int formal paramter");
            p = p->s1;
        }
    }

    //PRE: PTR to a Write node.
    //POST: MIPS code to perform Write.
    void emit_write(ASTnode *p, FILE *fp){
        char s[100];
        if(p->name != NULL) {//Then it is a string
            emit(fp,"","li $v0, 4", "print a string");
            sprintf(s,"la $a0, %s", p->label);
            emit(fp,"", s,"print fetch string location");
            emit(fp,"","syscall","Perform a write string");
            fprintf(fp,"\n\n");
        }

        else{ //Then it is an expression
            emit_expr(p->s1, fp);
            emit(fp,"","li $v0 1", "Print the number");
            emit(fp,"","syscall","system call for print number");
            fprintf(fp,"\n\n");
        }
    }

    //PRE: PTR to a expression tree component. ie. (A_EXPR, A_NUM, A_CALL etc)
    //POST: $a0 will have the value set by the generated MIPS code.
    void emit_expr(ASTnode * p, FILE * fp){
        if(p == NULL){
            printf("Illegal use of EMIT_EXPR with NULL pointer.\n");
            exit(1);
        }

        char s[100];

        //Deal with base cases
        switch(p->nodetype){

            case A_TRUE:
            case A_FALSE:
            case A_NUM: sprintf(s,"li $a0, %d", p->value);
                        emit(fp,"",s,"Expression is a constant");
                        return;
                        break;

            case A_VAR: emit_var(p,fp);
                        emit(fp, "", "lw $a0, ($a0)", "Expressions is a var, get value.");
                        return;
                        break;

            case A_CALL: emit_call(p, fp);
                        break;

            case A_ARGLIST: emit_arglist(p, fp);
                            break;

            case A_EXPR: 
                switch (p->operator){
                        char placeholder[100]; //New place holder char array instead of using s[].
                        //-----------CASES FOR ADDOP EXPRESSIONS------------------
                        case A_PLUS:
                            emit_expr_helper(p, fp);
                            emit(fp, "", "add $a0, $a0, $a1","EXPR ADD");
                            break;

                        case A_MINUS:
                            emit_expr_helper(p, fp);
                            emit(fp, "", "sub $a0, $a0, $a1", "EXPR SUB");
                            break;

                        //-----------CASES FOR RELOP EXPRESSIONS------------------
                        case A_LE: //Less than or equal to
                            emit_expr_helper(p, fp);
                            emit(fp, "", "add $a1 ,$a1, 1", "EXPR LE add one to do compare");
                            emit(fp, "", "slt $a0, $a0, $a1", "EXPR LE");
                            break;

                        case A_LESSTHAN:
                            emit_expr_helper(p, fp);
                            emit(fp, "", "slt $a0, $a0, $a1", "EXPR LESSTHAN");
                            break;

                        case A_GREATERTHAN:
                            emit_expr_helper(p, fp);
                            emit(fp, "", "slt $a0, $a1, $a0", "EXPR GREATERTHAN");
                            break;

                        case A_GE:
                            emit_expr_helper(p, fp);
                            emit(fp, "", "add $a0 ,$a0, 1", "EXPR ADD GE");
                            emit(fp, "", "slt $a0, $a1, $a0", "EXPR GREATERTHAN");
                            break;

                        case A_EQ:
                            emit_expr_helper(p, fp);
                            emit(fp,"","slt $t2 ,$a0, $a1","EXPR EQUAL");
                            emit(fp, "", "slt $t3 ,$a1, $a0", "EXPR EQUAL");
                            emit(fp, "", "nor $a0 ,$t2, $t3", "EXPR EQUAL");
                            emit(fp, "", "andi $a0, 1", "EXPR EQUAL");
                            break;

                        case A_NE:
                            emit_expr_helper(p, fp);
                            emit(fp,"","slt $t2 ,$a0, $a1","EXPR NOT EQUAL");
                            emit(fp, "", "slt $t3 ,$a1, $a0", "EXPR NOT EQUAL");
                            emit(fp, "", "or $a0 ,$t2, $t3", "EXPR NOT EQUAL");
                            break;

                        //-----------CASES FOR RELOP EXPRESSIONS------------------
                        case A_TIMES:
                            emit_expr_helper(p, fp);
                            emit(fp, "", "mult $a0 $a1", "EXPR MULT");
                            emit(fp, "", "mflo $a0", "EXPR MULT");
                            break;

                        case A_DIVIDES:
                            emit_expr_helper(p, fp);
                            emit(fp, "", "div $a0 $a1", "EXPR DIV");
                            emit(fp, "", "mflo $a0", "EXPR DIV");
                            break;
                    
                        case A_AND:
                            emit_expr_helper(p, fp);
                            emit(fp, "", "and $a0, $a0, $a1", "EXPR AND");
                            break;

                        case A_OR:
                            emit_expr_helper(p, fp);
                            emit(fp, "", "or $a0, $a0, $a1", "EXPR OR");
                            break;

                        case A_NOT:
                            emit_expr(p->s1, fp); //Emit the Factor of A_NOT
                            emit(fp, "", "or $a0, $a0, $zero", "EXPR NOT"); //XOR to flip the bits to get inverse of value.
                            emit(fp, "", "slti $a0, $a0, 1", "EXPR NOT"); 
                            break;

                        default:
                            printf("unknown operator %d\n", p->operator);
                            exit(1);
                            break;
                        }//End of operator cases.   
                        break;

            default: printf("emit_expr base case not know.\n");
                     printf("unknown nodetype %d\n", p->nodetype);
                    exit(1);
        }//End of switch of base cases
    }

// PRE: PTR to A_CALL
// POST: Generates MIPS code for function call, including setting up function parameters and making the function call.
void emit_call(ASTnode * p, FILE * fp){
    ARGMAX = 0;
    char s[100], j[100];
    int t = 0;

    // Generate the jump and link (jal) instruction for the function call
    sprintf(s,"jal %s", p->name);

    // Comments for setting up the function call
    emit(fp,"","","Setting Up Function Call");
    emit(fp,"","","Evaluate Function Parameters");

    // Generate code to evaluate and store function parameters on the stack
    if(p->s1 != NULL) emit_expr(p->s1, fp);

    if(ARGMAX > 7){
        printf("More than 7 arguments.. Exiting\n");
        exit(1);
    }

    emit(fp,"","","Place Parameters into T registers");

    // Check if there are parameters to process
    if(p->s1 != NULL){
        // Load the first argument from the stack
        sprintf(j, "lw $a0, %d($sp)", p->s1->symbol->offset * WSIZE);
        emit(fp, "", j, "Pull out stored Arg"); 
        // Move the first argument to a temporary register
        sprintf(j, "move $t%d, $a0", t);
        emit(fp, "", j, "Move arg into temp");
        
        // Loop through the remaining arguments
        while(p->s1->s2 != NULL){
            t++;
            // Load the next argument from the stack
            sprintf(j, "lw $a0, %d($sp)", p->s1->s2->symbol->offset * WSIZE);
            emit(fp, "", j, "Pull out stored Arg"); 
            // Move the argument to a temporary register
            sprintf(j, "move $t%d, $a0", t);
            emit(fp, "", j, "Move arg into temp");
            p = p->s1;
        }
    }
    fprintf(fp, "\n");

    // Emit the jal instruction to call the function
    emit(fp, "", s, "Call the function\n");
}


    //PRE: PTR to arg list
    //POST: All compainion create temp are set to right value
void emit_arglist(ASTnode * p, FILE * fp){
    char s[100];

    // Check if the argument list is empty
    if(p == NULL){
        return;
    }

    // Evaluate the expression of the first argument and store the result on the stack
    emit_expr(p->s1, fp);
    sprintf(s, "sw $a0, %d($sp)", p->symbol->offset * WSIZE);
    emit(fp, "", s, "Store call Arg temporarily\n");
    ARGMAX++;

    // Check if there is a second argument
    if(p->s2 != NULL){
        // Evaluate the expression of the second argument
        emit_expr(p->s2, fp);
    }
}

    //PRE: PTR to A_VAR
    //POST: $a0 will be the memory location of the variable.
void emit_var(ASTnode *p, FILE *fp) {
    char s[100];

    // Check if the variable is an array
    if (p->symbol->SubType == SYM_ARRAY) {
        // Evaluate the array index expression and store the result in $a0
        emit_expr(p->s1, fp);
        emit(fp, "", "move $a1, $a0", "VAR copy index array in a1");
        emit(fp, "", "sll $a1, $a1, 2", "multiply the index by word size via SLL");
    }

    // Check if the variable is a global variable
    if (p->symbol->level == 0) {
        // Load the address of the global variable into $a0
        sprintf(s, "la $a0, %s", p->name);
        emit(fp, "", s, "Emit var global variable");
    } else {
        // Variable is a local variable, make a copy of the stack pointer
        emit(fp, "", "move $a0, $sp", "VAR local make a copy of stack pointer");
        // Adjust the stack pointer by the offset to get the local variable's address
        sprintf(s, "addi $a0, $a0, %d", p->symbol->offset * WSIZE);
        emit(fp, "", s, "VAR local stack pointer plus offset");
    }

    // Check if the variable is an array and add the internal offset
    if (p->symbol->SubType == SYM_ARRAY) {
        emit(fp, "", "add $a0, $a0, $a1", "VAR array add internal offset");
    }
}


    //PRE: PTR to A_READ
    //POST: MIPS code to generate the location of a var and read it.
    void emit_read(ASTnode * p, FILE * fp){
        emit_var(p->s1, fp);
        emit(fp,"","li $v0, 5","Read a number from input.");
        emit(fp,"","syscall","Reading a number.");
        emit(fp,"","sw $v0, ($a0)","Store the read into a memory location.");
        fprintf(fp,"\n\n");
    }

    //PRE: PTR to A_EXPR
    //POST: MIPS code to store the LHS temporarily, getting the RHS, and restoring LHS from memory.
    void emit_expr_helper(ASTnode * p, FILE * fp){
        char placeholder[100];
        emit_expr(p->s1, fp);
        sprintf(placeholder,"sw $a0, %d($sp)",p->symbol->offset*WSIZE);
        emit(fp, "", placeholder, "expression store LHS temporarily");
        emit_expr(p->s2, fp);
        emit(fp, "", "move $a1, $a0", "right hand side needs to be a1");
        sprintf(placeholder, "lw $a0, %d($sp)", p->symbol->offset*WSIZE);
	    emit(fp, "", placeholder, "expression restore LHS from memory");
    }

    //PRE: PTR to A_ASSIGNMENT
    //POST: MIPS code to handle assignment statements that stores the RHS temporarily and placing the RHS into memory.
    void emit_assignment(ASTnode * p, FILE * fp){
        char s[100];
        emit_expr(p->s2, fp);
        sprintf(s,"sw $a0, %d($sp)",p->symbol->offset*WSIZE);
        emit(fp, "", s, "Assign store RHS temporarily");
        emit_var(p->s1, fp);
        sprintf(s,"lw $a1 %d($sp)",p->symbol->offset*WSIZE);
        emit(fp, "", s, "Assign get RHS temporarily");
	    emit(fp, "", "sw $a1 ($a0)", "Assign place RHS into memory");
    }

    //PRE: PTR to A_IF
    //POST: MIP code to handle if and if-else statements by handling statements with labels.
    void emit_if(ASTnode *p, FILE *fp) {
        char s[100];
        char *label_else = CreateLabel();
        char *label_end = CreateLabel();

        // Emit the conditional check
        emit_expr(p->s1, fp);

        // Branch to the else part if the condition is false
        sprintf(s, "beq $a0 $0 %s", label_else);
        emit(fp, "", s, "IF branch to else part\n\n");
        emit(fp, "", "", "the positive portion of IF");

        // Emit the positive portion of the IF statement
        emit_ast(p->s2->s1, fp);

        // Jump to the end of the IF statement
        sprintf(s, "j %s", label_end);
        emit(fp, "", s, "IF S1 end\n");

        // Emit the label for the else part
        emit(fp, label_else, "", "ELSE target");
        emit(fp, "", "", "the negative  portion of IF if there is an else");
        emit(fp, "", "", "otherwise just these lines\n");

        // Emit the negative portion of the IF statement (else part)
        emit_ast(p->s2->s2, fp);

        // Emit the label for the end of the IF statement
        emit(fp, label_end, "", "End of IF");
    }

    //PRE: PTR to A_ITERATIONSTMT 
    //POST: MIPS code to generate labels and handling iterations with breakout/jump points.
    void emit_iterationstmt(ASTnode * p, FILE * fp){
        char s[100];
        // Create 2 labels that will be for the start of the while loop and another for the end of the while look
        char * startWhileLabel = CreateLabel();
        char * endWhileLabel = CreateLabel();
        
        emit(fp, startWhileLabel, "", "WHILE TOP target");
        emit_expr(p->s1, fp);
        sprintf(s, "beq $a0 $0 %s", endWhileLabel); 
        emit(fp, "", s, "WHILE branch out");
        emit_ast(p->s2, fp);
        sprintf(s, "j %s", startWhileLabel);
        emit(fp, "", s, "WHILE Jump back\n");
        emit(fp, endWhileLabel, "", "End of WHILE");
    }