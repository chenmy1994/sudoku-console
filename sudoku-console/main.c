#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Parser.h"
#include "SPBufferset.h"
#define GETCOMMANDMSG "Enter a command:\n"
#define EOFMSG "Error: Reached EOF. \n"

/*Initializing game and printing the user the available commands*/
void initGame(Game* game){
    int seed;
    /*Opening message*/
    printf("Welcome to our magnificent Sudoku-Console game!\n");
    printf("Please enter one of the following commands:  \n");
    printf("solve X\n");
    printf("edit X \n");
    printf("Where 'X' includes a full or relative path to the board file  \n");
    printf("For Edit command ONLY - In case 'X' is NOT provided a 9X9 board is initialized \n");

    /*Setting indicator to release memory to 0, since there is no memory to release yet*/
    game->memRelease = 0;

    /*Setting the game mode to init*/
    game->mode = 0;

    /*Setting seed to srand, since we use randomization later*/
    seed = 17;
    srand(seed);
}

/*The game is waiting for user's commands and respond to them*/
void playGame(Game* game){
    int cmdVal = 0;
    char cmd[1025];

    /*While no exit command*/
    while(cmdVal != 2) {
        printf(GETCOMMANDMSG);
        cmdVal = getCommand(cmd, game);
        /*if EOF*/
        if(cmdVal == -1){
            printf(EOFMSG);
            exitGame(game);
            return;
        }
        /*if invalid command (0), exit(2) or command that didn't change the board(10)*/
        if(cmdVal == 0 || cmdVal == 2 || cmdVal == 10){
            continue;
        }
        /*if we are in init mode*/
        if(game->mode == 0){
            continue;
        }
        printBoard(game);

        /*If we are in solve mode and the board is completed*/
        if(game->mode == 2 && game->cellsToFill == 0){
            if(game->numOfErrors == 0){
                printf(PUZZLESOLVED);
                /*Change mode to init*/
                changeMode(0 ,game);
                continue;
            }
            printf(ERRORSOL);
        }
    }
}

int main() {
    Game game;
	SP_BUFF_SET()

    initGame(&game);
    playGame(&game);

    return 0;
}
