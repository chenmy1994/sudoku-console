/*
*	Created by CMY on 29/07/2019.
 */

#include "LinkedList.h"
#include <stdlib.h>

/*Inserts new node to linked list*/
void insert(LinkedList* lst,Point** points){
    Node* newNode;
    newNode = (Node*)malloc(sizeof(Node));
    newNode->data = points;
    newNode->next= 0;
    if(lst->count>0) {
        lst->current->next = newNode;
        newNode->prev = lst->current;
    }
    lst->current = newNode;
    lst->count++;
}
/*Update current node to previous node*/
void prevCurr(LinkedList* lst){
    if(lst->count>0) {
        lst->current = lst->current->prev;
    }
}

/*Update current node to next node*/
