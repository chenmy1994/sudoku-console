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
int fillArray(Game* game, Truple** transArray, int* transCounter,
                           int**** rowsCon, int**** colsCon, int**** blocksCon);

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


/* Fills the coefficents of the objection function
 * ind = 0 means we are in ILP,
 * ind = 1 means we are in LP*/
void fillObjFun(double** obj, int ind);


/* Add the coefficients of the objective function to the model
 * and add all the relevant constraints */
int addAllCons(GRBmodel** model, GRBenv** env, int**** rowsCon, int**** colsCon, int**** blocksCon,
                      int** ind, double** val, int N);


/*Add vars to model*/
int addVars(GRBmodel** model,int transCounter, char** vtype, double** obj, GRBenv** env);

/*set var type of the vtype array, it will be set to set
 * where set can be GRB_BINART and etc..*/
void setVarType(char** vtype, int transCounter, char set);


/*General solve function which will use both LP and ILP
 * decided by the indicator.
 * indicator = 0 is ILP
 * indicator = 1 is LP*/
int solveGenral(int indicator, Game* game, int opCode, int x, int y);



/*Free all the malloc we made in order to make the gurobi optimize the model*/
void freeArr(double** sol, int** ind, double** val, double** obj, int N, Truple** transArray,
             char** vtype,int**** rowsCon, int**** colsCon, int**** blocksCon);


/*fill the arrays with zeroes*/
void cleanArr(int** ind, double ** val, int transCounter);

/*Add constraints to each cell*/
int addCellCons(int** ind, double** val, Truple** transArray,
                GRBenv** env, GRBmodel** model, int transCounter);
#endif /*SUDOKU_CONSOLE_GUROBIFUNC_H*/
