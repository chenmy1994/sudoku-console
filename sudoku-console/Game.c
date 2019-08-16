/*
 *  Created by CMY on 29/07/2019.
 */

#include "Game.h"
/* Creates LinkedList of moves for game*/
void initMoves(Game* game) {
	LinkedList* lst=initLinkedList();
    game->moves= lst;
}

/* Creates board mallocs and call to initBlockCells*/
void initBoard(Game* game) {
    int i;
    (*game).board.board = (Block**) malloc ((*game).n * sizeof(Block*));
    if((*game).board.board == NULL )
    {
        /* Malloc failed, deal with it */
        printf(MALLOCFAIL);
    }
    for (i=0; i<(*game).n; i++){
        (*game).board.board[i] = (Block*) malloc((*game).m * sizeof(Block));
        if((*game).board.board[i] == NULL )
        {
            /* Malloc failed, deal with it */
            printf(MALLOCFAIL);
        }
    }

    initBlockCells(game);
}


/*Creates a malloc for all the blocks and call to initBlock*/
void initBlockCells(Game * game) {
    int j,k,l;
    for (j = 0; j < (*game).n ; j++) {
        for (k = 0; k < (*game).m; k++) {
            (*game).board.board[j][k].block = (Cell**) malloc((*game).m * sizeof(Cell*));

            if((*game).board.board[j][k].block == NULL )
            {
                /* Malloc failed, deal with it */
                printf(MALLOCFAIL);
            }
            for (l = 0; l<(*game).m;l++){
                (*game).board.board[j][k].block[l] = (Cell*) malloc((*game).n* sizeof(Cell));
                if((*game).board.board[j][k].block[l] == NULL )
                {
                    /* Malloc failed, deal with it */
                    printf(MALLOCFAIL);
                }
            }
            initBlock(j,k, game);
        }
    }
}

/*Sets the block cells to be ' '*/
void initBlock (int i, int j, Game* game){
    int k, l;
    for (l = 0; l < (*game).m ; l++) {
        for (k = 0 ; k < (*game).n; k++) {
            (*game).board.board[i][j].block[l][k].appendix = ' ';
            (*game).board.board[i][j].block[l][k].fixed = ' ';
            (*game).board.board[i][j].block[l][k].val = 0;
            (*game).board.board[i][j].block[l][k].auxiliary = (double*) calloc((*game).n*(*game).m, sizeof(double));
            (*game).board.board[i][j].block[l][k].cntErr = 0;
        }
    }

}

/*prints dashes*/
void printDashes(int numOfDashes){
    int i;
    for (i =0; i<numOfDashes; i++){
        printf("-");
        if (i == numOfDashes - 1){
            printf("\n");
        }
    }
}
/*Prints the current board*/
void printBoard(Game* game) {
    int i, j, rowLen, colLen, x, y;
    Point block, cell;
    char appendix, fixed;
    int numOfDashes;
    numOfDashes = 4*game->n*game->m + game->m + 1;
    y = 1;
    colLen = (*game).n * (*game).m + (*game).m + 1;
    rowLen = (*game).n * (*game).m ;
    for (j = 0; j < rowLen; j++) {
        if(j % (*game).m == 0){
            printDashes(numOfDashes);
        }
        x = 1;
        for (i = 0; i < colLen; i++) {
            if (i % ((*game).n + 1) == 0) {
                printf("|");

            }
            else {
                block = getBlockIndex(x, y, game);
                cell = getCellIndex(x, y, game);
                if ((*game).board.board[block.y][block.x].block[cell.y][cell.x].val == 0){
                    printf("    ");
                }
                else{
                    printf(" %2d",(*game).board.board[block.y][block.x].block[cell.y][cell.x].val);
                    appendix = (*game).board.board[block.y][block.x].block[cell.y][cell.x].appendix;
                    fixed = (*game).board.board[block.y][block.x].block[cell.y][cell.x].fixed;


                    if(game->board.markError == 0){
                        printf("%c",fixed);
                    }
                    else{
                        if(game->mode == 1){
                            if(appendix == '*'){
                                printf("%c",appendix);
                            }
                            else{
                                printf("%c",fixed);
                            }
                        }
                        else{
                            if(appendix == '*' && fixed != '.'){
                                printf("%c",appendix);
                            }
                            else{
                                printf("%c",fixed);
                            }
                        }
                    }
                }
                x++;
            }
            if (i == colLen - 1){
                printf("\n");
            }
        }
        y++;

        if (j == rowLen - 1){
            printDashes(numOfDashes);
            break;
        }
    }
}

