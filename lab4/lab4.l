/*
	LAB4 -- Symbol Table with YACC 
	Name: Dominik Trujillo
	Date: 09/14/2023
	Purpose: The purpose of this lab in relation to this LEX file is to incorporate the symbol table with YACC and the use of this LEX file is to 
	have the ability to return the proper tokens for YACC to use.
	
	--------------------Changes made during lab4-------------------------
	
	Log:    Added an 'int' keyword that will return a T_INT 
		Updated the input [a-zA-z][a-zA-Z0-9_]* and updated yylval.string = strdup(yytext);
			So we can have the companion value yylval's string value will be assigned the duplicate of yytext.
		Updated the input [0-9][0-9]* and updated yylval.num = atoi((const char*) yytext); 
		Updated the input [=\-+*'/%&|();] *Note the added ' before the / is so that the comment block doesn't end,4
			but added a semicolon to account for a variable declaration
		Added an int variable called lineno that will keep track of line number for error messages produced in YACC.
		Turned off debug (set to 0) so that small comments such as 'Digit' found wouldn't be produced in the terminal.
		
*/
%{

int mydebug=0;
int lineno = 0;
#include "y.tab.h"
%}


%%

"int"				{return (T_INT);}

[a-zA-Z][a-zA-Z0-9_]*		{if (mydebug) fprintf(stderr,"ID found %s\n", yytext); 
                      		 yylval.string=strdup(yytext); return(VARIABLE);}
                      		 
[0-9][0-9]*			{if (mydebug) fprintf(stderr,"Digit found\n"); 
                       		yylval.num=atoi((const char *)yytext); return(INTEGER);}
                       		
[ \t]				{if (mydebug) fprintf(stderr,"Whitespace found\n");}

[=\-+*/%&|();] 			{ if (mydebug) fprintf(stderr,"return a token %c\n",*yytext); // Added the '(' ')' characters so that it will capture those symbols for operations
                       		return (*yytext);}
                       		
\n 				{ 
				lineno++; //Increment lineno when seeing a new line
				if (mydebug) fprintf(stderr,"cariage return %c\n",*yytext); 
	                       		return (*yytext);}

%%

/*
This is the implementation of the yywrap function which is called when LEX reaches the end of input. It returns 1, indicating that there is no more input to process.
Pre-condition: N/A
Post-condition: yywrap returns 1
*/
int yywrap(void)
{ return 1;}


