/*
 *	Created by CMY on 29/07/2019.
 */

#ifndef SUDOKU_CONSOLE_GAME_H
#define SUDOKU_CONSOLE_GAME_H

#include "Board.h"
#include "LinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BOARDISVALID "Board is valid\n"
#define BOARDISNOTVALID "Board is not valid\n"
#define ERRORSOLVEMODE "Error: This command is available in Solve mode ONLY.\n"
#define MALLOCFAIL "Error: malloc has failed\n"
#define ERROROPENFILE "Error: failed opening file\n"
#define FAILEDREADINGFILE "Error: failed reading file\n"
#define PUZZLESOLVED "Well Done! \nThe puzzle was solved successfully.\nThe game is now set to Init mode\n"
#define ERRORSOL "Sorry..\nThe solution is erroneous.\n"
#define errorErroneous "Error: The board is erroneous and therefore %s can't be executed\n"
#define FUNFAILGUR "Error: The function %s has failed because of Gurobi.\nPlease Try again.\n"

/* Sudoku contains:
 * m - num of column in block which is also num of block rows in board
 * n - num of rows in block which is also num of block cols in board
 * board - Game board, array n*m of blocks
 * rows - Contains a binary array of possible numbers in rows[i] for each i
 * cols - Contains a binary array of possible numbers in cols[i] for each i
 * blocks - Contains a binary array of possible numbers in blocks[i] for each i
 * memRelease - 1 if there is memory to release, 0 otherwise
 * cellsToFill - number of cells still empty
 * mode - 0 for init, 1 for edit, 2 for solve.
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
    int numOfErrors; /*Will be updated each time set command is called*/

}Game;

/*In case of adding a move in the middle of game.moves List - Clearing the redo part of the list*/
void removeTail(Game* game);

/*Receives an array of points and its size and adding to game.moves*/
void addMove(Point** points, int cnt, Game* game);

/*Checks whether the x y cell contains z,
 * if it does - marks it as an erroneous and updates the errors counter*/
int checkAndMarkCellError(int x, int y, int z, Game* game);

/* Creates LinkedList of moves for game*/
void initMoves(Game* game);

/*Sets the block cells to be ' '*/
void initBlock (int i, int j, Game * game);

/*Creates a malloc for all the blocks and call to initBlock*/
void initBlockCells(Game * game) ;

/* Creates board mallocs and call to initBlockCells*/
void initBoard(Game* game) ;

/*Prints the current board*/
void printBoard(Game* game);

/*Marks the erroneous cells with '*' */
void markErrors(int x, int y, int z,Game* game);

/*prints dashes*/
void printDashes(int numOfDashes);

/* Gets x - column and y - row from user and returns the normalized cell points*/
Point getCellIndex (int x, int y, Game* game);

/* Gets x - column and y - row from user and returns the normalized block points*/
Point getBlockIndex (int x, int y, Game* game);

/*Receives a cell <x,y> and a value z and return 1 if z is a valid value for that cell, 0 otherwise */
int isValidValue (int x, int y, int z,Game *game);

/*Receives a point representing the block index and return the relative ID */
int pointToID(int x,int y, Game *game);

/*updates blocks[blockID][value] = set */
void updateBlock(int blockID, int value, int set, Game* game);

/*updates cols[col][value] = set */
void updateCol(int col, int value, int set, Game* game);

/*updates rows[row][value] = set */
void updateRow(int row, int value, int set, Game* game);

/*initialize the binary array of possible numbers
*  in rows[i] for each i*/
void initRows(Game* game);

/*initialize the binary array of possible numbers
*  in blocks[i] for each i*/
void initBlocks(Game* game);

/*initialize the binary array of possible numbers
*  in cols[i] for each i*/
void initCols(Game* game);

/*set rows, cols and blocks to zero*/
void setZero(Game* game);

/*Fills the given array with 0's*/
void fillZeroes(int *arr, Game* game);

/*Free malloc allocations*/
void freeMem(Game* game);


/*Initialize all memory allocations*/
void initAll (Game* game);

/*Empties the board completely*/
void emptyBoard(Point** moveCell, Game* game);

/*Checks whether the cell in col x and row y is fixed or not*/
int isFixed(int x, int y, Game* game);

/*Computes all helpful arrays from the beginning*/
int updateAllArrs(Point** moveCell, int cnt, Game* game);

/*Fill double array with zeroes*/
void fillDoubleArray(double** arr, int len, double set);

/*Fills all cntErr field of the whole board with 0's'*/
void fillZeroesCntErr(Game* game);

#endif /*SUDOKU_CONSOLE_GAME_H*/
