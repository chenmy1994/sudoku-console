/*
*	Created by CMY on 29/07/2019.
 */

#ifndef SUDOKU_CONSOLE_GENERALFUNC_H
#define SUDOKU_CONSOLE_GENERALFUNC_H
#define EXITGAME "Exiting game...\n"

#include "EditFunc.h"
#include "SolveFunc.h"
#include "Stack.h"
#include <stdlib.h>
#include <stdio.h>
#include "GurobiFunc.h"

#define BOARDSAVED "Board saved to: %s\n"
#define ERRORSETSOLVE "In Solve mode it is NOT possible to set a fixed cell.\n"

#define UNDOLIMIT "Error: There are no more moves to Undo\n"
#define REDOLIMIT "Error: There are no more moves to Redo\n"

void popAndUpdate(Game* game,Stack** stk);

/*Checks whether the x y cell contains z,
 * if it does - marks it as an erroneous and updates the errors counter*/
int checkAndMarkCellError(int x, int y, int z, Game* game);

/*Dealing with the edit command received by user*/
int edit(char* X, Game* game);

/*Dealing with the exit command received by user*/
void exitGame(Game* game);

/*Dealing with the solve command received by user*/
int solve(char* X, Game* game);

/*Saves the current game board to the specified file*/
void save(char* X, Game* game);

/*Set new value of cell
 *  * receives an indicator (1) if the set command was sent from a undo or redo command, 0 otherwise*/
int set(int x, int y, int z, Game* game,int undoOrRedo);

/*Revert the last move done by the user according to the linked list current move (updates to previous move)
 * also receives an indicator=1 'reset' preventing output for every move undone if undo was called from reset*/
int undo(Game* game,int reset);

/*Cancel the last revert action according to the linked list current move (updates to next move)*/
int redo(Game* game);

/*Prints the number of solution for the current board*/
int num_solutions(Game* game);

/*Undo all moves, reverting the board to its original loaded state.*/
int reset(Game* game);

/*Fills the game board with the values given from the file in X*/
int fillBoard(char* X, Game* game, int mode);

/*Fills the game board with the values given from the file in X*/
int loadBoard(char* X, Game* game, int mode);

/*Changes the mode of the game to the newMode*/
void changeMode(int newMode, Game* game);

/*Checks whether char n represent a digit or dot or not*/
int isDigitOrDot(char n);

/*Creates an empty 9x9 board*/
void createEmptyBoard(Game* game);

/*builds number from current place on char* buff*/
int buildNumber (char* buff, int* i);

/*Update the "mark errors" setting according to users input */
void updateMarkErrors(Game* game, int setValue);

/*prints that reading file has failed and closes fp*/
void failedReadingFile(FILE** fp, Game* game);

/*Return 1 if there is no contradiction between this cell to another fixed cell
 * Otherwise return 0*/
int checkCellValid(int x, int y, int z, Game* game);

/*Checks whether the board can be solved (1) or not (0)*/
/*uses the ILP solver*/
int validate(Game* game, int ind);

#endif /*SUDOKU_CONSOLE_GENERALFUNC_H*/
