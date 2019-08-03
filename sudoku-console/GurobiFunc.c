/*
*	Created by CMY on 30/07/2019.
 */

/*

#include "GurobiFunc.h"
int solveILP (Game* game, int opCode, int x, int y){
    GRBenv   *env   = NULL;
    GRBmodel *model = NULL;
    char* conName="cell.";
    char c;
    int conCounter=0,transCounter=0,indCounter=0,i,j,k, id,l;
    Point block, cell,p;
    int N=(*game).m*(*game).n;
    int varNum = (*game).cellsToFill;
    int       error = 0;
    int*** rowsCon = (int***)malloc(N* sizeof(int**));
    int*** colsCon = (int***)malloc(N* sizeof(int**));
    int*** blocksCon = (int***)malloc(N* sizeof(int**));
    double*    sol;
    int*       ind;
    double*    val;
    double*    obj;
    char*      vtype;
    int       optimstatus; /*status of calculation, was the optimization successfull?
    double    objval; result of target function for calculated values
    Truple*  transArray = (Truple*)malloc(varNum*N*sizeof(Truple));

    TODO - Move Create environment to setGlobals
     Create environment - log file is mip1.log
    error = GRBloadenv(&env, "mip1.log");
    if (error) {
        printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }

    turn off gurobi's prints
    error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
    if (error) {
        printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }

     Create an empty model named "mip1"
    error = GRBnewmodel(env, &model, "mip1", 0, NULL, NULL, NULL, NULL, NULL);
    if (error) {
        printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }

    for(i=0; i<N; i++){
        rowsCon[i] = (int**)malloc(N* sizeof(int*));
        colsCon[i] = (int**)malloc(N* sizeof(int*));
        blocksCon[i] = (int**)malloc(N* sizeof(int*));
        for(j=0;j<N;j++){
            rowsCon[i][j] = (int*)calloc((N+1),sizeof(int));
            colsCon[i][j] = (int*)calloc((N+1), sizeof(int));
            blocksCon[i][j] = (int*)calloc((N+1), sizeof(int));
        }
    }

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            block = getBlockIndex(x, y, game);
            cell = getCellIndex(x, y, game);
            if the cell is empty - add the relevant k values as variables
            if((*game).board.board[block.y][block.x].block[cell.y][cell.x].val==0){
                for (k=0;k<N;k++){
                    if (isValidValue(j,i,k,game)==1){
                        transArray[transCounter].i=i;
                        transArray[transCounter].j=j;
                        transArray[transCounter].k=k;
                        ind[indCounter]=transCounter;
                        val[indCounter]=1;
                        rowsCon[i][k][++rowsCon[i][k][0]] = transCounter;
                        colsCon[j][k][++colsCon[j][k][0]] = transCounter;
                        p = getBlockIndex(j,i, game);
                        id = pointToID(p.x, p.y,game);
                        blocksCon[id][k][++blocksCon[id][k][0]] = transCounter;
                        transCounter++;
                        indCounter++;
                    }
                }
                if(indCounter>0) {
                     add constraint to model - note size indCounter + operator GRB_EQUAL
                     -- equation value (1.0) + unique constraint name
                    c =conCounter+'0';
                    strcat(conName,&c);
                    conCounter++;
                    error = GRBaddconstr(model, indCounter, ind, val, GRB_EQUAL, 1.0, conName);
                    if (error) {
                        printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
                        return -1;
                    }
                    indCounter=0;
                }

            }
        }
    }

    sol = (double*)malloc(transCounter*sizeof(double)); calculated values for variables
    ind = (int*)malloc(transCounter*sizeof(int)); variable array
    val = (double*)malloc(transCounter*sizeof(double)); coefficients of constraint
    obj = (double*)calloc(transCounter,sizeof(double)); coefficients of target function
    vtype = (char*)malloc(transCounter*sizeof(char)); type of variable
    conName="row.";
    conCounter=0;
    for(i=0; i < N; i++){
        for(k=0; k < N; k++){
            for(l = 0; l<rowsCon[i][k][0];l++){
                ind[l] = rowsCon[i][k][l+1];
                val[l] = 1;
            }
            if(l > 0) {
                 add constraint to model - note size indCounter + operator GRB_EQUAL
                 -- equation value (1.0) + unique constraint name
                c =conCounter+'0';
                strcat(conName,&c);
                error = GRBaddconstr(model, l, ind, val, GRB_EQUAL, 1.0, conName);
                if (error) {
                    printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
                    return -1;
                }
                conCounter++;
            }
        }
    }

    conName = "col.";
    conCounter = 0;
    for(j=0; j < N; j++){
        for(k=0; k < N; k++){
            for(l = 0; l<rowsCon[j][k][0];l++){
                ind[l] = rowsCon[j][k][l+1];
                val[l] = 1;
            }
            if(l > 0) {
                 add constraint to model - note size indCounter + operator GRB_EQUAL
                 -- equation value (1.0) + unique constraint name
                c =conCounter+'0';
                strcat(conName,&c);
                error = GRBaddconstr(model, l, ind, val, GRB_EQUAL, 1.0, conName);
                if (error) {
                    printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
                    return -1;
                }
                conCounter++;
            }
        }
    }

    conName="block.";
    conCounter=0;
    for(id=0; id < N; id++){
        for(k=0; k < N; k++){
            for(l = 0; l<rowsCon[id][k][0];l++){
                ind[l] = rowsCon[id][k][l+1];
                val[l] = 1;
            }
            if(l > 0) {
                 add constraint to model - note size indCounter + operator GRB_EQUAL
                 -- equation value (1.0) + unique constraint name
                c =conCounter+'0';
                strcat(conName,&c);
                error = GRBaddconstr(model, l, ind, val, GRB_EQUAL, 1.0, conName);
                if (error) {
                    printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
                    return -1;
                }
                conCounter++;
            }
        }
    }


     Add variables

     coefficients - for x (cells 0,1,2 in "obj")
    All cells of obj were set to 0 with calloc

     variable types - for x,y,z (cells 0,1,2 in "vtype")
     other options: GRB_INTEGER, GRB_CONTINUOUS
    for(i=0; i<transCounter; i++){
        vtype[i] = GRB_BINARY;
    }

     add variables to model
    error = GRBaddvars(model, transCounter, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype, NULL);
    if (error) {
        printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }

     Change objective sense to maximization
    error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
    if (error) {
        printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }

     update the model - to integrate new variables
    error = GRBupdatemodel(model);
    if (error) {
        printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }


     Optimize model - need to call this before calculation
    error = GRBoptimize(model);
    if (error) {
        printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }

     Write model to 'mip1.lp' - this is not necessary but very helpful
    error = GRBwrite(model, "mip1.lp");
    if (error) {
        printf("ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }

     Get solution information
    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) {
        printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }

     get the objective -- the optimal result of the function
    error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
    if (error) {
        printf("ERROR %d GRBgettdblattr(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }

     get the solution - the assignment to each variable
     transCounter-- number of variables, the size of "sol" should match
    error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, transCounter, sol);
    if (error) {
        printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }

     print results
    printf("\nOptimization complete\n");


    if (opCode!=2){
        solAssign(sol,transArray, transCounter, opCode, x, y, game);
    }

     solution found
    if (optimstatus == GRB_OPTIMAL) {
        printf("Optimal objective: %.4e\n", objval);
        printf("  x=%.2f, y=%.2f, z=%.2f\n", sol[0], sol[1], sol[2]);
    }
         no solution found
    else if (optimstatus == GRB_INF_OR_UNBD) {
        printf("Model is infeasible or unbounded\n");
    }
         error or calculation stopped
    else {
        printf("Optimization was stopped early\n");
    }

     IMPORTANT !!! - Free model and environment
    GRBfreemodel(model);
    TODO - Move free env to exit
    GRBfreeenv(env);
     solution found
    if (optimstatus == GRB_OPTIMAL) {
        return 1;
    }
     no solution found
    else (optimstatus == GRB_INF_OR_UNBD) {
        return 0;
    }
}


 Fills the relevant cell's ILPVals
 * generate = 0 fills all cells, hint = 1 fills only X Y cell, validate = 2
 * Returns 0 if there is no solution
void solAssign (double* solArray,Truple* transArray, int transCounter, int opCode, int X, int Y, Game* game){
    int l;
    Point block = getBlockIndex(X,Y,game);
    Point cell = getCellIndex(X,Y,game);
    if (opCode==1){ hint, should update ILPVal for X Y only
        for (l=0;l<transCounter;l++){
            if((transArray[l].i==Y)&&(transArray[l].j==X)&&(solArray[l]==1)){
                game->board.board[block.y][block.x].block[cell.y][cell.x].ILPVal=transArray[l].k;
                return ;
            }
        }
    }
    else{
        for (l=0;l<transCounter;l++){
            if(solArray[l]==1){
                block = getBlockIndex(transArray[l].j,transArray[l].i,game);
                cell = getCellIndex(transArray[l].j,transArray[l].i,game);
                game->board.board[block.y][block.x].block[cell.y][cell.x].ILPVal=transArray[l].k;
                return ;
            }
        }
    }
}*/

