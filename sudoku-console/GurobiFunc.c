#include "GurobiFunc.h"

/*Add vars to model*/
int addVars(GRBmodel** model,int transCounter, char** vtype, double** obj){
    double* upperBound;
    double* lowerBound;
    int error = 0;
    /*Create double arrays to be the lower and upper bounds of the variables*/
    lowerBound = (double*) mallocWithGuard(transCounter * sizeof(double));
    upperBound = (double*) mallocWithGuard(transCounter * sizeof(double));
    /*Fill them to be 0.0 (lower) and 1.0(upper*/
    fillDoubleArray(&lowerBound, transCounter, 0.0);
    fillDoubleArray(&upperBound, transCounter, 1.0);

    /*Add variables to model*/
    error = GRBaddvars((*model), transCounter, 0, NULL, NULL, NULL, (*obj), lowerBound, upperBound, (*vtype), NULL);
    /*Free the bound arrays*/
    free(upperBound);
    free(lowerBound);
    if(error){
        return -1;
    }
    return 1;
}

/*updates model*/
int updateModel(GRBmodel** model){
    int error = 0;
    /*Change objective sense to maximization*/
    error = GRBsetintattr((*model), GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
    if(error){
        return -1;
    }

    /*update the model - to integrate new variables*/
    error = GRBupdatemodel((*model));
    if(error){
        return -1;
    }
    return 1;
}

/*Add constraints to each cell*/
int addCellCons(int** ind, double** val, Truple** transArray
        , GRBmodel** model, int transCounter){
    int error = 0, i, cnt = 0;
    char* conName = "cell.";
    Truple tmp;

    /*Set tmp to first truple on transArray*/
    if(transCounter > 0){
        tmp = (*transArray)[0];
        (*ind)[0] = 0;
        (*val)[0] = 1;
        cnt++;
    }
    else{
        return -1;
    }
    /*Go on every variable in transArray*/
    for(i = 1; i < transCounter; i++){
        /*If it has the same indexes as tmp add it to the var and ind arrays*/
        if((*transArray)[i].i == tmp.i && (*transArray)[i].j == tmp.j){
            (*ind)[cnt] = i;
            (*val)[cnt] = 1;
            cnt++;
        }
        /*Otherwise, add the constraint that was built until now and reset the arrays*/
        else{
            if(cnt == 0){
                continue;
            }
            error = GRBaddconstr((*model), cnt, (*ind), (*val), GRB_EQUAL, 1.0, conName);
            if(error){
                return -1;
            }
            fillIntArr(ind, transCounter, 0);
            fillDoubleArray(val, transCounter, 0);
            cnt = 0;
            (*ind)[cnt] = i;
            (*val)[cnt] = 1;
            cnt++;
        }
        tmp = (*transArray)[i];
    }

    /*Add the last cell constraint*/
    error = GRBaddconstr((*model), cnt, (*ind), (*val), GRB_EQUAL, 1.0, conName);
    if(error){
        return -1;
    }

    /*If we reached here, the function has been done*/
    return 1;
}
/*Free helpful arrays of gurobi*/
void freeHelpArr(int**** rowsCon, int**** colsCon, int**** blocksCon, int N){
    int i, j;
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            free((*rowsCon)[i][j]);
            free((*colsCon)[i][j]);
            free((*blocksCon)[i][j]);
        }
        free(((*rowsCon)[i]));
        free(((*colsCon)[i]));
        free(((*blocksCon)[i]));
    }
    free((*rowsCon));
    free((*colsCon));
    free((*blocksCon));
}

/*Free all the malloc we made in order to make the gurobi optimize the model*/
void freeArr(double** sol, int** ind, double** val, double** obj, int N, Truple** transArray,
             char** vtype,int**** rowsCon, int**** colsCon, int**** blocksCon){
    freeHelpArr(rowsCon, colsCon, blocksCon, N);
    free((*ind));
    free((*val));
    free((*sol));
    free((*obj));
    free((*vtype));
    free((*transArray));

}


