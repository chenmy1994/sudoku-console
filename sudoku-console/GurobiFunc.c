/*
*	Created by CMY on 30/07/2019.
 */

/*
 * transArray - a Truple array: every index represents the number of the variable
 *              in the gurobi model and each cell contains a Truple which is
 *              i, j, k that represent row, col, value.
 *              For example, let's say transArray[8] contains the Truple 2, 4, 7
 *              it means that the variable X1,3,6 is named 8.
 * rowsCon - a 3dim int array: every cell holds the number of the variable
 *           in the gurobi model which belongs to the corresponding constraint.
 *           For example, rowsCon[2][4] is a one dim array of ints that holds
 *           all the numbers of the variables in the gurobi model which are in
 *           form X3,5,k for any k
 * */

#include "GurobiFunc.h"

void calculateObjFun(double **obj);

/*initialize the malloc of the given arrays*/
void initArr(int**** arrR, int**** arrC, int**** arrB,int N){
    int i, j;
    (*arrR) = (int***)malloc(N* sizeof(int**));
    (*arrC) = (int***)malloc(N* sizeof(int**));
    (*arrB) = (int***)malloc(N* sizeof(int**));

    for(i = 0; i < N; i++){
        (*arrR)[i] = (int**)malloc(N* sizeof(int*));
        (*arrC)[i] = (int**)malloc(N* sizeof(int*));
        (*arrB)[i] = (int**)malloc(N* sizeof(int*));
        for(j = 0; j < N;j++){
            (*arrR)[i][j] = (int*)calloc((N+1),sizeof(int));
            (*arrC)[i][j] = (int*)calloc((N+1), sizeof(int));
            (*arrB)[i][j] = (int*)calloc((N+1), sizeof(int));
        }
    }
}


/*Fills the transArray with the corresponding Truple values*/
void fillArray(Game* game, Truple** transArray, int* transCounter,
                            int**** rowsCon, int**** colsCon, int**** blocksCon){
    int i,j,k;
    Point block, cell;
    int id, N = game->m * game->n;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            block = getBlockIndex(j+1, i+1, game);
            cell = getCellIndex(j+1, i+1, game);
            /*if the cell is empty - add the relevant k values as variables*/
            if((*game).board.board[block.y][block.x].block[cell.y][cell.x].val == 0){
                for (k = 0; k < N; k++){
                    if (isValidValue(j+1,i+1,k + 1,game) == 1){
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

            }
        }
    }
}

/*Add constraints to the model from the given array - could be rows, cols or blocks*/
int addCons(int N, int** ind, double** val, char* conName, int**** arr, GRBenv** env, GRBmodel** model){
    int i, k ,l, conCounter = 0, error;
 /*   char c;*/

    for(i = 0; i < N; i++){
        for(k = 0; k < N; k++){
            for(l = 0; l < (*arr)[i][k][0];l++){
                (*ind)[l] = (*arr)[i][k][l+1];
                (*val)[l] = 1;
            }
            if(l > 0) {
                /*   add constraint to model - note size indCounter + operator GRB_EQUAL*/
                /*-- equation value (1.0) + unique constraint name*/
                /*c = conCounter + '0';
                strcat(conName,&c);*/
                error = GRBaddconstr((*model), l, (*ind), (*val), GRB_EQUAL, 1.0, conName);
                if(checkAndHandleError(error, env, "GRBaddconstr()") == -1){
                    return -1;
                }
                conCounter++;
            }
        }
    }
    return 1;
}

/*Checks and handles error during Gurobi work*/
int checkAndHandleError(int error,GRBenv** env, char* string){
    if (error) {
        printf("ERROR %d, %s: %s\n", error,string, GRBgeterrormsg((*env)));
        return -1;
    }
    return 1;
}

