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
#define BOARDSAVED "Board saved to: %s\n"
#define ERRORSETSOLVE "In Solve mode it is NOT possible to set a fixed cell.\n"
#define PUZZLESOLVED "Well Done! \n The puzzle was solved successfully.\n"
#define ERRORSOL "Sorry..\n The solution is erroneous.\n"


/*Checks whether the x y cell contains z,
 * if it does - marks it as an erroneous and updates the errors counter*/
void checkAndMarkCellError(int x, int y, int z, Game* game);

/*Dealing with the edit command received by user*/
int edit(char* X, Game* game);

/*Dealing with the exit command received by user*/
void exitGame(Game* game);

/*Dealing with the solve command received by user*/
int solve(char* X, Game* game);

/*Saves the current game board to the specified file*/
/*TODO - should be splitting between SolveFunc and EditFunc */
void save(char* X, Game* game);

/*Set new value of cell*/
void set(int x, int y, int z, Game* game);

/*Revert the last move done by the user according to the linked list current move (updates to previous move)*/
void undo();

/*Cancel the last revert action according to the linked list current move (updates to next move)*/
void redo();

/*Prints the number of solution for the current board*/
void num_solutions();

/*Undo all moves, reverting the board to its original loaded state.*/
void reset();

/*Checks whether the cell in col x and row y is fixed or not*/
int isFixed(int x, int y, Game* game);

/*Fills the game board with the values given from the file in X*/
int fillBoard(char* X, Game* game);

/*Fills the game board with the values given from the file in X*/
int loadBoard(char* X, Game* game);

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

/*Marks the erroneous cells with '*' */
void markErrors(int x, int y, int z,Game* game);

/*prints that reading file has failed and closes fp*/
void failedReadingFile(FILE** fp);


/*Return 1 if there is no contradiction between this cell to another fixed cell
 * Otherwise return 0*/
int checkCellValid(int x, int y, int z, Game* game);
#endif /*SUDOKU_CONSOLE_GENERALFUNC_H*/
