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