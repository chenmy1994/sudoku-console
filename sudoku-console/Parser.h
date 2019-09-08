/**
 * Parser Summary:
 * This module is responsible for getting the command from the user and interpreting them
 *
 * Public Functions:
 * getCommand - Fills all cell values with probability greater than users input
 *
 * Private Functions:
 * isNumeric - Return 1 if the string input is a number, 0 otherwise
 * isDouble - Return 1 if the string input is a valid double, 0 otherwise
 * enumToStr - Converts an enum item to his char* value (for string comparison)
 * printInvalidRange - Prints a relevant error message when value is not in range
 * checkArgValue - Checks the given argument is a number in the correct range (ind-N)
 * modeToName - Returns the name of each mode
 * checkValidModeForCmd - Step 1 - Check if the current mode is valid for the user's command
 * strToEnumIndex - Converts input from user (command) to the matching item index for enum cmdType
 *                  if command doesnt exist - return (-1)
 * checkValidInput - Receives the user input (command and arguments) and return relevant message
 *
 */

#ifndef SUDOKU_CONSOLE_PARSER_H
#define SUDOKU_CONSOLE_PARSER_H
#include "GeneralFunc.h"

#define CMDTOOLONG "Error: Too many characters were entered in a single line.\nPlease enter 256 characters at most.\n"
#define errorCommand "Error: Invalid command.\n"
#define errorCurrMode "Error: The command %s is unavailable in %s mode.\n"
#define cmdIsValidInModes "The available modes for %s command are %s%s.\n"
#define errorArgValue "%s command input should be %s."
#define errorArgRange "Error: There is a problem with your %s argument.\n"
#define NUMOFARG "Error: The command %s requires %d arguments only.\n"

typedef enum {cmdSolve,cmdEdit,cmdMarkErr,cmdPrint,cmdSet,cmdValidate,cmdGuess,
			  cmdGenerate,cmdUndo,cmdRedo,cmdSave,cmdHint,cmdGuessHint,cmdSolNum,
			  cmdAutofill,cmdReset,cmdExit,TypeAmount} cmdType;


/*Receives command from using and acting accordingly:
 * solve,exit,edit will be sent to functions defined under Parser module
 * other functions will be sent to functions defined under GeneralFunc module*/
int getCommand (char* input,Game* game);

#endif /*SUDOKU_CONSOLE_PARSER_H*/
