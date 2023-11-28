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
	Lab 2_2 YACC and LEX 
	Name: Dominik Trujillo
	Date: 08/25/2023
	Purpose: The purpose of this lab, specifially this YACC file, is to take the values that were returned by the LEX portion
	and to determine which of the following operations will be applied. Starting from the bottom to top, we will check the expression and do that operation.

	Log: Changes that were made was adding in operations to have a working '*' or multiplication operation working by taking an expression with a '*' and ending with another expression
	to do multiplication with the given values.
	Also fixed the unary minus by taking in an expression that is started with a '-' character and taking that value and making it negative.
*/


	/* begin specs */
#include <stdio.h>
#include <ctype.h>


int regs[26];
int base, debugsw;

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

%start list

%token INTEGER
%token  VARIABLE

%left '|'
%left '&'
%left '+' '-'
%left '*' '/' '%'
%left UMINUS


%%	/* end specs, begin rules */

list	:	/* empty */
	|	list stat '\n'
	|	list error '\n'
			{ yyerrok; }
	;

stat	:	expr
			{ fprintf(stderr,"the anwser is%d\n", $1); }
	|	VARIABLE '=' expr
			{ regs[$1] = $3; }
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
			{ $$ = regs[$1]; fprintf(stderr,"found a variable value =%d\n",$1); }
	|	INTEGER {$$=$1; fprintf(stderr,"found an integer\n");}
	;



%%	/* end of rules, start of program */

/*
*/
int main()
{ yyparse();
} //End of main
