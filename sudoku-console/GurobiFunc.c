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

/*initialize the malloc of the given arrays*/
void initArr(int**** arrR, int**** arrC, int**** arrB, int** ind, double ** val, int N){
    int i, j;
    for(i=0; i < N; i++){
        *(arrR[i]) = (int**)malloc(N* sizeof(int*));
        *(arrC[i]) = (int**)malloc(N* sizeof(int*));
        *(arrB[i]) = (int**)malloc(N* sizeof(int*));
        for(j=0;j<N;j++){
            *(arrR[i][j]) = (int*)calloc((N+1),sizeof(int));
            *(arrC[i][j]) = (int*)calloc((N+1), sizeof(int));
            *(arrB[i][j]) = (int*)calloc((N+1), sizeof(int));
        }
    }
    *(ind) = (int*) malloc (N * N * N * sizeof(int));
    *(val) = (double *) malloc (N * N * N * sizeof(double));
}


/*Fills the transArray with the corresponding Truple values
 * and while doing so it adds the constraints per cell to the model*/
int fillArrayAndAddCellCon(Game* game, Truple** transArray, int* transCounter, int** ind, double** val,
                            int**** rowsCon, int**** colsCon, int**** blocksCon, GRBenv** env, GRBmodel** model){
    int i,j,k, indCounter = 0, conCounter = 0, error = 0;
    Point block, cell, p;
    char c;
    char* conName = "cell.";
    int id, N = game->m * game->n;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            block = getBlockIndex(j+1, i+1, game);
            cell = getCellIndex(j+1, i+1, game);
            /*if the cell is empty - add the relevant k values as variables*/
            if((*game).board.board[block.y][block.x].block[cell.y][cell.x].val == 0){
                for (k = 0; k < N; k++){
                    if (isValidValue(j,i,k,game) == 1){
                        (*transArray)[(*transCounter)].i = i;
                        (*transArray)[(*transCounter)].j = j;
                        (*transArray)[(*transCounter)].k = k;
                        (*ind)[indCounter] = (*transCounter);
                        (*val)[indCounter] = 1;
                        (*rowsCon)[i][k][++(*rowsCon)[i][k][0]] = (*transCounter);
                        (*colsCon)[j][k][++(*colsCon)[j][k][0]] = (*transCounter);
                        p = getBlockIndex(j,i, game);
                        id = pointToID(p.x, p.y,game);
                        (*blocksCon)[id][k][++(*blocksCon)[id][k][0]] = (*transCounter);
                        (*transCounter)++;
                        indCounter++;
                    }
                }
                if(indCounter>0) {
                    /*add constraint to model - note size indCounter + operator GRB_EQUAL*/
                    /*-- equation value (1.0) + unique constraint name*/
                    c = conCounter + '0';
                    strcat(conName,&c);
                    conCounter++;
                    error = GRBaddconstr((*model), indCounter, (*ind), (*val), GRB_EQUAL, 1.0, conName);
                    if(checkAndHandleError(error, env, "GRBaddconstr()") == -1){
                        return -1;
                    }
                    indCounter = 0;
                }

            }
        }
    }
    return 1;
}

