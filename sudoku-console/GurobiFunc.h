/**
 * GurobiFunc Summary:
 * This module contains all gurobi set-up and execution functions
 * and defines the structs we are using in this module
 *
 * Main objects in this module:
 * Truple struct - contains i, j, k
 *                 where i represents row, j represent col
 *                 and k represent value
 * transArray - a Truple array: every index represents the number of the variable
 *              in the gurobi model and each cell contains a Truple which is
 *              i, j, k that represent row, col, value.
 *              For example, let's say transArray[8] contains the Truple 2, 4, 7
 *              it means that the variable X2,4,7 is named 8(since index is 8).
 * rowsCon - a 3dim int array: every cell holds the number of the variable
 *           in the gurobi model which belongs to the corresponding constraint.
 *           For example, rowsCon[2][4] is a one dim array of ints that holds
 *           all the numbers of the variables in the gurobi model which are in
 *           form X3,5,k for any k
 * There are 3dim arrays for cols and blocks as well.
 *
 *
 * Public Functions:
 * solveLP - Solves the current board using Linear Programming.
 * solveILP - Solves the current board using Integer Linear Programming.
 *
 * Private Functions:
 * addVars - Add vars to model
 * updateModel - Updates model
 * addCellCons - Add constraints to each cell
 * freeHelpArr - Free helpful arrays of gurobi
 * freeArr - Free all the malloc we made in order to make the gurobi optimize the model
 * freeEverything - Free everything that was allocated
 * initArr - initialize the malloc of the given arrays
 * fillArray - Fills the transArray with the corresponding Truple values
 * addCons - Add constraints to the model from the given array - could be rows, cols or blocks
 * initGrbArr - initialize mallocWithGuard of gurobi needed arrays
 * turnOffPrintAndCreateModel - Turn printing off in gurobi and creates the model
 * setVarType - Set var type of the vtype array, it will be set to set
 *              where set can be GRB_BINARY and etc
 * optimizeModel - Optimize model
 * clearLP - Set all auxiliary arrays to 0
 * clearIlpVal - Set all ilp values to 0
 * randCoeff - Randomize the coefficients(range: 1-N) of the objective function
 *             Only happens when we are in LP
 * fillObjFun - Fills the coefficients of the objection function
 * solAssign - Fills the relevant cell's ILPVals or Auxiliary arrays
 * addAllCons - Add all the relevant constraints
 * solveGenral - General solve function which will use both LP and ILP
 *
 */


#ifndef SUDOKU_CONSOLE_GUROBIFUNC_H
#define SUDOKU_CONSOLE_GUROBIFUNC_H

#include "Game.h"
#include "Stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gurobi_c.h"

/*Truple struct - contains i, j, k
 * where i represents row, j represent col
 * and k represent value*/
typedef struct{
    int i;
    int j;
    int k;
} Truple;

/*Solves the current board using Linear Programming
 * Fills the probs array of the relvant cells in the board
 * guess = 0 fills all cells, guessHint = 1 fills only X Y cell
 * Return 0 if there is no solution*/
int solveLP (Game* game, int opCode, int x, int y);

/*Solves the current board using Integer Linear Programming
 * Fills the relevant cell's ILPVals
 * generate = 0 fills all cells, hint = 1 fills only X Y cell, validate = 2
 * Returns 0 if there is no solution*/
int solveILP (Game* game, int opCode, int x, int y);

#endif

