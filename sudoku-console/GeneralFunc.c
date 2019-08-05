/*
*	Created by CMY on 29/07/2019.
 */

#include "GeneralFunc.h"

/*Dealing with the edit command received by user*/
void edit(){

}

/*Dealing with the exit command received by user*/
void exitGame(){

}

/*Dealing with the solve command received by user*/
void solve(char* X, Game* game){

}

/*Saves the current game board to the specified file*/
/*TODO - should be splitting between SolveFunc and EditFunc */
void save(char* X, Game* game){
    FILE* fp;
    int i, j;
    Point block, cell;
    char appendix;
    if(game->mode==1){
        if(saveEdit(game) == 0){
            return;
        }
    }
    fp = fopen (X, "w+");
    fprintf(fp, "%d %d\n", game->m, game->n);

    for(i = 0; i < game->n * game->m; i++){
        for(j = 0; j < game->n * game->m; j++){
            block = getBlockIndex(j + 1, i + 1, game);
            cell = getCellIndex(j + 1, i + 1, game);
            fprintf(fp, "%d", (*game).board.board[block.y][block.x].block[cell.y][cell.x].val);
            appendix = (*game).board.board[block.y][block.x].block[cell.y][cell.x].appendix;
            if(appendix == '.'){
                fprintf(fp, "%c ", appendix);
            }
            if(j < game->n * game->m - 1){
                fprintf(fp, " ");
            }
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    printf(BOARDSAVED, X);
}

/*Set new value of cell*/
void set(int x, int y, int z, Game* game){
    Point block, cell;
    block = getBlockIndex(x,y,game);
    cell = getCellIndex(x,y,game);
    if (game->mode == 1){

    }
    else if (game->mode == 2){
        if(isFixed(x,y, game) == 1){
            printf(ERRORSETSOLVE);
            return;
        }
        game->board.board[block.y][block.x].block[cell.y][cell.x].val = z;
    }
    markErrors(game, x, y, z);
}

/*Checks whether the board can be solved (1) or not (0)*/
/*uses the ILP solver*/
int validate(Game* game){
	return 1;
}

/*Revert the last move done by the user according to the linked list current move (updates to previous move)*/
void undo(){

}

/*Cancel the last revert action according to the linked list current move (updates to next move)*/
void redo(){

}

/*Prints the number of solution for the current board*/
void num_solutions(){

}

/*Undo all moves, reverting the board to its original loaded state.*/
void reset(){

}

/*Checks whether the cell in col x and row y is fixed or not*/
int isFixed(int x, int y, Game* game){
    Point block, cell;
    block = getBlockIndex(x,y,game);
    cell = getCellIndex(x,y,game);
    if(game->board.board[block.y][block.x].block[cell.y][cell.x].appendix == '.'){
        return 1;
    }
    return 0;
}