/*
*	Created by CMY on 29/07/2019.
 */

#include "MainAux.h"
#include <stdio.h>
#include <stdlib.h>

/*Allocating memory using Malloc with error catch*/
void* mallocWithGuard(int size){
    void* arr = malloc(size);
    if(arr == NULL){
        printf(MALLOCFAIL);
        exit(-1);
    }

    return arr;
}


/*Allocating memory using Calloc with error catch*/
void* callocWithGuard(int arrLen, int sizeOf){
    void* arr = calloc(arrLen, sizeOf);
    if(arr == NULL){
        printf(MALLOCFAIL);
        exit(-1);
    }

    return arr;
}
