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

#endif