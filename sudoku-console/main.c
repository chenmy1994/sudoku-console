#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Parser.h"
#include "SPBufferset.h"
int main() {
	SP_BUFF_SET()
    /*Opening message*/
    printf("Welcome to our magnificent Sudoku-Console game!\n");
    printf("Please enter one of the following commands:  \n");
    printf("Solve X\n");
    printf("Edit X \n");
    printf("Where 'X' includes a full or relative path to the board file  \n");
    printf("For Edit command ONLY - In case 'X' is NOT provided a 9X9 board is initialized \n");

    /*TODO - receive input file from user and update board dimensions*/
    Game game;
    char cmd[1025];
    game.m=4;
    game.n=3;
    game.memRelease = 0;
    edit("./test.txt" ,&game);
   /* initBoard(&game);*/
    printBoard(&game);
    getCommand(cmd,&game);

    /*TODO - parsing user commands and acting accordingly*/
    return 0;
}
