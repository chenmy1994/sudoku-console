/*
*	Created by CMY on 29/07/2019.
 */

#include "SolveFunc.h"
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define autofillMSG "Set single legal value %d for Cell <%d,%d>\n"

/*Fills all cell values with probability greater than users input*/
void guess(int x, Game* game){
    /*this is just bullshit so make will compile*/
    x = game->n;
    if ( x > 0){
        return;
    }
}

int checkBeforeHint(int x, int y, Game* game){
    int val;
    Point block, cell;
    if(game->mode != 2){
        printf(ERRORSOLVEMODE);
        return 0;
    }
    if(isFixed(x,y,game) == 1){
        printf(CELLISFIXED, x, y);
        return 0;
    }
    block = getBlockIndex(x,y, game);
    cell = getCellIndex(x,y, game);
    val = game->board.board[block.y][block.x].block[cell.y][cell.x].val;
    if(val != 0){
        printf(CELLISFILLED, x, y);
        return 0;
    }
    return 1;
}
/*Shows a guess to the user for a single cell X,Y*/
void guessHint(int x, int y, Game* game){
    if(checkBeforeHint(x,y, game) == 0){
        return;
    }
    if(solveLP(game, 1, x, y) == 1){
        printGuessAndScores(x,y,game);
        return;
    }
    printf(BOARDISNOTVALID);
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

/*Give a hint to the user by showing the solution of the input cell*/
void hint(int x, int y,Game* game){
    Point block, cell;
    int hint;
    block = getBlockIndex(x,y, game);
    cell = getCellIndex(x,y, game);
    if(checkBeforeHint(x,y, game) == 0){
        return;
    }
    if(solveILP(game, 1, x, y) == 1){
        hint = game->board.board[block.y][block.x].block[cell.y][cell.x].ILPVal;
        printf("Hint: set cell <%d,%d> to %d\n",x , y, hint);
        return;
    }
    printf(BOARDISNOTVALID);
}

/*Automatically fill obvious values - cells which contain a single legal value.
 * return 0 if the board has erroneous, otherwise 1*/
int autofill(Game* game){
	int cnt,singleVal,i,j,N,size;
	Point block,cell,**moveCell;
	if((*game).numOfErrors>0){
		printf(errorErroneous);
		return 0;
	}
	cnt=0;
	N=(*game).n*(*game).m;
	size=(*game).cellsToFill;
	(moveCell)=(Point**)malloc(sizeof(Point*));
	(*moveCell)=(Point*)malloc(size*sizeof(Point));

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

	fillSingleValue(moveCell,cnt,game);
	addMove(moveCell,cnt,game);
	return 1;

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


