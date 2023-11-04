#Dominik Trujillo
#11/24/2023
#Lab 9 Makefile
#LAB 9 ALGOL -- add symbol table and type checking

all: lab9

lab9:	ast.c ast.h symtable.c symtable.h lab9.y lab9.l emit.c emit.h
	yacc -dv lab9.y
	lex lab9.l
	gcc lex.yy.c y.tab.c ast.c symtable.c emit.c -o lab9	

clean: 
	rm -f lab9
