/*
*	Created by CMY on 29/07/2019.
 */

#include "SolveFunc.h"

/*Fills all cell values with probability greater than users input*/
void guess(){

}

/*Shows a guess to the user for a single cell X,Y*/
void guessHint(){

}

/*Give a hint to the user by showing the solution of the input cell*/
void hint(Game* game,int x, int y){
    Point block, cell;
    int hint;
    block = getBlockIndex(x,y, game);
    cell = getCellIndex(x,y, game);

    if(game->mode != 2){
        printf(NOTINSOLVE);
        return;
    }

    if(solveILP(game, 1, x, y) == 1){
        hint = game->board.board[block.y][block.x].block[cell.y][cell.x].ILPVal;
        printf("Hint: set cell %d %d to %d",x , y, hint);
        return;
    }



}

/*Automatically fill obvious values - cells which contain a single legal value.*/
void autofill(){

}
