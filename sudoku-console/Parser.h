/*
*	Created by CMY on 29/07/2019.
 */

#ifndef SUDOKU_CONSOLE_PARSER_H
#define SUDOKU_CONSOLE_PARSER_H
#include "GeneralFunc.h"
typedef enum {cmdSolve,cmdEdit,cmdMarkErr,cmdPrint,cmdSet,cmdValidate,cmdGuess,
			  cmdGenerate,cmdUndo,cmdRedo,cmdSave,cmdHint,cmdGuessHint,cmdSolNum,
			  cmdAutofill,cmdReset,cmdExit,TypeAmount} cmdType;


/*Receives command from using and acting accordingly:
 * solve,exit,edit will be sent to functions defined under Parser module
 * other functions will be sent to functions defined under GeneralFunc module*/
int getCommand (char* input,Game* game);

#endif /*SUDOKU_CONSOLE_PARSER_H*/
