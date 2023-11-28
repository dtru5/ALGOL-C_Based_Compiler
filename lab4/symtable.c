
/*
Dominik Trujillo 
09/15/2023
LAB4 -- Symbol Table with YACC 

------- Previous Purpose for Lab 3 ---------
The purpose of this lab is to understand a linked list and how we can use this data structure for our symbol table and operations of a symbol table.

The code was pulled from this website: http://forgetcode.com/C/101-Symbol-table

The changes of the code was updating the SymbTab object so that instead of two arrays of label and symbol of size ten, it was replaced with a char pointer variable symbol.
The Modify method was removed completely.
Moved all user interactions to be done in main.
Updated the methods of Insert, Delete, and Search by changing the parameters.
Insert now takes in a char pointer and an int address, Delete now takes a char pointer same with Search().


The main data structure in this code is a linked list. The fields are its data, in this case it is the char pointer symbol and int addr. Next would be another "Node" in this case that points to the next 
node of the current. It is built by assigning the first node as first and last and also maintaing the size of it with the global variable size. When a new node is added, we select that 
node to be the last's next node. Since one node is only in the list, when we add another, that node becomes the next and assigned as the last. It will continue like that for every entry after the first
as been installed.
-------------------------------------------------

Purpose in relation to lab 5: The purpose of this lab is to use this symbol table in accordance with YACC so that we're able to insert variables with the
previous calculator lab.

*/

#include<stdio.h>
/* #include<conio.h> */
#include<malloc.h>
#include<string.h>
#include<stdlib.h>
#include "symtable.h"

//Global variable int size will keep the size of the linked list
int size=0;

/*
Changed char label[10],symbol[10]; with
	char * symbol;
	
	This data structure is a linked list
*/
struct SymbTab{
	char * symbol;
	int addr;
	struct SymbTab *next;
};

//Global object SymbTabl will keep track of the first and last nodes of the linked list	
struct SymbTab *first,*last;



/*
 Inserts a new symbol with its corresponding address into a linked list.
 
 sym - A pointer to the symbol string to be inserted.
 address - The address associated with the symbol.

 Pre-condition: Valid input of the parameters 
 Post-condition: The linked list is updated.
 */
void Insert(char * sym, int address){
	struct SymbTab *p;
	p=malloc(sizeof(struct SymbTab));
	p->symbol = sym;
	p->addr = address;
	p->next=NULL;
	if(size==0){
		first=p;
		last=p;
	}
	else{
		last->next=p;
		last=p;
	}
	size++;
}//End of Insert

/* 
Displays the contents of the linked list, including symbols and their associated addresses.
Pre-Condition: N/A
Post-Condition: Prints out the symbol table.
*/
void Display(){
	int i;
	struct SymbTab *p;
	p=first;
	printf("\n\t\tSYMBOL\t\tADDRESS\n");
	for(i=0;i<size;i++){
		printf("\t\t%s\t\t%d\n",p->symbol,p->addr);
		p=p->next;
	}
}//End of Display

/*
Searches for a symbol in the linked list and returns whether the symbol is found or not.
Parameter: s
Pre-Condition: if empty, then the linked list should have first be pointed to NULL and valid parameter inputs
Post-Condition: Returns 1 or 0 depending if found or not.
*/
int Search(char * s){
	int i,flag=0;
	struct SymbTab *p;
	p=first;
	for(i=0;i<size;i++){
		if(strcmp(p->symbol, s)==0)
			flag=1;
		p=p->next;
	}
	return flag;
}//End of Search

/*
Deletes a symbol from the linked list if it exists.
Parameter: s
Pre-Condition: s if a valid string
Post-Condition: Removes the label from the linked list and is updated.
*/
void Delete(char *s){
	int a;
	char l[10];
	struct SymbTab *p,*q;
	p=first;
	if(strcmp(first->symbol,s)==0)
		first=first->next;
	else if(strcmp(last->symbol,s)==0){
		q=p->next;
		while(strcmp(q->symbol,s)!=0){
			p=p->next;
			q=q->next;
		}
		p->next=NULL;
		last=p;
	}
	else{
		q=p->next;
		while(strcmp(q->symbol,s)!=0){
			p=p->next;
			q=q->next;
		}
		p->next=q->next;
	}
	size--;
	printf("\n\tAfter Deletion:\n");
	Display();
	
}//End of Delete

//PRE-- string that is in thesymbol table
//post -- the address assigned

int FetchAddress(char * symbol){
	int i;
	struct SymbTab *p;
	p=first;
	
	for(i=0;i<size;i++){
		if(strcmp(p->symbol, symbol)==0)
			return p->addr;
		p=p->next;
	}
	
	return (0);
}//Fetch Address