/*Add constraints to the model from the given array - could be rows, cols or blocks*/
int addCons(int N, int** ind, double** val, char* conName, int**** arr, GRBenv** env, GRBmodel** model){
    int i, k ,l, conCounter = 0, error;
    char c;

    for(i = 0; i < N; i++){
        for(k = 0; k < N; k++){
            for(l = 0; l < (*arr)[i][k][0];l++){
                (*ind)[l] = (*arr)[i][k][l+1];
                (*val)[l] = 1;
            }
            if(l > 0) {
                /*   add constraint to model - note size indCounter + operator GRB_EQUAL*/
                /*-- equation value (1.0) + unique constraint name*/
                c = conCounter + '0';
                strcat(conName,&c);
                error = GRBaddconstr((*model), l, (*ind), (*val), GRB_EQUAL, 1.0, conName);
                if(checkAndHandleError(error, env, "GRBaddconstr()") == -1){
                    return -1;
                }
                conCounter++;
            }
        }
    }
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
    error = GRBsetintparam((*env), GRB_INT_PAR_LOGTOCONSOLE, 0);
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

void setVarType(char** vtype, int transCounter, char set){
    int i;
    for(i = 0; i < transCounter; i++){
        (*vtype)[i] = set;
    }
}

int optimizeModel(GRBenv** env, GRBmodel** model, int* optimstatus, int transCounter, double** sol){
    int error = 0, objval;
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
    error = GRBgetintattr((*model), GRB_INT_ATTR_STATUS, &optimstatus);
    if(checkAndHandleError(error, env, "GRBgetintattr()") == -1){
        return -1;
    }


    /*  get the objective -- the optimal result of the function*/
    error = GRBgetdblattr((*model), GRB_DBL_ATTR_OBJVAL, &objval);
    if(checkAndHandleError(error, env, "GRBgettdblattr()") == -1){
        return -1;
    }


    /*  get the solution - the assignment to each variable*/
    /*transCounter--; */
    /*number of variables, the size of "sol" should match*/

    error = GRBgetdblattrarray((*model), GRB_DBL_ATTR_X, 0, transCounter, sol);
    if(checkAndHandleError(error, env, "GRBgetdblattrarray()") == -1){
        return -1;
    }

}

/*Solves the current board using Linear Programming
 * Fills the probs array of the relvant cells in the board
 * guess = 0 fills all cells, guessHint = 1 fills only X Y cell
 * Return 0 if there is no solution*/
int solveILP (Game* game, int opCode, int x, int y){
    GRBenv   *env   = NULL;
    GRBmodel *model = NULL;
    int transCounter = 0, i, N = (*game).m*(*game).n, error = 0;
    int varNum = (*game).cellsToFill;
    int*** rowsCon;
    int*** colsCon;
    int*** blocksCon;
    double*    sol;
    int*       ind;
    double*    val;
    double*    obj;
    char*      vtype;
    int       optimstatus; /*status of calculation, was the optimization successfull?*/
    double    objval; /*result of target function for calculated values*/
    Truple*  transArray = (Truple*)malloc(varNum * N * sizeof(Truple));

    initArr(&rowsCon, &colsCon, &blocksCon, &ind, &val, N);

    /*   TODO - Move Create environment to setGlobals*/
    /*  Create environment - log file is mip1.log*/
    error = GRBloadenv(&env, "mip1.log");
    if(checkAndHandleError(error, &env, "GRBloadenv()") == -1){
        return -1;
    }

    if(turnOffPrintAndCreateModel(&env, &model) == -1){
        return -1;
    }

    /*Fill mapping array and add constraints per cell*/
    if(fillArrayAndAddCellCon(game, &transArray,&transCounter, &ind,&val,
                             &rowsCon, &colsCon, &blocksCon, &env, &model) == -1){
        return -1;
    }

    /*initialize the mallocs while using a more specified number*/
    initGrbArr(&sol, &ind, &val, &obj, &vtype, transCounter);

    /*  add variables to model*/
    error = GRBaddvars(model, transCounter, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype, NULL);
    if(checkAndHandleError(error, &env, "GRBaddvars()") == -1){
        return -1;
    }

    if(addCons(N, &ind, &val, "row.", &rowsCon, &env, &model) == -1){
        return -1;
    }

    if(addCons(N, &ind, &val, "con.", &colsCon, &env, &model) == -1){
        return -1;
    }

    if(addCons(N, &ind, &val, "block.", &blocksCon, &env, &model) == -1){
        return -1;
    }

    setVarType(&vtype, transCounter, GRB_BINARY);

    if(optimizeModel(&env, &model, &optimstatus, transCounter, &sol) == -1){
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
    GRBfreemodel(model);

    /* TODO - Move free env to exit*/
    GRBfreeenv(env);

    /*if we reached this we finished running Gurobi and everything went well*/
    return 1;
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

