%{
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


	/* begin specs */
#include <stdio.h>
#include <ctype.h>
#include "ast.h"
#include "symtable.h"

int yylex();

int LEVEL = 0; //How many compound statements deep we're in.
int OFFSET = 0; //How many words have we seen at GLOBAL or inside a function.
int GOFFSET; //Holder for global offset when we enter and exit a fucntion definition.
int maxoffset; //Total number of words a function needs

extern int lineno; //Global variable for line number counter.
extern ASTnode * program; //Brought over program from ast.c

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
	 enum OPERATORS operator; //added operators to the union
	 }

/* Creating tokens for T_NUM to be type num, T_ID to be type string, and T_STRING to be type string */	
%token <num> T_NUM
%token <string> T_ID
%token <string> T_STRING
/* Declaring the rest of the tokens that will be used for this program */
%token T_INT T_VOID T_BOOLEAN T_BEGIN T_END T_RETURN T_READ T_WRITE T_LE T_GE T_EQ T_NE T_AND T_OR T_NOT T_IF T_THEN T_ELSE T_ENDIF T_WHILE T_DO T_TRUE T_FALSE

/*Making most non terminals to be type node*/
%type <node> Declaration Declarationlist VarDeclaration VarList FunDeclaration CompoundStmt LocalDeclarations AssignmentStmt
%type <node> StatementList Statement WriteStmt Expression SimpleExpression AdditiveExpression Term Factor ReadStmt Var SelectionStmt
%type <node> ExpressionStmt IterationStmt Call ArgList Args Param Params ParamList ReturnStmt
%type <datatype> TypeSpecifier
%type <operator> Addop Relop Multop

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
							p->symbol->Declared_Type = $1;
							p = p->s1;
							}
					}
	       			;
/* 4a. A VarList can be a T_ID, a T_ID followed by an open bracket, T_NUM, and close bracket, a T_ID followed by a comma and VarList */
/* a T_ID followed by an open bracket, T_NUM, close bracket, comma, and a VarList */	       
VarList 			: T_ID  
						{
							if (Search($1, LEVEL, 0) == NULL){
								//Symbol not there, stick it in
								$$ = ASTCreateNode(A_VARDEC); //Create a new A_VARDEC node
								$$->name = $1; //Set the name with the given ID
								//The prototype for insert for reference.
								//Insert(char *name, enum DataTypes my_assigned_type, enum  SYMBOL_SUBTYPE subtype, int  level, int mysize, int offset )
								//Set p's symbol to be the node that is returned after inserting it into the symtable
								$$->symbol = Insert($1, A_UNKNOWN, SYM_SCALAR, LEVEL, 1, OFFSET);
								OFFSET++;
							}
							else{
								yyerror($1);
								yyerror("Already defined");
								exit(1);
							}

						}
					| T_ID '[' T_NUM ']' 
						{												
							if (Search($1, LEVEL, 0) == NULL){
								$$ = ASTCreateNode(A_VARDEC); //Create a new A_VARDEC node
								$$->name = $1; //Set the name to be the ID
								$$->value = $3; //Set the value with the given NUM
								//Insert(char *name, enum DataTypes my_assigned_type, enum  SYMBOL_SUBTYPE subtype, int  level, int mysize, int offset )
								$$->symbol = Insert($1, A_UNKNOWN, SYM_ARRAY, LEVEL, $3, OFFSET);
								OFFSET += $3;
							}
							else{
								yyerror($1);
								yyerror("Already defined");
								exit(1);
							}
						} 
					| T_ID ',' VarList 
						{
							if (Search($1, LEVEL, 0) == NULL){
								$$ = ASTCreateNode(A_VARDEC); //Create a new A_VARDEC node
								$$->name = $1; //Set the name with ID
								$$->s1 = $3; //Set the s1 branch to be another Varlist

								//The prototype for insert for reference.
								//Insert(char *name, enum DataTypes my_assigned_type, enum  SYMBOL_SUBTYPE subtype, int  level, int mysize, int offset )
								
								//Set p's symbol to be the node that is returned after inserting it into the symtable
								$$->symbol = Insert($1, A_UNKNOWN, SYM_SCALAR, LEVEL, 1, OFFSET);
								OFFSET++; 
							}
							else{
								yyerror($1);
								yyerror("Already defined");
								exit(1);
							}
						} 
					| T_ID '[' T_NUM ']' ',' VarList 
						{
							if (Search($1, LEVEL, 0) == NULL){
								$$ = ASTCreateNode(A_VARDEC); //Create a new A_VARDEC node
								$$->name = $1; //Set the name with the ID
								$$->value = $3; //Set the value with the NUM
								$$->s1 = $6; //Set the s1 branch to be another Varlist.

								//The prototype for insert for reference.
								//Insert(char *name, enum DataTypes my_assigned_type, enum  SYMBOL_SUBTYPE subtype, int  level, int mysize, int offset )
								
								//Set p's symbol to be the node that is returned after inserting it into the symtable
								$$->symbol = Insert($1, A_UNKNOWN, SYM_ARRAY, LEVEL, $3, OFFSET);
								OFFSET += $3;
							}
							else{
								yyerror($1);
								yyerror("Already defined");
								exit(1);
							}
						}
						;
