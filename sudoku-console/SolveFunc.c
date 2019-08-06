/*
*	Created by CMY on 29/07/2019.
 */

#include "SolveFunc.h"
/*Update the "mark errors" setting according to users input */
void updateMarkErrors(Game* game, int setValue){
	game->board.markError = setValue;
}

/*Checks whether the x y cell contains z,
 * if it does - marks it as an erroneous and updates the errors counter*/
void checkAndMarkCellError(int x, int y, int z, Game* game){
    Point block = getBlockIndex(x,y, game);
    Point cell = getCellIndex(x,y, game);
    if(game->board.board[block.y][block.x].block[cell.y][cell.x].val == z) {
        game->board.board[block.y][block.x].block[cell.y][cell.x].appendix = '*';
        game->board.board[block.y][block.x].block[cell.y][cell.x].cntErr++;
        game->numOfErrors++;
    }
}

/*Marks the erroneous cells with '*' */
void markErrors(int x, int y, int z,Game* game){
    int i, j;
    Point block = getBlockIndex(x,y, game);
    int id = pointToID(block.x,block.y,game);
    if(isValidValue(x,y,z,game) == 1){
        printf("here");
        return;
    }
    /*If we got here it means that this is an erroneous and we need to mark it*/

    /*Mark the current cell as erroneous*/
    checkAndMarkCellError(x,y,z,game);

    /*Mark every erroneous cell in the row*/
    if((*game).rows[y - 1][z-1] == 1){
        for(i = 1; i <= game->m*game->n; i++){
            if(i == x){
                continue;
            }
            checkAndMarkCellError(i,y,z,game);
        }
    }

    /*Mark every erroneous cell in the column*/
    if((*game).cols[x - 1][z-1] == 1){
        for(i = 1; i <= game->m*game->n; i++){
            if(i == y){
                continue;
            }
            checkAndMarkCellError(x,i,z,game);
        }
    }

    /*Mark every erroneous cell in the block*/
    if((*game).blocks[id][z-1] == 1){
        for(i = 0; i < game->m; i++){
            for(j = 0; j < game->n; j++){
                if(game->board.board[block.y][block.x].block[i][j].val == z){
                    game->board.board[block.y][block.x].block[i][j].appendix = '*';
                    game->board.board[block.y][block.x].block[i][j].cntErr++;
                    game->numOfErrors++;
                }
            }
        }
    }
}
/*Fills all cell values with probability greater than users input*/
void guess(){

}

/*Shows a guess to the user for a single cell X,Y*/
void guessHint(){

}

/*Give a hint to the user by showing the solution of the input cell*/
void hint(){

}

/*Automatically fill obvious values - cells which contain a single legal value.*/
void autofill(){

}
