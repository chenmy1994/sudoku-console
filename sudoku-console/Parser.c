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


/* Get command from user, call the matching function for a valid command.*/
int getCommand (char* stream,Game* game){
	char* cmdType;
	int cmdIndex;
    char* x;
    char* y;
    char* z;
	/*Received input from user successfully*/
	 if(fgets(stream,1024,stdin)!=NULL){
		/*user input isn't empty*/
		if (*stream!='\n'){
			cmdType= strtok(stream," \t\r\n"); /*cmdType*/
			x=strtok(NULL," \t\r\n"); /*first argument*/
			y=strtok(NULL," \t\r\n"); /*second argument*/
			z=strtok(NULL," \t\r\n"); /*third argument*/

			cmdIndex=strToEnumIndex(cmdType);

			switch(cmdIndex){
			case cmdSolve:
				if(checkValidInput(cmdIndex,x,y,z,game)!=0){
                    return solve(x, game);
				}
				return 1;
			case cmdEdit:
				if(checkValidInput(cmdIndex,x,y,z,game)!=0){
                    return edit(x, game);
				}
				return 1;
			case cmdMarkErr:
				if(checkValidInput(cmdIndex,x,y,z,game)!=0){
					updateMarkErrors(game, atoi(x));
				}
				return 1;
			case cmdPrint:
				if(checkValidInput(cmdIndex,x,y,z,game)!=0){
					printBoard(game);
				}
				return 1;
			case cmdSet:
				if(checkValidInput(cmdIndex,x,y,z,game)!=0){
					set(atoi(x), atoi(y), atoi(z), game,0);
				}
				return 1;
			case cmdValidate:
				if(checkValidInput(cmdIndex,x,y,z,game)!=0){
					validate(game);
				}
				return 1;
			case cmdGuess:
				if(checkValidInput(cmdIndex,x,y,z,game)!=0){
					guess();
				}
				return 1;
			case cmdGenerate:
				if(checkValidInput(cmdIndex,x,y,z,game)!=0){
					generate(atoi(x), atoi(y), game);
				}
				return 1;
			case cmdUndo:
				if(checkValidInput(cmdIndex,x,y,z,game)!=0){
					undo(game,0);
				}
				return 1;
			case cmdRedo:
				if(checkValidInput(cmdIndex,x,y,z,game)!=0){
					redo(game);
				}
				return 1;
			case cmdSave:
				if(checkValidInput(cmdIndex,x,y,z,game)!=0){
					save(x, game);
				}
				return 1;
			case cmdHint:
				if(checkValidInput(cmdIndex,x,y,z,game)!=0){
					hint(atoi(x), atoi(y),game);
				    return 1;
				}
				return 0;
			case cmdGuessHint:
				if(checkValidInput(cmdIndex,x,y,z,game)!=0){
					guessHint();
				}
				return 1;
			case cmdSolNum:
				if(checkValidInput(cmdIndex,x,y,z,game)!=0){
					num_solutions();
				}
				return 1;
			case cmdAutofill:
				if(checkValidInput(cmdIndex,x,y,z,game)!=0){
					autofill(game);
				}
				return 1;
			case cmdReset:
				if(checkValidInput(cmdIndex,x,y,z,game)!=0){
					reset(game);
				}
				return 1;
			case cmdExit:
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
		break;
	case cmdEdit:
		return "edit";
		break;
	case cmdMarkErr:
		return "mark_errors";
		break;
	case cmdPrint:
		return "print_board";
		break;
	case cmdSet:
		return "set";
		break;
	case cmdValidate:
		return "validate";
		break;
	case cmdGuess:
		return "guess";
		break;
	case cmdGenerate:
		return "generate";
		break;
	case cmdUndo:
		return "undo";
		break;
	case cmdRedo:
		return "redo";
		break;
	case cmdSave:
		return "save";
		break;
	case cmdHint:
		return "hint";
		break;
	case cmdGuessHint:
		return "guess_hint";
		break;
	case cmdSolNum:
		return "num_solutions";
		break;
	case cmdAutofill:
		return "autofill";
		break;
	case cmdReset:
		return "reset";
		break;
	case cmdExit:
		return "exit";
		break;
	default:
		return "";
		break;
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
int checkValidInput(int cmdIndex,char* x, char* y, char* z,Game* game){
	int N=(*game).n*(*game).m;
	switch(cmdIndex){
	case cmdSolve:
		/*input checks for solve*/
		break;
	case cmdEdit:
		/*input checks for edit*/
		break;
	case cmdSave:
		/*input checks for save*/
		break;
	case cmdMarkErr:
		if((*game).mode!=0){
			if((*game).mode!=1){
				if(x!=NULL){
					if((isNumeric(x))&&((atoi(x)==1) || (atoi(x)==0))){
						return 1;
					}
					printf(errorArgValue,"Error: ","1 or 0\n");
				}
				else{	printf(errorArgNum,1);	}
			}
			else{	printf(errorSpecMode,"Solve");	}
		}
		else{	printf(errorEditSolve);	}
		return 0;
		break;
	case cmdSet:
		if((*game).mode!=0){
			if ( (x!=NULL) && (y!=NULL) && (z!=NULL) ){
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
			else{	printf(errorArgNum,3);	}
		}
		else{	printf(errorEditSolve);	}
		return 0;
		break;
	case cmdGuess:
		if((*game).mode!=0){
			if((*game).mode!=1){
				if (x!=NULL){
					if((isDouble(x)) && (atof(x)<=1) && (atof(x)>=0) ){
						return 1;
					}
					printf(errorArgValue,"Error: ","a valid floating point value between 0 and 1\n");
				}
				else{	printf(errorArgNum,1);	}
			}
			else{	printf(errorSpecMode,"Solve");	}
		}
		else{	printf(errorEditSolve);	}
		return 0;
		break;
	case cmdGenerate:
		if((*game).mode!=0){
			if((*game).mode!=2){
				return 1;
			}
			else{	printf(errorSpecMode,"Edit");	}
		}
		else{	printf(errorEditSolve);	}
		return 0;
		break;
	case cmdAutofill:
		if((*game).mode!=0){
			if((*game).mode!=1){
				return 1;
			}
			else{	printf(errorSpecMode,"Solve");	}
		}
		else{	printf(errorEditSolve);	}
		return 0;
		break;
	case cmdHint: 	case cmdGuessHint:
		if((*game).mode!=0){
			if((*game).mode!=1){
				if ( (x!=NULL) && (y!=NULL) ){
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
				else{	printf(errorArgNum,2);	}
			}
			else{	printf(errorSpecMode,"Solve");	}
		}
		else{	printf(errorEditSolve);	}
		return 0;
		break;
	case cmdPrint: 	case cmdValidate: 	case cmdReset: 	case cmdSolNum:	case cmdUndo:	case cmdRedo:
		if((*game).mode!=0){
			return 1;
		}
		else{	printf(errorEditSolve);	}
		return 0;
		break;
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
