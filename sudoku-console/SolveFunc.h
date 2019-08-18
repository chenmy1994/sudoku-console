/*
*	Created by CMY on 29/07/2019.
 */

#ifndef SUDOKU_CONSOLE_SOLVEFUNC_H
#define SUDOKU_CONSOLE_SOLVEFUNC_H
#include "Game.h"
#include "GurobiFunc.h"
#define CELLISFIXED "Error: cell <%d, %d> is fixed.\n"

/*Fills all cell values with probability greater than users input*/
void guess();

/*Shows a guess to the user for a single cell X,Y*/
void guessHint();

/*Give a hint to the user by showing the solution of the input cell*/
void hint(int x, int y,Game* game);

/*the Set value part of autofill function*/
void setAF(int x, int y, int z, Game* game);

/*Receives an array of points and its size - filling the relevant cells with the single value*/
void fillSingleValue(Point** valArray,int cnt,Game* game);

/*Receives coordinates of a cell and return the single valid value of this cell if exists
 * otherwise - return 0;
 */
int singleValidValue(int col,int row,Game* game);


/*Automatically fill obvious values - cells which contain a single legal value.
 * return 0 if the board has erroneous, otherwise 1*/
int autofill(Game* game);

#endif /*SUDOKU_CONSOLE_SOLVEFUNC_H*/
