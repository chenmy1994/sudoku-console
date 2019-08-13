/*
*	Created by CMY on 29/07/2019.
 */

#include "Parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define errorCommand "Error: invalid command\n"


/* Get command from user, call the matching function for a valid command.*/
int getCommand (char* stream,Game* game){
	char* cmdType;
	int cmdIndex;
    char* x;
    char* y;
    char* z;
    int intX, intY, intZ;
	/*Received input from user successfully*/
	 if(fgets(stream,1024,stdin)!=NULL){
		/*user input isn't empty*/
		if (*stream!='\n'){
			cmdType= strtok(stream," \t\r\n"); /*cmdType*/
			printf("input: %s \n",cmdType);

			x=strtok(NULL," \t\r\n"); /*first argument*/
			printf("input: %s \n",x);
			y=strtok(NULL," \t\r\n"); /*second argument*/
			z=strtok(NULL," \t\r\n"); /*third argument*/
			if(z==NULL){
				z="-1";
			}
			if(x==NULL){
				x="0";
			}
			if(y==NULL){
				y="0";
			}
			cmdIndex=strToEnumIndex(cmdType,game);
			intX = atoi(x);
			intY = atoi(y);
			intZ = atoi(z);
			switch(cmdIndex){
			case cmdSolve:
				if(checkValidInput(cmdIndex,x,y,z,game)!=0){
                    return solve(x, game);
				}
			case cmdEdit:
				if(checkValidInput(cmdIndex,x,y,z,game)!=0){
                    return edit(x, game);
				}
				return 1;
			case cmdMarkErr:
				if(checkValidInput(cmdIndex,x,y,z,game)!=0){
					updateMarkErrors(game, intX);
				}
				return 1;
			case cmdPrint:
				printBoard(game);
				return 1;
			case cmdSet:
				if(checkValidInput(cmdIndex,x,y,z,game)!=0){
					set(intX, intY, intZ, game);
				}
				return 1;
			case cmdValidate:
				validate(game);
				return 1;
			case cmdGuess:
				if(checkValidInput(cmdIndex,x,y,z,game)!=0){
					guess();
				}
				return 1;
			case cmdGenerate:
				if(checkValidInput(cmdIndex,x,y,z,game)!=0){
					generate();
				}
				return 1;
			case cmdUndo:
				undo();
				break;
			case cmdRedo:
				redo();
				return 1;
			case cmdSave:
				if(checkValidInput(cmdIndex,x,y,z,game)!=0){
					save(x, game);
				}
				return 1;
			case cmdHint:
				if(checkValidInput(cmdIndex,x,y,z,game)!=0){
					hint(game, intX, intY);
				    return 1;
				}
				return 0;
			case cmdGuessHint:
				if(checkValidInput(cmdIndex,x,y,z,game)!=0){
					guessHint();
				}
				return 1;
			case cmdSolNum:
				num_solutions();
				return 1;
			case cmdAutofill:
				autofill();
				return 1;
			case cmdReset:
				reset();
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
char* enumToStr(cmdType cmd, Game* game){
	switch(cmd){
	case cmdSolve:
		return "solve";
		break;
	case cmdEdit:
		return "edit";
		break;
	case cmdMarkErr:
		if((*game).mode==2){
			return "mark_errors";
		}
		else{	return "";}
		break;
	case cmdPrint:
		if((*game).mode!=0){
			return "print_board";
		}
		else{	return "";}
		break;
	case cmdSet:
		if((*game).mode!=0){
			return "set";
		}
		else{	return "";}
		break;
	case cmdValidate:
		if((*game).mode!=0){
			return "validate";
		}
		else{	return "";}
		break;
	case cmdGuess:
		if((*game).mode==2){
			return "guess";
		}
		else{	return "";}
		break;
	case cmdGenerate:
		if((*game).mode==1){
			return "generate";
		}
		else{	return "";}
		break;
	case cmdUndo:
		if((*game).mode!=0){
			return "undo";
		}
		else{	return "";}
		break;
	case cmdRedo:
		if((*game).mode!=0){
			return "redo";
		}
		else{	return "";}
		break;
	case cmdSave:
		if((*game).mode!=0){
			return "save";
		}
		else{	return "";}
		break;
	case cmdHint:
		if((*game).mode==2){
			return "hint";
		}
		else{	return "";}
		break;
	case cmdGuessHint:
		if((*game).mode==2){
			return "guess_hint";
		}
		else{	return "";}
		break;
	case cmdSolNum:
		if((*game).mode!=0){
			return "num_solutions";
		}
		else{	return "";}
		break;
	case cmdAutofill:
		if((*game).mode==2){
			return "autofill";
		}
		else{	return "";}
		break;
	case cmdReset:
		if((*game).mode!=0){
			return "reset";
		}
		else{	return "";}
		break;
	case cmdExit:
		return "exit";
		break;
	default:
		return "";
		break;
	}
}

/*Converts input from user (command) to the matching item index for enum cmdType*/
int strToEnumIndex(char* cmdStr,Game* game){
	int i;
	char* enumTrans;
	for (i=0;i<TypeAmount;i++){
		enumTrans = enumToStr(i,game);
		if(strcmp(cmdStr,enumTrans)==0){
			return i;
		}
	}

	return -1;
}

/*Receives the user input (command and arguments) and return:
 * for valid input and arguments - return 1
 * for invalid number of arguments or value not in range, prints relevant error - return 0 */
int checkValidInput(int cmdIndex,char* x, char* y, char* z,Game* game){
	int N=(*game).n*(*game).m;
	switch(cmdIndex){
	case cmdSolve:
		/*input checks for solve*/
		break;
	case cmdEdit:
		/*input checks for edit*/
		break;
	case cmdMarkErr:
		if(x!=NULL){
			if( (atoi(x)==1) || (atoi(x)==0) ){
				return 1;
			}
			/*print error for wrong value*/
		}
		/*print error for wrong number of arg*/
		return 0;
		break;
	case cmdSet:
		if ( (x!=NULL) && (y!=NULL) && (z!=NULL) ){
			if( (atoi(x)<=N) && (atoi(y)<=N) && (atoi(z)<=N) ){
				return 1;
			}
			/*print error for wrong value*/
		}
		/*print error for wrong number of arg*/
		return 0;
		break;
	case cmdGuess:
		if (x!=NULL){
			if( (atof(x)<=1) && (atof(x)>=0) ){
				return 1;
			}
			/*print error for wrong value*/
		}
		/*print error for wrong number of arg*/
		return 0;
		break;
	case cmdGenerate:
		/*input checks for generate*/
		break;
	case cmdSave:
		/*input checks for save*/
		break;
	case cmdHint:
		if ( (x!=NULL) && (y!=NULL) ){
			if( (atoi(x)<=N) && (atoi(y)<=N) ){
				return 1;
			}
			/*print error for wrong value*/
		}
		/*print error for wrong number of arg*/
		return 0;
		break;
	case cmdGuessHint:
		if ( (x!=NULL) && (y!=NULL) ){
			if( (atoi(x)<=N) && (atoi(y)<=N) ){
				return 1;
			}
			/*print error for wrong value*/
		}
		/*print error for wrong number of arg*/
		return 0;
		break;
	}
	return -1;
}