/*initialize malloc of gurobi needed arrays*/
void initGrbArr(double** sol,int** ind,double** val,double** obj,char** vtype,int transCounter){
    (*sol) = (double*)malloc(transCounter*sizeof(double)); /*calculated values for variables*/
    (*ind) = (int*)malloc(transCounter*sizeof(int)); /*variable array*/
    (*val) = (double*)malloc(transCounter*sizeof(double)); /*coefficients of constraint*/
    (*obj) = (double*)calloc(transCounter,sizeof(double)); /*coefficients of target function*/
    (*vtype) = (char*)malloc(transCounter*sizeof(char)); /*type of variable*/
}

/*Turn printing off in gurobi and creates the model*/
int turnOffPrintAndCreateModel(GRBenv** env, GRBmodel** model){
    int error = 0;
    /*  turn off gurobi's prints*/
    error = GRBsetintparam((*env), GRB_INT_PAR_LOGTOCONSOLE, 1);
    if(checkAndHandleError(error, env, "GRBsetintattr()") == -1){
        return -1;
    }

    /*    Create an empty model named "mip1"*/
    error = GRBnewmodel((*env), model, "mip1", 0, NULL, NULL, NULL, NULL, NULL);
    if(checkAndHandleError(error, env, "GRBnewmodel()") == -1){
        return -1;
    }

    return 1;
}

/*set var type of the vtype array, it will be set to set
 * where set can be GRB_BINART and etc..*/
void setVarType(char** vtype, int transCounter, char set){
    int i;
    for(i = 0; i < transCounter; i++){
        (*vtype)[i] = set;
    }
}

/*Optimize model*/
int optimizeModel(GRBenv** env, GRBmodel** model, int* optimstatus){
    int error = 0;

    /*  Optimize model - need to call this before calculation*/
    error = GRBoptimize((*model));
    if(checkAndHandleError(error, env, "GRBoptimize()") == -1){
        return -1;
    }

    /*  Write model to 'mip1.lp' - this is not necessary but very helpful*/
    error = GRBwrite((*model), "mip1.lp");
    if(checkAndHandleError(error, env, "GRBwrite()") == -1){
        return -1;
    }

    /*  Get solution information*/
    error = GRBgetintattr((*model), GRB_INT_ATTR_STATUS, optimstatus);
    if(checkAndHandleError(error, env, "GRBgetintattr()") == -1){
        return -1;
    }
    printf("optim is %d\n", (*optimstatus));

    /*get the objective -- the optimal result of the function*/



    return 1;

}

/*Solves the current board using Linear Programming
 * Fills the probs array of the relvant cells in the board
 * guess = 0 fills all cells, guessHint = 1 fills only X Y cell
 * Return 0 if there is no solution*/
int solveLP (Game* game, int opCode, int x, int y){
    return solveGenral(1, game, opCode, x, y);

}

/*Solves the current board using Linear Programming
 * Fills the probs array of the relvant cells in the board
 * guess = 0 fills all cells, guessHint = 1 fills only X Y cell
 * Return 0 if there is no solution*/
int solveILP (Game* game, int opCode, int x, int y){
    return solveGenral(0, game, opCode, x, y);
}

/*Fill double array with zeroes*/
void fillZeroesDouble(double** arr, int len){
    int i;
    for(i=0; i< len; i++){
        (*arr)[i] = 0;
    }
}

/* Fills the coefficents of the objection function
 * indicator = 0 means we are in ILP,
 * indicator = 1 means we are in LP*/
void fillObjFun(double** obj, int indicator, int transCount) {
    if(indicator == 0){
        fillZeroesDouble(obj, transCount);
    }
    else{
        calculateObjFun(obj);
    }
}

/*calculates the coefficents of the objection function in LP*/
void calculateObjFun(double **obj) {
    (*obj)[0] = 1;
    /*TODO - think of a good objection function*/

}


/* Fills the relevant cell's ILPVals
 * generate = 0 fills all cells, hint = 1 fills only X Y cell, validate = 2
 * Returns 0 if there is no solution*/
