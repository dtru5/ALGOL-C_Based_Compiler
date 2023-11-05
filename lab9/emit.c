/*
Name: Dominik Trujillo
Date: 11/4/2023
Lab:
Purpose:
*/

#include "emit.h"

//PRE: PTR to ASTnode
//POST: All MIPS code directly and through helper functions.
void EMIT(ASTnode * p, FILE * fp){
    fprintf(fp, "# Compilers MIPS code Fall 2023 \n\n");
    fprintf(fp, ".data\n\n");
    fprintf(fp, ".align 2 \n\n");
    fprintf(fp, ".text \n\n");
    fprintf(fp, ".globl main \n\n");
}