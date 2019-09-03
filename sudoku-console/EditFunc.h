/*
 *  Created by CMY on 29/07/2019.
 */

#ifndef SUDOKU_CONSOLE_EDITFUNC_H
#define SUDOKU_CONSOLE_EDITFUNC_H
#define ERRORBOARD "Error: board contains erroneous, please fix it before saving.\n"
#define NOTENOUGHCELLS "Error: the board doesn't contain %d empty cells.\n"
#define ERRORINPUZZLEGEN "Error: Generate function tried 1000 iterations and couldn't succeed.\n"

#include "Game.h"
#include "GurobiFunc.h"


/*Fills the given array with points that represent the empty cells in the board*/
void fillEmptyCellsArray(Game* game, Point** emptyCells);

/*Generates a puzzle by randomly filling number of cells provided by user*/
int generate(int x, int y, Game* game);

/*Save has extra limitations in edit mode*/
int saveEdit(Game* game);

/*Checks if there are errors in the board*/
int checkError(Game* game);

/*Reset board to original state before starting generate function*/
int resetBoardOnGenerate(Point** moveCell, Game* game, int opCode, int resetCellsToFill);

/*fills the given col, row in the board with a valid value and update the relevant arrays*/
int fillValid(int col, int row, Game* game);

/*Chooses an empty cell from the board and returns it*/
Point chooseEmptyCell(Game* game);

/*Randomly chooses y cells from the board and clear the values of all other cells*/
void chooseYCellsAndClearThem(Point** moveCell, Game* game, int y);


#endif /*SUDOKU_CONSOLE_EDITFUNC_H*/
