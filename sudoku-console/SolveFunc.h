/*
*	Created by CMY on 29/07/2019.
 */

#ifndef SUDOKU_CONSOLE_SOLVEFUNC_H
#define SUDOKU_CONSOLE_SOLVEFUNC_H
#include "Game.h"
#include "GurobiFunc.h"


/*Fills all cell values with probability greater than users input*/
void guess();

/*Shows a guess to the user for a single cell X,Y*/
void guessHint();

/*Give a hint to the user by showing the solution of the input cell*/
void hint(Game* game, int x, int y);

/*Automatically fill obvious values - cells which contain a single legal value.*/
void autofill();

#endif /*SUDOKU_CONSOLE_SOLVEFUNC_H*/
