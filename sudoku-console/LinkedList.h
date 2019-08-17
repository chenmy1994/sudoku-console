/*
*	Created by CMY on 29/07/2019.
 */

#ifndef SUDOKU_CONSOLE_LINKEDLIST_H
#define SUDOKU_CONSOLE_LINKEDLIST_H
#include "Point.h"

typedef struct Node{
    Point** data;
    int pointNum;
    struct Node* next;
    struct Node* prev;
}Node;

typedef struct {
    Node* current;
    Node* first;
    Node* last;
    int count;
}LinkedList;

/*return 1 if we reached the end of the list (last), 0 otherwise*/
int theEnd(LinkedList* lst);

/*return 1 if we reached the start of the list (first), 0 otherwise*/
int theStart(LinkedList* lst);

/*Delete a given node from lst*/
void deleteNode(LinkedList* lst, Node* node);

/*Initialize LinkedList Object*/
LinkedList* initLinkedList();

/*Receives 2 node: node1, node2. Setting node1's next to be node2 and node2 prev to be node1*/
void setNext(Node* node1, Node* node2);

/*Inserts new node to linked list*/
void insert(LinkedList* lst,Point** points,int len);

/*Returns the current node of a given LinkedList*/
Node* getCurrent(LinkedList* lst);

/*Update current node to previous node*/
void prevCurr(LinkedList* lst);

/*Update current node to next node*/
void nextCurr(LinkedList* lst);

/*Free malloc allocations of given linked list*/
void freeListMem(LinkedList* lst);

#endif /*SUDOKU_CONSOLE_LINKEDLIST_H*/
