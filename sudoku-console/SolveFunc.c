/*
*	Created by CMY on 29/07/2019.
 */

#include "SolveFunc.h"
/*Update the "mark errors" setting according to users input */
void updateMarkErrors(Game* game, int setValue){
	game->board.markError = setValue;
}

/*Marks the erroneous cells with '*' */
void markErrors(Game* game, int x, int y, int z){
    int i;
    Point p = getBlockIndex(x,y, game);
    Point cell = getCellIndex(x,y, game);
    if(((*game).rows[y - 1][z-1]==0)&&((*game).cols[x - 1][z-1]==0)&&((*game).blocks[pointToID(p.x,p.y,game)][z-1]==0)) {
        return;
    }
    game->board.board[p.y][p.x].block[cell.y][cell.x].appendix = '*';
    game->numOfErrors++;
    if((*game).rows[y - 1][z-1] == 1){
        for(i = 0; i < game->m*game->n; i++){
            p = getBlockIndex(i,y, game);
            cell = getCellIndex(i,y, game);
            if(i == x){
                continue;
            }
            if(game->board.board[p.y][p.x].block[cell.y][cell.x].val == z){
                game->board.board[p.y][p.x].block[cell.y][cell.x].appendix = '*';
                game->numOfErrors++;
            }
        }
    }

    if((*game).cols[x - 1][z-1] == 1){
        for(i = 0; i < game->m*game->n; i++){
            p = getBlockIndex(x,i, game);
            cell = getCellIndex(x,i, game);
            if(i == y){
                continue;
            }
            if(game->board.board[p.y][p.x].block[cell.y][cell.x].val == z){
                game->board.board[p.y][p.x].block[cell.y][cell.x].appendix = '*';
                game->numOfErrors++;
            }
        }
    }
/*TODO - Check which cell is erroneous in block and mark it*/
    if((*game).blocks[pointToID(p.x,p.y,game)][z-1] == 1){
        for(i = 0; i < game->m*game->n; i++){
            p = getBlockIndex(i,y, game);
            cell = getCellIndex(i,y, game);
            if(game->board.board[p.y][p.x].block[cell.y][cell.x].val == z){
                game->board.board[p.y][p.x].block[cell.y][cell.x].appendix = '*';
                game->numOfErrors++;
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