void solAssign (double** solArray,Truple** transArray, int transCounter, int opCode, int X, int Y, Game* game){
    int l;
    Point block = getBlockIndex(X,Y,game);
    Point cell = getCellIndex(X,Y,game);
    if (opCode == 1){ /*hint, should update ILPVal for X Y only*/
        for (l = 0; l < transCounter; l++){
            if(((*transArray)[l].i==Y) && ((*transArray)[l].j==X) && ((*solArray)[l]==1)){
                game->board.board[block.y][block.x].block[cell.y][cell.x].ILPVal = (*transArray)[l].k;
                return ;
            }
        }
    }
    else{
        for (l = 0; l < transCounter; l++){
            if((*solArray)[l] == 1){
                block = getBlockIndex((*transArray)[l].j,(*transArray)[l].i,game);
                cell = getCellIndex((*transArray)[l].j,(*transArray)[l].i,game);
                game->board.board[block.y][block.x].block[cell.y][cell.x].ILPVal = (*transArray)[l].k;
                return ;
            }
        }
    }
}


/* Add the coefficients of the objective function to the model
 * and add all the relevant constraints */
int addAllCons(GRBmodel** model, GRBenv** env, int**** rowsCon, int**** colsCon, int**** blocksCon,
                        int** ind, double** val, int N){
    if(addCons(N, ind, val, "row.", rowsCon, env, model) == -1){
    return -1;
    }

    if(addCons(N, ind, val, "col.", colsCon, env, model) == -1){
    return -1;
    }

    if(addCons(N, ind, val, "block.", blocksCon, env, model) == -1){
    return -1;
    }

    return 1;
}

/*General solve function which will use both LP and ILP
 * decided by the indicator.
 * indicator = 0 is ILP
 * indicator = 1 is LP*/
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


    Truple*  transArray = (Truple*)malloc(varNum * N * sizeof(Truple));

    initArr(&rowsCon, &colsCon, &blocksCon, N);

    /*Fill transArray by the explanation in top of this file*/
    fillArray(game, &transArray,&transCounter, &rowsCon, &colsCon, &blocksCon);

    /*initialize the mallocs while using a more specified number*/
    initGrbArr(&sol, &ind, &val, &obj, &vtype, transCounter);

    /*   TODO - Move Create environment to setGlobals*/
    /*  Create environment - log file is mip1.log*/
    error = GRBloadenv(&env, "mip1.log");
    if(checkAndHandleError(error, &env, "GRBloadenv()") == -1){
        return -1;
    }

    if(turnOffPrintAndCreateModel(&env, &model) == -1){
        return -1;
    }


    /*fill the objection function*/
    fillObjFun(&obj, indicator, transCounter);

    if(indicator == 0){
        setVarType(&vtype, transCounter, GRB_BINARY);
    }
    else{
        setVarType(&vtype, transCounter, GRB_CONTINUOUS);
    }

    /*Add vars to the model*/
    if(addVars(&model, transCounter, &vtype, &obj, &env) == -1){
        return -1;
    }


    /*updates the model*/
    if(updateModel(&env, &model) == -1){
        return -1;
    }
    /*add constraints per cell*/
    if(addCellCons(&ind, &val, &transArray,&env, &model, transCounter) == -1){
        return -1;
    }

    if(addAllCons(&model, &env, &rowsCon, &colsCon, &blocksCon, &ind, &val, N) == -1){
        return -1;
    }

    if(optimizeModel(&env, &model, &optimstatus) == -1){
        return -1;
    }

    error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
    if(checkAndHandleError(error, &env, "GRBgettdblattr()") == -1){
        return -1;
    }


    /*  get the solution - the assignment to each variable*/
    /*transCounter--; */
    /*number of variables, the size of "sol" should match*/

    error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, transCounter, sol);
    if(checkAndHandleError(error, &env, "GRBgetdblattrarray()") == -1){
        return -1;
    }

    /*  solution found*/
    if (optimstatus == GRB_OPTIMAL || optimstatus == GRB_INF_OR_UNBD) {
        /*If we are in guess command or hint we need to fill the ILPVal field in each cell*/
        if (opCode != 2){
            solAssign(&sol, &transArray, transCounter, opCode, x, y, game);
        }
    }
        /*    error or calculation stopped*/
    else {
        printf("Optimization was stopped early\n");
        return -1;
    }

    /*  IMPORTANT !!! - Free model and environment*/
    freeArr(&sol, &ind, &val, &obj, N, &transArray, &vtype,&rowsCon, &colsCon, &blocksCon);
    GRBfreemodel(model);

    /* TODO - Move free env to exit*/
    GRBfreeenv(env);
    return 1;

}

