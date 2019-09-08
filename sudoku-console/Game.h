/**
 * Game Summary:
 * This module contains all game initialize,free,set-up functions
 * and defines the main struct(game) we are using in the program
 *
 * Main objects in this module:
 * Game struct - Contains relavant game fields such as board, dimensions and etc
 *               All the fields are described next to the game declaration.
 *
 * Public Functions:
 * removeTail - In case of adding a move in the middle of game.moves List - Clearing the redo part of the list
 * addMove - Receives an array of points and its size and adding to game.moves
 * printBoard - Prints the current board
 * markErrors - Marks the erroneous cells that cell x,y caused
 * updateBlock - updates blocks[blockID][value]
 * updateCol - updates cols[col][value]
 * updateRow - updates rows[row][value]
 * pointToID - Receives a point representing the block index and return the relative ID
 * freeMem - Free malloc allocations
 * getCellIndex - Gets x and y from user and returns the normalized points
 * getBlockIndex - Gets x and y from user and returns the normalized Block points
 * isValidValue - Receives a cell <x,y> and a value z and return 1 if z is a valid value for that cell, 0 otherwise
 * initAll - Initialize all memory allocations
 * emptyBoard - Empties the board completely
 * isFixed - Checks whether the cell in col x and row y is fixed or not
 * isFilled - Checks whether the cell in col x and row y is filles or not
 * updateAllArrs - Computes all helpful arrays from the beginning
 * fillDoubleArray - Fill double array with set
 * fillZeroesCntErr - Fills all cntErr field of the whole board with 0's
 * deepCopyGame - Deep copy of struct game from gameToCopy to game
 * fillIntArr - Fills the given array with set
 *
 * Private Functions:
 * initMoves - Creates LinkedList of moves for game
 * initBoard - Creates board mallocs and call to initBlockCells
 * initBlocks - Sets the block cells to be ' '
 * initBlockCells - Creates a malloc for all the blocks and call to initBlock
 * initRows - initialize the binary array of possible numbers
 *            in rows[i] for each i
 * initCols - initialize the binary array of possible numbers
 *            in cols[i] for each i
 * initBlocks - initialize the binary array of possible numbers
 *              in blocks[i] for each i
 * printDashes - Prints dashes
 * setZero - set rows, cols and blocks to zero
 * freeAux - Frees malloc of auxiliary
 * checkAndMarkCellError - Checks whether the x y cell contains z,
 *                         if it does - marks it as an erroneous and updates the errors counter.
 * deepCopyArrDouble - Copies array double values from arrToCopy to newArr
 * deepCopyBoard - Copies the values of the board
 * deepCopyArr - Copies array int values from arrToCopy to newArr
 * deepCopyHelpArr - Copies all values of helpful arrays
 *
 */

#ifndef SUDOKU_CONSOLE_GAME_H
#define SUDOKU_CONSOLE_GAME_H

#include "Board.h"
#include "LinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BOARDISVALID "The current board is solvable.\n"
#define BOARDISNOTVALID "The current board is NOT solvable.\n"
#define ERROROPENFILE "Error: Failed opening file.\n"
#define FAILEDREADINGFILE "Error: Failed reading file.\n"
#define PUZZLESOLVED "Well Done! \nThe puzzle was solved successfully.\nThe game is now set to Init mode.\n"
#define ERRORSOL "Sorry..\nThe solution is erroneous.\n"
#define errorErroneous "Error: The board is erroneous and therefore %s can't be executed\n"
#define FUNFAILGUR "Error: The function %s has failed because of Gurobi.\nPlease Try again.\n"

/* Game contains:
 * m - num of column in block which is also num of block rows in board
 * n - num of rows in block which is also num of block cols in board
 * board - Game board, array n*m of blocks
 * rows - Contains a helpful array of possible numbers in rows[i] for each i
 * cols - Contains a helpful array of possible numbers in cols[i] for each i
 * blocks - Contains a helpful array of possible numbers in blocks[i] for each i
 * memRelease - 1 if there is memory to release, 0 otherwise
 * cellsToFill - number of cells still empty
 * mode - 0 for init, 1 for edit, 2 for solve.
 * moves - a linked list of moves done on the board
 * numOfErrors - number of errors in the current board
 * */

typedef struct{
    int m;
    int n;
    Board board;

    int** rows;
    int** cols;
    int** blocks;

    int memRelease;
    int cellsToFill;

    int mode;
    LinkedList* moves;
    int numOfErrors;

}Game;

/*In case of adding a move in the middle of game.moves List - Clearing the redo part of the list*/
void removeTail(Game* game);

/*Receives an array of points and its size and adding to game.moves*/
void addMove(Point** points, int cnt, Game* game);

/*Prints the current board*/
void printBoard(Game* game);

/*Marks the erroneous cells that cell x,y caused*/
void markErrors(int x, int y, int z,Game* game);

/* Gets x - column and y - row from user and returns the normalized cell points*/
Point getCellIndex (int x, int y, Game* game);

/* Gets x - column and y - row from user and returns the normalized block points*/
Point getBlockIndex (int x, int y, Game* game);

/*Receives a cell <x,y> and a value z and return 1 if z is a valid value for that cell, 0 otherwise */
int isValidValue (int x, int y, int z,Game *game);

/*Receives a point representing the block index and return the relative ID */
int pointToID(int x,int y, Game *game);

/*updates blocks[blockID][value] according to set: 1 = ++, 0 = -- */
void updateBlock(int blockID, int value, int set, Game* game);

/*updates cols[col][value] according to set: 1 = ++, 0 = -- */
void updateCol(int col, int value, int set, Game* game);

/*updates rows[row][value] according to set: 1 = ++, 0 = -- */
void updateRow(int row, int value, int set, Game* game);

/*Free malloc allocations*/
void freeMem(Game* game);

/*Initialize all memory allocations*/
void initAll (Game* game);

/*Empties the board completely*/
void emptyBoard(Point** moveCell, Game* game);

/*Checks whether the cell in col x and row y is fixed or not*/
int isFixed(int x, int y, Game* game);

/*Checks whether the cell in col x and row y is filled(1) or not(0)*/
int isFilled(int x, int y, Game* game);

/*Computes all helpful arrays from the beginning*/
int updateAllArrs(Point** moveCell, int cnt, Game* game);

/*Fill double array with set*/
void fillDoubleArray(double** arr, int len, double set);

/*Fills all cntErr field of the whole board with 0's'*/
void fillZeroesCntErr(Game* game);

/*Deep copy of struct game from gameToCopy to game*/
void deepCopyGame(Game* newGame, Game* gameToCopy);

/*Fills the given array with set*/
void fillIntArr(int **arr, int len, int set);

#endif /*SUDOKU_CONSOLE_GAME_H*/
