%{
/*
Name: Dominik Trujillo
Date: 09/15/2023
Lab 5 Algol-C into YACC and LEX 
Purpose: The purpose of this lab is to create YACC and LEX routines that will parse the given input
and match that with the defined language definition. The main learning objective was to develop 
a deeper understanding of how to parse inputs and defining rules for the syntax of the returned tokens that are read
from LEX and then translated by YACC to be able to read and determine whether or not a given input is grammatically correct.

Rules:
	Every time T_ID is used it will print out the ID with the specific production rule. 
	Strings will be printed out.
*/


	/* begin specs */
#include <stdio.h>
#include <ctype.h>

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
  printf ("%s on line %d\n", s, lineno);
}


%}
/*  defines the start symbol, what values come back from LEX and how the operators are associated  */

/* Setting start to be at Program */
%start Program

/* creating a union of type int and char pointer for num and string */
%union { int num;
	 char * string; }

/* Creating tokens for T_NUM to be type num, T_ID to be type string, and T_STRING to be type string */	
%token <num> T_NUM
%token <string> T_ID
%token <string> T_STRING
/* Declaring the rest of the tokens that will be used for this program */
%token T_INT T_VOID T_BOOLEAN T_BEGIN T_END T_RETURN T_READ T_WRITE T_LE T_GE T_EQ T_NE T_AND T_OR T_NOT T_IF T_THEN T_ELSE T_ENDIF T_WHILE T_DO T_TRUE T_FALSE

%left '|'
%left '&'
%left '+' '-'
%left '*' '/' '%'
%left UMINUS


%%	/* end specs, begin rules */
/* 1. A Program can be a Declarationlist*/
Program 		: Declarationlist
			;
/* 2. A Declarationlist can be a Declaration or a Declaration followed by a Declarationlist */
Declarationlist 	: Declaration 
			| Declaration Declarationlist
			;
/* 3. A Declaration can be a VarDeclaration or a FunDeclaration */		 
Declaration 		: VarDeclaration
	    		| FunDeclaration 
	    		;
/* 4. A VarDeclaration can be a TypeSpecifier followed by a VarList and a semicolon */	    
VarDeclaration 		: TypeSpecifier VarList ';'
	       		;
/* 4a. A VarList can be a T_ID, a T_ID followed by an open bracket, T_NUM, and close bracket, a T_ID followed by a comma and VarList */
/* a T_ID followed by an open bracket, T_NUM, close bracket, comma, and a VarList */	       
VarList 		: T_ID  {fprintf(stderr,"VARLIST ID IS %s\n", $1); }
			| T_ID '[' T_NUM ']' {fprintf(stderr,"VARLIST ID IS %s\n", $1); }
			| T_ID ',' VarList {fprintf(stderr,"VARLIST ID IS %s\n", $1); }
			| T_ID '[' T_NUM ']' ',' VarList {fprintf(stderr,"VARLIST ID IS %s\n", $1); }
			;
/* 5. A TypeSpecifier can be a T_INT, T_VOID, or a T_BOOLEAN */	
TypeSpecifier   	: T_INT 
			| T_VOID 
			| T_BOOLEAN
			;
/* 6. A FunDeclaration can be a TypeSpecifier followed by a T_ID, open parenthesis, Params, close parenthesis, and a CompoundStmt */		
FunDeclaration 		: TypeSpecifier T_ID '(' Params ')' CompoundStmt {fprintf(stderr,"FUNDEC ID NAME IS %s\n", $2); }
			;
/* 7. A Params can be a T_VOID or a ParamList */		
Params 			: T_VOID 
			| ParamList
			;
			
/* 8. A ParamList can be a Param or a Param followed by a comma and another Param */
ParamList		: Param
			| Param ',' Param
			;
/* 9. A Param can be a TypeSpecifier followed by a T_ID or a TypeSpecifier followed by a T_ID open and close brackets */
Param 			: TypeSpecifier T_ID {fprintf(stderr,"PARAM T_ID value %s\n", $2); }
			| TypeSpecifier T_ID '[' ']' {fprintf(stderr,"PARAM T_ID value %s\n", $2); }
			;
/* 10. A CompoundStmt can be a T_BEGIN followed by LocalDeclarations, StatementList, and T_END */	
CompoundStmt 		: T_BEGIN LocalDeclarations StatementList T_END
			;
/* 11. LocalDeclarations can be either empty or a VarDeclaration followed by a LocalDeclarations */
LocalDeclarations 	: /* empty */
		  	| VarDeclaration LocalDeclarations 
		  	;
