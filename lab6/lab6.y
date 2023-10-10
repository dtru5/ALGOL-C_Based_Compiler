%{
/*
Name: Dominik Trujillo
Date: 10/01/2023
LAB 6 ALGOL Abstract Syntax Tree
Purpose: The purpose of this lab is to 
*/


	/* begin specs */
#include <stdio.h>
#include <ctype.h>
#include "ast.h"

int yylex();
#define MAX 26
int regs[MAX];
int base, debugsw;
int oneup = 0;
extern int lineno;
extern ASTnode * program;

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
	 char * string; 
	 ASTnode * node; //Added ASTNode pointer called node into the union.
	 enum DataTypes datatype; //Added DataTypes into the union.
	 }

/* Creating tokens for T_NUM to be type num, T_ID to be type string, and T_STRING to be type string */	
%token <num> T_NUM
%token <string> T_ID
%token <string> T_STRING
/* Declaring the rest of the tokens that will be used for this program */
%token T_INT T_VOID T_BOOLEAN T_BEGIN T_END T_RETURN T_READ T_WRITE T_LE T_GE T_EQ T_NE T_AND T_OR T_NOT T_IF T_THEN T_ELSE T_ENDIF T_WHILE T_DO T_TRUE T_FALSE

/*Making Declaration be type node*/
%type <node> Declaration Declarationlist VarDeclaration VarList FunDeclaration CompoundStmt LocalDeclarations
%type <node> StatementList Statement
%type <datatype> TypeSpecifier

%left '|'
%left '&'
%left '+' '-'
%left '*' '/' '%'
%left UMINUS


%%	/* end specs, begin rules */
/* 1. A Program can be a Declarationlist*/
Program             : Declarationlist
                    {
						program = $1;
					}
					;
/* 2. A Declarationlist can be a Declaration or a Declaration followed by a Declarationlist */
Declarationlist 	: Declaration 
					{
						$$ = ASTCreateNode(A_DEC_LIST); //Set the companion value to be a node with type A_DEC_LIST
						$$->s1 = $1; //Set s1 of the node to be the Declaration value
					}
					| Declaration Declarationlist 
					{
						$$ = ASTCreateNode(A_DEC_LIST); //Set the companion value to be a node with type A_DEC_LIST;
						$$->s1 = $1; //Set the s1 branch to be a Declaration
						$$->s2 = $2; //Set the s2 branch to be a Declarationlist
					}
					;
/* 3. A Declaration can be a VarDeclaration or a FunDeclaration */		 
Declaration 		: VarDeclaration {$$ = $1;}
	    			| FunDeclaration {$$ = $1;}
	    			;
/* 4. A VarDeclaration can be a TypeSpecifier followed by a VarList and a semicolon */	    
VarDeclaration 		: TypeSpecifier VarList ';'
 					{
						$$ = $2;
						ASTnode *p;
						p = $2;
						while(p != NULL){
							p->datatype = $1;
							p = p->s1;
							}
					}
	       			;
/* 4a. A VarList can be a T_ID, a T_ID followed by an open bracket, T_NUM, and close bracket, a T_ID followed by a comma and VarList */
/* a T_ID followed by an open bracket, T_NUM, close bracket, comma, and a VarList */	       
VarList 			: T_ID  
						{
						$$ = ASTCreateNode(A_VARDEC); //Create a new A_VARDEC node
						$$->name = $1; //Set the name with the given ID
						}
					| T_ID '[' T_NUM ']' 
						{
						$$ = ASTCreateNode(A_VARDEC); //Create a new A_VARDEC node
						$$->name = $1; //Set the name to be the ID
						$$->value = $3; //Set the value with the given NUM
						} 
					| T_ID ',' VarList 
						{
						$$ = ASTCreateNode(A_VARDEC); //Create a new A_VARDEC node
						$$->name = $1; //Set the name with ID
						$$->s1 = $3; //Set the s1 branch to be another Varlist
						} 
					| T_ID '[' T_NUM ']' ',' VarList 
						{
						$$ = ASTCreateNode(A_VARDEC); //Create a new A_VARDEC node
						$$->name = $1; //Set the name with the ID
						$$->value = $3; //Set the value with the NUM
						$$->s1 = $6; //Set the s1 branch to be another Varlist.
						}
						;
/* 5. A TypeSpecifier can be a T_INT, T_VOID, or a T_BOOLEAN */	
TypeSpecifier   	: T_INT {$$ = A_INTTYPE;}
					| T_VOID {$$ = A_VOIDTYPE;}
					| T_BOOLEAN {$$ = A_BOOLEANTYPE;}
					;
/* 6. A FunDeclaration can be a TypeSpecifier followed by a T_ID, open parenthesis, Params, close parenthesis, and a CompoundStmt */		
FunDeclaration 		: TypeSpecifier T_ID '(' Params ')' CompoundStmt 
						{
						$$ = ASTCreateNode(A_FUNDEC);
						$$->name = $2; //Setting the name with the ID
						$$->datatype = $1; //Setting the datatype with the given TypeSpecifier
						$$->s1 = NULL; //FIX ME Setting the s1 branch to be the Params
						$$->s2 = $6; //Setting s2 branch to be the Compound statement
						}
					;