/*Free everything that was allocated*/
void freeEverything(double** sol, int** ind, double** val, double** obj, int N, Truple** transArray,
                    char** vtype,int**** rowsCon, int**** colsCon, int**** blocksCon,
                    GRBenv** env, GRBmodel** model, int indModel){
    freeArr(sol, ind, val, obj, N, transArray, vtype,rowsCon, colsCon, blocksCon);
    /*If model was created successfully*/
    if(indModel == 1){
        GRBfreemodel(*model);
    }
    GRBfreeenv(*env);
}

/*initialize the malloc of the given arrays*/
void initArr(int**** arrR, int**** arrC, int**** arrB,int N){
    int i, j;
    (*arrR) = (int***)mallocWithGuard(N* sizeof(int**));
    (*arrC) = (int***)mallocWithGuard(N* sizeof(int**));
    (*arrB) = (int***)mallocWithGuard(N* sizeof(int**));
    for(i = 0; i < N; i++){
        (*arrR)[i] = (int**)mallocWithGuard(N* sizeof(int*));
        (*arrC)[i] = (int**)mallocWithGuard(N* sizeof(int*));
        (*arrB)[i] = (int**)mallocWithGuard(N* sizeof(int*));
        for(j = 0; j < N;j++){
            (*arrR)[i][j] = (int*)callocWithGuard((N+1),sizeof(int));
            (*arrC)[i][j] = (int*)callocWithGuard((N+1), sizeof(int));
            (*arrB)[i][j] = (int*)callocWithGuard((N+1), sizeof(int));
        }
    }
}

/*Fills the transArray with the corresponding Truple values*/
int fillArray(Game* game, Truple** transArray, int* transCounter,
                            int**** rowsCon, int**** colsCon, int**** blocksCon){
    int i,j,k;
    Point block, cell;
    int id, cnt =0, N = game->m * game->n;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            cnt = 0;
            block = getBlockIndex(j+1, i+1, game);
            cell = getCellIndex(j+1, i+1, game);
            /*if the cell is empty - add the relevant k values as variables*/
            if((*game).board.board[block.y][block.x].block[cell.y][cell.x].val == 0){
                for (k = 0; k < N; k++){
                    if (isValidValue(j+1,i+1,k + 1,game) == 1){
                        cnt++;
                        (*transArray)[(*transCounter)].i = i + 1;
                        (*transArray)[(*transCounter)].j = j + 1;
                        (*transArray)[(*transCounter)].k = k + 1;
                        (*rowsCon)[i][k][++((*rowsCon)[i][k][0])] = (*transCounter);
                        (*colsCon)[j][k][++((*colsCon)[j][k][0])] = (*transCounter);
                        id = pointToID(block.x, block.y,game);
                        (*blocksCon)[id][k][++((*blocksCon)[id][k][0])] = (*transCounter);
                        (*transCounter)++;
                    }
                }
                /*If there is no legal value for the current empty cell*/
                if(cnt == 0){
                    return 0;
                }

            }
        }
    }
    return 1;
}

/*Add constraints to the model from the given array - could be rows, cols or blocks*/
int addCons(int N, int** ind, double** val, char* conName, int**** arr, GRBmodel** model){
    int i, k ,l, conCounter = 0, error;
    for(i = 0; i < N; i++){
        for(k = 0; k < N; k++){
            for(l = 0; l < (*arr)[i][k][0];l++){
                (*ind)[l] = (*arr)[i][k][l+1];
                (*val)[l] = 1;
            }
            if(l > 0) {
                error = GRBaddconstr((*model), l, (*ind), (*val), GRB_EQUAL, 1.0, conName);
                if(error){
                    return -1;
                }
                conCounter++;
            }
        }
    }
    return 1;
}

/*initialize mallocWithGuard of gurobi needed arrays*/
void initGrbArr(double** sol,int** ind,double** val,double** obj,char** vtype,int transCounter){
    (*sol) = (double*)mallocWithGuard(transCounter*sizeof(double)); /*calculated values for variables*/
    (*ind) = (int*)mallocWithGuard(transCounter*sizeof(int)); /*variable array*/
    (*val) = (double*)mallocWithGuard(transCounter*sizeof(double));/* coefficients of constraint*/
    (*obj) = (double*)callocWithGuard(transCounter,sizeof(double));/* coefficients of target function*/
    (*vtype) = (char*)mallocWithGuard(transCounter*sizeof(char));/* type of variable*/
}

