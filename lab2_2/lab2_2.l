/*                 Small LEX routine which returns two formal tokens (INTEGER and VARIABLE)
                   along with single string elements like '+'.
 
                    This LEX definition is the companion to the docalc.y YACC routine which
                   is a simple calculator

                    Shaun Cooper
                    January 2015
*/

/*
	Lab 2_2 YACC and LEX 
	Name: Dominik Trujillo
	Date: 08/25/2023
	Purpose: This lab is to understand how we can connect LEX and YACC to develop a simple working calculator by reading in tokens from LEX
	and returning the value to YACC by returning VARIABLE if a character is found, INTEGER when an integer is detected, the token and it's corresponding symbol.

	Log: In lab2_2, the changes that were made was including the symbols '(' ')' to braces that contain the mathematical operations
*/
%{

int mydebug=1;
#include "y.tab.h"
%}


%%

[a-z]		{if (mydebug) fprintf(stderr,"Letter found\n"); 
                       yylval=*yytext-'a'; return(VARIABLE);}
[0-9][0-9]*	{if (mydebug) fprintf(stderr,"Digit found\n"); 
                       yylval=atoi((const char *)yytext); return(INTEGER);}
[ \t]		{if (mydebug) fprintf(stderr,"Whitespace found\n");}
[=\-+*/%&|()] 	{ if (mydebug) fprintf(stderr,"return a token %c\n",*yytext); // Added the '(' ')' characters so that it will capture those symbols for operations
                       return (*yytext);}
\n 		{ if (mydebug) fprintf(stderr,"cariage return %c\n",*yytext); 
                       return (*yytext);}

%%

/*
This is the implementation of the yywrap function which is called when LEX reaches the end of input. It returns 1, indicating that there is no more input to process.
Pre-condition: N/A
Post-condition: yywrap returns 1
*/
int yywrap(void)
{ return 1;}


