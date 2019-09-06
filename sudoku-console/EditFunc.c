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
            /*If the cell is empty add it to the array*/
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
    Point* emptyCells = (Point*)mallocWithGuard(game->cellsToFill * sizeof(Point));
    fillEmptyCellsArray(game, &emptyCells);
    index = rand() % game->cellsToFill;
    p.x = emptyCells[index].x;
    p.y = emptyCells[index].y;
    free(emptyCells);
    return p;
}

/*Fills the given col, row in the board with a valid value and update the relevant arrays*/
int fillValid(int col, int row, Game* game){
    int k;
    Point cell, block;
    cell = getCellIndex(col, row, game);
    block = getBlockIndex(col, row, game);
    for(k = 0; k < game->m * game->n; k++){
        /*If the value (k+1) is valid for cell (col,row)*/
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
int resetBoardOnGenerate(Point** moveCell,Game* game, int opCode, int resetCellsToFill){
    int i, j, k, ilpVal, cnt=0;
    Point block, cell;
    game->cellsToFill = resetCellsToFill;
    for(i = 0; i < game->m * game->n; i++) {
        for (j = 0; j < game->m * game->n; j++) {
            cell = getCellIndex(j + 1, i + 1, game);
            block = getBlockIndex(j + 1, i + 1, game);
            ilpVal = game->board.board[block.y][block.x].block[cell.y][cell.x].ILPVal;
            /*If generate function succeeded (opCode == 1)*/
            if(opCode == 1){
                if(ilpVal != 0){
                    game->board.board[block.y][block.x].block[cell.y][cell.x].val = ilpVal;
                    game->board.board[block.y][block.x].block[cell.y][cell.x].fixed = '.';
                }
            }
            k  = game->board.board[block.y][block.x].block[cell.y][cell.x].val;

            if((k!=0)&&(opCode==1)&&(game->board.board[block.y][block.x].block[cell.y][cell.x].appendix == ' ')){
            	game->board.board[block.y][block.x].block[cell.y][cell.x].appendix = 'c';
    			(*moveCell)[cnt].x=j+1;
    			(*moveCell)[cnt].y=i+1;
    			(*moveCell)[cnt].prev=k;
    			(*moveCell)[cnt].curr=0;
    			cnt++;
            }
            /*If generate function had failure during the execution
             * then we need to reset the board and the helpful arrays*/
            if((game->board.board[block.y][block.x].block[cell.y][cell.x].appendix == 'r') ||
            	(game->board.board[block.y][block.x].block[cell.y][cell.x].appendix == 's' )){
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
    return cnt;

}


/*Randomly chooses y cells from the board and clear the values of all other cells*/
void chooseYCellsAndClearThem(Point** moveCell,Game* game, int y){
    int i, cellsToClear, cnt, row, col, k, N;
    Point block, cell;
    N = game->m * game->n * game->m * game->n;
    /*Set the new board fields*/
    cnt = resetBoardOnGenerate(moveCell,game, 1, N - y);
    cellsToClear = N - y;
    game->numOfErrors = 0;
    fillZeroesCntErr(game);
    /*Delete (N-y) cells so at the end we will have y cells on the printed board*/
    for(i = 0; i < cellsToClear; i++){
        row = rand() % (game->n * game->m);
        col = rand() % (game->n * game->m);
        cell = getCellIndex(col + 1, row + 1, game);
        block = getBlockIndex(col + 1, row + 1, game);
        k = game->board.board[block.y][block.x].block[cell.y][cell.x].val;
        /*If cell's value was 0 (since we already choose it)
         * randomize another cell*/
        if(k == 0){
            i--;
            continue;
        }
        /*Clean the choosen cell*/
        game->board.board[block.y][block.x].block[cell.y][cell.x].val = 0;
        game->board.board[block.y][block.x].block[cell.y][cell.x].fixed = ' ';
        game->board.board[block.y][block.x].block[cell.y][cell.x].appendix = ' ';
    }

    /*Update all helpful arrays*/
    cnt = updateAllArrs(moveCell,cnt,game);
	addMove(moveCell,cnt,game);
}

/*Generates a puzzle by randomly filling number of cells provided by user*/
int generate(int x, int y, Game* game){
    Point p, **moveCell;
    int i, cnt = 0, resetCellsToFill,N=(*game).n*(*game).m;
    resetCellsToFill = game->cellsToFill;
    /*If we have less cells to fill than the given x
     * stop execting the generate function*/
    if(game->cellsToFill < x){
        printf(NOTENOUGHCELLS, x);
        return 0;
    }

	(moveCell)=(Point**)mallocWithGuard(sizeof(Point*));
	(*moveCell)=(Point*)mallocWithGuard(2*N*N*sizeof(Point));

    /*when y is 0 we need to empty the board*/
    if(y == 0){
        emptyBoard(moveCell,game);
        return 1;
    }

    /*We need to stop trying when we reach 1000 tries*/
    while(cnt < 1000){
        for(i = 0; i < x; i++){
            /*Choose an empty cell*/
            p = chooseEmptyCell(game);
            /*Try to fill it with valid value
             * If failed to fill, bring the board to it's previous state
             * and count it as "failure"*/
            if(fillValid(p.x, p.y, game) == 0){
                resetBoardOnGenerate(moveCell,game, 0, resetCellsToFill);
                cnt++;
                i = 0;
            }
            /*If we reached 1000 tries - stop trying*/
            if(cnt == 1000){
                printf(ERRORINPUZZLEGEN);
                return 0;
            }
        }
        /*Check if the given board with x added cells is valid
         * If it is not valid, count it as "failure"*/
        if(solveILP(game, 0, 0, 0) != 1){
            cnt++;
        }
        /* If it is valid - choose Y cells and clear everything but them*/
        else{
            chooseYCellsAndClearThem(moveCell,game, y);
            return 1;
        }
        /*Clean board since solveILP failed*/
        resetBoardOnGenerate(moveCell,game, 0, resetCellsToFill);
    }

    /*If we reached here it means cnt == 1000*/
    printf(ERRORINPUZZLEGEN);
    return 0;
}

/*Save has extra limitations in edit mode*/
int saveEdit(Game* game){
    if(game->numOfErrors > 0){
        printf(ERRORBOARD);
        return 0;
    }
    return 1;
}