/*Turn printing off in gurobi and creates the model*/
int turnOffPrintAndCreateModel(GRBenv** env, GRBmodel** model){
    int error = 0;
     /* turn off gurobi's prints*/
    error = GRBsetintparam((*env), GRB_INT_PAR_LOGTOCONSOLE, 0);
    if(error){
        return -1;
    }

     /*Create an empty model named "mip1"*/
    error = GRBnewmodel((*env), model, "mip1", 0, NULL, NULL, NULL, NULL, NULL);
    if(error){
        return -1;
    }

    return 1;
}

/*set var type of the vtype array, it will be set to set
 * where set can be GRB_BINARY and etc..*/
void setVarType(char** vtype, int transCounter, char set){
    int i;
    for(i = 0; i < transCounter; i++){
        (*vtype)[i] = set;
    }
}

/*Optimize model*/
int optimizeModel(GRBmodel** model, int* optimstatus){
    int error = 0;

     /* Optimize model - need to call this before calculation*/
    error = GRBoptimize((*model));
    if(error){
        return -1;
    }

     /* Write model to 'mip1.lp' - this is not necessary but very helpful*/
    error = GRBwrite((*model), "mip1.lp");
    if(error){
        return -1;
    }

     /* Get solution information*/
    error = GRBgetintattr((*model), GRB_INT_ATTR_STATUS, optimstatus);
    if(error){
        return -1;
    }

    return 1;

}

/*Set all auxiliary arrays to 0*/
void clearLP(Game* game){
    int i, j, N;
    Point cell, block;
    N = game->n * game->m;
    for(i = 0; i < game->m * game->n; i++) {
        for (j = 0; j < game->m * game->n; j++) {
            block = getBlockIndex(j + 1, i + 1, game);
            cell = getCellIndex(j + 1, i + 1, game);
            fillDoubleArray(&(game->board.board[block.y][block.x].block[cell.y][cell.x].auxiliary), N, 0.0);
        }
    }
}

/*Set all ilp values to 0*/
void clearIlpVal(Game* game){
    int i, j;
    Point cell, block;
    for(i = 0; i < game->m * game->n; i++){
        for(j = 0; j< game->m * game->n; j++) {
            block = getBlockIndex(j + 1, i + 1, game);
            cell = getCellIndex(j + 1, i + 1, game);
            game->board.board[block.y][block.x].block[cell.y][cell.x].ILPVal = 0;
        }
    }
}

/*Randomize the coefficients(range: 1-N) of the objective function
 * Only happens when we are in LP
 *
 * Explanation of this objective function:
 * After checking several functions, we could see that
 * a randomized is the best. Then we decided to randomize
 * a number between 1 to N since it is one dimension of
 * the board and it worked better than 1-N*M.
 * Only happens when we are in LP*/
void randCoeff (double** obj, Game* game, int transCount){
    int i, N;
    N = game->n;
    for(i = 0; i < transCount; i++){
        (*obj)[i] = rand() % N + 1;
    }
}

 /*Fills the coefficients of the objection function
 * indicator = 0 means we are in ILP,
 * indicator = 1 means we are in LP*/
void fillObjFun(double** obj, int indicator, int transCount, Game* game) {
    if(indicator == 0){
        fillDoubleArray(obj, transCount, 0.0);
    }
    /*If this is an LP we randomize the coefficients*/
    else{
        randCoeff(obj, game, transCount);
    }

}

 /*Fills the relevant cell's ILPVals or Auxiliary arrays
 * opCode == 1
 * hint - should update ILPVal for X Y only or (indicator == 0)
 * guessHint - should update auxiliary array of X Y only (indicator == 1)
 *
 * opCode == 0
 * generate - should fill the whole board with ILPVal (indicator == 0)
 * guess - should fill the whole board auxiliary arrays (indicator == 1)
 */
