/*
*	Created by CMY on 29/07/2019.
 */

#include "SolveFunc.h"
#define autofillMSG "Set single legal value %d for Cell <%d,%d>\n"
#define NOCELLSTOFILL "There were no 'obvious' values to fill\n"

/* Fill the given cell x - col, y - row with the value z
 * we can use this function since we fill only legal values
 * therefore we don't need the complex set function*/
void fillValue(int x, int y, int z, Game* game){
    Point block, cell;
    block = getBlockIndex(x,y, game);
    cell = getCellIndex(x,y, game);
    /*Fill the cell's value and update the helpful arrays*/
    game->board.board[block.y][block.x].block[cell.y][cell.x].val = z;
    updateCol(x, z, 1, game);
    updateRow(y, z, 1, game);
    updateBlock(pointToID(block.x, block.y, game), z, 1, game);

}

/* When given cell x - col, y - row, it randomize a legal value
 * Legal value means it is possible for it to be put in that cell
 * and has a higher score than the threshold*/
int randValue(int x, int y, Point* moveCell,Game* game, double threshold){
    int i, cnt = 0;
    double* scores ,sum = 0.0, randVal;
    double* probs;
    int* index;
    Point block, cell;
    /*create an array for probabilities*/
    probs = (double*) mallocWithGuard(game->m * game->n * sizeof(double));
    /*create an array for the index values of the probabilities*/
    index = (int*) mallocWithGuard(game->m * game->n * sizeof(int));

    block = getBlockIndex(x,y, game);
    cell = getCellIndex(x,y, game);
    /*scores is the values we get from the gurobi function*/
    scores = game->board.board[block.y][block.x].block[cell.y][cell.x].auxiliary;
    /*For every possible value*/
    for (i = 0; i < game->n * game->m; i++){
        /*If the value score is higher than threshold*/
        if(scores[i] >= threshold){
            /*If the value is legal for the cell (x,y)*/
            if(isValidValue(x,y,i + 1, game) == 1){
                sum+= scores[i];
                probs[cnt] = scores[i];
                index[cnt] = i + 1;
                cnt++;
            }
        }
    }
    /*Normalize the probabilities*/
    for(i = 0; i < cnt; i++){
        probs [i] = probs[i] / sum;
    }

    /*Randomize a legal value with the corresponding probabilities*/
    randVal = (double) rand() / RAND_MAX;
    for(i = 0; i < cnt; i++){
        sum += probs[i];
        if(randVal < sum){
            fillValue(x,y,index[i], game);
            (*moveCell).x=x;
            (*moveCell).y=y;
            (*moveCell).prev=0;
            (*moveCell).curr=index[i];
            free(probs);
            free(index);
            return 1;
        }
    }
    free(probs);
    free(index);
    return 0;
}


/* Fills the values of each cell by the solution we got from solveLP
 * Also computes it by the orders in the exercise*/
void fillGuessValues(Game* game, double threshold){
    int i, j, val, size, cnt = 0;
    Point block, cell, **moveCell;
	size = (*game).cellsToFill;
	(moveCell) = (Point**)mallocWithGuard(sizeof(Point*));
	(*moveCell) = (Point*)mallocWithGuard(size*sizeof(Point));

    for(i = 0; i <game->n * game->m; i++){
        for(j = 0; j <game->n * game->m; j++) {
            block = getBlockIndex(j + 1, i + 1, game);
            cell = getCellIndex(j + 1, i + 1, game);
            val = game->board.board[block.y][block.x].block[cell.y][cell.x].val;
            if(val == 0){
                if(randValue(j + 1, i + 1, &((*moveCell)[cnt]), game, threshold) == 1){
                    val = game->board.board[block.y][block.x].block[cell.y][cell.x].val;
                    printf("Guess: cell <%d,%d> was set to %d.\n", j+1,i+1, val);
                    cnt++;
                }
            }
        }
    }
    /*Update number of cellsToFill*/
    (*game).cellsToFill-=cnt;
	addMove(moveCell,cnt,game);
}

/*When given cell x,y (col, row) it prints all the values that got a score higher than 0
 * on the LP solver and their scores.*/
void printGuessAndScores(int x, int y, Game* game){
    Point block, cell;
    int i;
    double* aux;
    block = getBlockIndex(x,y, game);
    cell = getCellIndex(x,y, game);
    aux = game->board.board[block.y][block.x].block[cell.y][cell.x].auxiliary;
    printf("Hint: below is a list of (values,scores) of cell <%d, %d>:\n", x, y);
    for(i = 0; i < game->m * game->n; i++){
        if(aux[i] > 0){
            printf("(%d,%f)\n", i + 1, aux[i]);
        }
    }
}

/*General check which need to be executed before hint and guess_hint*/
int checkBeforeHint(int x, int y, Game* game){
    int val;
    Point block, cell;

    /*Checks if we are in Solve mode*/
    if(game->mode != 2){
        printf(ERRORSOLVEMODE);
        return 0;
    }

    /*Checks if the user wants to get hint for a fixed cell,
     * if he does - an Error is printed*/
    if(isFixed(x,y,game) == 1){
        printf(CELLISFIXED, x, y);
        return 0;
    }

    /*Checks if the user wants to get hint for an already filled cell,
     * if he does - an Error is printed*/
    block = getBlockIndex(x,y, game);
    cell = getCellIndex(x,y, game);
    val = game->board.board[block.y][block.x].block[cell.y][cell.x].val;
    if(val != 0){
        printf(CELLISFILLED, x, y);
        return 0;
    }

    /*If we got here then everything is alright*/
    return 1;
}

