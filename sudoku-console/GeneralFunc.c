/*
*	Created by CMY on 29/07/2019.
 */

#include "GeneralFunc.h"

/*Unmark erroneous on a given cell (x - col, y - row)
 * where z is the value we had in the cell or 0 if we just changed the cell*/
void unMarkErrorsCell(int x, int y, int z, Game* game){
    Point block = getBlockIndex(x,y, game);
    Point cell = getCellIndex(x,y, game);
    /*If we just changed the x,y cell we need to reset its' fields.*/
    if(z == 0){
        game->board.board[block.y][block.x].block[cell.y][cell.x].cntErr = 0;
        game->board.board[block.y][block.x].block[cell.y][cell.x].appendix = ' ';
        game->numOfErrors--;
    }
    /*If it a cell that contains a value that equals the cell's value we changed
     * then we need to decrease it's cntErr and change the board as well if needed*/
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
    /*If we  are in edit mode*/
    if(game->mode == 1){
        if(saveEdit(game) == 0){
            return;
        }
        if(validate(game, 0) != 1){
            printf("Error: Couldn't save the board because it is not solvable.\n");
            return;
        }
    }
    fp = fopen (X, "w+");
    if(fp == NULL){
        printf("Error: Couldn't save board since file couldn't be created.\n");
        return;
    }
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

    if(fclose(fp)==EOF){
        printf(ERRORCLOSE);
        exit(-1);
    }
    printf(BOARDSAVED, X);
}


/*Set new value of cell
 * receives an indicator (1) if the set command was sent from a undo or redo command, 0 otherwise*/
int set(int x, int y, int z, Game* game,int undoOrRedo){
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
            return 0;
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
        /*Update helpful arrays*/
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
		(moveCell)=(Point**)mallocWithGuard(sizeof(Point*));
		(*moveCell)=(Point*)mallocWithGuard(sizeof(Point));

		(**moveCell).x=x;
		(**moveCell).y=y;
		(**moveCell).curr=z;
		(**moveCell).prev=prevVal;
		addMove(moveCell,1,game);
    }
    return 1;
}



/*Revert the last move done by the user according to the linked list current move (updates to previous move)
 * also receives an indicator=1 'reset' preventing output for every move undone if undo was called from reset*/
