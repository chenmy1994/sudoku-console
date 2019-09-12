/**
 * GeneralFunc Summary:
 * This module contains functions that are available in more than one mode
 * and some helpful functions that are relavant in all modes
 *
 * Public Functions:
 * popAndUpdate - Pop the first object from top of stack and update the game helpful arrays
 * edit - Dealing with the edit command received by user
 * exitGame - Dealing with the exit command received by user
 * solve - Dealing with the solve command received by user
 * save - Saves the current game board to the specified file
 * set - Set new value of cell
 * undo - Revert the last move done by the user
 * redo - Cancel the last revert action
 * num_solutions - Prints the number of solution for the current board
 * reset - Undo all moves, reverting the board to its original loaded state
 * checkCellValid - checks if there is no contradiction between this cell to another fixed cell
 * validate - Checks whether the board can be solved (1) or not (0)
 * changeMode - Changes the mode of the game to the newMode
 * updateMarkErrors - Update the "mark errors" setting according to users input
 *
 *
 * Private Functions:
 * unMarkErrorsCell - Unmark erroneous on a given cell (x - col, y - row)
 * unMarkErrors - Unmark erroneous that was caused from cell in col x and row y and value z
 * failedReadingFile - prints that reading file has failed and closes fp
 * isDigit - Checks whether char n represent a digit or dot or not
 * isDot - Checks whether char n represent a dot or not
 * checkValidRange - Check that the given num is in valid range
 * fillBoard - Fills the game board with the values given from the file in X
 * createEmptyBoard - Creates an empty 9x9 board
 * loadBoard - Load the new board from X
 *
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
#define ERRORCLOSE "Error: Couldn't close file.\n"
#define UNDOLIMIT "Error: There are no more moves to Undo.\n"
#define REDOLIMIT "Error: There are no more moves to Redo.\n"
#define UNDOMSG "Undo: Deleted value %d from cell <%d,%d>\n"
#define REDOMSG "Redo: Re-Set value %d for cell <%d,%d>\n"


/*Pop the first object from top of stack and update
 * the game helpful arrays*/
void popAndUpdate(Game* game,Stack** stk);

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

/*Return 1 if there is no contradiction between this cell to another fixed cell
 * Otherwise return 0*/
int checkCellValid(int x, int y, int z, Game* game);

/*Checks whether the board can be solved (1) or not (0)*/
/*uses the ILP solver*/
int validate(Game* game, int ind);

/*Changes the mode of the game to the newMode*/
void changeMode(int newMode, Game* game);

/*Update the "mark errors" setting according to users input */
void updateMarkErrors(Game* game, int setValue);

#endif /*SUDOKU_CONSOLE_GENERALFUNC_H*/
