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

int labelNUM = 0;
char labelBuffer[20]; 

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

void EMIT_STRINGS(ASTnode * p, FILE * fp){
    if(p == NULL) return;

    if(p->nodetype == A_WRITE && p->name != NULL){
        p->label = CreateLabel();
        fprintf(fp,"%s: .asciiz\t%s\n", p->label, p->name);
    }

    EMIT_STRINGS(p->s1, fp);
    EMIT_STRINGS(p->s2, fp);
}

char* CreateLabel() {
    snprintf(labelBuffer, sizeof(labelBuffer), "_L%d", labelNUM++);
    return labelBuffer;
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

    //PRE: PTR to a Write node.
    //POST: MIPS code to perform Write.
    void emit_write(ASTnode *p, FILE *fp){

    }

}