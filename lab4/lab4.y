%{

/*
 *			**** CALC ****
 *
 * This routine will function like a desk calculator
 * There are 26 integer registers, named 'a' thru 'z'
 *
 */

/* This calculator depends on a LEX description which outputs either VARIABLE or INTEGER.
   The return type via yylval is integer 

   When we need to make yylval more complicated, we need to define a pointer type for yylval 
   and to instruct YACC to use a new type so that we can pass back better values
 
   The registers are based on 0, so we substract 'a' from each single letter we get.

   based on context, we have YACC do the correct memmory look up or the storage depending
   on position

   Shaun Cooper
    January 2015

   problems  fix unary minus, fix parenthesis, add multiplication
   problems  make it so that verbose is on and off with an input argument instead of compiled in
*/

/*
	Lab 4
	Name: Dominik Trujillo 
	Date: 08/25/2023
	Purpose: The purpose of this lab, specifially this YACC file, is to take the values that were returned by the LEX portion
	and to determine which of the following operations will be applied. Starting from the bottom to top, we will check the expression and do that operation; and 
	then we will use the symbol table that we developed the past lab to check if variables have been inserted/declared before and using the functionality 
	in YACC to search those variables in the symbol table.

	Log: Changes that were made was adding in operations to have a working '*' or multiplication operation working by 
	taking an expression with a '*' and ending with another expression to do multiplication with the given values.
	Also fixed the unary minus by taking in an expression that is started with a '-' character and taking that value and making it negative.
	
	Changes made for Lab4 on 09/15/2023
		
		Included: new max constant to ensure that values don't go over the array value of regs. 
		Added a new counter variable called oneup to count how the number of symbols added to the table.
		Included an extern int from LEX called lineno that will be the line number an error is found.
		
		Added:
		%start P to start at P

		%union { int num;
			 char * string; } To have a union of both an int and char pointer.
			
		%token <num> INTEGER
		%token <string> VARIABLE
		%token T_INT
		
		to have a new starting point of P
		to have a union of both an int num and a char pointer (*) to string
		to have 3 new tokens added that INTEGER will be type num
		VARIABLE will have type string, and new token T_INT
		
		
*/


	/* begin specs */
#include <stdio.h>
#include <ctype.h>
#include "symtable.h"

int yylex();
#define MAX 26
int regs[MAX];
int base, debugsw;
int oneup = 0;
extern int lineno;

/*
This method catches any errors made from input
Parameters: a variable s
Pre-condition: If any errors are made
Post-condition: To print out the error
*/
void yyerror (s)  /* Called by yyparse on error */
     char *s;
{
  printf ("%s\n", s);
}


%}
/*  defines the start symbol, what values come back from LEX and how the operators are associated  */

%start P

%union { int num;
	 char * string; }
	
%token <num> INTEGER
%token <string> VARIABLE
%token T_INT

%type <num> expr

%left '|'
%left '&'
%left '+' '-'
%left '*' '/' '%'
%left UMINUS


%%	/* end specs, begin rules */
P	: DECLS list
	;
	
DECLS	: DECLS DECL 
	| 	/* empty */
	;
	
DECL	: T_INT VARIABLE ';' '\n'
		{//If $2 is in the symbol table print an error
		//else insert ($2, ONEUP);
		//ONEUP++;
		
			//is there  room inthe regs house
			//make sure you do not allow more variables 
			//than what is in REGS
			//MAX
			
		if(oneup == MAX) //if there is a max number of values in the array print out an error
			printf("ERROR ON LINE %d: HAVE REACHED MAX VALUE\n", lineno);
		else{
			if(Search($2) == 1)//else, if the variable is already declare, print out error
				printf("ERROR ON LINE %d: ALREADY IN SYMBOL TABLE\n", lineno);
			else //else, else store that variable symbol into the symbol table at the address of oneup
				Insert($2, oneup);
				fprintf(stderr,"%s has been inserted\n",$2); 
				oneup++; //increment oneup
		
		}
		}
	;

list	:	/* empty */
	|	list stat '\n'
	|	list error '\n'
			{ yyerrok; }
	;

stat	:	expr
			{ fprintf(stderr,"the anwser is %d\n", $1); }
	|	VARIABLE '=' expr
			{ 
			if(Search($1) == 0) printf("ERROR ON LINE %d: VARIABLE HAS NOT BEEN DECLARED\n", lineno); //If not declared then produce error message
			else regs[FetchAddress($1)] = $3; } //Else, set the value of that variable, in relation to reg's address, with the value of the expr
	;

expr	:	'(' expr ')' 
			{ $$ = $2; }
	|	expr '-' expr
			{ $$ = $1 - $3; }
	|	expr '+' expr
			{ $$ = $1 + $3; }
	|	expr '/' expr
			{ $$ = $1 / $3; }
	|	expr '%' expr
			{ $$ = $1 % $3; }
	|	expr '&' expr
			{ $$ = $1 & $3; }
	|	expr '*' expr	/* Added an expression for multiplication and taking the product of the 1st and 2nd expression and storing it into $$*/
			{ $$ = $1 * $3; }
	|	expr '|' expr
			{ $$ = $1 | $3; }
	|	'-' expr	%prec UMINUS /* Changed unary minus so that the whole expression is represented by a '-' followed by another expr */
			{ $$ = -$2; } /* Store that value with the value from the expr represented by $2 and to make negative*/
	|	VARIABLE
			{ 
			if(Search($1) == 0){
				printf("ERROR ON LINE %d: VARIABLE HAS NOT BEEN DECLARED\n", lineno); //Added a print error if variable isn't declared
				$$ = 0; //Have the companion value be equal to 0
				}
			else{
				$$ = regs[FetchAddress($1)]; //Store the companion value with the reg's value at the address of the variable
				}
			} 
	|	INTEGER {$$=$1; fprintf(stderr,"found an integer\n");}
	;



%%	/* end of rules, start of program */

/*
*/
int main()
{ yyparse();
} //End of main
