%{
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


	/* begin specs */
#include <stdio.h>
#include <stdlib.h> //Added stdlib.h to get rid of warning errors on exit call.
#include <ctype.h>
#include <string.h>
#include "ast.h"
#include "symtable.h"
#include "emit.h"

int yylex();

int LEVEL = 0; //How many compound statements deep we're in.
int OFFSET = 0; //How many words have we seen at GLOBAL or inside a function.
int GOFFSET; //Holder for global offset when we enter and exit a fucntion definition.
int maxoffset; //Total number of words a function needs

extern int lineno; //Global variable for line number counter.
extern int mydebug;
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
							p->symbol->Declared_Type = $1; //Setting the symbol's declared type with the TypeSpecifier
							//Although redundant this helps with other functions like in check params.
							p = p->s1;
							}
					}
	       			;
/* 4a. A VarList can be a T_ID, a T_ID followed by an open bracket, T_NUM, and close bracket, a T_ID followed by a comma and VarList */
/* a T_ID followed by an open bracket, T_NUM, close bracket, comma, and a VarList */	       
VarList 			: T_ID  
						{
							//Symbol not there, stick it in
							if (Search($1, LEVEL, 0) == NULL){
								$$ = ASTCreateNode(A_VARDEC); //Create a new A_VARDEC node
								$$->name = $1; //Set the name with the given ID
								$$->value = 1;
								//Set symbol to be the node that is returned after inserting it into the symtable, type scalar.
								$$->symbol = Insert($1, A_UNKNOWN, SYM_SCALAR, LEVEL, 1, OFFSET);
								OFFSET++; //Increment offset.
							}
							else{ //Else, the symbol is in the table already, then exit.
								yyerror($1);
								yyerror("Already defined");
								exit(1);
							}

						}
					| T_ID '[' T_NUM ']' 
						{					
							//Symbol not there, stick it in							
							if (Search($1, LEVEL, 0) == NULL){
								$$ = ASTCreateNode(A_VARDEC); //Create a new A_VARDEC node
								$$->name = $1; //Set the name to be the ID
								$$->value = $3; //Set the value with the given NUM
								//Set symbol to be the node that is returned after inserting it into the symtable, type array.
								$$->symbol = Insert($1, A_UNKNOWN, SYM_ARRAY, LEVEL, $3, OFFSET);
								OFFSET += $3; //Increase offset with the value of the array.
							}
							else{ //Else, the symbol is in the table already, then exit.
								yyerror($1);
								yyerror("Already defined");
								exit(1);
							}
						} 
					| T_ID ',' VarList 
						{
							//Symbol not there, stick it in
							if (Search($1, LEVEL, 0) == NULL){
								$$ = ASTCreateNode(A_VARDEC); //Create a new A_VARDEC node
								$$->name = $1; //Set the name with ID
								$$->s1 = $3; //Set the s1 branch to be another Varlist
								$$->value = 1;
								//Set p's symbol to be the node that is returned after inserting it into the symtable
								$$->symbol = Insert($1, A_UNKNOWN, SYM_SCALAR, LEVEL, 1, OFFSET);
								OFFSET++; //Increment offset.
							}
							else{ //Else, the symbol is in the table already, then exit.
								yyerror($1);
								yyerror("Already defined");
								exit(1);
							}
						} 
					| T_ID '[' T_NUM ']' ',' VarList 
						{
							//Symbol not there, stick it in
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
							else{ //Else, the symbol is in the table already, then exit.
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
								GOFFSET = OFFSET; //Set the global offset to be current offset.
								OFFSET = 2; //Reserve the 2 spaces needed for registers by setting offset to be 2.
								maxoffset = OFFSET; //Set maxoffset to be offset.
							}
							else{
								//Exit if the function has already been defined.
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
							$$ = ASTCreateNode(A_FUNDEC); //Create a new ASTNode called A_FUNDEC.
							$$->name = $2; //Setting the name with the ID
							$$->datatype = $1; //Setting the datatype with the given TypeSpecifier
							$$->s1 = $5; //Setting the s1 branch to be the Params
							$$->s2 = $8; //Setting s2 branch to be the Compound statement
							$$->symbol = Search($2, LEVEL, 0); //Set the symbol to be the searched symbol.
							$$->symbol->offset = maxoffset; //Set the symbol's offset to be the max offset.
							OFFSET = GOFFSET; //Set offset to be the global offset.
						}
					;
/* 7. A Params can be a T_VOID or a ParamList */		
Params 				: T_VOID 
					{
						$$ = NULL; //If T_VOID is read, then set the node to be NULL.
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
						//If the symbol hasn't been inserted,
						if(Search($2, 1, 0) == NULL){
							$$ = ASTCreateNode(A_PARAM); //Set the node to be an ASTNode of type A_PARAM.
							$$->name = $2; //Set the name with T_ID.
							$$->symbol = Insert($2, $1, SYM_SCALAR, LEVEL+1, 1, OFFSET); //Set the symbol with the returned symtab from the inserting call.
							$$->datatype = $$->symbol->Declared_Type; //Set the datatype of the node with the symbol's declared type.
							OFFSET++; //Increment offset.
						}
						else{ // Else, if parameter is already in use, then exit.
							yyerror($2);
							yyerror("Parameter name already used");
							exit(1);
						}
					}
					| TypeSpecifier T_ID '[' ']' 
					{
						if(Search($2, 1, 0) == NULL){
							$$ = ASTCreateNode(A_PARAM); //Set the node to be an ASTNode of type A_PARAM.
							$$->name = $2; //Set the name with T_ID.
							$$->value = -1; //Set the value to be -1 for other functions to know that they're processing an array A_PARAM.
							$$->symbol = Insert($2, $1, SYM_ARRAY, LEVEL+1, 1, OFFSET); //Set the symbol with the returned symtab from the inserting call.
							$$->datatype = $$->symbol->Declared_Type; //Set the datatype of the node with the symbol's declared type.
							OFFSET++; //Increment offset.
						}
						else{ // Else, if parameter is already in use, then exit.
							yyerror($2);
							yyerror("Parameter name already used");
							exit(1);
						}
					}
					;
/* 10. A CompoundStmt can be a T_BEGIN followed by LocalDeclarations, StatementList, and T_END */	
CompoundStmt 		: T_BEGIN {LEVEL++;} //Increment the level when a T_BEGIN is read in
					  LocalDeclarations StatementList T_END
					{
						$$ = ASTCreateNode(A_COMPOUND); //Set the node to be an ASTNode called A_COMPOUND
						$$->s1 = $3; //Set the s1 branch to be LocalDeclarations.
						$$->s2 = $4; //Set the s2 branch to be StatementList
						if(OFFSET > maxoffset){ //If the current offset is greater then the max,
							maxoffset = OFFSET; //then set the maxoffset to be the current offset value.
						}
						if(mydebug){
							Display(); //Display the symbol table
						}
						OFFSET -= Delete(LEVEL); //Set offset with the subtraction of the value returned from the called Delete function given the param LEVEL.
						LEVEL--; //Decrement LEVEL.
					}
					;
/* 11. LocalDeclarations can be either empty or a VarDeclaration followed by a LocalDeclarations */
LocalDeclarations 	: /* empty */
					{
						$$ = NULL; //If local declaration is empty, then set the node to be null.
					}
		  			| VarDeclaration LocalDeclarations 
					{
						$$ = $1; //Set the node to be a VarDeclaration
						$$->s2 = $2; //Set the s2 branch to be LocalDeclarations.
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
						if($1->datatype != $3->datatype){ //If the datatypes don't match, then exit
							yyerror("Assignment type mismatch");
							exit(1);
						}
						$$ = ASTCreateNode(A_ASSIGNMENTSTMT);
						$$->s1 = $1;
						$$->s2 = $3;
						$$->symbol = Insert(CreateTemp(), $1->datatype, SYM_SCALAR, LEVEL, 1, OFFSET++); //Set the symbol to be the returned symtab 
						//from Insert() and setting the name to be a value produced by CreateTemp.
					}
					;	
/* 21. An Expression can be a SimpleExpression */		
Expression 			: SimpleExpression {$$ = $1;}
					;
/* 22. A Var can be a T_ID or a T_ID followed by an open bracket, Expression, close bracket */		
Var 				: T_ID 
					{	//If ID is not there then BARF
						//If it is the wrong subtype then BARF
						struct SymbTab *p; //Create a SymbTab pointer called p
						p = Search($1, LEVEL, 1); //Set p to be the search value of the T_ID

						if(p == NULL){ //If p is NULL, then exit.
							yyerror($1);
							yyerror("Variable used but not defined");
							exit(1);
						}

						if(p->SubType != SYM_SCALAR){ //If the subtype is not a scalar, then exit
							yyerror($1);
							yyerror("Variable is wrong subtype, should be scalar");
							exit(1);
						}
						$$ = ASTCreateNode(A_VAR); //Set the node to be an A_VAR 
						$$->name = $1; //Set the node's name to be the T_ID
						$$->symbol = p; //Set the node's symbol to be p
						$$->datatype = p->Declared_Type; //Set the data type to be p's declared type.
					}
					| T_ID '[' Expression ']' 
					{
						struct SymbTab *p;//Create a SymbTab pointer called p
						p = Search($1, LEVEL, 1);//Set p to be the search value of the T_ID

						if(p == NULL){ //If p is NULL, then exit
							yyerror($1);
							yyerror("Variable used but not defined");
							exit(1);
						}

						if(p->SubType != SYM_ARRAY){ //If the subtype is not array type, then exit
							yyerror($1);
							yyerror("Variable is wrong subtype, should be array");
							exit(1);
						}

						if($3->datatype != p->Declared_Type){ //if the datatype doesn't match the declared type, then exit
							yyerror($1);
							yyerror("Index of array should be an integer");
							exit(1);
						}

						$$ = ASTCreateNode(A_VAR); //Set the node to be an A_VAR 
						$$->name = $1; //Set name to be T_ID
						$$->s1 = $3; //Set the s1 branch to be the Expression
						$$->symbol = p; //Set the node's symbol to be p.
						$$->datatype = p->Declared_Type; //Set the node's datatype with p's declared typed
					}
					;	   
/* 23. A SimpleExpression can be an AdditiveExpression or a SimpleExpression followed by a Relop and AdditiveExpression */		
SimpleExpression 	: AdditiveExpression {$$ = $1;}
					| SimpleExpression Relop AdditiveExpression
					{
						if($1->datatype != $3->datatype){ //If the data types don't match for simple and additive expression, then exit.
							yyerror("Type mismatch on expression");
							exit(1);
						}
						$$ = ASTCreateNode(A_EXPR);
						$$->s1 = $1;
						$$->s2 = $3;
						$$->operator = $2; //Set the operator to be the Relop ops
						$$->datatype = A_BOOLEANTYPE; //Set the datatype to be type boolean
						$$->symbol = Insert(CreateTemp(), $$->datatype, SYM_SCALAR, LEVEL, 1, OFFSET++); //Set the node's symbol to be the 
						//returned pointer symtab using the CreateTemp for the name.
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
						//If the datatype's don't match between Additive Expression and Term, then exit.
						if($1->datatype != $3->datatype){
							yyerror("Type mismatch on expression");
							exit(1);
						}
						$$ = ASTCreateNode(A_EXPR); //Set the node to be an A_EXPR node.
						$$->s1 = $1; //Set the s1 branch to be an AdditiveExpression 
						$$->s2 = $3; //Set the s2 branch to be the Term
						$$->operator = $2; //Set the node's operator to be the Addop ops.
						$$->datatype = $1->datatype; //Set the datatype of the node to be the datatype of AdditiveExpression
						$$->symbol = Insert(CreateTemp(), $1->datatype, SYM_SCALAR, LEVEL, 1, OFFSET++); //Set the nodes symbol
						//with the returned pointer from Insert() giving the name a CreateTemp name.
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
						//If the datatype's between Term and Factor don't match, then exit.
						if($1->datatype != $3->datatype){
							yyerror("Type mismatch on expression");
							exit(1);
						}
						$$ = ASTCreateNode(A_EXPR); //Set the node to be an A_EXPR
						$$->s1 = $1; //Set the s1 branch to be Term
						$$->s2 = $3; //Set the s2  branch to be Factor
						$$->operator = $2; //Set the node's operator to be a Multop
						$$->datatype = $1->datatype; //Set the node's datatype to be the datatype from Term
						$$->symbol = Insert(CreateTemp(), $1->datatype, SYM_SCALAR, LEVEL, 1, OFFSET++); //Set the node's symbol
						//with the returned pointer from Insert() giving the name a CreateTemp name.
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
						$$ = $2; //Set the node to point to Expression
					}
					| T_NUM 
					{
						$$ = ASTCreateNode(A_NUM); //Set the node to be an A_NUM
						$$->value = $1; //Set the value of the node to be the value of T_NUM
						$$->datatype = A_INTTYPE; //Set the datatype to be A_INTTYPE
					}
					| Var {$$ = $1;}
					| Call {$$ = $1;}
					| T_TRUE 
					{
						$$ = ASTCreateNode(A_TRUE); //Set the node to be an A_TRUE node
						$$->value = 1;
						$$->datatype = A_BOOLEANTYPE; //Set the datatype to be boolean type
					}
					| T_FALSE 
					{
						$$ = ASTCreateNode(A_FALSE); //Set the node to be an A_FALSE node
						$$->value = 0;
						$$->datatype = A_BOOLEANTYPE; //Set the datatype to be boolean type
					}
					| T_NOT Factor 
					{
						//If the datatype of Factor is not boolean type, then exit.
						if($2->datatype != A_BOOLEANTYPE){
							yyerror("NOT operator expects boolean");
							exit(1);
						}
						$$ = ASTCreateNode(A_EXPR); //Set the node to point to an A_EXPR node
						$$->operator = A_NOT; //Set the operator to be an A_NOT.
						$$->s1 = $2; //Set the s1 branch to be the Factor
						$$->datatype = $2->datatype; //Set the node's datatype with the datatype of Factor
					}
					;
/* 30. A Call can be a T_ID followed by an open paranthesis, Args, and close paranthesis */			
Call 				: T_ID '(' Args ')' 
					{
						//Check if it is in the symtable
						struct SymbTab *p; //Create a SymbTab pointer p
						p = Search($1, 0, 0); //Set p to be the returned searched symtab
						//If p is NULL, then exit
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

						$$ = ASTCreateNode(A_CALL); //Set the node to point to an A_CALL
						$$->s1 = $3; //Set the s1 branch to be Args
						$$->name = $1; //Set the name to T_ID
						$$->symbol = p; //Set the node's symbol to reference p.
						$$->datatype = p->Declared_Type; //Set the node's type to be p's declared type.
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
						$$ = ASTCreateNode(A_ARGLIST); //Set the node to be an A_ARGLIST
						$$->s1 = $1; //Set the s1 branch to be Expression
						//Set the node's symbol to be the returned symtab with the name from CreateTemp
						$$->symbol = Insert(CreateTemp(), $1->datatype, SYM_SCALAR, LEVEL, 1, OFFSET++);
					}
					| Expression ',' ArgList
					{
						$$ = ASTCreateNode(A_ARGLIST); //Set the node to be an A_ARGLIST
						$$->s1 = $1; //Set the s1 branch to be Expression
						$$->s2 = $3; //Set the s2 branch to be ArgList
						//Set the node's symbol to be the returned symtab with the name from CreateTemp
						$$->symbol = Insert(CreateTemp(), $1->datatype, SYM_SCALAR, LEVEL, 1, OFFSET++);
					}
					;
			
	   
%%	/* end of rules, start of program */

/*
Preconditions: Parser has been properly initialized and configured with proper tokens.
Postconditions: yyparse is called and main exits.
*/
int main(int argc, char * argv[]){ 
	FILE *fp = NULL;
	char s[100];

	for(int i = 1; i < argc; i++){
		if(strcmp("-d", argv[i]) == 0){
			mydebug = 1;
		}

		if(strcmp("-o", argv[i]) == 0){
			//Assume that argv[i+1] is on the line and we are not going to check.
			strcpy(s, argv[i+1]);
			strcat(s,".asm");

			if(mydebug){
				printf("Opening file %s\n", s);
			}

			fp=fopen(s,"w");
			if(fp == NULL){
				printf("Unable to open %s\n", s);
				exit(1);
			} //end of if fopen
		} //end of strcmp -o
	} //end of for loop

	if(fp == NULL){
		printf("No filename provided, must use -o option\n", s);
		exit(1);
	} //end of if not -o option.

	yyparse();

	if(mydebug){
		fprintf(stderr, "The input is syntactically correct\n");
		Display();
		ASTprint(0, program);	
	}

	EMIT(program, fp);
} //End of main 
