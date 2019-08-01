//
// Created by CMY on 29/07/2019.
//

#include "EditFunc.h"
#include "SolveFunc.h"
#ifndef SUDOKU_CONSOLE_GENERALFUNC_H
#define SUDOKU_CONSOLE_GENERALFUNC_H
/*Dealing with the edit command received by user*/
void edit();

/*Dealing with the exit command received by user*/
void exit();

/*Dealing with the solve command received by user*/
void solve();

/*Saves the current game board to the specified file*/
/*TODO - should be splitting between SolveFunc and EditFunc */
void save();

/*Set new value of cell*/
void set();

/*Checks whether the board can be solved (1) or not (0)*/
/*uses the ILP solver*/
int validate();

/*Revert the last move done by the user according to the linked list current move (updates to previous move)*/
void undo();

/*Cancle the last revert action according to the linked list current move (updates to next move)*/
void redo();

/*Prints the number of solution for the current board*/
void num_solutions();

/*Undo all moves, reverting the board to its original loaded state.*/
void reset();
#endif //SUDOKU_CONSOLE_GENERALFUNC_H