void solAssign (double** solArray,Truple** transArray, int transCounter,
                int opCode, int X, int Y, Game* game, int indicator){
    int l, cnt = 0;
    Point block = getBlockIndex(X,Y,game);
    Point cell = getCellIndex(X,Y,game);
    /* hint or guessHint*/
    if (opCode == 1){
        for (l = 0; l < transCounter; l++){
            /*hint*/
            if(indicator == 0){
                if(((*transArray)[l].i==Y) && ((*transArray)[l].j==X) && ((*solArray)[l]==1)){
                    game->board.board[block.y][block.x].block[cell.y][cell.x].ILPVal = (*transArray)[l].k;
                    return ;
                }
            }
            /*guessHint*/
            else{
                if(((*transArray)[l].i==Y) && ((*transArray)[l].j==X)){
                    game->board.board[block.y][block.x].block[cell.y][cell.x].auxiliary[(*transArray)[l].k - 1] = (*solArray)[l];
                    cnt++;
                }
                if (cnt == game->m * game->n){
                    return;
                }
            }
        }
    }
    /*guess or generate*/
    else{
        for (l = 0; l < transCounter; l++){
            block = getBlockIndex((*transArray)[l].j,(*transArray)[l].i,game);
            cell = getCellIndex((*transArray)[l].j,(*transArray)[l].i,game);
            /*generate*/
            if(indicator == 0){
                if((*solArray)[l] == 1){
                    game->board.board[block.y][block.x].block[cell.y][cell.x].ILPVal = (*transArray)[l].k;
                    game->board.board[block.y][block.x].block[cell.y][cell.x].appendix = 's';
                }
            }
            /*guess*/
            else{
                if((*solArray)[l] > 0){
                    game->board.board[block.y][block.x].block[cell.y][cell.x].auxiliary[(*transArray)[l].k - 1] = (*solArray)[l];
                }
            }
        }
    }
}

/*Add all the relevant constraints*/
int addAllCons(GRBmodel** model, int**** rowsCon, int**** colsCon, int**** blocksCon,
                        int** ind, double** val, int N, Truple** transArray, int transCounter){

     /*Add constraints per cell*/
     if(addCellCons(ind, val, transArray, model, transCounter) == -1){
         return -1;
     }

     /*Add rows constraints*/
    if(addCons(N, ind, val, "row.", rowsCon, model) == -1){
    return -1;
    }

     /*Add cols constraints*/
    if(addCons(N, ind, val, "col.", colsCon, model) == -1){
    return -1;
    }

     /*Add blocks constraints*/
    if(addCons(N, ind, val, "block.", blocksCon, model) == -1){
    return -1;
    }

    return 1;
}

/*General solve function which will use both LP and ILP
 * decided by the indicator.
 * indicator = 0 is ILP
 * indicator = 1 is LP
 * returns 1 if was done successfully
 * returns 0 if all technical part went well and the optimization failed
 * returns -1 if a technical gurobi part failed*/