/* Gets x - column and y - row from user and returns the normalized block points*/
Point getBlockIndex (int x, int y, Game* game){
    /* 0 - base the inputs*/
    Point point;
    x = x - 1;
    y = y - 1;
    point.x = x / (*game).n;
    point.y = y / (*game).m;
    return point;
}

/* Gets x - column and y - row from user and returns the normalized cell points*/
Point getCellIndex (int x, int y, Game* game){
    Point point;
    x = x - 1;
    y = y - 1;
    point.x = x % (*game).n;
    point.y = y % (*game).m;

    return point;
}

/*Receives a cell <x,y> and a value z and return 1 if z is a valid value for that cell, 0 otherwise */
int isValidValue (int x, int y, int z,Game *game){
    Point p = getBlockIndex(x,y, game);
    int id = pointToID(p.x, p.y, game);
    if(((*game).rows[y - 1][z-1]==0)&&((*game).cols[x - 1][z-1]==0)&&((*game).blocks[id][z-1]==0)){
        return 1;
    }
    return 0;
}

/*Receives a point representing the block index and return the relative ID */
int pointToID(int x,int y, Game *game){
    return(x+y*(*game).m);
}



/*updates rows[row][value] = set */
void updateRow(int row, int value, int set, Game* game){
    if(set == 1) {
        (*game).rows[row - 1][value - 1]++;
        return;
    }
    (*game).rows[row - 1][value - 1]--;

}

/*updates cols[col][value] = set */
void updateCol(int col, int value, int set, Game* game){
    if(set == 1) {
        (*game).cols[col - 1][value - 1]++;
        return;
    }
    (*game).cols[col - 1][value - 1]--;
}

/*updates blocks[blockID][value] = set */
void updateBlock(int blockID, int value, int set, Game* game){
    if(set == 1){
        (*game).blocks[blockID][value - 1]++;
        return;
    }
    (*game).blocks[blockID][value - 1]--;

}


/*initialize the binary array of possible numbers
*  in rows[i] for each i*/
void initRows(Game* game){
    int i;

    (*game).rows = (int **) malloc ((*game).n * (*game).m * sizeof(int*));
    if (game->rows == NULL){
        printf(MALLOCFAIL);
        return;
    }
    for(i = 0; i < ((*game).n * (*game).m); i++){
        (*game).rows[i] = (int*) malloc((*game).n * (*game).m * sizeof(int));
        if (game->rows[i] == NULL){
            printf(MALLOCFAIL);
            return;
        }
    }
}


/*initialize the binary array of possible numbers
*  in blocks[i] for each i*/
void initBlocks(Game* game) {
    int i;
    (*game).blocks = (int **) malloc ((*game).n * (*game).m * sizeof(int*));
    if (game->blocks == NULL){
        printf(MALLOCFAIL);
        return;
    }
    for(i = 0; i < (*game).n * (*game).m; i++) {

        (*game).blocks[i] = (int *) malloc((*game).n * (*game).m * sizeof(int));
        if (game->blocks[i]== NULL){
            printf(MALLOCFAIL);
            return;
        }
    }
}

/*initialize the binary array of possible numbers
*  in cols[i] for each i*/
void initCols(Game* game){
    int i;
    (*game).cols = (int **) malloc ((*game).n * (*game).m * sizeof(int*));
    if (game->cols== NULL){
        printf(MALLOCFAIL);
        return;
    }
    for(i = 0; i < (*game).n * (*game).m; i++) {
        (*game).cols[i] = (int *) malloc((*game).n * (*game).m * sizeof(int));
        if (game->cols[i] == NULL){
            printf(MALLOCFAIL);
            return;
        }
    }
}

/*set rows, cols and blocks to zero*/
void setZero(Game* game) {
    int i;
    for (i = 0; i < (*game).n * (*game).m; i++) {
        fillZeroes((*game).rows[i], game);
        fillZeroes((*game).blocks[i], game);
        fillZeroes((*game).cols[i], game);
    }

}

/*Fills the given array with 0's*/
void fillZeroes(int *arr, Game* game){
    int i;
    for (i = 0; i < (*game).n * (*game).m; i++){
        arr[i] = 0;
    }
}

