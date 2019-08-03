/*
 *  Created by CMY on 29/07/2019.
 */

#include "Game.h"
#include <stdio.h>
#include <stdlib.h>
#define MALLOCFAIL "Error: malloc has failed\n"

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
    int j,k, l;
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
                printf(" %2d",(*game).board.board[block.y][block.x].block[cell.y][cell.x].val);
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
    if(((*game).rows[y - 1][z-1]==0)&&((*game).cols[x - 1][z-1]==0)&&((*game).blocks[pointToID(p.x,p.y,game)][z-1]==0)){
        return 1;
    }
    return 0;
}

/*Receives a point representing the block index and return the relative ID */
int pointToID(int x,int y, Game *game){
    return(x+y*(*game).m);
}