/*Free all the malloc we made in order to make the gurobi optimize the model*/
void freeArr(double** sol, int** ind, double** val, double** obj, int N, Truple** transArray,
             char** vtype,int**** rowsCon, int**** colsCon, int**** blocksCon){
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

    free((*ind));
    free((*val));
    free((*sol));
    free((*obj));
    free((*vtype));
    free((*transArray));

}

/*Add vars to model*/
int addVars(GRBmodel** model,int transCounter, char** vtype, double** obj, GRBenv** env){
    int error = 0;
    /*  add variables to model*/
    error = GRBaddvars((*model), transCounter, 0, NULL, NULL, NULL, (*obj), NULL, NULL, (*vtype), NULL);
    if(checkAndHandleError(error, env, "GRBaddvars()") == -1){
        return -1;
    }
    return 1;
}

/*Add constraints to each cell*/
int addCellCons(int** ind, double** val, Truple** transArray,
                GRBenv** env, GRBmodel** model, int transCounter){
    int error = 0, i, cnt = 0;
    char* conName = "Cell.";
    Truple tmp;
    if(transCounter > 0){
        tmp = (*transArray)[0];
        (*ind)[0] = 0;
        (*val)[0] = 1;
    }
    else{
        return -1;
    }
    for(i = 1; i < transCounter; i++){
        if((*transArray)[i].i == tmp.i && (*transArray)[i].j == tmp.j){
            cnt++;
            (*ind)[cnt] = i;
            (*val)[cnt] = 1;
        }
        else{
            if(cnt == 0){
                continue;
            }
            error = GRBaddconstr((*model), cnt, (*ind), (*val), GRB_EQUAL, 1.0, conName);
            if(checkAndHandleError(error, env, "GRBaddconstr()") == -1){
                return -1;
            }
            cleanArr(ind, val, transCounter);
            cnt = 0;
            (*ind)[cnt] = i;
            (*val)[cnt] = 1;
        }
        tmp = (*transArray)[i];

    }
    cnt++;
    (*ind)[cnt] = i;
    (*val)[cnt] = 1;
    error = GRBaddconstr((*model), cnt, (*ind), (*val), GRB_EQUAL, 1.0, conName);
    if(checkAndHandleError(error, env, "GRBaddconstr()") == -1){
        return -1;
    }

    return 1;

}

/*fill the arrays with zeroes*/
void cleanArr(int** ind, double** val, int transCounter){
    int i;
    for(i = 0; i< transCounter; i++){
        (*ind)[i] = 0;
        (*val)[i] = 0;
    }
}

/*updates model*/
int updateModel(GRBenv** env, GRBmodel** model){
    int error = 0;
    /*Change objective sense to maximization*/
    error = GRBsetintattr((*model), GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
    if(checkAndHandleError(error, env, "GRBsetintattr()") == -1){
        return -1;
    }

    /*update the model - to integrate new variables*/
    error = GRBupdatemodel((*model));
    if(checkAndHandleError(error, env, "GRBupdatemodel()") == -1){
        return -1;
    }
    return 1;
}