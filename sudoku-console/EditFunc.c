/*
* Created by CMY on 29/07/2019.
*/

#include "EditFunc.h"
#include <stdio.h>

/*Fills the given array with points that represent the empty cells in the board*/
void fillEmptyCellsArray(Game* game, Point** emptyCells){
    int i, j, cnt = 0;
    Point block, cell, p;
    for(i = 0; i < game->m * game->n; i++){
        for(j = 0; j < game->m * game->n; j++) {
            p.x = j + 1;
            p.y = i + 1;
            cell = getCellIndex(j + 1, i + 1, game);
            block = getBlockIndex(j + 1, i + 1, game);
            if(game->board.board[block.y][block.x].block[cell.y][cell.x].val == 0){
                (*emptyCells)[cnt].x = p.x;
                (*emptyCells)[cnt].y = p.y;
                cnt++;
            }
        }
    }
}

/*Chooses an empty cell from the board and returns it*/
Point chooseEmptyCell(Game* game){
    int index;
    Point p;
    Point* emptyCells = (Point*)malloc(game->cellsToFill * sizeof(Point));
    fillEmptyCellsArray(game, &emptyCells);
    index = rand() % game->cellsToFill;
    p.x = emptyCells[index].x;
    p.y = emptyCells[index].y;
    free(emptyCells);
    return p;
}

/*fills the given col, row in the board with a valid value and update the relevant arrays*/
int fillValid(int col, int row, Game* game){
    int k;
    Point cell, block;
    cell = getCellIndex(col, row, game);
    block = getBlockIndex(col, row, game);
    for(k = 0; k < game->m * game->n; k++){
        if(isValidValue(col, row, k + 1, game) == 1){
            game->board.board[block.y][block.x].block[cell.y][cell.x].val = k + 1;
            updateRow(row, k + 1, 1, game);
            updateCol(col, k + 1, 1, game);
            updateBlock(pointToID(block.x, block.y, game), k + 1, 1, game);
            /*Mark that this is a random cell*/
            game->board.board[block.y][block.x].block[cell.y][cell.x].appendix = 'r';
            game->cellsToFill--;
            return 1;
        }
    }
    return 0;
}

/*Reset board to original state before starting generate function*/
void resetBoardOnGenerate(Game* game, int opCode, int resetCellsToFill){
    int i, j, k, ilpVal;
    Point block, cell;
    game->cellsToFill = resetCellsToFill;
    for(i = 0; i < game->m * game->n; i++) {
        for (j = 0; j < game->m * game->n; j++) {
            cell = getCellIndex(j + 1, i + 1, game);
            block = getBlockIndex(j + 1, i + 1, game);
            ilpVal = game->board.board[block.y][block.x].block[cell.y][cell.x].ILPVal;
            if(opCode == 1){
                if(ilpVal != 0){
                    game->board.board[block.y][block.x].block[cell.y][cell.x].val = ilpVal;
                    game->board.board[block.y][block.x].block[cell.y][cell.x].fixed = '.';
                }
            }
            if(game->board.board[block.y][block.x].block[cell.y][cell.x].appendix == 'r'){
                k  = game->board.board[block.y][block.x].block[cell.y][cell.x].val;
                game->board.board[block.y][block.x].block[cell.y][cell.x].appendix = ' ';
                if(opCode == 0){
                    game->board.board[block.y][block.x].block[cell.y][cell.x].val = 0;
                    updateRow(i + 1, k, 0, game);
                    updateCol(j + 1, k, 0, game);
                    updateBlock(pointToID(block.x, block.y, game), k, 0, game);
                }
            }
        }
    }

}


/*Randomly chooses y cells from the board and clear the values of all other cells*/
void chooseYCellsAndClearThem(Game* game, int y){
    int i, cellsToClear, row, col, k, N;
    Point block, cell;
    N = game->m * game->n * game->m * game->n;
    resetBoardOnGenerate(game, 1, N - y);
    cellsToClear = N - y;

    for(i = 0; i < cellsToClear; i++){
        row = rand() % (game->n * game->m);
        col = rand() % (game->n * game->m);
        cell = getCellIndex(col + 1, row + 1, game);
        block = getBlockIndex(col + 1, row + 1, game);
        k = game->board.board[block.y][block.x].block[cell.y][cell.x].val;
        if(k == 0){
            i--;
            continue;
        }
        game->board.board[block.y][block.x].block[cell.y][cell.x].val = 0;
        game->board.board[block.y][block.x].block[cell.y][cell.x].fixed = ' ';
        game->board.board[block.y][block.x].block[cell.y][cell.x].appendix = ' ';
    }
    updateAllArrs(game);
}

/*Generates a puzzle by randomly filling number of cells provided by user*/
void generate(int x, int y, Game* game){
    Point p;
    int i, cnt = 0, resetCellsToFill;
    resetCellsToFill = game->cellsToFill;
    if(game->cellsToFill < x){
        printf(NOTENOUGHCELLS, x);
        return;
    }

    if(y > game->m * game->n * game->m * game->n){
        printf(TOOMANYCELL, y);
        return;
    }
    /*when y is 0 we need to empty the board*/
    if(y == 0){
        emptyBoard(game);
        return;
    }
    while(cnt < 1000){
        for(i = 0; i < x; i++){
            p = chooseEmptyCell(game);
            if(fillValid(p.x, p.y, game) == 0){
                resetBoardOnGenerate(game, 0, resetCellsToFill);
                cnt++;
                i = 0;
            }
            if(cnt == 1000){
                printf(ERRORINPUZZLEGEN);
                return;
            }
        }
        if(solveILP(game, 0, 0, 0) != 1){
            cnt++;
        }
        else{
            chooseYCellsAndClearThem(game, y);
            return;
        }

        /*clean board since solveILP failed*/
        resetBoardOnGenerate(game, 0, resetCellsToFill);

    }
    printf(ERRORINPUZZLEGEN);
}


int saveEdit(Game* game){
    int isErr;
    isErr = checkError(game);
    if(isErr == 0){
        printf(ERRORBOARD);
        return 0;
    }

    return 1;
}

/*Checks if there are errors in the board*/
int checkError(Game* game){
    if(game->numOfErrors == 0){
        return 1;
    }
    return 0;
}
