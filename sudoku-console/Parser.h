//
// Created by CMY on 29/07/2019.
//

#ifndef SUDOKU_CONSOLE_PARSER_H
#define SUDOKU_CONSOLE_PARSER_H

#include "GeneralFunc.h"
/*Receives command from using and acting accordingly:
 * solve,exit,edit will be sent to functions defined under Parser module
 * other functions will be sent to functions defined under GeneralFunc module*/
/*TODO - setting enums for different commands*/
void getCommand();



#endif //SUDOKU_CONSOLE_PARSER_H
