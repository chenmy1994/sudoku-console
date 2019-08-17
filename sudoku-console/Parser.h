/*
*	Created by CMY on 29/07/2019.
 */

#ifndef SUDOKU_CONSOLE_PARSER_H
#define SUDOKU_CONSOLE_PARSER_H
#include "GeneralFunc.h"
typedef enum {cmdSolve,cmdEdit,cmdMarkErr,cmdPrint,cmdSet,cmdValidate,cmdGuess,
			  cmdGenerate,cmdUndo,cmdRedo,cmdSave,cmdHint,cmdGuessHint,cmdSolNum,
			  cmdAutofill,cmdReset,cmdExit,TypeAmount} cmdType;

/*Return 1 if the string input is a number, 0 otherwise*/
int isNumeric(const char *str);

/*Return 1 if the string input is a valid double, 0 otherwise*/
int isDouble(const char *s);

/*Receives the user input (command and arguments) and return:
* for valid input and arguments - return 1
* for invalid number of arguments or value not in range, prints relevant error - return 0 */
int checkValidInput(int cmdIndex,char* x, char* y, char* z,Game* game);

/*Converts an enum item to his char* value (for string comparison)*/
char* enumToStr (cmdType cmd);

/*Converts input from user (command) to the matching item index for enum cmdType*/
int strToEnumIndex (char* cmdStr);

/*Receives command from using and acting accordingly:
 * solve,exit,edit will be sent to functions defined under Parser module
 * other functions will be sent to functions defined under GeneralFunc module*/
int getCommand (char* input,Game* game);

#endif /*SUDOKU_CONSOLE_PARSER_H*/