/* 12. A StatementList can either be empty or a Statement followed by a StatementList */	
StatementList 		: /* empty */
			| Statement StatementList
			;
/* 13. A Statement can be any of the following listed below. */
Statement 		: ExpressionStmt
			| CompoundStmt
			| SelectionStmt
			| IterationStmt
			| AssignmentStmt
			| ReturnStmt
			| ReadStmt
			| WriteStmt
			;
/* 14. A ExpressionStmt can be an Expression followed by a semicolon or just a semicolon */
ExpressionStmt		: Expression ';'
			| ';'
			;
/* 15. A SelectionStmt can be a sequence of a T_IF followed by an Expression, T_THEN, Statement, and T_ENDIF or */
/* can be sequence of a T_IF followed by an Expression, T_THEN, Statement, T_ELSE, Statement, and T_ENDIF */
SelectionStmt		: T_IF Expression T_THEN Statement T_ENDIF
			| T_IF Expression T_THEN Statement T_ELSE Statement T_ENDIF
			;
/* 16. A IterationStmt can be a sequence of a T_WHILE followed by Expression, T_DO, Statement */
IterationStmt 		: T_WHILE Expression T_DO Statement
			;
/* 17. A ReturnStmt can be a T_RETURN followed by a semicolon or a T_RETURN followed by an Expression and semicolon */	
ReturnStmt 		: T_RETURN ';'
			| T_RETURN Expression ';'
	   		;
/* 18. A ReadStmt can be a T_READ followed by a Var and a semicolon */   		
ReadStmt 		: T_READ Var ';'
			;
/* 19. A WriteStmt can be a sequence of T_WRITE followed by an Expression and a semicolon or a T_WRITE followed by a T_STRING and a semicolon */		
WriteStmt 		: T_WRITE Expression ';'
			| T_WRITE T_STRING ';' 
				{ fprintf(stderr,"write string with value: %s\n", $2); }
			;
/* 20. A AssignmentStmt can be a Var followed by an equals sign, SimpleExpression, and a semicolon */		
AssignmentStmt 		: Var '=' SimpleExpression ';'
			;	
/* 21. An Expression can be a SimpleExpression */		
Expression 		: SimpleExpression
			;
/* 22. A Var can be a T_ID or a T_ID followed by an open bracket, Expression, close bracket */		
Var 			: T_ID {fprintf(stderr,"inside a var with value %s\n", $1); }
			| T_ID '[' Expression ']' {fprintf(stderr,"inside a var array with value %s\n", $1); }
			;	   
/* 23. A SimpleExpression can be an AdditiveExpression or a SimpleExpression followed by a Relop and AdditiveExpression */		
SimpleExpression 	: AdditiveExpression 
			| SimpleExpression Relop AdditiveExpression
			;
/* 24. A Relop can be any of the following tokens below */			
Relop 			: T_LE 
			| '<' 
			| '>' 
			| T_GE 
			| T_EQ 
			| T_NE 
      			; 
/* 25. An AdditiveExpression can be a Term or an AdditiveExpression followed by an Addop and Term */
AdditiveExpression 	: Term 
			| AdditiveExpression Addop Term 
			; 
/* 26. An Addop can be a plus sign or a minus sign */
Addop 			: '+' 
			| '-' 
      			; 
/* 27. A Term can be a Factor or a Term followed by a Multop and Factor */
Term 			: Factor 
     			| Term Multop Factor 
			; 
/* 28. A Multop can be any of the following tokens below */
Multop 			: '*' 
			| '/' 
			| T_AND 
			| T_OR 
			; 
/* 29. A Factor can be an open paranthesis followed by an Expression and close paranthesis or any of the tokens or productions below */	
Factor			: '(' Expression ')'
			| T_NUM
			| Var
			| Call
			| T_TRUE
			| T_FALSE
			| T_NOT Factor
			;
/* 30. A Call can be a T_ID followed by an open paranthesis, Args, and close paranthesis */			
Call 			: T_ID '(' Args ')' {fprintf(stderr,"CALL ID NAME IS %s\n", $1); }
			;
/* 31. An Args can be empty or an ArgList */			
Args			: /* empty */
			| ArgList
			;
	
/* 32. An ArgList can be an Expression or an Expression followed by a comma and ArgList */		
ArgList 		: Expression 
			| Expression ',' ArgList
			;
			
	   
%%	/* end of rules, start of program */

/*
Preconditions: Parser has been properly initialized and configured with proper tokens.
Postconditions: yyparse is called and main exits.
*/
int main()
{ yyparse();
} //End of main
