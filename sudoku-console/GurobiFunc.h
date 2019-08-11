/*
*	Created by CMY on 30/07/2019.
 */
#ifndef SUDOKU_CONSOLE_GUROBIFUNC_H
#define SUDOKU_CONSOLE_GUROBIFUNC_H

#include "Game.h"
#include "Stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gurobi_c.h"

typedef struct{
    int i;
    int j;
    int k;
} Truple;


/*Add constraints to the model from the given array - could be rows, cols or blocks*/
int addCons(int N, int** ind, double** val, char* conName, int**** arr, GRBenv** env, GRBmodel** model);


/*initialize the malloc of the given arrays*/
void initArr(int**** arrR, int**** arrC, int**** arrB, int** ind, double ** val, int N);


/*Fills the transArray with the corresponding Truple values
 * and while doing so it adds the constraints per cell to the model*/
int fillArrayAndAddCellCon(Game* game, Truple** transArray, int* transCounter, int** ind, double** val,
                           int**** rowsCon, int**** colsCon, int**** blocksCon, GRBenv** env, GRBmodel** model);

/*Checks and handles error during Gurobi work*/
int checkAndHandleError(int error,GRBenv** env, char* string);

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

/* Fills the relevant cell's ILPVals
 * generate = 0 fills all cells, hint = 1 fills only X Y cell, validate = 2
 * Returns 0 if there is no solution*/
void solAssign (double** solArray,Truple** transArray, int transCounter, int opCode, int X, int Y, Game* game);


/*initialize malloc of gurobi needed arrays*/
void initGrbArr(double** sol,int** ind,double** val,double** obj,char** vtype,int transCounter);

/*Turn printing off in gurobi and creates the model*/
int turnOffPrintAndCreateModel(GRBenv** env, GRBmodel** model);
#endif /*SUDOKU_CONSOLE_GUROBIFUNC_H*/
