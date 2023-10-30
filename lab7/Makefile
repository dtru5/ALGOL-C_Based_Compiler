#Dominik Trujillo
#10/20/2023
#Lab7 Makefile
#LAB 7 ALGOL -- add symbol table and type checking

all: lab7

lab7:	ast.c ast.h symtable.c symtable.h lab7.y lab7.l
	yacc -dv lab7.y
	lex lab7.l
	gcc y.tab.c lex.yy.c ast.c symtable.c -o lab7	

clean: 
	rm -f lab7