int solveGenral(int indicator, Game* game, int opCode, int x, int y){
    GRBenv   *env   = NULL;
    GRBmodel *model = NULL;
    int transCounter = 0, N = (*game).m*(*game).n, error = 0;
    int varNum = (*game).cellsToFill;
    int*** rowsCon;
    int*** colsCon;
    int*** blocksCon;
    double*    sol;
    int*       ind;
    double*    val;
    double*    obj;
    char*      vtype;
    int       optimstatus; /*status of calculation, was the optimization succesfull?*/
    double objval;

    /*Initializing arrays that are not depended on transCounter*/
    Truple*  transArray = (Truple*)mallocWithGuard(varNum * N * sizeof(Truple));
    initArr(&rowsCon, &colsCon, &blocksCon, N);

    /*Fill transArray by the explanation in top of this file and update transCounter
     * If there is a cell that there is no legal value for it - return 0*/
    if(fillArray(game, &transArray,&transCounter, &rowsCon, &colsCon, &blocksCon) == 0){
        free(transArray);
        freeHelpArr(&rowsCon, &colsCon, &blocksCon, N);
        return 0;
    }

   /*Initializing the needed gurobi arrays with the number of variables - transCounter */
    initGrbArr(&sol, &ind, &val, &obj, &vtype, transCounter);

    /*Create environment - log file is mip1.log*/
    error = GRBloadenv(&env, "mip1.log");
    if(error){
        freeEverything(&sol,&ind,&val,&obj,N, &transArray,
                       &vtype,&rowsCon,&colsCon, &blocksCon, &env, & model, 0);

        return -1;
    }

    /*Turns off printing and creates the model for this board*/
    if(turnOffPrintAndCreateModel(&env, &model) == -1){
        freeEverything(&sol,&ind,&val,&obj,N, &transArray,
        &vtype,&rowsCon,&colsCon, &blocksCon, &env, & model, 0);
        return -1;
    }

   /*Fill the objection function*/
    fillObjFun(&obj, indicator, transCounter, game);

    /*Fills the vtype array according to the indicator (0 - ILP, 1 - LP)*/
    if(indicator == 0){
        setVarType(&vtype, transCounter, GRB_BINARY);
    }
    else{
        setVarType(&vtype, transCounter, GRB_CONTINUOUS);
    }

    /*Add vars to the model*/
    if(addVars(&model, transCounter, &vtype, &obj) == -1){
        freeEverything(&sol,&ind,&val,&obj,N, &transArray,
                       &vtype,&rowsCon,&colsCon, &blocksCon, &env, & model,1);
        return -1;
    }

    /*Updates the model*/
    if(updateModel(&model) == -1){
        freeEverything(&sol,&ind,&val,&obj,N, &transArray,
                       &vtype,&rowsCon,&colsCon, &blocksCon, &env, & model,1);
        return -1;
    }

    /*Add all the relevant constraints*/
    if(addAllCons(&model, &rowsCon, &colsCon, &blocksCon, &ind, &val, N, &transArray, transCounter) == -1){
        freeEverything(&sol,&ind,&val,&obj,N, &transArray,
                       &vtype,&rowsCon,&colsCon, &blocksCon, &env, & model, 1);
        return -1;
    }

    /* Optimize the model*/
    if(optimizeModel(&model, &optimstatus) == -1){
        freeEverything(&sol,&ind,&val,&obj,N, &transArray,
                       &vtype,&rowsCon,&colsCon, &blocksCon, &env, & model,1);
        return 0;
    }

    error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
    if(error){
        freeEverything(&sol,&ind,&val,&obj,N, &transArray,
                       &vtype,&rowsCon,&colsCon, &blocksCon, &env, & model,1);
        return 0;
    }


    error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, transCounter, sol);
    if(error){
        freeEverything(&sol,&ind,&val,&obj,N, &transArray,
                       &vtype,&rowsCon,&colsCon, &blocksCon, &env, & model,1);
        return 0;
    }

    /*  solution found*/
    if (optimstatus == GRB_OPTIMAL || optimstatus == GRB_INF_OR_UNBD) {
        /*If we are in guess command or hint we need to fill the ILPVal field in each cell
         * Same goes for guess_hint or generate, then we need to fill auiliary arrays*/
        if (opCode != 2){
                solAssign(&sol, &transArray, transCounter, opCode, x, y, game, indicator);
        }
    }
    /* error or calculation stopped*/
    else {
        freeEverything(&sol,&ind,&val,&obj,N, &transArray,
                       &vtype,&rowsCon,&colsCon, &blocksCon, &env, & model,1);
        return -1;
    }

    /*If we got here, it means everything went alright.
     * So now, we need to free every mallocWithGuard we have done*/
    freeEverything(&sol,&ind,&val,&obj,N, &transArray,
                   &vtype,&rowsCon,&colsCon, &blocksCon, &env, & model,1);
    return 1;
}


/*Solves the current board using Linear Programming
 * Fills the probs array of the relvant cells in the board
 * guess = 0 fills all cells, guessHint = 1 fills only X Y cell
 * Return 0 if there is no solution*/
int solveLP (Game* game, int opCode, int x, int y){
    clearLP(game);
    return solveGenral(1, game, opCode, x, y);
}


/*Solves the current board using Linear Programming
 * Fills the probs array of the relvant cells in the board
 * guess = 0 fills all cells, guessHint = 1 fills only X Y cell
 * Return 0 if there is no solution*/
int solveILP (Game* game, int opCode, int x, int y){
    clearIlpVal(game);
    return solveGenral(0, game, opCode, x, y);
}
