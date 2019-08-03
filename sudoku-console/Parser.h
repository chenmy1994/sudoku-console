/*
*	Created by CMY on 29/07/2019.
 */

#ifndef SUDOKU_CONSOLE_PARSER_H
#define SUDOKU_CONSOLE_PARSER_H
typedef enum {cmdSolve,cmdEdit,cmdMarkErr,cmdPrint,cmdSet,cmdValidate,cmdGuess,
			  cmdGenerate,cmdUndo,cmdRedo,cmdSave,cmdHint,cmdGuessHint,cmdSolNum,
			  cmdAutofill,cmdReset,cmdExit,TypeAmount} cmdType;

#include "GeneralFunc.h"

/*Receives the user input (command and arguments) and return:
* for valid input and arguments - return 1
* for invalid number of arguments or value not in range, prints relevant error - return 0 */
int checkValidInput(int cmdIndex,char* x, char* y, char* z,Game* game);

/*Converts an enum item to his char* value (for string comparison)*/
char* enumToStr (cmdType cmd,Game* game);

/*Converts input from user (command) to the matching item index for enum cmdType*/
int strToEnumIndex (char* cmdStr,Game* game);

/*Receives command from using and acting accordingly:
 * solve,exit,edit will be sent to functions defined under Parser module
 * other functions will be sent to functions defined under GeneralFunc module*/
/*TODO - setting enums for different commands*/
int getCommand (char* input,Game* game);
#endif /*SUDOKU_CONSOLE_PARSER_H*/