/* 5. A TypeSpecifier can be a T_INT, T_VOID, or a T_BOOLEAN */	
TypeSpecifier   	: T_INT {$$ = A_INTTYPE;}
					| T_VOID {$$ = A_VOIDTYPE;}
					| T_BOOLEAN {$$ = A_BOOLEANTYPE;}
					;
/* 6. A FunDeclaration can be a TypeSpecifier followed by a T_ID, open parenthesis, Params, close parenthesis, and a CompoundStmt */		
FunDeclaration 		: TypeSpecifier T_ID '('
						{
							//Check to see if function name is known, if it is barf.
							//else install it in the symbol table.
							//manage offset value.
							if(Search($2,LEVEL, 0) == NULL){
								//insert
								Insert($2, $1, SYM_FUNCTION, LEVEL, 0, 0);
								GOFFSET = OFFSET;
								OFFSET = 2;
								maxoffset = OFFSET;
							}
							else{
								//BARF
								yyerror($2);
								yyerror("Cannot create function, name is use");
								exit(1);
							}
						}
						Params ')'
						{
							//Update symtable with parameter
							//Allow us to have recursive functions.
							Search($2, LEVEL, 0)->fparms = $5;
						} 
						CompoundStmt 
						{
							$$ = ASTCreateNode(A_FUNDEC);
							$$->name = $2; //Setting the name with the ID
							$$->datatype = $1; //Setting the datatype with the given TypeSpecifier
							$$->s1 = $5; //Setting the s1 branch to be the Params
							$$->s2 = $8; //Setting s2 branch to be the Compound statement
							$$->symbol = Search($2, LEVEL, 0);
							$$->symbol->offset = maxoffset;
						}
					;
/* 7. A Params can be a T_VOID or a ParamList */		
Params 				: T_VOID 
					{
						$$ = ASTCreateNode(A_PARAMS);
						$$->datatype = T_VOID;
						
					}
					| ParamList 
					{
						$$ = $1;
					}
					;
			
/* 8. A ParamList can be a Param or a Param followed by a comma and another Param */
ParamList			: Param 
					{
						$$ = $1;
					}
					| Param ',' ParamList
					{
						$$ = $1;
						$$->s1 = $3;
					}
					;