/*Initialize all memory allocations*/
void initAll (Game* game){
    game->memRelease = 1;
    game->numOfErrors = 0;
    game->board.markError = 0;
    game->cellsToFill = game->m * game-> n *game->m * game-> n;
    initMoves(game);
    initBoard(game);
    initRows(game);
    initCols(game);
    initBlocks(game);
    setZero(game);
}

/*Frees malloc of auxiliary*/
void freeAux(Game* game){
    int i, j, k, l;
    for(i = 0; i< game->n; i++){
        for(j = 0 ; j< game->m; j++){
            for(l = 0; l < game->m; l++){
                for(k = 0; k < game->n ; k++){
                    free((*game).board.board[i][j].block[l][k].auxiliary);
                }
            }
        }
    }

}

/*Free malloc allocations*/
void freeMem(Game* game){
    int i, j, k;
    if((*game).memRelease == 1){
        freeAux(game);

        /*Free 2 dim Cell array*/
        for(i = 0; i < game->n; i++) {
            for (j = 0; j < game->m; j++) {
                for(k = 0; k < game -> m; k++){
                    free(game->board.board[i][j].block[k]);
                }
                free(game->board.board[i][j].block);
            }
        }


        /*Free 2 dim blocks array*/
        for (i=0; i<(*game).n; i++){
            free((*game).board.board[i]);
        }
        free((*game).board.board);

        for(i=0;i<((*game).n*(*game).m);i++){
            free(game->rows[i]);
            free((*game).cols[i]);
            free((*game).blocks[i]);
        }

        free((*game).rows);
        free((*game).cols);
        free((*game).blocks);
        (*game).memRelease=0;
    }

}

/*Empties the board completely*/
void emptyBoard(Game* game){
    int i,j,k;
    Point cell, block;

    for(i = 0; i < game->m * game->n; i++) {
        for (j = 0; j < game->m * game->n; j++) {
            cell = getCellIndex(j + 1, i + 1, game);
            block = getBlockIndex(j + 1, i + 1, game);
            k  = game->board.board[block.y][block.x].block[cell.y][cell.x].val;
            game->board.board[block.y][block.x].block[cell.y][cell.x].appendix = ' ';
            game->board.board[block.y][block.x].block[cell.y][cell.x].fixed = ' ';
            if(k != 0){
                game->board.board[block.y][block.x].block[cell.y][cell.x].val = 0;
                updateRow(i + 1, k, 0, game);
                updateCol(j + 1, k, 0, game);
                updateBlock(pointToID(block.x, block.y, game), k, 0, game);
            }
        }
    }

}

/*Marks the erroneous cells with '*' */
void markErrors(int x, int y, int z,Game* game){
    int i, j;
    Point block = getBlockIndex(x,y, game);
    Point cell = getCellIndex(x,y, game);
    int id = pointToID(block.x,block.y,game);

    if(isValidValue(x,y,z,game) == 1){
        return;
    }

    /*If we got here it means that this is an erroneous and we need to mark it*/

    /*Mark the current cell as erroneous*/
    checkAndMarkCellError(x,y,z,game);

    /*Mark every erroneous cell in the row*/
    if((*game).rows[y - 1][z-1] > 0){
        for(i = 1; i <= game->m*game->n; i++){
            if(i == x){
                continue;
            }
            checkAndMarkCellError(i,y,z,game);
        }
    }

    /*Mark every erroneous cell in the column*/
    if((*game).cols[x - 1][z-1] > 0){
        for(i = 1; i <= game->m*game->n; i++){
            if(i == y){
                continue;
            }
            checkAndMarkCellError(x,i,z,game);
        }
    }

    /*Mark every erroneous cell in the block*/
    if((*game).blocks[id][z-1] > 0){
        for(i = 0; i < game->m; i++){
            for(j = 0; j < game->n; j++){
                if(game->board.board[block.y][block.x].block[i][j].val == z){
                    if(cell.x == j && cell.y == i){
                        continue;
                    }
                    if(game->board.board[block.y][block.x].block[i][j].appendix != '*'){
                        game->board.board[block.y][block.x].block[i][j].appendix = '*';
                        game->numOfErrors++;
                    }
                    game->board.board[block.y][block.x].block[i][j].cntErr++;
                }
            }
        }
    }
}
