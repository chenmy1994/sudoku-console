/*
*	Created by CMY on 29/07/2019.
 */

#include "Parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define errorCommand "Error: invalid command\n"
#define errorSpecMode "Error: This command is only available in %s mode\n"
#define errorEditSolve "You can only use 'Edit X' or 'Solve X' command\n"
#define errorArgNum "Error: You're missing arguments, number of arguments require for the command is %d\n"
#define errorArgValue "%sCommand input should be %s"
#define errorArgRange "Error: There is a problem with your %s input.\n"
#define NUMOFARG "Error: The command %s requires %d arguments only.\n"

/* Get command from user, call the matching function for a valid command.*/
int getCommand (char* stream,Game* game){
	char* cmdType;
	int cmdIndex;
    char* x;
    char* y;
    char* z;
    char* extra;
	/*Received input from user successfully*/
	 if(fgets(stream,1024,stdin)!=NULL){
		/*user input isn't empty*/
		if (*stream!='\n'){
			cmdType= strtok(stream," \t\r\n"); /*cmdType*/
			x=strtok(NULL," \t\r\n"); /*first argument*/
			y=strtok(NULL," \t\r\n"); /*second argument*/
			z=strtok(NULL," \t\r\n"); /*third argument*/
			extra=strtok(NULL," \t\r\n"); /*third argument*/

			cmdIndex=strToEnumIndex(cmdType);

			switch(cmdIndex){
			case cmdSolve:
				if(checkValidInput(cmdIndex,x,y,z,extra,game)==0){
                    return 0;
				}
				return solve(x, game);
			case cmdEdit:
				if(checkValidInput(cmdIndex,x,y,z,extra,game)==0){
				    return 0;
				}
				return edit(x, game);
			case cmdMarkErr:
				if(checkValidInput(cmdIndex,x,y,z,extra,game)==0) {
                    return 0;
                }
				updateMarkErrors(game, atoi(x));
    			return 10;
			case cmdPrint:
				if(checkValidInput(cmdIndex,x,y,z,extra,game)==0){
                    return 0;
				}
				printBoard(game);
				return 10;
			case cmdSet:
				if(checkValidInput(cmdIndex,x,y,z,extra,game)==0){
                    return 0;
				}
				return set(atoi(x), atoi(y), atoi(z), game,0);
			case cmdValidate:
				if(checkValidInput(cmdIndex,x,y,z,extra,game)==0){
                    return 0;
				}
				validate(game);
				return 10;
			case cmdGuess:
				if(checkValidInput(cmdIndex,x,y,z,extra,game)==0){
                    return 0;
				}
				guess(atof(x), game);
				return 1;
			case cmdGenerate:
				if(checkValidInput(cmdIndex,x,y,z,extra,game)==0){
                    return 0;
				}
				generate(atoi(x), atoi(y), game);
				return 1;
			case cmdUndo:
				if(checkValidInput(cmdIndex,x,y,z,extra,game)==0){
                    return 0;
				}
				undo(game,0);
				return 1;
			case cmdRedo:
				if(checkValidInput(cmdIndex,x,y,z,extra,game)==0){
                    return 0;
				}
				redo(game);
				return 1;
			case cmdSave:
				if(checkValidInput(cmdIndex,x,y,z,extra,game)==0){
                    return 0;
				}
				save(x, game);
				return 10;
			case cmdHint:
				if(checkValidInput(cmdIndex,x,y,z,extra,game)==0){
				    return 0;
				}
                hint(atoi(x), atoi(y),game);
                return 10;
			case cmdGuessHint:
				if(checkValidInput(cmdIndex,x,y,z,extra,game)==0){
                    return 0;
				}
				guessHint(atoi(x), atoi(y), game);
				return 10;
			case cmdSolNum:
				if(checkValidInput(cmdIndex,x,y,z,extra,game)==0){
                    return 0;
				}
				num_solutions(game);
				return 10;
			case cmdAutofill:
				if(checkValidInput(cmdIndex,x,y,z,extra,game)==0){
                    return 0;
				}
				autofill(game);
				return 1;
			case cmdReset:
				if(checkValidInput(cmdIndex,x,y,z,extra,game)==0){
                    return 0;
				}
				reset(game);
				return 1;
			case cmdExit:
			    if(checkValidInput(cmdIndex,x,y,z,extra,game)==0){
                    return 0;
                }
				exitGame(game);
				return 2;
			default:
				printf(errorCommand);
			}
		}
	}
	return 0;
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
 * for invalid number of arguments, value not in range or not matching game mode - prints relevant error and return 0 */
int checkValidInput(int cmdIndex,char* x, char* y, char* z, char* extra,Game* game){
	int N=(*game).n*(*game).m;
	switch(cmdIndex){
	case cmdSolve:
        if (x == NULL || y != NULL){
            printf(NUMOFARG, "solve",1);
            return 0;
        }
		break;
	case cmdEdit:
        if (y != NULL){
            printf(NUMOFARG, "edit",1);
            return 0;
        }
		break;
	case cmdSave:
        if (y != NULL || x == NULL){
            printf(NUMOFARG, "save",1);
            return 0;
        }
        if((*game).mode==0) {
            printf(errorEditSolve);
            return 0;
        }

        return 1;

	case cmdMarkErr:
        if (x == NULL || y != NULL){
            printf(NUMOFARG, "mark_errors",1);
            return 0;
        }
		if((*game).mode!=0){
			if((*game).mode!=1){
                if((isNumeric(x))&&((atoi(x)==1) || (atoi(x)==0))){
                    return 1;
                }
                printf(errorArgValue,"Error: ","1 or 0\n");
			}
			else{	printf(errorSpecMode,"Solve");	}
		}
		else{	printf(errorEditSolve);	}
		return 0;
	case cmdSet:
        if (extra != NULL || x == NULL || y == NULL || z == NULL){
            printf(NUMOFARG, "set",3);
            return 0;
        }
		if((*game).mode!=0){
            if((isNumeric(x)) && (atoi(x)<=N) && (atoi(x)>0)){
                if((isNumeric(y)) && (atoi(y)<=N) && (atoi(y)>0)){
                    if ((isNumeric(x)) && (atoi(z)<=N) && (atoi(x)>=0)){
                        return 1;
                    }
                    printf(errorArgRange,"third");
                }
                else{	printf(errorArgRange,"second");	}
            }
            else{	printf(errorArgRange,"first");	}
            printf(errorArgValue,"","a valid number between 0 and ");
            printf("%d.\n",N);
        }
		else{	printf(errorEditSolve);	}
		return 0;
	case cmdGuess:
        if (y != NULL || x == NULL){
            printf(NUMOFARG, "guess",1);
            return 0;
        }
		if((*game).mode!=0){
			if((*game).mode!=1){
                if((isDouble(x)) && (atof(x)<=1) && (atof(x)>=0) ){
                    return 1;
                }
                printf(errorArgValue,"Error: ","a valid floating point value between 0 and 1\n");
			}
			else{	printf(errorSpecMode,"Solve");	}
		}
		else{	printf(errorEditSolve);	}
		return 0;
	case cmdGenerate:
        if (z != NULL || x == NULL || y == NULL){
            printf(NUMOFARG, "generate",2);
            return 0;
        }
		if((*game).mode!=0){
			if((*game).mode!=2){

                if(atoi(x) < 0 || atoi(x) > N * N || atoi(y) < 0 || atoi(y) > N * N){
                    printf(errorArgValue, "Error: ", "numbers between 0 and ");
                    printf("%d.\n",N * N);
                    return 0;
                }
			    return 1;
			}
			else{	printf(errorSpecMode,"Edit");	}
		}
		else{	printf(errorEditSolve);	}
		return 0;
	case cmdAutofill:
        if (x != NULL){
            printf(NUMOFARG, "autofill",0);
            return 0;
        }
		if((*game).mode!=0){
			if((*game).mode!=1){
				return 1;
			}
			else{	printf(errorSpecMode,"Solve");	}
		}
		else{	printf(errorEditSolve);	}
		return 0;
	case cmdHint: 	case cmdGuessHint:
        if (z != NULL || x == NULL || y == NULL){
            printf(NUMOFARG, "hint/guess_hint",2);
            return 0;
        }
		if((*game).mode!=0){
			if((*game).mode!=1){
                if((isNumeric(x)) && (atoi(x)<=N)){
                    if ((isNumeric(y)) && (atoi(y)<=N)) {
                        return 1;
                    }
                    printf(errorArgRange,"second");
                }
                else{	printf(errorArgRange,"first");	}
                printf(errorArgValue,"","a valid number between 0 and ");
                printf("%d.\n",N);
			}
			else{	printf(errorSpecMode,"Solve");	}
		}
		else{	printf(errorEditSolve);	}
		return 0;
	case cmdPrint: 	case cmdValidate: 	case cmdReset: 	case cmdSolNum:	case cmdUndo:	case cmdRedo:
        if (x != NULL){
            printf(NUMOFARG, enumToStr(cmdIndex),0);
            return 0;
        }
	    if((*game).mode!=0){
			return 1;
		}else{
		    printf(errorEditSolve);
		}
		return 0;
    case cmdExit:
        if (x != NULL){
            printf(NUMOFARG, enumToStr(cmdIndex),0);
            return 0;
        }
        return 1;

	}
	return -1;
}

/*Return 1 if the string input is a number, 0 otherwise*/
int isNumeric(const char *str)
{
    while(*str != '\0')
    {
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
