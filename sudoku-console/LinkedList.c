/*
*	Created by CMY on 29/07/2019.
 */

#include "LinkedList.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Initialize LinkedList Object*/
LinkedList* initLinkedList(){
	LinkedList* lst=(LinkedList*)mallocWithGuard(sizeof(LinkedList));
	lst->first=(Node*)mallocWithGuard(sizeof(Node));
	lst->last=(Node*)mallocWithGuard(sizeof(Node));
	setNext(lst->first,lst->last);
	lst->current=lst->first;
	lst->count=0;
	return lst;
}
/*Inserts new node to linked list*/
void insert(LinkedList* lst,Point** points,int len){
    Node* newNode = (Node*)mallocWithGuard(sizeof(Node));
    newNode->pointNum=len;
    newNode->data = points;
    setNext(newNode,lst->last);
    setNext(lst->current,newNode);
    lst->current = newNode;
    lst->count++;
}

/*Update current node to previous node*/
void prevCurr(LinkedList* lst){
    lst->current = lst->current->prev;
}

/*Delete a given node from lst*/
void deleteNode(LinkedList* lst, Node* node){
	setNext(node->prev,node->next);
	free(*(node->data));
	free(node->data);
	free(node);
    lst->count--;
}

/*Update current node to next node*/
void nextCurr(LinkedList* lst){
    if(lst->current->next!=lst->last) {
        lst->current = lst->current->next;
    }

}

/*return 1 if we reached the end of the list (last), 0 otherwise*/
int theEnd(LinkedList* lst){
	if((lst->current==lst->last)||(lst->current->next==lst->last)){
		return 1;
	}
	return 0;
}

/*return 1 if we reached the start of the list (first), 0 otherwise*/
int theStart(LinkedList* lst){
	if(lst->current==lst->first){
		return 1;
	}
	return 0;
}
/*Receives 2 node: node1, node2. Setting node1's next to be node2 and node2 prev to be node1*/
void setNext(Node* node1, Node* node2){
	node1->next=node2;
	node2->prev=node1;
}

/*Free malloc allocations of given linked list*/
void freeListMem(LinkedList* lst){
	lst->current=lst->first;
	while(lst->count>0){
		deleteNode(lst,lst->current->next);
	}
	free(lst->first);
	free(lst->last);
	free(lst);
}