int undo(Game* game,int reset){
	int movesNum,x,y,z,cur,i;
	Node* currMove;
	Point** currPoint;
	LinkedList* movesList = (*game).moves;
	if(theStart(movesList)){
		printf(UNDOLIMIT);
        return 0;
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
    return 1;
}

/*Cancel the last revert action according to the linked list current move (updates to next move)*/
int redo(Game* game){
	int movesNum,x,y,z,i;
	Node* currMove;
	Point** currPoint;
	LinkedList* movesList = (*game).moves;
	if(theEnd(movesList)){
		printf(REDOLIMIT);
        return 0;
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
    return 1;
}

/*Prints the number of solution for the current board*/
int num_solutions(Game* game){
	int dir, guess, val, i=0, j=0, done=0, solNum=0, N=((*game).n)*((*game).m);
	Point block, cell, *moveCell;
	Elem* elem;
	Stack* stk = initStack();
	/*direction 1 = moving forward, 0 = moving backwards*/
	dir=1;
	/*If the board is erroneous it is an error*/
	if((*game).numOfErrors>0){
		printf(errorErroneous, "num_solutions");
		return 0;
	}
	/*Keep searching for more solutions until you're back to
	 * first empty cell and done with it*/
	while((!((isEmpty(stk)) && done)&&((*game).cellsToFill>0))){
		block = getBlockIndex(j + 1, i + 1, game);
		cell = getCellIndex(j + 1, i + 1, game);
		val = game->board.board[block.y][block.x].block[cell.y][cell.x].val;
		/*if it's an empty new cell or if we're going backwards*/
		if((val == 0)||(dir==0)){
			for(guess=val;guess<N;guess++){
				if(isValidValue(j+1,i+1,guess+1,game)==1){
					/*if we updated a guess value for a cell*/
					if(val!=0){
						/*pop the previous guess from the stack*/
						popAndUpdate(game,&stk);
					}
					game->board.board[block.y][block.x].block[cell.y][cell.x].val = guess+1;
					updateCol(j+1, guess+1, 1, game);
					updateRow(i+1, guess+1, 1, game);
					updateBlock(pointToID(block.x, block.y, game), guess+1, 1, game);
					moveCell=(Point*)mallocWithGuard(sizeof(Point));
					(*moveCell).x=j+1;
					(*moveCell).y=i+1;
					(*moveCell).prev=0;
					(*moveCell).curr=guess+1;
					/*push the new guess to the stack*/
					push(stk,moveCell);
					dir=1;
					/*if the number of valid guesses for each empty cell equals cellsToFill -
					 * count as additional valid solution*/
					if((*stk).count==(*game).cellsToFill){
						solNum++;
					}

					/*we found a valid guess for that cell - stop searching*/
					break;
				}
			}
			/* if we have guessed last cell in board
			 * OR
			 * if we didn't find valid guess for a cell*/
			if((guess==N)||(*stk).count==(*game).cellsToFill){
				if(isEmpty(stk)==0){
					elem=top(stk);
					moveCell=elem->data;
					/*Finished guessing for current head cell - pop head of stack*/
					if(((*moveCell).x==j+1)&&((*moveCell).y==i+1)){
						popAndUpdate(game,&stk);
					}
					/*update current cell indexes to head of stack*/
					if(isEmpty(stk)==0){
						elem=top(stk);
						moveCell=elem->data;
						i=(*moveCell).y-1;
						j=(*moveCell).x-1;
					}
					/*mark we're going backwards*/
					dir=0;
					/*mark we're done with guessing for this cell*/
					done=1;
				}

			}
			/*We still got guesses for this cell*/
			else{
				done=0;
			}
		}

		/*Promote i,j indexes when it wasn't updated to head cell*/
		if(!done){
			if(j==N-1){
				i++;
				j=0;
			}
			else{
				j++;
			}
		}

	}
	printf("The number of solutions for this board is %d\n", solNum);
	free(stk);
	return 1;
}

/*Pop the first object from top of stack and update
 * the game helpful arrays*/
void popAndUpdate(Game* game,Stack** stk){
	Elem* elem;
	Point block, cell,*moveCell;
	elem=pop(*stk);
	moveCell=elem->data;
	block = getBlockIndex((*moveCell).x, (*moveCell).y, game);
	cell = getCellIndex((*moveCell).x, (*moveCell).y, game);
	game->board.board[block.y][block.x].block[cell.y][cell.x].val = 0;
	updateCol((*moveCell).x, (*moveCell).curr, 0, game);
	updateRow((*moveCell).y, (*moveCell).curr, 0, game);
	updateBlock(pointToID(block.x, block.y, game), (*moveCell).curr, 0, game);
	free(moveCell);
	free(elem);
}

/*Undo all moves, reverting the board to its original loaded state.*/
int reset(Game* game){
	LinkedList* movesList = (*game).moves;
	if(movesList->count == 0){
        return 0;
	}
	while(!theStart(movesList)){
		undo(game,1);
	}
    return 1;
}

/*prints that reading file has failed and closes fp*/
void failedReadingFile(FILE** fp, Game* game){
    printf(FAILEDREADINGFILE);
    if(fclose(*fp) == EOF){
        printf(ERRORCLOSE);
    }
    freeMem(game);
}

/*Checks whether char n represent a digit or dot or not*/
int isDigit(char n){
    int num = n - '0';
    if(num >= 0 && num < 10){
        return 1;
    }
    return 0;
}

/*Checks whether char n represent a digit or dot or not*/
int isDigitOrDot(char n){
    if (n == '.' || isDigit(n) == 1){
        return 1;
    }
    return 0;
}

/*Check that the given num is in valid range*/
int checkValidRange(Game* game, int num, FILE**fp){
    if (num < 0 || num > game->m * game->n) {
        failedReadingFile(fp,game);
        printf("File contains a value which is not in correct range, 1 - %d\n",
               game->m * game->n);
        return 0;
    }
    return 1;
}

/*Fills the game board with the values given from the file in X*/
int fillBoard(char* X, Game* game, int mode){
    FILE *fp;
    int setM = 0, setN = 0, x = 1, y = 1, num = 0;
    char ch;
    Point block, cell;
    fp = fopen(X, "r");
    /*Of opening file failed*/
    if(fp == NULL){
        printf(ERROROPENFILE);
        return 0;
    }

    /*While there are more chars in the file*/
    ch = fgetc(fp);
    while(ch != EOF){
        /*If it is a space - skip*/
        if (isspace(ch)) {
            ch = fgetc(fp);
            continue;
        }
        /*If its is not a digit nor a dot - then it's an invalid board*/
        if(isDigitOrDot(ch) == 0){
            failedReadingFile(&fp,game);
            printf("File contains an invalid value\n");
            return 0;
        }
        /*If one of the 2 first arguments on the file are not digits (there are dots)
         * since we checked already if it is digit or dot*/
        if((setM == 0 || setN == 0) && isDigit(ch) == 0){
            failedReadingFile(&fp,game);
            printf("First 2 arguments in the file should be game dimensions.\n");
            return 0;
        }

        /*Fill board*/
        if(setM == 0){
            setM = 1;
            /*Build number*/
            num = 0;
            while(isDigit(ch)){
                num = num * 10 + (ch - '0');
                ch = fgetc(fp);
            }

            game->m = num;
            continue;
        }
        if(setN == 0){
            setN = 1;
            /*Build number*/
            num = 0;
            while(isDigit(ch)){
                num = num * 10 + (ch - '0');
                ch = fgetc(fp);
            }
            game->n = num;
            /*If board dimensions are too big*/
            if(game->m * game->n > 99){
                printf("Error: board size is larger than 99\n");
                if(fclose(fp) == EOF){
                    printf(ERRORCLOSE);
                }
                return 0;
            }
            /*Initialize game fields*/
            initAll(game);
            continue;
        }

        if (y <= game->m * game->n) {
            block = getBlockIndex(x, y, game);
            cell = getCellIndex(x, y, game);
            /*Build number*/
            num = 0;
            while(isDigit(ch)){
                num = num * 10 + (ch - '0');
                ch = fgetc(fp);
            }
            /*check if val is in valid range*/
            if(checkValidRange(game, num, &fp) == 0){
                return 0;
            }
            /*Update the board cell with the built number*/
            (*game).board.board[block.y][block.x].block[cell.y][cell.x].val = num;
            /*Update the board fields that are affected by this num if it is not 0*/
            if (num != 0) {
                game->cellsToFill--;
                markErrors(x,y, num, game);
                updateCol(x, num, 1, game);
                updateRow(y, num, 1, game);
                updateBlock(pointToID(block.x, block.y, game), num, 1, game);
            }
            /*If the cell is fixed and we are on solve mode we need to
             * make sure there are no contradiction between 2 fixed cells*/
            if (ch == '.' && mode == 2) {
                (*game).board.board[block.y][block.x].block[cell.y][cell.x].fixed = '.';
                if(num != 0 && checkCellValid(x,y,num, game) == 0){
                    failedReadingFile(&fp,game);
                    printf("File contains contradiction between 2 fixed cells\n");
                    return 0;
                }
            }
            else if(mode == 1){
                (*game).board.board[block.y][block.x].block[cell.y][cell.x].fixed = '.';
            }

            x++;
            ch = fgetc(fp);
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
    /*If there are not enough digits*/
    if(y != game->m * game->n + 1 || x != 1){
        failedReadingFile(&fp,game);
        printf("Not enough digits in file\n");
        return 0;
    }
    if(fclose(fp) == EOF){
        printf(ERRORCLOSE);
    }
    return 1;
}

/*Creates an empty 9x9 board*/
void createEmptyBoard(Game* game){
    game->m = 3;
    game->n = 3;
    initAll(game);
}

/*Load the new board from X*/
int loadBoard(char* X, Game* game, int mode){
    Game tmpGame;
    tmpGame.memRelease = 0;
    /*if a path were provided*/
    if((X!=NULL)) {
        /*If the loading went well (1) then deepCopy the loaded board to game*/
        if(fillBoard(X, &tmpGame,mode) == 1){
            if(game->memRelease == 1){
                freeMem(game);
            }
            deepCopyGame(game, &tmpGame);
            freeMem(&tmpGame);
            return 1;
        }
        /*If loading failed we keep the previous board*/
        else{
            if(tmpGame.memRelease == 1){
                freeMem(&tmpGame);
            }
            return 0;
        }
    }
    else{
        if(game->memRelease == 1){
            freeMem(game);
        }
        /*If a path wasn't given and we are in edit mode
         * (since we checked the arguments for solve function already)
         * we create an empty 9x9 board*/
        createEmptyBoard(game);
        return 1;
    }
}

/*Update the "mark errors" setting according to users input */
void updateMarkErrors(Game* game, int setValue){
    game->board.markError = setValue;
}

/*Changes the mode of the game to the newMode*/
void changeMode(int newMode, Game* game){
    game->mode = newMode;
    /*Default mark_errors is 1*/
    updateMarkErrors(game, 1);

}

/*Return 1 if there is no contradiction between this cell to another fixed cell
 * Otherwise return 0*/
int checkCellValid(int x, int y, int z, Game* game){
    int i,j;
    Point cell;
    Point block = getBlockIndex(x,y,game);
    int id = pointToID(block.x, block.y,game);
    /*If according to our helpful arrays there is no contradiction*/
    if(((*game).rows[y - 1][z-1]==1)&&((*game).cols[x - 1][z-1]==1)&&((*game).blocks[id][z-1]==1)) {
        return 1;
    }
    /*Check if there is a contradiction with another Fixed cell*/
    for(i = 0; i < game->m * game->n; i++){
        for(j = 0; j < game->m * game->n; j++) {
            /*If we reached the cell we are currently in then
             * we can return 1 since this function is being
             * called through fillBoard which calls each
             * char in the board one by one*/
            if(x == j + 1 && y == i + 1){
                return 1;
            }
            cell = getCellIndex(j+1,i+1,game);
            block = getBlockIndex(j+1,i+1,game);
            /*If we are in the same col or row or block*/
            if(x == j + 1 || y == i + 1 || pointToID(block.x,block.y,game) == id){
                /*If the cell is fixed*/
                if(isFixed(j + 1,i + 1,game) == 1){
                    /*If it is the same value return 0*/
                    if(game->board.board[block.y][block.x].block[cell.y][cell.x].val == z){
                        return 0;
                    }
                }
            }
        }
    }
    /*If we got here then there is no contradiction between 2 fixed cells*/
    return 1;
}

/*Checks whether the board can be solved (1) or not (0)
 * Also decides if to print the result (1) or not (0)*/
/*uses the ILP solver*/
int validate(Game* game, int ind){
    int ilp=0;
    /*If board is erroneous then the board isn't solvable*/
    if(game->numOfErrors > 0){
        printf(BOARDISNOTVALID);
        return 0;
    }
    ilp = solveILP(game, 2, 0, 0);
    /*If the function was called through another one(e.g. save)*/
    if(ind == 0){
        return ilp;
    }
    if(ilp == 1){
        printf(BOARDISVALID);
    }
    else if (ilp == 0){
        printf(BOARDISNOTVALID);
    }
    else{
        printf(FUNFAILGUR, "validate");
    }
    return ilp;
}

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
