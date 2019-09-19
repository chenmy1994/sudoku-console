/**
 * EditFunc Summary:
 * This module contains functions that are available in edit mode only
 * and some helpful functions that are relevant in edit mode
 *
 * Public Functions:
 * generate - Generates a puzzle by randomly filling number of cells provided by user
 * saveEdit - Save has extra limitations in edit mode
 *
 * Private Functions:
 * fillEmptyCellsArray - Fills the given array with points that represent the empty cells in the board
 * chooseEmptyCell - Chooses an empty cell from the board and returns it
 * fillValid - Fills the given col, row in the board with a valid value and update the relevant arrays
 * resetBoardOnGenerate - Reset board to original state before starting generate function
 * chooseYCellsAndClearThem - Randomly chooses y cells from the board and clear the values of all other cells
 *
 */

#ifndef SUDOKU_CONSOLE_EDITFUNC_H
#define SUDOKU_CONSOLE_EDITFUNC_H
#define ERRORBOARD "Error: Board contains erroneous, please fix it before saving.\n"
#define NOTENOUGHCELLS "Error: The board doesn't contain %d empty cells.\nThe first argument should be in range 0-%d.\n"
#define ERRORINPUZZLEGEN "Error: Generate function tried 1000 iterations and couldn't succeed.\n"

#include "GurobiFunc.h"

/*Generates a puzzle by randomly filling number of cells provided by user*/
int generate(int x, int y, Game* game);

/*Save has extra limitations in edit mode*/
int saveEdit(Game* game);


#endif /*SUDOKU_CONSOLE_EDITFUNC_H*/
