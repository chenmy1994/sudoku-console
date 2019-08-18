/*
*	Created by CMY on 29/07/2019.
 */

#include "GeneralFunc.h"
#define UNDOMSG "Undo: Deleted value %d for cell <%d,%d>\n"
#define REDOMSG "Redo: Re-Set value %d from cell <%d,%d>\n"


/*Dealing with the edit command received by user*/
int edit(char* X, Game* game){
    if(loadBoard(X, game,1)==1){
        /*change mode to edit mode*/
        changeMode(1, game);
        return 1;
    }
    return 0;
}

/*Dealing with the exit command received by user*/
void exitGame(Game* game){
    freeMem(game);
    changeMode(0, game);
    printf(EXITGAME);
}

/*Dealing with the solve command received by user*/
int solve(char* X, Game* game){
    if(loadBoard(X, game,2)==1) {
        /*change mode to solve mode*/
        changeMode(2, game);
        return 1;
    }
    return 0;
}

void unMarkErrorsCell(int x, int y, int z, Game* game){
    Point block = getBlockIndex(x,y, game);
    Point cell = getCellIndex(x,y, game);
    if(z == 0){
        game->board.board[block.y][block.x].block[cell.y][cell.x].cntErr = 0;
        game->board.board[block.y][block.x].block[cell.y][cell.x].appendix = ' ';
        game->numOfErrors--;
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
    Point cell = getCellIndex(x,y, game);
    int id = pointToID(block.x,block.y,game);
    if(game->board.board[block.y][block.x].block[cell.y][cell.x].appendix != '*'){
        return;
    }
    /*UnMark the current cell as erroneous*/
    unMarkErrorsCell(x,y,0,game);

    /*UnMark every erroneous cell in the row*/
    if((*game).rows[y - 1][z-1] > 0){
        for(i = 1; i <= game->m*game->n; i++){
            if(i == x){
                continue;
            }
            unMarkErrorsCell(i,y,z,game);
        }
    }

    /*UnMark every erroneous cell in the column*/
    if((*game).cols[x - 1][z-1] > 0){
        for(i = 1; i <= game->m*game->n; i++){
            if(i == y){
                continue;
            }
            unMarkErrorsCell(x,i,z,game);
        }
    }

    /*UnMark every erroneous cell in the block*/
    if((*game).blocks[id][z-1] > 0){
        for(i = 0; i < game->m; i++){
            for(j = 0; j < game->n; j++){
                /*Might change it here to a function that computes the relevant x y from i and j and block id*/
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
    int i, j, val;
    Point block, cell;
    char fixed;
    if(game->mode == 1){
        if(saveEdit(game) == 0 && validate(game) != 1){
            return;
        }
    }
    fp = fopen (X, "w+");
    fprintf(fp, "%d %d\n", game->m, game->n);

    for(i = 0; i < game->n * game->m; i++){
        for(j = 0; j < game->n * game->m; j++){
            block = getBlockIndex(j + 1, i + 1, game);
            cell = getCellIndex(j + 1, i + 1, game);
            val = (*game).board.board[block.y][block.x].block[cell.y][cell.x].val;
            fprintf(fp, "%d", val);
            if(val != 0){
                fixed = (*game).board.board[block.y][block.x].block[cell.y][cell.x].fixed;
                if(game->mode == 1 || fixed == '.'){
                    fprintf(fp, "%c", '.');
                }
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


/*Set new value of cell
 * receives an indicator (1) if the set command was sent from a undo or redo command, 0 otherwise*/
void set(int x, int y, int z, Game* game,int undoOrRedo){
    Point block, cell;
    int prevVal, id;
    Point **moveCell;

    block = getBlockIndex(x,y,game);
    cell = getCellIndex(x,y,game);
    prevVal = game->board.board[block.y][block.x].block[cell.y][cell.x].val;
    id = pointToID(block.x, block.y, game);

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
        game->cellsToFill--;

        updateBlock(id, z,1,game);
        updateRow(y, z,1,game);
        updateCol(x, z,1,game);
    }
    /*If user deleted a cell*/
    else if (z == 0){
        unMarkErrors(x, y, prevVal, game);
        game->board.board[block.y][block.x].block[cell.y][cell.x].val = z;
        game->cellsToFill++;

        /*if(isFixed(x,y, game) == 1) {
            game->board.board[block.y][block.x].block[cell.y][cell.x].fixed = ' ';
        }*/
        updateBlock(id, prevVal,0,game);
        updateRow(y, prevVal,0,game);
        updateCol(x, prevVal,0,game);
    }
    /*If user overwritten a value with a new valid value */
    else if(prevVal!=z){
        unMarkErrors(x, y, prevVal, game);
        game->board.board[block.y][block.x].block[cell.y][cell.x].val = z;
        markErrors(x, y, z, game);

        updateBlock(id, z,1,game);
        updateRow(y, z,1,game);
        updateCol(x, z,1,game);
        updateBlock(id, prevVal,0,game);
        updateRow(y, prevVal,0,game);
        updateCol(x, prevVal,0,game);
    }

    if(!undoOrRedo){
		(moveCell)=(Point**)malloc(sizeof(Point*));
		(*moveCell)=(Point*)malloc(sizeof(Point));

		(**moveCell).x=x;
		(**moveCell).y=y;
		(**moveCell).curr=z;
		(**moveCell).prev=prevVal;
		addMove(moveCell,1,game);
    }

    if(game->mode == 2 && game->cellsToFill == 0){
        if(game->numOfErrors == 0){
            printBoard(game);
            printf(PUZZLESOLVED);
            /*Change mode to init*/
            game->mode = 0;
            return;
        }
        printf(ERRORSOL);
        return;
    }
}



/*Revert the last move done by the user according to the linked list current move (updates to previous move)
 * also receives an indicator=1 'reset' preventing output for every move undone if undo was called from reset*/
void undo(Game* game,int reset){
	int movesNum,x,y,z,cur,i;
	Node* currMove;
	Point** currPoint;
	LinkedList* movesList = (*game).moves;
	if(theStart(movesList)){
		printf(UNDOLIMIT);
	}
	else{
		currMove = (*movesList).current;
		movesNum=(*currMove).pointNum;
		currPoint = (*currMove).data;
		for (i=0;i<movesNum;i++){
			x=(*currPoint)[i].x;
			y=(*currPoint)[i].y;
			z=(*currPoint)[i].prev;
			cur=(*currPoint)[i].curr;
			if(!reset){
				printf(UNDOMSG,cur,x,y);
			}
			set(x,y,z,game,1);
		}
		prevCurr((*game).moves);
	}
}

/*Cancel the last revert action according to the linked list current move (updates to next move)*/
void redo(Game* game){
	int movesNum,x,y,z,i;
	Node* currMove;
	Point** currPoint;
	LinkedList* movesList = (*game).moves;
	if(theEnd(movesList)){
		printf(REDOLIMIT);
	}
	else{
		currMove = (*(*movesList).current).next;
		movesNum=(*currMove).pointNum;
		currPoint = (*currMove).data;
		for (i=0;i<movesNum;i++){
			x=(*currPoint)[i].x;
			y=(*currPoint)[i].y;
			z=(*currPoint)[i].curr;
			printf(REDOMSG,z,x,y);
			set(x,y,z,game,1);
		}
		nextCurr((*game).moves);
	}
}

/*Prints the number of solution for the current board*/
void num_solutions(){

}

/*Undo all moves, reverting the board to its original loaded state.*/
void reset(Game* game){
	LinkedList* movesList = (*game).moves;
	while(!theStart(movesList)){
		undo(game,1);
	}
}



/*prints that reading file has failed and closes fp*/
void failedReadingFile(FILE** fp, Game* game){
    printf(FAILEDREADINGFILE);
    fclose(*fp);
    freeMem(game);
}

/*Checks whether char n represent a digit or dot or not*/
int isDigitOrDot(char n){
    int num = n - '0';
    if(num >= 0 && num < 10){
        return 1;
    }
    if (n == '.'){
        return 1;
    }
    return 0;
}

/*builds number from current place on char* buff*/
int buildNumber (char* buff, int* i){
    int num = 0;
    int len = strlen(buff);
    while(isDigitOrDot(buff[*i]) == 1 && (*i) < len){
        if(buff[*i] == '.'){
            return num;
        }
        num = num * 10 + (buff[*i] - '0');
        (*i)++;
    }
    return num;
}

/*Fills the game board with the values given from the file in X*/

int fillBoard(char* X, Game* game, int mode){
    FILE *fp;
    int setM = 0, setN = 0, x = 1, y = 1, val, i, len;
    char buff[4096];
    Point block, cell;
    fp = fopen(X, "r");

    if(fp == NULL){
        printf(ERROROPENFILE);
        return 0;
    }

    while(fgets(buff, sizeof(buff), (FILE*)fp) != 0) {
        len = strlen(buff);
        for (i = 0; i < len ; i++) {
            /*If it is a space - skip*/
            if (isspace(buff[i])) {
                continue;
            }

            /*If its is not a digit nor a dot - then it's an invalid board*/
            if(isDigitOrDot(buff[i]) == 0){
                failedReadingFile(&fp,game);
                printf("File contains an invalid value\n");
                return 0;
            }

            /*Fill board*/
            if(setM == 0){
                setM = 1;
                val = buildNumber(buff, &i);
                game->m = val;
                continue;
            }
            if(setN == 0){
                setN = 1;
                val = buildNumber(buff, &i);
                game->n = val;
                if(game->m * game->n > 99){
                    printf("Error: board size is larger than 99\n");
                    fclose(fp);
                    return 0;
                }
                initAll(game);
                continue;
            }

            if (y <= game->m * game->n) {
                block = getBlockIndex(x, y, game);
                cell = getCellIndex(x, y, game);
                val = buildNumber(buff, &i);
                /*check if val is in valid range*/
                if (val < 0 || val > game->m * game->n) {
                    failedReadingFile(&fp,game);
                    printf("File contains a value which is not in correct range, 1 - %d\n",
                           game->m * game->n);
                    return 0;
                }

                (*game).board.board[block.y][block.x].block[cell.y][cell.x].val = val;
                if (val != 0) {
                    game->cellsToFill--;
                    markErrors(x,y, val, game);
                    updateCol(x, val, 1, game);
                    updateRow(y, val, 1, game);
                    updateBlock(pointToID(block.x, block.y, game), val, 1, game);
                }
                if(i < len){
                    if (buff[i] == '.' && mode == 2) {
                        (*game).board.board[block.y][block.x].block[cell.y][cell.x].fixed = '.';
                        if(val != 0 && checkCellValid(x,y,val, game) == 0){
                            failedReadingFile(&fp,game);
                            printf("File contains contradiction between 2 fixed cells\n");
                            return 0;
                        }
                    }
                    else if(mode == 1){
                        (*game).board.board[block.y][block.x].block[cell.y][cell.x].fixed = '.';
                    }
                }
                x++;
                if (x > game->m * game->n) {
                    y++;
                    x = 1;
                }
            }
            else{
                failedReadingFile(&fp,game);
                printf("Too many digits in file\n");
                return 0;
            }
        }
    }
    if(y != game->m * game->n + 1 || x != 1){
        failedReadingFile(&fp,game);
        printf("Not enough digits in file\n");
        return 0;
    }
    fclose(fp);
    return 1;

}

/*Frees and Allocates the memory of the game*/
int loadBoard(char* X, Game* game, int mode){
	/*There was already a game open*/
    if(game->memRelease == 1){
        freeMem(game);
    }
    /*A path were provided*/
    if((X!=NULL)&&(strlen(X) > 1)) {
        return fillBoard(X, game,mode);
    }
    createEmptyBoard(game);
    return 1;
}

/*Creates an empty 9x9 board*/
void createEmptyBoard(Game* game){
    game->m = 3;
    game->n = 3;
    initAll(game);
}

/*Changes the mode of the game to the newMode*/
void changeMode(int newMode, Game* game){
    game->mode = newMode;
    if(newMode == 1){
        game->board.markError = 1;
    }
}

/*Update the "mark errors" setting according to users input */
void updateMarkErrors(Game* game, int setValue){
    game->board.markError = setValue;
}

/*Checks whether the x y cell contains z,
 * if it does - marks it as an erroneous and updates the errors counter*/
int checkAndMarkCellError(int x, int y, int z, Game* game){
    Point block = getBlockIndex(x,y, game);
    Point cell = getCellIndex(x,y, game);
    if(game->board.board[block.y][block.x].block[cell.y][cell.x].val == z) {
        if(game->board.board[block.y][block.x].block[cell.y][cell.x].appendix != '*'){
            game->board.board[block.y][block.x].block[cell.y][cell.x].appendix = '*';
            game->numOfErrors++;
        }
        game->board.board[block.y][block.x].block[cell.y][cell.x].cntErr++;
        return 1;
    }
    return 0;
}


/*Return 1 if there is no contradiction between this cell to another fixed cell
 * Otherwise return 0*/
int checkCellValid(int x, int y, int z, Game* game){
    int i,j;
    Point cell;
    Point block = getBlockIndex(x,y,game);
    int id = pointToID(block.x, block.y,game);
    if(((*game).rows[y - 1][z-1]==1)&&((*game).cols[x - 1][z-1]==1)&&((*game).blocks[id][z-1]==1)) {
        return 1;
    }
    for(i = 0; i < game->m * game->n; i++){
        for(j = 0; j < game->m * game->n; j++) {
            if(x == j + 1 && y == i + 1){
                return 1;
            }
            cell = getCellIndex(j+1,i+1,game);
            block = getBlockIndex(j+1,i+1,game);
            if(x == j + 1 || y == i + 1 || pointToID(block.x,block.y,game) == id){
                if(isFixed(j + 1,i + 1,game) == 1){
                    if(game->board.board[block.y][block.x].block[cell.y][cell.x].val == z){
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

/*Checks whether the board can be solved (1) or not (0)*/
/*uses the ILP solver*/
int validate(Game* game){
    int ilp;
    ilp = solveILP(game, 2, 0, 0);
    if(ilp == 1){
        printf(BOARDISVALID);
    }
    else{
        printf(BOARDISNOTVALID);
    }
    return ilp;
	return 1;
}
