/*
*	Created by CMY on 29/07/2019.
 */

#include "Parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/*Return 1 if the string input is a number, 0 otherwise*/
int isNumeric(const char *str){
    while(*str != '\0'){
        if(*str < '0' || *str > '9')
            return 0;
        str++;
    }
    return 1;
}

/*Return 1 if the string input is a valid double, 0 otherwise*/
int isDouble(const char *s) {
  char *end;
  strtod(s, &end);
  return s != end;
}

/*Converts an enum item to his char* value (for string comparison)*/
char* enumToStr(cmdType cmd){
	switch(cmd){
	case cmdSolve:
		return "solve";
	case cmdEdit:
		return "edit";
	case cmdMarkErr:
		return "mark_errors";
	case cmdPrint:
		return "print_board";
	case cmdSet:
		return "set";
	case cmdValidate:
		return "validate";
	case cmdGuess:
		return "guess";
	case cmdGenerate:
		return "generate";
	case cmdUndo:
		return "undo";
	case cmdRedo:
		return "redo";
	case cmdSave:
		return "save";
	case cmdHint:
		return "hint";
	case cmdGuessHint:
		return "guess_hint";
	case cmdSolNum:
		return "num_solutions";
	case cmdAutofill:
		return "autofill";
	case cmdReset:
		return "reset";
	case cmdExit:
		return "exit";
	default:
		return "";
	}
}

/*Prints a relevant error message when value is not in range*/
void printInvalidRange(int N, char* arg, int ind){
    printf(errorArgRange,arg);
    printf(errorArgValue,arg,"a valid number between ");
    printf("%d and %d.\n",ind, N);
}

/*Checks the given argument is a number in the correct range (ind-N)*/
int checkArgValue (char* argVal, int N, int ind, char* argName){
    if(isNumeric(argVal) == 0 || atoi(argVal) > N || atoi(argVal) < ind){
        printInvalidRange(N, argName, ind);
        return 0;
    }
    return 1;
}

/*Returns the name of each mode*/
char* modeToName(int mode){
    if (mode == 0){
        return "<Init>";
    }
    if(mode == 1){
        return "<Edit>";
    }
    if(mode == 2){
        return "<Solve>";
    }
    return "";
}

/*Step 1 - Check if the current mode is valid for the user's command*/
int checkValidModeForCmd(Game* game, int opt1, int opt2, int cmdIndex){
    if(game->mode == opt1 || game ->mode == opt2){
        return 1;
    }
    printf(errorCurrMode, enumToStr(cmdIndex), modeToName(game->mode));
    printf(cmdIsValidInModes, enumToStr(cmdIndex), modeToName(opt1), modeToName(opt2));
    return 0;
}

/*Converts input from user (command) to the matching item index for enum cmdType
 * if command doesnt exist - return (-1)*/
int strToEnumIndex(char* cmdStr){
	int i;
	char* enumTrans;
	for (i=0;i<TypeAmount;i++){
		enumTrans = enumToStr(i);
		if(strcmp(cmdStr,enumTrans)==0){
			return i;
		}
	}
	return -1;
}

/*Receives the user input (command and arguments) and return:
 * for valid input and arguments - return 1
 * for invalid number of arguments, value not in range or not matching game mode - prints relevant error and return 0
 * for invalid command - returns -1 and prints message to the user*/
