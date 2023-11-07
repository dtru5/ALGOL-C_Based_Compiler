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
        int space = 4 * p->value;
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

        default: printf("emit_ast unknown nodetype %d\n", p->nodetype);
                 printf("Exiting program: FIX ME\n");
                 exit(1);
    }
}//End of emit_ast