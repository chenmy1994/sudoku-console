/*
 *  Created by CMY on 29/07/2019.
 */

#ifndef SUDOKU_CONSOLE_EDITFUNC_H
#define SUDOKU_CONSOLE_EDITFUNC_H
#define ERRORBOARD "Error: board contains erroneous, please fix it before saving.\n"
#define NOTENOUGHCELLS "Error: the board doesn't contain %d empty cells.\n"
#define ERRORINPUZZLEGEN "Error: Generate function tried 1000 iterations and couldn't succeed.\n"

#include "GurobiFunc.h"


/*Generates a puzzle by randomly filling number of cells provided by user*/
int generate(int x, int y, Game* game);

/*Save has extra limitations in edit mode*/
int saveEdit(Game* game);


#endif /*SUDOKU_CONSOLE_EDITFUNC_H*/
