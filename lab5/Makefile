#Dominik Trujillo
#09/14/2023
#Lab5 Makefile
#Lab 5 Algol-C into YACC and LEX 

all: lab5

lab5:	lab5.y lab5.l
	lex lab5.l
	yacc -d lab5.y
	gcc 	y.tab.c lex.yy.c -o lab5
	
clean: 
	rm -f lab5
