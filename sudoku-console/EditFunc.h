/*
 *  Created by CMY on 29/07/2019.
 */

#ifndef SUDOKU_CONSOLE_EDITFUNC_H
#define SUDOKU_CONSOLE_EDITFUNC_H
#define ERRORBOARD "Board contains erroneous, please fix it before saving\n"
#define NOSOL "Board doesn't have a solution, please fix it before saving\n"

#include "Game.h"
#include "GurobiFunc.h"


/*Generates a puzzle by randomly filling number of cells provided by user*/
void generate();


int saveEdit(Game* game);

/*Checks if there are errors in the board*/
int checkError(Game* game);
#endif /*SUDOKU_CONSOLE_EDITFUNC_H*/
