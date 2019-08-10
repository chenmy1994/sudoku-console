/*
 *  Created by CMY on 29/07/2019.
 */

#ifndef SUDOKU_CONSOLE_EDITFUNC_H
#define SUDOKU_CONSOLE_EDITFUNC_H
#include "Game.h"
#define ERRORBOARD "Board contains erroneous, please fix it before saving\n"
#define NOSOL "Board doesn't have a solution, please fix it before saving\n"

/*Generates a puzzle by randomly filling number of cells provided by user*/
void generate();


/*Checks whether the board can be solved (1) or not (0)*/
/*uses the ILP solver*/
int validate(Game* game);

int saveEdit(Game* game);

/*Checks if there are errors in the board*/
int checkError(Game* game);
#endif /*SUDOKU_CONSOLE_EDITFUNC_H*/
