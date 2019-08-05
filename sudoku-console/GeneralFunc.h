/*
*	Created by CMY on 29/07/2019.
 */

#include "EditFunc.h"
#include "SolveFunc.h"
#ifndef SUDOKU_CONSOLE_GENERALFUNC_H
#define SUDOKU_CONSOLE_GENERALFUNC_H
#include <stdlib.h>
#include <stdio.h>
#define BOARDSAVED "Board saved to: %s\n"
#define ERRORSETSOLVE "In Solve mode it is NOT possible to set a fixed cell"

/*Dealing with the edit command received by user*/
void edit();

/*Dealing with the exit command received by user*/
void exitGame();

/*Dealing with the solve command received by user*/
void solve(char* X, Game* game);

/*Saves the current game board to the specified file*/
/*TODO - should be splitting between SolveFunc and EditFunc */
void save(char* X, Game* game);

/*Set new value of cell*/
void set(int x, int y, int z, Game* game);

/*Revert the last move done by the user according to the linked list current move (updates to previous move)*/
void undo();

/*Cancel the last revert action according to the linked list current move (updates to next move)*/
void redo();

/*Prints the number of solution for the current board*/
void num_solutions();

/*Undo all moves, reverting the board to its original loaded state.*/
void reset();

/*Checks whether the cell in col x and row y is fixed or not*/
int isFixed(int x, int y, Game* game);

#endif /*SUDOKU_CONSOLE_GENERALFUNC_H*/
