/**
 * SolveFunc Summary:
 * This module contains functions that are available in solve mode only
 * and some helpful functions that are relevant in solve mode
 *
 * Public Functions:
 * guess - Fills all cell values with probability greater than users input
 * hint - Give a hint to the user by showing the solution of the input cell
 * guessHint - Shows a guess to the user for a single cell X,Y
 * autofill - Automatically fill obvious values
 *
 * Private Functions:
 * fillValue - Fill the given cell x - col, y - row with the value z
 * randValue - When given cell x - col, y - row, it randomize a legal value
 * fillGuessValues - Fills the values of each cell by the solution we got from solveLP
 * printGuessAndScores - When given cell x,y (col, row) it prints all the values that got a score higher than 0
 * checkBeforeHint - General check which need to be executed before hint and guess_hint
 * setAF - the Set value part of autofill function
 * singleValidValue - Receives coordinates of a cell and return the single valid value of this cell if exists
 * fillSingleValue - Receives an array of points and its size - filling the relevant cells with the single value
 *
 */
#ifndef SUDOKU_CONSOLE_SOLVEFUNC_H
#define SUDOKU_CONSOLE_SOLVEFUNC_H
#include "Game.h"
#include "GurobiFunc.h"
#define CELLISFIXEDORFILLED "Error: cell <%d, %d> is %s.\n"
#define autofillMSG "Set single legal value %d for Cell <%d,%d>.\n"
#define NOCELLSTOFILL "There were no 'obvious' values to fill.\n"

/*Fills all cell values with probability greater than users input*/
int guess(double x, Game* game);

/*Shows a guess to the user for a single cell X,Y*/
void guessHint(int x, int y, Game* game);

/*Give a hint to the user by showing the solution of the input cell*/
void hint(int x, int y,Game* game);

/*Automatically fill obvious values - cells which contain a single legal value.
 * return 0 if the board has erroneous, otherwise 1*/
int autofill(Game* game);

#endif /*SUDOKU_CONSOLE_SOLVEFUNC_H*/
