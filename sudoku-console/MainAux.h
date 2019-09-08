/*
 * MainAux.h
 *
 *  Created on: Aug 1, 2019
 *      Author: Noa
 */

#ifndef SUDOKU_CONSOLE_POINT_H
#define SUDOKU_CONSOLE_POINT_H
#define MALLOCFAIL "Error: Malloc has failed\nGame is exiting uncleanly.\n"

/* Point contains:
 * x and y representing column and row on board
 * prev representing previous value for the <x,y> cell on board
 * curr representing current value for the <x,y> cell on board
 * */
typedef struct {
    int x;
    int y;
    int prev;
    int curr;
}Point;


/*Allocating memory using Calloc with error catch*/
void* callocWithGuard(int sizeOf, int arrLen);

/*Allocating memory using Malloc with error catch*/
void* mallocWithGuard(int size);
#endif /*SUDOKU_CONSOLE_POINT_H*/
