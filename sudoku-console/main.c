#include <stdio.h>
#include "Game.h"
int main() {
    Game game;
    /*need to set values of m and n*/
    game.m=4;
    game.n=1;

    initBoard(&game);
    printBoard(&game);
    return 0;
}