//
// Created by CMY on 30/07/2019.
//
#include "Game.h"
#include "gurobi_c.h"
#include <stdlib.h>
#include <stdio.h>
#include "LinkedList.h"
typedef struct{
    int i;
    int j;
    int k;
} Truple;
#ifndef SUDOKU_CONSOLE_GUROBIFUNC_H
#define SUDOKU_CONSOLE_GUROBIFUNC_H

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
#endif //SUDOKU_CONSOLE_GUROBIFUNC_H