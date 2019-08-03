/*
*	Created by CMY on 29/07/2019.
 */

#include "Stack.h"

/*Inserts new element to the stack*/
void push(Stack* stk, Point* point){
    Elem* elem;
    elem->data=point;
    if(stk->count>0) {
        elem->prev = stk->head;
    }
    stk->head=elem;
    stk->count++;
}

/*Removes head of stack, new head is previous element*/
Elem* pop(Stack* stk){
    stk->head=stk->head->prev;
    stk->count--;
}
