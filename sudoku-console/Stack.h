/*
*	Created by CMY on 29/07/2019.
 */

#include "Point.h"
#ifndef SUDOKU_CONSOLE_STACK_H
#define SUDOKU_CONSOLE_STACK_H

typedef struct Elem{
    Point* data;
    struct Elem* prev;
}Elem;

typedef struct {
    Elem* head;
    int count;
}Stack;

/*Inserts new element to the stack*/
void push(Stack* stk, Point* point);

/*Removes head of stack, new head is previous element*/
Elem* pop(Stack* stk);
#endif //SUDOKU_CONSOLE_STACK_H