int checkValidInput(int cmdIndex,char* x, char* y, char* z, char* extra,Game* game){
	int N = (*game).n*(*game).m;

	switch(cmdIndex){
        case cmdSolve:
            /* Step 1 - Solve command is valid in all modes*/
            /* Step 2 - If there is no first argument (x == NULL)
             * or there is an extra argument (y != NULL)
             * then the command doesn't have the correct number of parameters*/
            if (x == NULL || y != NULL){
                printf(NUMOFARG, "solve",1);
                return 0;
            }
            /*Step 1 and 2 went well. Steps 3-5 are checked in solve function*/
            return 1;
        case cmdEdit:
            /* Step 1 - Edit command is valid in all modes*/
            /* Step 2 - If there is an extra argument (y != NULL)
             * then the command doesn't have the correct number of parameters*/
            if (y != NULL){
                printf(NUMOFARG, "edit",1);
                return 0;
            }
            /*Step 1 and 2 went well. Steps 3-5 are checked in edit function*/
            return 1;
        case cmdSave:
            /*Step 1 - Save command is valid in edit (1) and solve (2) modes*/
            if(checkValidModeForCmd(game, 1, 2, cmdIndex) == 0){
                return 0;
            }
            /* Step 2 - If there is no first argument(x == NULL)
             * or there is an extra argument (y != NULL)
             * then the command doesn't have the correct number of parameters*/
            if (y != NULL || x == NULL){
                printf(NUMOFARG, "save",1);
                return 0;
            }
            /*Step 1 and 2 went well. Steps 3-5 are checked in edit function*/
            return 1;
        case cmdMarkErr:
            /*Step 1 - mark_errors command is valid in Solve mode(2) only*/
            if(checkValidModeForCmd(game, 2, -1, cmdIndex) == 0){
                return 0;
            }
            /* Step 2 - If there is no first argument (x == NULL)
             * or there is an extra argument (y != NULL)
             * then the command doesn't have the correct number of parameters*/
            if (x == NULL || y != NULL){
                printf(NUMOFARG, "mark_errors",1);
                return 0;
            }
            /*Step 3 - Check if the given parameters are correct*/
            if((isNumeric(x))&&((atoi(x)!=1) && (atoi(x)!=0))){
                printf(errorArgValue,"Error: ","1 or 0\n");
                return 0;
            }
            /* Step 1, 2 and 3 went well.
             * Steps 4-5 are checked in the function*/
            return 1;
        case cmdSet:
            /*Step 1 - set command is valid in Edit mode(1) and Solve mode(2) only*/
            if(checkValidModeForCmd(game, 1, 2, cmdIndex) == 0){
                return 0;
            }
            /*Step 2 - set command requires 3 arguments only*/
            if (extra != NULL || x == NULL || y == NULL || z == NULL){
                printf(NUMOFARG, "set",3);
                return 0;
            }
            /*Step 3 - the parameters are correct*/
            if(checkArgValue(x, N, 1, "first") == 0){
                return 0;
            }
            if(checkArgValue(y, N, 1, "second") == 0){
                return 0;
            }
            if(checkArgValue(z, N, 1, "third") == 0){
                return 0;
            }
            /* Step 1, 2 and 3 went well.
             * Steps 4-5 are checked in the function*/
            return 1;
        case cmdGuess:
            /*Step 1 - guess command is valid in Solve mode(2) only*/
            if(checkValidModeForCmd(game, 2, -1, cmdIndex) == 0){
                return 0;
            }
            /* Step 2 - If there is no path gives (x == NULL)
             * or there is an extra argument (y != NULL)
             * then the command doesn't have the correct number of parameters*/
            if (y != NULL || x == NULL){
                printf(NUMOFARG, "guess",1);
                return 0;
            }
            /*Step 3 - Check that the given argument is correct*/
            if((isDouble(x)) == 0 || (atof(x)>1) || (atof(x)<0) ){
                printf(errorArgValue,"Error: ","a valid floating point value between 0 and 1.\n");
                return 0;
            }
            /* Step 1, 2 and 3 went well.
            * Steps 4-5 are checked in the function*/
            return 1;
        case cmdGenerate:
            /*Step 1 - generate command is valid in Edit mode(1) only*/
            if(checkValidModeForCmd(game, 1, -1, cmdIndex) == 0){
                return 0;
            }
            /* Step 2 - If there is no first parameter (x == NULL)
             * or there is no second parameter (y == NULL)
             * or there is an extra argument (z != NULL)
             * then the command doesn't have the correct number of parameters*/
            if (z != NULL || x == NULL || y == NULL){
                printf(NUMOFARG, "generate",2);
                return 0;
            }
            /*Step 3 - Check the parameters are correct*/
            if(checkArgValue(x, N*N, 0,"first") == 0){
                return 0;
            }
            if(checkArgValue(y, N*N, 0,"second") == 0){
                return 0;
            }
            /* Step 1, 2 and 3 went well.
            * Steps 4-5 are checked in the function*/
            return 1;
        case cmdAutofill:
            /*Step 1 - autofill command is valid in Solve mode(2) only*/
            if(checkValidModeForCmd(game, 2, -1, cmdIndex) == 0){
                return 0;
            }
            /*Step 2 - autofill shouldn't have any arguments*/
            if (x != NULL){
                printf(NUMOFARG, "autofill",0);
                return 0;
            }
            /* Step 1, 2 and 3 went well.
           * Steps 4-5 are checked in the function*/
            return 1;
        case cmdHint: 	case cmdGuessHint:
            /*Step 1 - hint and guess_hint commands are valid in Solve mode(2) only*/
            if(checkValidModeForCmd(game, 2, -1, cmdIndex) == 0){
                return 0;
            }
            /* Step 2 - If there is no first parameter (x == NULL)
            * or there is no second parameter (y == NULL)
            * or there is an extra argument (z != NULL)
            * then the command doesn't have the correct number of parameters*/
            if (z != NULL || x == NULL || y == NULL){
                printf(NUMOFARG, enumToStr(cmdIndex),2);
                return 0;
            }
            /*Step 3 - check the parameters are correct*/
            if(checkArgValue(x, N, 1,"first") == 0){
                return 0;
            }
            if(checkArgValue(y, N, 1,"second") == 0){
                return 0;
            }
            /* Step 1, 2 and 3 went well.
            * Steps 4-5 are checked in the function*/
            return 1;

        case cmdPrint: 	case cmdValidate: 	case cmdReset: 	case cmdSolNum:	case cmdUndo:	case cmdRedo:
            /*Step 1 - these commands are valid in Edit mode(1) and Solve mode(2) only*/
            if(checkValidModeForCmd(game, 1, 2, cmdIndex) == 0){
                return 0;
            }
            /*Step 2 - These commands shouldn't have arguments*/
            if (x != NULL){
                printf(NUMOFARG, enumToStr(cmdIndex),0);
                return 0;
            }
            /* Step 1, 2 and 3 went well.
           * Steps 4-5 are checked in the function*/
            return 1;
        case cmdExit:
            /*Step 1 - Exit command is available in all modes*/
            /*Step 2 - Exit command shouldn't have arguments*/
            if (x != NULL){
                printf(NUMOFARG, enumToStr(cmdIndex),0);
                return 0;
            }
            /* Step 1, 2 and 3 went well.
             * Steps 4-5 are checked in the function*/
            return 1;
        /*If we got here, the command isn't valid*/
        default:
            printf(errorCommand);
            return -1;
	}
}

