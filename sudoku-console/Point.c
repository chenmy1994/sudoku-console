/*
*	Created by CMY on 29/07/2019.
 */

#include "Point.h"
#include <stdio.h>
#include <stdlib.h>

/*set x value of a given point p*/
/*void setX(Point* p, int x){
	(*p).x=x;
}*/
/*set y value of a given point p*/
/*void setY(Point* p, int y){
	(*p).y=y;
}
*/

/*Malloc with guard*/
void* mallocWithGuard(int size){
    void* arr = malloc(size);
    if(arr == NULL){
        printf(MALLOCFAIL);
        exit(-1);
    }

    return arr;
}


/*Calloc with guard*/
void* callocWithGuard(int arrLen, int sizeOf){
    void* arr = calloc(arrLen, sizeOf);
    if(arr == NULL){
        printf(MALLOCFAIL);
        exit(-1);
    }

    return arr;
}