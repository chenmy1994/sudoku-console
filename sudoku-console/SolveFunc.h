/*
*	Created by CMY on 29/07/2019.
 */

#ifndef SUDOKU_CONSOLE_SOLVEFUNC_H
#define SUDOKU_CONSOLE_SOLVEFUNC_H
#include "Game.h"
#include "GurobiFunc.h"
#define CELLISFIXED "Error: cell <%d, %d> is fixed.\n"
#define CELLISFILLED "Error: cell <%d, %d> is filled.\n"

/*Fills all cell values with probability greater than users input*/
int guess(double x, Game* game);

/*Shows a guess to the user for a single cell X,Y*/
void guessHint(int x, int y, Game* game);

/*When given cell x,y (col, row) it prints all the values that got a score higher than 0
 * on the LP solver and their scores.*/
void printGuessAndScores(int x, int y, Game* game);

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

/* Fills the values of each cell by the solution we got from solveLP
 * Also computes it by the orders in the excerise*/
void fillGuessValues(Game* game, double threshold);

/* When given cell x - col, y - row, it randomize a legal value
 * Legal value means it is possible for it to be put in that cell
 * and has a higher score than the threshold*/
int randValue(int x, int y, Point* moveCell, Game* game, double threshold);


/* Fill the given cell x - col, y - row with the value z*/
void fillValue(int x, int y, int z, Game* game);

#endif /*SUDOKU_CONSOLE_SOLVEFUNC_H*/
