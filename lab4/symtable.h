/*
Dominik Trujillo
09/15/2023
LAB4 -- Symbol Table with YACC 
This is a header file for symtable c file
*/

#ifndef SYMTABLE_H_
#define SYMTABLE_H_

void Insert(char * sym, int address);

void Display();

void Delete(char *s);

int Search(char * s);

int FetchAddress(char * symbol);

#endif
