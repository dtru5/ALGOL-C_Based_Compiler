#Dominik Trujillo
#09/26/2023
#Lab6 Makefile
#LAB 6 ALGOL Abstract Syntax Tree

all: lab6

lab6:	ast.c ast.h lab6.y lab6.l
	yacc -dv lab6.y
	lex lab6.l
	gcc y.tab.c lex.yy.c ast.c -o lab6	

clean: 
	rm -f lab6
