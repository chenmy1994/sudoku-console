/*
*	Created by CMY on 29/07/2019.
 */

#ifndef SUDOKU_CONSOLE_SOLVEFUNC_H
#define SUDOKU_CONSOLE_SOLVEFUNC_H
#include "GurobiFunc.h"
#define CELLISFIXED "Error: cell <%d, %d> is fixed.\n"
#define CELLISFILLED "Error: cell <%d, %d> is filled.\n"

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
