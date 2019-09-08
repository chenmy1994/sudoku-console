/**
 * Stack Summary:
 * This module defines Stack and Elem
 * and contains all the relevant functions
 *
 * Public Functions:
 * push - Inserts new element to the stack
 * pop - Removes head of stack, new head is previous element
 * isEmpty - Checks if stack is empty, return 1 if empty, 0 otherwise
 * initStack - Initialize Stack Object
 * top - Returns head of stack
 *
 */

#ifndef SUDOKU_CONSOLE_STACK_H
#define SUDOKU_CONSOLE_STACK_H
#include "MainAux.h"

typedef struct Elem{
    Point* data;
    struct Elem* prev;
}Elem;

typedef struct {
    Elem* head;
    int count;
}Stack;

/*Initialize LinkedList Object*/
Stack* initStack();

/*Inserts new element to the stack*/
void push(Stack* stk, Point* point);

/*Removes head of stack, new head is previous element*/
Elem* pop(Stack* stk);

/*Checks if stack is empty, return 1 if empty, 0 otherwise*/
int isEmpty(Stack* stk);

/*Returns head of stack*/
Elem* top(Stack* stk);
#endif /*SUDOKU_CONSOLE_STACK_H*/
