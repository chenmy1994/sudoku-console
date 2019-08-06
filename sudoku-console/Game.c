/*
 *  Created by CMY on 29/07/2019.
 */

#include "Game.h"

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
    char appendix;
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
            if (i % ((*game).n + 1) == 0 && i != colLen - 1) {
                printf("|");

            } else if(i % ((*game).n + 1) == 0 && i == colLen - 1)
            {
                printf("|");
            }
            else {
                block = getBlockIndex(x, y, game);
                cell = getCellIndex(x, y, game);
                if ((*game).board.board[block.y][block.x].block[cell.y][cell.x].val == 0){
                    printf("   ");
                }
                else{
                    printf(" %2d",(*game).board.board[block.y][block.x].block[cell.y][cell.x].val);
                }
                appendix = (*game).board.board[block.y][block.x].block[cell.y][cell.x].appendix;

                if(appendix == '*'){
                    if(game->board.markError==0){
                        printf(" ");
                    }
                    else{
                        printf("%c",appendix);
                    }
                }
                else{
                    printf("%c",appendix);
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
    printf("here");
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
    (*game).rows[row - 1][value - 1] = set;
}

/*updates cols[col][value] = set */
void updateCol(int col, int value, int set, Game* game){
    (*game).cols[col - 1][value - 1] = set;
}

/*updates blocks[blockID][value] = set */
void updateBlock(int blockID, int value, int set, Game* game){
    (*game).blocks[blockID][value - 1] = set;
}


/*initialize the binary array of possible numbers
*  in rows[i] for each i*/
void initRows(Game* game){
    int i;
    (*game).rows = (int **) malloc ((*game).n * (*game).m * sizeof(int*));
    for(i = 0; i < (*game).n * (*game).m; i++){
        (*game).rows[i] = (int*) malloc((*game).n * (*game).m * sizeof(int));
    }
}


/*initialize the binary array of possible numbers
*  in blocks[i] for each i*/
void initBlocks(Game* game) {
    int i;
    (*game).blocks = (int **) malloc ((*game).n * (*game).m * sizeof(int*));
    for(i = 0; i < (*game).n * (*game).m; i++) {
        (*game).blocks[i] = (int *) malloc((*game).n * (*game).m * sizeof(int));
    }
}

/*initialize the binary array of possible numbers
*  in cols[i] for each i*/
void initCols(Game* game){
    int i;
    (*game).cols = (int **) malloc ((*game).n * (*game).m * sizeof(int*));
    for(i = 0; i < (*game).n * (*game).m; i++) {
        (*game).cols[i] = (int *) malloc((*game).n * (*game).m * sizeof(int));
    }
}

/*set rows, cols and blocks to zero*/
void setZero(Game* game){
    int i;
    for(i = 0; i < (*game).n * (*game).m; i++){
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


/*Fills the game board with the values given from the file in X*/
void fillBoard(char* X, Game* game){
    FILE *fp;
    char buff[4096];
    int i, setM = 0, setN = 0, x = 1, y = 1, val;
    Point block, cell;

    fp = fopen(X, "r");

    while(fgets(buff, sizeof(buff), (FILE*)fp) != 0 && y < game->m * game->n){
        for(i = 0; i < strlen(buff); i++){
            printf("%c", buff[i]);
            if(buff[i] == ' ' || buff[i] == '\0' || buff[i] == '\n'){
                continue;
            }
            if (setM == 0){
                game->m = buff[i] - '0';
                setM = 1;
                continue;
            }
            if (setN == 0){
                game->n = buff[i] - '0';
                setN = 1;
                continue;
            }
            block = getBlockIndex(x,y, game);
            cell = getCellIndex(x,y, game);

            if (buff[i] == '.'){
                (*game).board.board[block.y][block.x].block[cell.y][cell.x].appendix = '.' ;
                continue;
            }

            val = buff[i] - '0';
            (*game).board.board[block.y][block.x].block[cell.y][cell.x].val = val;
            updateCol(x, val, 1, game);
            updateRow(y, val, 1, game);
            updateBlock(pointToID(block.x,block.y,game), val, 1, game);

            x++;
            if(x > game->m * game->n){
                y++;
                x = 1;
            }
        }
    }

    fclose(fp);


}

/*Frees and Allocates the memory of the game*/
void loadBoard(char* X, Game* game){
    if(game->memRelease == 1){
        freeMem(game);
    }
    initBoard(game);
    initRows(game);
    initCols(game);
    initBlocks(game);

    fillBoard(X, game);

}

/*Changes the mode of the game to the newMode*/
void changeMode(int newMode, Game* game){
    game->mode = newMode;
}


/*Free malloc allocations*/
void freeMem(Game* game){
    int i;
    if((*game).memRelease==1){
        for (i=0; i<(*game).m; i++){
            free((*game).board.board[i]);
        }
        free((*game).board.board);
        for(i=0;i<(*game).m*(*game).n;i++){
            free((*game).rows[i]);
            free((*game).cols[i]);
            free((*game).blocks[i]);
        }
        free((*game).rows);
        free((*game).cols);
        free((*game).blocks);
        (*game).memRelease=0;
    }

}