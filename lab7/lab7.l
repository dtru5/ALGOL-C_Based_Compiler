/*
    Name: Dominik Trujillo
    Date: 09/26/2023
    Lab: LAB 6 ALGOL Abstract Syntax Tree
    Purpose: The purpose of this lab is to enhance our existing parsing and syntax-checking capabilities by extending 
    the functionality of the YACC program. Building upon the knowledge gained in previous
    labs, the primary objective is to create an Abstract Syntax Tree (AST), which serves as an Intermediate Representation 
    (IR) data structure for the parsed input program. This AST will facilitate the execution of multiple passes over the 
    source code that is given.

    To achieve this, our primary task is to modify the YACC program so that it constructs 
    the AST during the shift/reduce processes. This involves adding semantic actions to each production rule, 
    enabling the creation of AST nodes, linking these nodes to represent the program's structure, and ensuring 
    that the relevant information is attached to the yylval companion stack. These AST nodes will be of different 
    types, providing us with valuable insights into the program's structure.

    Upon successful parsing and construction of the AST using the YACC program, the ultimate objective is to 
    have a main() program that prints out the AST. This printout should be designed to reflect the structure of the 
    input program, similar to the example provided in the lab instructions.

    Key tasks for this lab include:

    -Creating (in this case we'll be using and updating the given via the canvas page) a separate "ast.c" and "ast.h" file 
    to house the Abstract Syntax Tree code.

    Adding semantic actions to each production rule in the ALGOL-C submission from the previous 
    lab to ensure AST construction.

    Developing an AST printing routine to assist in debugging the semantic actions.

    Using "AST()" directives as presented in class to guide the development of the Abstract Syntax Tree.

    Documenting all major differences introduced in this lab compared to previous submissions, especially 
    any changes to production rules.

    Preparing to explain and discuss the code during potential in-person assessments.

    Ensuring that the YACC code remains consistent with the LAB 5 submission, without altering 
    non-terminal and terminal names.

    Limiting the use of pointers in the AST to "s1" and "s2" as pointers to other AST nodes, without 
    introducing additional pointers or alternative names.

    Adhering to the naming conventions, such as starting AST enumerated types with "A_" and token names from 
    LEX with "T_" prefixes, to avoid deductions.

    By successfully completing these tasks, we aim to extend our compiler construction capabilities, enabling the 
    creation and utilization of an Abstract Syntax Tree for improved program analysis and code generation. 
    This lab represents a crucial step in achieving our ultimate goal of generating assembly code and running it on a simulator.
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


