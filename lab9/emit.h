/*
Name: Dominik Trujillo
Date: 11/4/2023
Lab:
Purpose:
*/

#ifndef EMIT_H
#define EMIT_H
#include "ast.h"

#define WSIZE 4
#define LOG_WSIZE 2

void EMIT(ASTnode * p, FILE * fp);

void EMIT_GLOBALS(ASTnode * p, FILE * fp);

char * CreateLabel();

void EMIT_STRINGS(ASTnode * p, FILE * fp);

void emit(FILE * fp, char * label, char * command, char * comment);

void emit_ast(ASTnode * p, FILE * fp);

void emit_function_dec(ASTnode * p, FILE * fp);

void emit_write(ASTnode *p, FILE *fp);

void emit_expr(ASTnode * p, FILE * fp);

void emit_var(ASTnode * p, FILE * fp);

void emit_read(ASTnode * p, FILE * fp);

void emit_expr_helper(ASTnode * p, FILE * fp);

void emit_assignment(ASTnode * p, FILE * fp);

void emit_if(ASTnode * p, FILE * fp);

void emit_iterationstmt(ASTnode * p, FILE * fp);

void emit_params(ASTnode * p, FILE * fp);

void emit_return(ASTnode * p, FILE * fp);

void emit_call(ASTnode * p, FILE * fp);

void emit_arglist(ASTnode * p, FILE * fp);

#endif