/* 9. A Param can be a TypeSpecifier followed by a T_ID or a TypeSpecifier followed by a T_ID open and close brackets */
Param 				: TypeSpecifier T_ID 
					{
						if(Search($2, 1, 0) == NULL){
							$$ = ASTCreateNode(A_PARAM);
							$$->name = $2;
							$$->symbol = Insert($2, $1, SYM_SCALAR, LEVEL+1, 1, OFFSET);
							$$->datatype = $$->symbol->Declared_Type;
							OFFSET++;
						}
						else{
							yyerror($2);
							yyerror("Parameter name already used");
							exit(1);
						}
					}
					| TypeSpecifier T_ID '[' ']' 
					{
						if(Search($2, 1, 0) == NULL){
							$$ = ASTCreateNode(A_PARAM);
							$$->name = $2;
							$$->value = -1;
							$$->symbol = Insert($2, $1, SYM_ARRAY, LEVEL+1, 1, OFFSET);
							$$->datatype = $$->symbol->Declared_Type;
							OFFSET++;
						}
						else{
							yyerror($2);
							yyerror("Parameter name already used");
							exit(1);
						}
					}
					;
/* 10. A CompoundStmt can be a T_BEGIN followed by LocalDeclarations, StatementList, and T_END */	
CompoundStmt 		: T_BEGIN {LEVEL++;} LocalDeclarations StatementList T_END
					{
						$$ = ASTCreateNode(A_COMPOUND);
						$$->s1 = $3;
						$$->s2 = $4;
						if(OFFSET > maxoffset){
							maxoffset = OFFSET;
						}
						OFFSET -= Delete(LEVEL);
						LEVEL--;
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
Statement 			: ExpressionStmt{$$ = $1;}
					| CompoundStmt{$$ = $1;}
					| SelectionStmt{$$ = $1;}
					| IterationStmt{$$ = $1;}
					| AssignmentStmt{$$ = $1;}
					| ReturnStmt{$$ = $1;}
					| ReadStmt{$$ = $1;}
					| WriteStmt{$$ = $1;}
					;
/* 14. A ExpressionStmt can be an Expression followed by a semicolon or just a semicolon */
ExpressionStmt		: Expression ';'
					{
						$$ = ASTCreateNode(A_EXPRESSIONSTMT);
						$$->s1 = $1;
					}
					| ';'
					{
						$$ = ASTCreateNode(A_EXPRESSIONSTMT);
					}
					;
/* 15. A SelectionStmt can be a sequence of a T_IF followed by an Expression, T_THEN, Statement, and T_ENDIF or */
/* can be sequence of a T_IF followed by an Expression, T_THEN, Statement, T_ELSE, Statement, and T_ENDIF */
SelectionStmt		: T_IF Expression T_THEN Statement T_ENDIF
					{
						$$ = ASTCreateNode(A_IF);
						$$->s1 = $2; //Set s1 branch of Selection to be the Expression
						ASTnode *p = ASTCreateNode(A_IF); //Create a new node A_IF that will hold the if statements
						p->s1 = $4; //Set the A_IF s1 branch with the statement, other will be NULL since it's not an else
						$$->s2=p; //Set Selection statement's s2 to be the A_IF we created.
					}
					| T_IF Expression T_THEN Statement T_ELSE Statement T_ENDIF
					{
						$$ = ASTCreateNode(A_IF);
						$$->s1 = $2; //Set s1 branch of Selection to be the Expression
						ASTnode *p = ASTCreateNode(A_IF); //Create a new node A_IF that will hold the if statements
						p->s1 = $4; //Set the A_IF s1 branch with the then statement
						p->s2 = $6; //Set the A_IF s2 branch with the else statment
						$$->s2=p; //Set Selection statement's s2 to be the A_IF we created.

					}
					;
/* 16. A IterationStmt can be a sequence of a T_WHILE followed by Expression, T_DO, Statement */
IterationStmt 		: T_WHILE Expression T_DO Statement
					{
						$$ = ASTCreateNode(A_ITERATIONSTMT);
						$$->s1 = $2;
						$$->s2 = $4;
					}
					;
/* 17. A ReturnStmt can be a T_RETURN followed by a semicolon or a T_RETURN followed by an Expression and semicolon */	
ReturnStmt 			: T_RETURN ';'
					{
						$$ = ASTCreateNode(A_RETURNSTMT);
					}
					| T_RETURN Expression ';'
					{
						$$ = ASTCreateNode(A_RETURNSTMT);
						$$->s1 = $2;
					}
	   				;
/* 18. A ReadStmt can be a T_READ followed by a Var and a semicolon */   		
ReadStmt 			: T_READ Var ';'
					{
						$$ = ASTCreateNode(A_READ);
						$$->s1 = $2;
					}
					;
/* 19. A WriteStmt can be a sequence of T_WRITE followed by an Expression and a semicolon or a T_WRITE followed by a T_STRING and a semicolon */		
WriteStmt 			: T_WRITE Expression ';'
					{
						$$ = ASTCreateNode(A_WRITE);
						$$->s1 = $2;
					}
					| T_WRITE T_STRING ';' 
					{
						$$ = ASTCreateNode(A_WRITE);
						$$->name = $2;
					}
					;
/* 20. A AssignmentStmt can be a Var followed by an equals sign, SimpleExpression, and a semicolon */		
AssignmentStmt 		: Var '=' SimpleExpression ';'
					{
						if($1->datatype != $3->datatype){
							yyerror("Assignment type mismatch");
							exit(1);
						}
						$$ = ASTCreateNode(A_ASSIGNMENTSTMT);
						$$->s1 = $1;
						$$->s2 = $3;
					}
					;	
/* 21. An Expression can be a SimpleExpression */		
Expression 			: SimpleExpression {$$ = $1;}
					;
/* 22. A Var can be a T_ID or a T_ID followed by an open bracket, Expression, close bracket */		
Var 				: T_ID 
					{	//If ID is not there then BARF
						//If it is the wrong subtype then BARF
						struct SymbTab *p;
						p = Search($1, LEVEL, 1);

						if(p == NULL){
							yyerror($1);
							yyerror("Variable used but not defined");
							exit(1);
						}

						if(p->SubType != SYM_SCALAR){
							yyerror($1);
							yyerror("Variable is wrong subtype, should be scalar");
							exit(1);
						}
						$$ = ASTCreateNode(A_VAR);
						$$->name = $1;
						$$->symbol = p;
						$$->datatype = p->Declared_Type;
					}
					| T_ID '[' Expression ']' 
					{
						//If ID is not there then BARF
						//If it is the wrong subtype then BARF
						struct SymbTab *p;
						p = Search($1, LEVEL, 1);

						if(p == NULL){
							yyerror($1);
							yyerror("Variable used but not defined");
							exit(1);
						}

						if(p->SubType != SYM_ARRAY){
							yyerror($1);
							yyerror("Variable is wrong subtype, should be array");
							exit(1);
						}

						if($3->datatype != p->Declared_Type){
							yyerror($1);
							yyerror("Index of array should be an integer");
							exit(1);
						}

						$$ = ASTCreateNode(A_VAR);
						$$->name = $1;
						$$->s1 = $3;
						$$->symbol = p;
						$$->datatype = p->Declared_Type;
					}
					;	   
/* 23. A SimpleExpression can be an AdditiveExpression or a SimpleExpression followed by a Relop and AdditiveExpression */		
SimpleExpression 	: AdditiveExpression {$$ = $1;}
					| SimpleExpression Relop AdditiveExpression
					{
						if($1->datatype != $3->datatype){
							yyerror("Type mismatch on expression");
							exit(1);
						}
						$$ = ASTCreateNode(A_EXPR);
						$$->s1 = $1;
						$$->s2 = $3;
						$$->operator = $2;
						$$->datatype = A_BOOLEANTYPE;
					}
					;
/* 24. A Relop can be any of the following tokens below */			
Relop 				: T_LE {$$ = A_LE;}
					| '<' {$$ = A_LESSTHAN;}
					| '>' {$$ = A_GREATERTHAN;}
					| T_GE {$$ = A_GE;}
					| T_EQ {$$ = A_EQ;}
					| T_NE {$$ = A_NE;}
					; 
/* 25. An AdditiveExpression can be a Term or an AdditiveExpression followed by an Addop and Term */
AdditiveExpression 	: Term 
					{
						$$ = $1;
					}
					| AdditiveExpression Addop Term 
					{
						if($1->datatype != $3->datatype){
							yyerror("Type mismatch on expression");
							exit(1);
						}
						$$ = ASTCreateNode(A_EXPR);
						$$->s1 = $1;
						$$->s2 = $3;
						$$->operator = $2;
						$$->datatype = $1->datatype;
					}
					; 
/* 26. An Addop can be a plus sign or a minus sign */
Addop 				: '+' {$$ = A_PLUS;}
					| '-' {$$ = A_MINUS;}
      				; 
/* 27. A Term can be a Factor or a Term followed by a Multop and Factor */
Term 				: Factor {$$ = $1;}
     				| Term Multop Factor 
					{
						if($1->datatype != $3->datatype){
							yyerror("Type mismatch on expression");
							exit(1);
						}
						$$ = ASTCreateNode(A_EXPR);
						$$->s1 = $1;
						$$->s2 = $3;
						$$->operator = $2;
						$$->datatype = $1->datatype;
					}
					; 
/* 28. A Multop can be any of the following tokens below */
Multop 				: '*' {$$ = A_TIMES;}
					| '/' {$$ = A_DIVIDES;}
					| T_AND {$$ = A_AND;}
					| T_OR {$$ = A_OR;}
					; 
/* 29. A Factor can be an open paranthesis followed by an Expression and close paranthesis or any of the tokens or productions below */	
Factor				: '(' Expression ')' 
					{
						$$ = $2;
					}
					| T_NUM 
					{
						$$ = ASTCreateNode(A_NUM);
						$$->value = $1;
						$$->datatype = A_INTTYPE;
					}
					| Var {$$ = $1;}
					| Call {$$ = $1;}
					| T_TRUE 
					{
						$$ = ASTCreateNode(A_TRUE);
						$$->datatype = A_BOOLEANTYPE;
					}
					| T_FALSE 
					{
						$$ = ASTCreateNode(A_FALSE); 
						$$->datatype = A_BOOLEANTYPE;
					}
					| T_NOT Factor 
					{
						if($2->datatype != A_BOOLEANTYPE){
							yyerror("NOT operator expects boolean");
							exit(1);
						}
						$$ = ASTCreateNode(A_EXPR);
						$$->operator = A_NOT;
						$$->s1 = $2;
						$$->datatype = $2->datatype;
					}
					;
/* 30. A Call can be a T_ID followed by an open paranthesis, Args, and close paranthesis */			
Call 				: T_ID '(' Args ')' 
					{
						//Check if it is in the symtable
						struct SymbTab *p;
						p = Search($1, 0, 0);
						if(p == NULL){
							yyerror($1);
							yyerror("Function is used but not in table");
							exit(1);
						}

						//Check is symbol is a function
						if(p->SubType != SYM_FUNCTION){
							yyerror($1);
							yyerror("Function name is not defined as a function");
							exit(1);
						}

						//Check to see if actual and formals match in length and type.
						if(check_params(p->fparms, $3) == 0){
							yyerror($1);
							yyerror("Parameter usage is incorrect");
							exit(1);
						}

						$$ = ASTCreateNode(A_CALL);
						$$->s1 = $3;
						$$->name = $1;
						$$->symbol = p;
						$$->datatype = p->Declared_Type;
					}
					;
/* 31. An Args can be empty or an ArgList */			
Args				: /* empty */
					{
						$$ = NULL;
					}
					| ArgList
					{
						$$ = $1;
					}
					;
	
/* 32. An ArgList can be an Expression or an Expression followed by a comma and ArgList */		
ArgList 			: Expression 
					{
						$$ = ASTCreateNode(A_ARGLIST);
						$$->s1 = $1;
					}
					| Expression ',' ArgList
					{
						$$ = ASTCreateNode(A_ARGLIST);
						$$->s1 = $1;
						$$->s2 = $3;
					}
					;
			
	   
%%	/* end of rules, start of program */

/*
Preconditions: Parser has been properly initialized and configured with proper tokens.
Postconditions: yyparse is called and main exits.
*/
int main(){ 
	yyparse();
	Display();
	ASTprint(0, program);	
} //End of main
