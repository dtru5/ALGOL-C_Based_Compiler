/*
Name: Dominik Trujillo
Date: 11/4/2023
Lab:
Purpose:
*/
#include <stdlib.h>
#include <string.h>
#include "emit.h"
#include "symtable.h"
#include "ast.h"

#define WSIZE 4

int GLABEL = 0;
//char labelBuffer[20]; QUESTION: WHY DIDN'T THIS WORK

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

//PRE:
//POST:
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

//PRE:
//POST:
void EMIT_STRINGS(ASTnode * p, FILE * fp){
    if(p == NULL) return;

    if(p->nodetype == A_WRITE && p->name != NULL){
        p->label = CreateLabel();
        fprintf(fp,"%s: .asciiz\t%s\n", p->label, p->name);
    }

    EMIT_STRINGS(p->s1, fp);
    EMIT_STRINGS(p->s2, fp);
}

//PRE:
//POST:
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

    emit_ast(p->s2, fp); //Calls for compound statement.


    emit(fp, "", "lw $ra 4($sp)", "restore old environment RA");
	emit(fp, "", "lw $sp ($sp)", "Return from function store SP");

    //Printing out the ending code to exit from main.
    if(strcmp(p->name, "main") == 0){
        emit(fp, "", "li $v0, 10", "Exit from Main we are done");
	    emit(fp, "", "syscall", "EXIT everything");
    }
    else{
        //FIX FIX FIX for function call return.
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

    //PRE: PTR to A_VAR
    //POST: $a0 will be the memory location of the variable.
    void emit_var(ASTnode *p, FILE *fp) {
    char s[100];

    if (p->symbol->SubType == SYM_ARRAY) {
        emit_expr(p->s1, fp);
        emit(fp, "", "move $a1, $a0", "VAR copy index array in a1");
        emit(fp, "", "sll $a1, $a1, 2", "multiply the index by word size via SLL");
    }

    if (p->symbol->level == 0) {
        sprintf(s, "la $a0, %s", p->name);
        emit(fp, "", s, "Emit var global variable");
    } else {
        emit(fp, "", "move $a0, $sp", "VAR local make a copy of stack pointer");
        sprintf(s, "addi $a0, $a0, %d", p->symbol->offset * WSIZE);
        emit(fp, "", s, "VAR local stack pointer plus offset");
    }

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

    //PRE:
    //POST:
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

    //PRE:
    //POST:
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

    //PRE:
    //POST:
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

    void emit_iterationstmt(ASTnode * p, FILE * fp){
        char s[100];
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