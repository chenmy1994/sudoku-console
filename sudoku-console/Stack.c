/*
*	Created by CMY on 29/07/2019.
 */

#include "Stack.h"
#include <stdlib.h>
/*Inserts new element to the stack*/
void push(Stack* stk, Point* point){
    Elem* elem;
    elem = (Elem*) malloc(sizeof(Elem));
    elem->data=point;
    if(stk->count>0) {
        elem->prev = stk->head;
    }
    stk->head=elem;
    stk->count++;
}

/*Removes head of stack, new head is previous element*/
void pop(Stack* stk){
    stk->head=stk->head->prev;
    stk->count--;
}
