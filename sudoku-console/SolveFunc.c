/*
*	Created by CMY on 29/07/2019.
 */

#include "SolveFunc.h"

/*Fills all cell values with probability greater than users input*/
void guess(){

}

/*Shows a guess to the user for a single cell X,Y*/
void guessHint(){

}

/*Give a hint to the user by showing the solution of the input cell*/
void hint(Game* game,int x, int y){
    Point block, cell;
    int hint;
    block = getBlockIndex(x,y, game);
    cell = getCellIndex(x,y, game);

    /*if(solveILP(game, 1, x, y) == 1){
        hint = game->board.board[block.y][block.x].block[cell.y][cell.x].ILPVal;
        printf("Hint: set cell %d %d to %d\n",x , y, hint);
        return;
    }*/



}

/*Automatically fill obvious values - cells which contain a single legal value.
 * return 0 if the board has erroneous, otherwise 1*/
int autofill(Game* game){
	if((*game).numOfErrors>0){
		printf(errorErroneous);
		return 0;
	}

	int cnt=0,singleVal,i,j,N=(*game).n*(*game).m;
	Point block,cell,p,**valCell;
	(valCell)=(Point**)malloc(sizeof(Point*));
	(*valCell)=(Point*)malloc(N*sizeof(Point));

	for (i=0;i<N;i++){
		for(j=0;j<N;j++){

			cell=getCellIndex(j+1,i+1,game);
			block=getBlockIndex(j+1,i+1,game);

			if(game->board.board[block.y][block.x].block[cell.y][cell.x].val == 0){
				singleVal=singleValidValue(game,j+1,i+1);
				if(singleVal!=0){
					p.x=j+1;
					p.y=i+1;
					p.prev=game->board.board[block.y][block.x].block[cell.y][cell.x].val;
					p.curr=singleVal;
					(*valCell)[cnt]=p;
					cnt++;
				}
			}
		}
	}

	fillSingleValue(game,valCell,cnt);
	return 1;
}

/*Receives coordinates of a cell and return the single valid value of this cell if exists
 * otherwise - return 0; */
int singleValidValue(Game* game,int col, int row){
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
void fillSingleValue(Game* game, Point** valArray,int cnt){
	int i,x,y,z;
	for (i=0;i<cnt;i++){
		x=(*valArray)[i].x;
		y=(*valArray)[i].y;
		z=(*valArray)[i].curr;
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
			printf(PUZZLESOLVED);
			/*Change mode to init*/
			game->mode = 0;
			return;
		}
        printf(ERRORSOL);
        return;
	}
}


