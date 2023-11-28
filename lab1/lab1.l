/*       
	Dominik Trujillo
	Lab 1 
	8/18/2023  

     	 This lex routine uses a counting array to match alphabeticstrings 
         and make a frequency count.
  
         The real item to notice is that yywrap() is called at EOF and then is run
         to do what we need to do.   yywrap() returns true when we have a successful 
         end to the program.  We may want to return false (0) if we want to lexing process 
         to fail 

        Shaun Cooper
        January 2015
        
        LEX CODE COMMENTS: 
        The [a-zA-Z]+	lgths[yyleng]++; portion takes a string containing any lowercase and uppercase characters and will increase the count while also increasing the 
        corresponding array of lgths.
        Added [0-9]+ which will take in any number from 0-9 starting at 0 and increases the variable nums to keep count the amount of times it appears 
        . symbol is to do everything but newline
        | pipe
        \n is a new line
        ; is do nothing
        
   */
	int lgths[100];
	int nums;
	
%%
[a-zA-Z]+	lgths[yyleng]++;
[0-9]+		nums++;
.	|
\n	;
%%

/*
	Pre-condition: The pre condition is that the file contains elements that is able to be read from
	Post-condition: The post condition is that any captured values of any words or numbers will be printed.
	yywrap() is responsible for iterating through the lgths array, while keeping track of the length of the word, 
	and display how many times a word/string appears with that specific length. Then it will print the value of nums which is the number of numbers counted from the file
*/
int yywrap()
{
	int i;
	printf("Length  No. words\n");
	for (i=1; i<100; i++) { 
	/*This for loop will iterate through the length of the lgths array of 100 and if the value of that position is more than 0, then print the value of how many times that number has apppeared
	with its corresponding word length */
		if (lgths[i] > 0) {
			printf("%5d%10d\n",i,lgths[i]);
		}//end of if
	}//end of for loop
	printf("The number of numbers that occur is %d.\n", nums);
	return(1);
}//end of yywrap

int main()
{  yylex();

}