/* 7. A Params can be a T_VOID or a ParamList */		
Params 				: T_VOID 
					| ParamList
					;
			
/* 8. A ParamList can be a Param or a Param followed by a comma and another Param */
ParamList			: Param
					| Param ',' Param
					;
/* 9. A Param can be a TypeSpecifier followed by a T_ID or a TypeSpecifier followed by a T_ID open and close brackets */
Param 				: TypeSpecifier T_ID {fprintf(stderr,"PARAM T_ID value %s\n", $2); }
					| TypeSpecifier T_ID '[' ']' {fprintf(stderr,"PARAM T_ID value %s\n", $2); }
					;
/* 10. A CompoundStmt can be a T_BEGIN followed by LocalDeclarations, StatementList, and T_END */	
CompoundStmt 		: T_BEGIN LocalDeclarations StatementList T_END
					{
						$$ = ASTCreateNode(A_COMPOUND);
						$$->s1 = $2;
						$$->s2 = $3;
					}
					;
/* 11. LocalDeclarations can be either empty or a VarDeclaration followed by a LocalDeclarations */
LocalDeclarations 	: /* empty */
					{
						$$ = NULL;
					}
		  			| VarDeclaration LocalDeclarations 
					{
						$$ = $1;
						$$->s2 = $2;
					}
		  			;
/* 12. A StatementList can either be empty or a Statement followed by a StatementList */	
StatementList 		: /* empty */
					{
						$$ = ASTCreateNode(A_STATEMENTLIST);
					}
					| Statement StatementList
					{
						$$ = ASTCreateNode(A_STATEMENTLIST);
						$$->s1 = $1;
						$$->s2 = $2;
					}
					;
/* 13. A Statement can be any of the following listed below. */
Statement 			: ExpressionStmt{$$ = NULL;}
					| CompoundStmt{$$ = NULL;}
					| SelectionStmt{$$ = NULL;}
					| IterationStmt{$$ = NULL;}
					| AssignmentStmt{$$ = NULL;}
					| ReturnStmt{$$ = NULL;}
					| ReadStmt{$$ = NULL;}
					| WriteStmt{$$ = NULL;}
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
ReturnStmt 			: T_RETURN ';'
					| T_RETURN Expression ';'
	   				;
/* 18. A ReadStmt can be a T_READ followed by a Var and a semicolon */   		
ReadStmt 			: T_READ Var ';'
					;
/* 19. A WriteStmt can be a sequence of T_WRITE followed by an Expression and a semicolon or a T_WRITE followed by a T_STRING and a semicolon */		
WriteStmt 			: T_WRITE Expression ';'
					| T_WRITE T_STRING ';' 
					{ fprintf(stderr,"write string with value: %s\n", $2); }
					;
/* 20. A AssignmentStmt can be a Var followed by an equals sign, SimpleExpression, and a semicolon */		
AssignmentStmt 		: Var '=' SimpleExpression ';'
					;	
/* 21. An Expression can be a SimpleExpression */		
Expression 			: SimpleExpression
					;
/* 22. A Var can be a T_ID or a T_ID followed by an open bracket, Expression, close bracket */		
Var 				: T_ID {fprintf(stderr,"inside a var with value %s\n", $1); }
					| T_ID '[' Expression ']' {fprintf(stderr,"inside a var array with value %s\n", $1); }
					;	   
/* 23. A SimpleExpression can be an AdditiveExpression or a SimpleExpression followed by a Relop and AdditiveExpression */		
SimpleExpression 	: AdditiveExpression 
					| SimpleExpression Relop AdditiveExpression
					;
/* 24. A Relop can be any of the following tokens below */			
Relop 				: T_LE 
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
Addop 				: '+' 
					| '-' 
      				; 
/* 27. A Term can be a Factor or a Term followed by a Multop and Factor */
Term 				: Factor 
     				| Term Multop Factor 
					; 
/* 28. A Multop can be any of the following tokens below */
Multop 				: '*' 
					| '/' 
					| T_AND 
					| T_OR 
					; 
/* 29. A Factor can be an open paranthesis followed by an Expression and close paranthesis or any of the tokens or productions below */	
Factor				: '(' Expression ')'
					| T_NUM
					| Var
					| Call
					| T_TRUE
					| T_FALSE
					| T_NOT Factor
					;
/* 30. A Call can be a T_ID followed by an open paranthesis, Args, and close paranthesis */			
Call 				: T_ID '(' Args ')' {fprintf(stderr,"CALL ID NAME IS %s\n", $1); }
					;
/* 31. An Args can be empty or an ArgList */			
Args				: /* empty */
					| ArgList
					;
	
/* 32. An ArgList can be an Expression or an Expression followed by a comma and ArgList */		
ArgList 			: Expression 
					| Expression ',' ArgList
					;
			
	   
%%	/* end of rules, start of program */

/*
Preconditions: Parser has been properly initialized and configured with proper tokens.
Postconditions: yyparse is called and main exits.
*/
int main(){ 
	yyparse();
	ASTprint(0, program);	
} //End of main