/* Get command from user, call the matching function for a valid command.
 * Returns 0 if it was invalid command/or command failed, 2 if it was exit command,
 * 10 if the command didn't change the board and 1 if command succeeded and changed the board*/
int getCommand (char* stream,Game* game){
	char* cmdType = NULL;
	int cmdIndex;
    char* x;
    char* y;
    char* z;
    char* extra;
    /*While we didn't get any command and we keep getting input from the user*/
    while(cmdType == NULL){
        /*EOF Reached*/
        if(fgets(stream,1024,stdin) == NULL){
            return -1;
        }
        if(strlen(stream) > 256){
            printf(CMDTOOLONG);
            return 0;
        }
        if ((*stream)!='\n') {
            cmdType = strtok(stream, " \t\r\n"); /*cmdType*/
        }
    }
	/*Received input from user successfully*/
			x=strtok(NULL," \t\r\n"); /*first argument*/
			y=strtok(NULL," \t\r\n"); /*second argument*/
			z=strtok(NULL," \t\r\n"); /*third argument*/
			extra=strtok(NULL," \t\r\n"); /*extra argument*/

			cmdIndex = strToEnumIndex(cmdType);
			/*Check if the input is valid*/
            if(checkValidInput(cmdIndex,x,y,z,extra,game) != 1){
                return 0;
            }
            /* If we got here - the input is valid
             * so we need to execute the command*/
            switch(cmdIndex){
                case cmdSolve:
                    return solve(x, game);
                case cmdEdit:
                    return edit(x, game);
                case cmdMarkErr:
                    updateMarkErrors(game, atoi(x));
                    return 10;
                case cmdPrint:
                    printBoard(game);
                    return 10;
                case cmdSet:
                    return set(atoi(x), atoi(y), atoi(z), game,0);
                case cmdValidate:
                    validate(game, 1);
                    return 10;
                case cmdGuess:
                    return guess(atof(x), game);
                case cmdGenerate:
                    return generate(atoi(x), atoi(y), game);
                case cmdUndo:
                    return undo(game,0);
                case cmdRedo:
                    return redo(game);
                case cmdSave:
                    save(x, game);
                    return 10;
                case cmdHint:
                    hint(atoi(x), atoi(y),game);
                    return 10;
                case cmdGuessHint:
                    guessHint(atoi(x), atoi(y), game);
                    return 10;
                case cmdSolNum:
                    num_solutions(game);
                    return 10;
                case cmdAutofill:
                    return autofill(game);
                case cmdReset:
                    return reset(game);
                case cmdExit:
                    exitGame(game);
                    return 2;
                /*We shouldn't get there anyway since we handled this in checkValidInput*/
                default:
                    printf(errorCommand);
                    return 0;
                }
		}

