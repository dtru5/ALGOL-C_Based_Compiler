/*
Name: Dominik Trujillo
Date: 11/4/2023
LAB9 ALGOL Create MIPS code from you AST
Purpose: The purpose of this lab is to implement a MIPS code generator for ALGOL structures using an Abstract Syntax Tree (AST).
The primary objectives include creating MIPS code for ALGOL constructs with jumps, labels, and MIPS directives, 
incrementally developing MIPS code for the ALGOL language, and gaining an understanding of activation records. 
The lab emphasizes updating the main program to accept command-line arguments, copying the AST printing routine to 
create an "emit" section for generating MIPS code, and implementing functions to handle standard header elements for 
MIPS to define arrays, scalars, and strings. Additional goals involve creating simplified code for basic functionalities
such as printing "hello world," handling integers, and managing variable input/output. The lab also focuses on updating 
helper functions for evaluating expressions, making assignments work correctly, enhancing the system to handle If/while 
constructs, and extending support for arrays The complexity increases gradually, allowing students
to tackle one functional unit at a time. The lab encourages documentation for each function and major construct in the 
emit.c file to facilitate understanding and evaluation. Overall, the lab aims to provide students with hands-on experience in 
MIPS code generation while reinforcing their understanding of ALGOL language construct; and ultimately, this lab aims to 
demonstrate our complete works from LEX, YACC, using ASTs, and emiting the corresponding MIPS code to have a relatively completed 
compiler.
*/

%{

int mydebug=0;
int lineno = 1;
#include "ast.h"
#include "y.tab.h"
%}


%%

int				{return (T_INT);} //If input is int, return token T_INT
void				{return (T_VOID);} //If input is void, return token T_VOID
boolean				{return (T_BOOLEAN);} //If input is boolean, return token T_BOOLEAN
begin 				{return(T_BEGIN);} //If input is begin, return token T_BEGIN
end   				{return(T_END);} //If input is end, return token T_END
write				{return(T_WRITE);} //If input is write, return token T_WRITE
return 				{return(T_RETURN);} //If input is return, return token T_RETURN
"<=" 				{return(T_LE);}  //If input is <=, return token T_LE for less than or equal
">="				{return(T_GE);} //If input is >=, return token T_GE for greater than or equal
"=="				{return(T_EQ);} //If input is ==, return token T_EQ for equals conditional
"!="				{return(T_NE);} //If input is !=, return token T_NE for not equals
and				{return(T_AND);} //If input is and, return token T_AND
or				{return(T_OR);} //If input is or, return token T_OR
not				{return(T_NOT);} //If input is not, return token T_NOT
if				{return(T_IF);} //If input is if, return token T_OR
else				{return(T_ELSE);} //If input is else, return token T_ELSE
read				{return(T_READ);} //If input is read, return token T_READ
then				{return(T_THEN);} //If input is then, return token T_THEN
endif				{return(T_ENDIF);} //If input is endif, return token T_ENDIF
while				{return(T_WHILE);} //If input is while, return token T_WHILE
do				{return(T_DO);} //If input is do, return token T_DO
false				{return(T_FALSE);} //If input is false, return T_FALSE
true				{return(T_TRUE);} //If input is true, return T_TRUE
\".*\"				{yylval.string = strdup(yytext); return(T_STRING);} //This will capture a string in two paranthesis and anything inbetween to return a T_STRING

[a-zA-Z][a-zA-Z0-9_]*		{if (mydebug) fprintf(stderr,"ID foundz %s\n", yytext); //For any ID's that is a letter from a-z 
				//and A-Z that can be followed a-z, A-Z, 0-9, or an underscore 
                      		 	yylval.string=strdup(yytext); return(T_ID);} //Duplicate the string from yytext and insert for companion value yylval
                      		 
[0-9][0-9]*			{if (mydebug) fprintf(stderr,"Digit found\n"); 
                       			yylval.num=atoi((const char *)yytext); return(T_NUM);} //If digit, change yytext to an int and store in yylval
                       		
[ \t]				{if (mydebug) fprintf(stderr,"Whitespace found\n");}

[/<>=\-+*%&|(),\[\]] 		{if (mydebug) fprintf(stderr,"return a token %c\n",*yytext);
                       			return (*yytext);}
                       		
[;] 			        {if (mydebug) fprintf(stderr,"return a semicolon %c\n",*yytext); //Seperate read for a semicolon that will return yytext
                       			return (*yytext);} 
                       			
"//".*				{} //Do nothing when encountering a comment that is two forward slashes                   		
                       		
\n 				{ 
				lineno++; //Increment lineno when seeing a new line
				if (mydebug) fprintf(stderr,"cariage return %c\n",*yytext); 
	                        }

%%

/*
This is the implementation of the yywrap function which is called when LEX reaches the end of input. It returns 1, indicating that there is no more input to process.
Pre-condition: N/A
Post-condition: yywrap returns 1
*/
int yywrap(void)
{ return 1;}


