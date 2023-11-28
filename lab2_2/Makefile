#Dominik Trujillo
#August 23, 2023
#Lab2_2 makefile

# "a = 3 + 5"
# "a"
#How these operations would work with our YACC and our LEX file is that LEX will read the following input
#The first character is a would be detected (white space is ignored) and the following = sign that will accounted for
#Now the array regs[] at the position of 'a', which is 0, will be stored with the following expression
#As for the expression of 3 + 5, LEX will read the integer 3, then the token +, followed by the integer 8
#YACC will add the values of the expression from the expr label and the results will be stored in regs at location 'a' or 0 in this case

#As for the "a" operation, YACC will get the value from regs at location 'a', or 0, and will print out the value store at that location and the location index itself.


all: lab2_2

lab2_2: lab2_2.y lab2_2.l
	lex lab2_2.l
	yacc -d lab2_2.y 
	gcc y.tab.c lex.yy.c -o lab2_2
	
clean: 
	rm -f lab2_2
