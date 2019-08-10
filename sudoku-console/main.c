#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Parser.h"
#include "SPBufferset.h"
#define GETCOMMANDMSG "Enter a command..\n"

int main() {
    Game game;
    int cmdVal = 0;
    char cmd[1025];
	SP_BUFF_SET()
    /*Opening message*/
    printf("Welcome to our magnificent Sudoku-Console game!\n");
    printf("Please enter one of the following commands:  \n");
    printf("Solve X\n");
    printf("Edit X \n");
    printf("Where 'X' includes a full or relative path to the board file  \n");
    printf("For Edit command ONLY - In case 'X' is NOT provided a 9X9 board is initialized \n");

    /*TODO - receive input file from user and update board dimensions*/
    game.memRelease = 0;

    while(cmdVal != 2) {
        printf(GETCOMMANDMSG);
        cmdVal = getCommand(cmd, &game);
        if(game.mode == 0 || cmdVal == 0){
            continue;
        }
        printBoard(&game);
    }
    /*TODO - parsing user commands and acting accordingly*/
    return 0;
}
