//
// Created by CMY on 29/07/2019.
//

#ifndef SUDOKU_CONSOLE_GAME_H
#define SUDOKU_CONSOLE_GAME_H
#include "Board.h"
#include "LinkedList.h"
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

    int **rows;
    int **cols;
    int **blocks;

    int memRelease;
    int cellsToFill;

    int mode;
    LinkedList moves;
}Game;

/*Sets the block cells to be ' '*/
void initBlock (int i, int j, Game * game);

/*Creates a malloc for all the blocks and call to initBlock*/
void initBlockCells(Game * game) ;

/* Creates board mallocs and call to initBlockCells*/
void initBoard(Game* game) ;


/*Prints the current board*/
void printBoard(Game* game);

/*prints dashes*/
void printDashes(int numOfDashes);

/* Gets x - column and y - row from user and returns the normalized cell points*/
Point getCellIndex (int x, int y, Game* game);

/* Gets x - column and y - row from user and returns the normalized block points*/
Point getBlockIndex (int x, int y, Game* game);

#endif //SUDOKU_CONSOLE_GAME_H
