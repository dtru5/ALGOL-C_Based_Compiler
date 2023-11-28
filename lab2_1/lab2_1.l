         /* simple lex program which removes comments from a source program
            The main key is that a variable "comment" is set when the start of a comment
            is seen and then unset when the ending set is seen.  It is possible to have
            two starts closed by on end.

            Shaun Cooper
            January 2015
 
         */
         
/*
LAB 2_1 Lex routine for comment processing 
Purpose: The purpose of this lab is to emphasize our use of LEX and to develop our documenting skills. 
Name: Dominik Trujillo
Date: 08/27/2023
Description: This LEX program is to remove the comments from the given source program. If the comment symbols are misused, then it will print out the 
line number the error has occured, else it will print out the text that is not a comment
*/         
        int comment = 0;
        int debug=1;  /* prints out debug statements if desired */
        int lineNum = 1; /* Integer variable lineNum to count the number of lines */
%%
"/*"	{
	if (comment && debug) fprintf(stderr,
		" >>>>>>>> line %d: Possible Nested comment <<<<<<<<<<\n", lineNum);
	comment = 1;
	}

"*/"	{
          if (!comment) printf("%s",yytext); /*print out if it is not in a comment */
          
  	  comment = 0;
	}
.       { if (!comment) printf("%s",yytext);}

\n      { //Added a '\n' newline character so that a new line is detected and is read
    	if (!comment) // If not in comment mode
        	printf("\n"); // Print a new line only when not in a comment to print out the text that is not a comment
    	
    	lineNum++; // Increment lineNum for the next line that is counted
}

%%

/*
Pre-conditions: N/A
Post-conditions: That yywrap will return 1
*/
int yywrap(void)
{ return 1;
} //End of yywrap

/*
This is main
Pre-conditions: N/A
Post-conditions: N/A
*/
main()
{ // Start of Main
  yylex();
} //End main

