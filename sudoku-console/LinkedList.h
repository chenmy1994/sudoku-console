/*
*	Created by CMY on 29/07/2019.
 */

#ifndef SUDOKU_CONSOLE_LINKEDLIST_H
#define SUDOKU_CONSOLE_LINKEDLIST_H
#include "Point.h"

typedef struct Node{
    Point** data;
    struct Node* next;
    struct Node* prev;
}Node;

typedef struct {
    Node* current;
    int count;
}LinkedList;

/*Inserts new node to linked list*/
void insert(LinkedList* lst, Point** points);

/*Update current node to previous node*/
void prevCurr(LinkedList* lst);

/*Update current node to next node*/
void nextCurr(LinkedList* lst);


#endif /*SUDOKU_CONSOLE_LINKEDLIST_H*/
