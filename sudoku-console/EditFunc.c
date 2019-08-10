/*
* Created by CMY on 29/07/2019.
*/

#include "EditFunc.h"
#include <stdio.h>
#include "GurobiFunc.h"
/*Generates a puzzle by randomly filling number of cells provided by user*/
void generate(){

}

/*Checks whether the board can be solved (1) or not (0)*/
/*uses the ILP solver*/
int validate(Game* game){
	int ilp;
	/*ToDO - update*/
	ilp = solveILP(game, 2, 0, 0);
    return ilp;
}


int saveEdit(Game* game){
    int isVal, isErr;
    isErr = checkError(game);
    if(isErr == 0){
        printf(ERRORBOARD);
        return 0;
    }
    isVal = validate(game);
    if (isVal == 1){
        return 1;
    }
    else
    {
        printf(NOSOL);
        return 0;
    }
}

/*Checks if there are errors in the board*/
int checkError(Game* game){
    if(game->numOfErrors == 0){
        return 1;
    }
    return 0;
}