/*Shows a guess to the user for a single cell X,Y*/
void guessHint(int x, int y, Game* game){
    int lp;
    /*Check conditions before executing the function*/
    if(checkBeforeHint(x,y, game) == 0){
        return;
    }
    /*Run gurobi and fill auxiliary fields*/
    lp = solveLP(game, 1, x, y);
    if(lp == 1){
        printGuessAndScores(x,y,game);
        return;
    }
    /*If the board isn't solvable*/
    else if (lp == 0){
        printf(BOARDISNOTVALID);
        return;
    }
    /*If ilp == -1 it means that there was a techincal error in gurobi
    * and we take care of it inside gurobi functions*/
    printf(FUNFAILGUR, "guess_hint");
}

/*Give a hint to the user by showing the solution of the input cell*/
void hint(int x, int y,Game* game){
    Point block, cell;
    int hint, ilp;
    block = getBlockIndex(x,y, game);
    cell = getCellIndex(x,y, game);
    /*Check conditions before executing the function*/
    if(checkBeforeHint(x,y, game) == 0){
        return;
    }
    /*Run gurobi and fill ILPVal fields*/
    ilp = solveILP(game, 1, x, y);
    if(ilp == 1){
        hint = game->board.board[block.y][block.x].block[cell.y][cell.x].ILPVal;
        printf("Hint: set cell <%d,%d> to %d\n",x , y, hint);
        return;
    }
    /*If the board isn't solvable*/
    else if (ilp == 0){
        printf(BOARDISNOTVALID);
        return;
    }
    /*If ilp == -1 it means that there was a techincal error in gurobi
     * and we take care of it inside gurobi functions*/
    printf(FUNFAILGUR, "hint");
}

/*the Set value part of autofill function*/
void setAF(int x, int y, int z, Game* game){
    Point block, cell;
    int id;
    block = getBlockIndex(x,y,game);
    cell = getCellIndex(x,y,game);
    id = pointToID(block.x, block.y, game);

    game->board.board[block.y][block.x].block[cell.y][cell.x].val = z;
    markErrors(x, y, z, game);
    game->cellsToFill--;

    updateBlock(id, z,1,game);
    updateRow(y, z,1,game);
    updateCol(x, z,1,game);

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

/*Receives coordinates of a cell and return the single valid value of this cell if exists
 * otherwise - return 0; */
int singleValidValue(int col, int row,Game* game){
    int optCount=0,singleValue=0,i,N=(*game).n*(*game).m;

    for(i = 0; i < N; i++){
        if(isValidValue(col, row, i + 1, game) == 1){
            optCount++;
            singleValue=i+1;
        }
    }
    if(optCount<2){
        return(singleValue);
    }
    return(0);
}

/*Receives an array of points and its size - filling the relevant cells with the single value*/
void fillSingleValue(Point** valArray,int cnt,Game* game){
    int i,x,y,z;
    for (i=0;i<cnt;i++){
        x=(*valArray)[i].x;
        y=(*valArray)[i].y;
        z=(*valArray)[i].curr;
        printf(autofillMSG,z,x,y);
        /*set AutoFill*/
        setAF(x,y,z,game);
    }
}

/*Automatically fill obvious values - cells which contain a single legal value.
 * return 0 if the board has erroneous, otherwise 1*/
int autofill(Game* game){
	int cnt,singleVal,i,j,N,size;
	Point block,cell,**moveCell;
	if((*game).numOfErrors>0){
		printf(errorErroneous, "autofill");
		return 0;
	}
	cnt=0;
	N=(*game).n*(*game).m;
	size=(*game).cellsToFill;
	(moveCell)=(Point**)mallocWithGuard(sizeof(Point*));
	(*moveCell)=(Point*)mallocWithGuard(size*sizeof(Point));

	for (i=0;i<N;i++){
		for(j=0;j<N;j++){
			cell=getCellIndex(j+1,i+1,game);
			block=getBlockIndex(j+1,i+1,game);
			if(game->board.board[block.y][block.x].block[cell.y][cell.x].val == 0){
				singleVal=singleValidValue(j+1,i+1,game);
				if(singleVal!=0){
					(*moveCell)[cnt].x=j+1;
					(*moveCell)[cnt].y=i+1;
					(*moveCell)[cnt].prev=game->board.board[block.y][block.x].block[cell.y][cell.x].val;
					(*moveCell)[cnt].curr=singleVal;
					cnt++;
				}
			}
		}
	}
    if(cnt == 0){
        printf(NOCELLSTOFILL);
    }
	fillSingleValue(moveCell,cnt,game);
	addMove(moveCell,cnt,game);
	return 1;

}


/*Fills all cell values with probability greater than users input*/
int guess(double x, Game* game){
    int lp;
    /*If the board is erroneous*/
    if(game->numOfErrors > 0){
        printf(errorErroneous, "guess");
        return 0;
    }
    /*Solve the board*/
    lp = solveLP(game, 0, 0, 0);
    /*if the board is solvable fill guess values*/
    if(lp == 1){
        fillGuessValues(game, x);
        return 1;
    }
    else if (lp == 0){
        printf(BOARDISNOTVALID);
        return 0;
    }
    printf(FUNFAILGUR, "guess");
    return 0;
}


