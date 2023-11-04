/*
    Name: Dominik Trujillo
    Date: 10/29/2023
    Lab: LAB 7 ALGOL -- add symbol table and type checking
    Purpose: The objective of this lab is to enhance a compiler's functionality by incorporating a symbol table, semantic actions, 
	and rudimentary type checking. The key goals and requirements of this lab include:

	Symbol Table Management: To facilitate proper scope management and variable tracking, the compiler will maintain a static 
	scope counter referred to as "level." When entering a compound statement, the level is incremented, and upon exiting, it is
	decremented. During the exit, all symbols defined at that level are removed from the symbol table. Offsets are adjusted 
	accordingly to account for the memory allocated to the removed symbols. New variables must be inserted into the symbol table 
	when declared, including information about their level and size. The offset for each variable is assigned based on its type, 
	with scalars assigned 1, arrays their respective sizes, and functions using registers for return values instead of memory.

	Type Checking: A fundamental type checking mechanism is implemented to prevent illegal operations, such as combining INT and 
	VOID types in expressions. Variables must be used in the correct context, distinguishing between scalars, arrays, and functions. 
	Type compatibility is enforced for assignment statements and expressions to ensure the integrity of the generated code.

	Symbol Table Requirements: The YACC file is modified to incorporate the symbol table, enabling the compiler to manage variable
	declarations and usages efficiently. Upon the completion of a compound statement, all variables defined within that context are 
	displayed, and subsequently, they are removed from the symbol table. The verification process ensures that variable names are both
	defined and used correctly. Type inheritance is implemented for expressions. Furthermore, the symbol table maintains the level for
	each entry, with level 0 reserved for global variables and functions, level 1 for function parameters and the primary compound 
	statement of the function. The level is incremented as nested compound statements are encountered and decremented when exiting these nested scopes.

	In summary, this lab's primary objective is to equip the compiler with the capability to manage
	symbol tables effectively, perform rudimentary type checking, and handle scope management. These enhancements aim to 
	improve the compiler's code generation process, making it more robust and reliable.
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


