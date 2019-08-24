/*
*	Created by CMY on 29/07/2019.
 */

#include "Stack.h"
#include <stdlib.h>
/*Initialize LinkedList Object*/
Stack* initStack(){
	Stack* stk=(Stack*)malloc(sizeof(Stack));
	stk->count=0;
	return stk;
}

/*Inserts new element to the stack*/
void push(Stack* stk, Point* point){
    Elem* elem;
    elem = (Elem*) malloc(sizeof(Elem));
    elem->data=point;
    if(!isEmpty(stk)) {
        elem->prev = stk->head;
    }
    stk->head=elem;
    stk->count++;
}

/*Checks if stack is empty, return 1 if empty, 0 otherwise*/
int isEmpty(Stack* stk){
	return (stk->count==0? 1 : 0);
}

/*Removes head of stack, new head is previous element
 * will only be called after a check that the stack isn't empty*/
Elem* pop(Stack* stk){
	Elem* elem;
	elem=stk->head;
	stk->head=stk->head->prev;
	stk->count--;
	return(elem);
}

/*Returns head of stack*/
Elem* top(Stack* stk){
	return stk->head;
}
