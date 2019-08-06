/*
*	Created by CMY on 29/07/2019.
 */

#include "GeneralFunc.h"

/*Dealing with the edit command received by user*/
void edit(char* X, Game* game){
    loadBoard(X, game);
    /*change mode to edit mode*/
    changeMode(1, game);
}

/*Dealing with the exit command received by user*/
void exitGame(){

}

/*Dealing with the solve command received by user*/
void solve(char* X, Game* game){

}

void unMarkErrorsCell(int x, int y, int z, Game* game){
    Point block = getBlockIndex(x,y, game);
    Point cell = getCellIndex(x,y, game);
    if(z == 0){
        game->board.board[block.y][block.x].block[cell.y][cell.x].cntErr = 0;
        game->board.board[block.y][block.x].block[cell.y][cell.x].appendix = ' ';
    }
    else if(game->board.board[block.y][block.x].block[cell.y][cell.x].val == z){
            if(--game->board.board[block.y][block.x].block[cell.y][cell.x].cntErr == 0){
                game->board.board[block.y][block.x].block[cell.y][cell.x].appendix = ' ';
                game->numOfErrors--;
            }
    }
}


/*Unmark erroneous that was caused from cell in col x and row y and value z*/
void unMarkErrors(int x, int y, int z, Game* game){
    int i, j;
    Point block = getBlockIndex(x,y, game);
    int id = pointToID(block.x,block.y,game);

    /*Mark the current cell as erroneous*/
    unMarkErrorsCell(x,y,0,game);

    /*UnMark every erroneous cell in the row*/
    if((*game).rows[y - 1][z-1] == 1){
        for(i = 1; i <= game->m*game->n; i++){
            if(i == x){
                continue;
            }
            unMarkErrorsCell(i,y,z,game);
        }
    }

    /*UnMark every erroneous cell in the column*/
    if((*game).cols[x - 1][z-1] == 1){
        for(i = 1; i <= game->m*game->n; i++){
            if(i == y){
                continue;
            }
            unMarkErrorsCell(x,i,z,game);
        }
    }

    /*UnMark every erroneous cell in the block*/
    if((*game).blocks[id][z-1] == 1){
        for(i = 0; i < game->m; i++){
            for(j = 0; j < game->n; j++){
                /*Might change it here to a function that computes the relvant x y from i and j and block id*/
                if(game->board.board[block.y][block.x].block[i][j].val == z){
                    if(--game->board.board[block.y][block.x].block[i][j].cntErr == 0){
                        game->board.board[block.y][block.x].block[i][j].appendix = ' ';
                        game->numOfErrors--;
                    }
                }
            }
        }
    }
}
/*Saves the current game board to the specified file*/
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
    int prevVal;
    block = getBlockIndex(x,y,game);
    cell = getCellIndex(x,y,game);
    prevVal = game->board.board[block.y][block.x].block[cell.y][cell.x].val;

    /*If we are in Solve mode*/
    if (game->mode == 2){
        if(isFixed(x,y, game) == 1){
            printf(ERRORSETSOLVE);
            return;
        }
    }

    /* If user entered a new val to an empty cell*/
    if (z != 0 && prevVal == 0){
        game->board.board[block.y][block.x].block[cell.y][cell.x].val = z;
        markErrors(x, y, z, game);
        printf("%d %d %d %d", x, y, z, prevVal);
        game->cellsToFill--;
    }
    /*If user deleted a cell*/
    else if (z == 0){
        unMarkErrors(x, y, prevVal, game);
        game->board.board[block.y][block.x].block[cell.y][cell.x].val = z;
    }
    /*If user overidden a value with a new valid value */
    else {
        unMarkErrors(x, y, prevVal, game);
        game->board.board[block.y][block.x].block[cell.y][cell.x].val = z;
        markErrors(x, y, z, game);
    }

    /*TODO - need to add what happens when we need to delete erroneous*/
    if(game->mode == 2 && game->cellsToFill == 0){
        if(game->numOfErrors == 0){
            printf(PUZZLESOLVED);
            /*Change mode to init*/
            game->mode = 0;
            return;
        }
        printf(ERRORSOL);
        return;
    }
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