/*
* Created by CMY on 29/07/2019.
*/

#include "EditFunc.h"
#include <stdio.h>

/*Generates a puzzle by randomly filling number of cells provided by user*/
void generate(){

}


int saveEdit(Game* game){
    int isVal = 1, isErr;
    isErr = checkError(game);
    if(isErr == 0){
        printf(ERRORBOARD);
        return 0;
    }
    /*isVal = validate(game);*/

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