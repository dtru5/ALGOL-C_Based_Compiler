#Dominik Trujillo
#09/14/2023
#Lab4 Makefile

all: lab4

lab4:	symtable.c symtable.h lab4.y lab4.l
	lex lab4.l
	yacc -d lab4.y
	gcc 	symtable.c y.tab.c lex.yy.c -o lab4
	
clean: 
	rm -f lab4
