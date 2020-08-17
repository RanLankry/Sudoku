/*
 * Parser.c
 *
 *  Created on: 17 במרס 2020
 *      Author: ASUS
 */

#include "Parser.h"


void getCommand(CommandList* game){
	while(1){
		int ret;
		char buf[MAX_SIZE]={0};
		char finalbuf[MAX_SIZE]={0};
		char* token;
		char* parameters[MAX_SIZE];
		int len=0;

		printf("\nplease enter a command:\n");
		if((getInput(buf,finalbuf))==1){
			exitProg(game);
			return;
		}
		token = strtok(finalbuf, " ");
		while (token != NULL) {
			parameters[len]=token;
			len++;
			token = strtok(NULL, " ");
		}
		/* solve */
		if (!strcmp(parameters[0],"solve")){
			Board* board;
			int err;
			if((ret=checkSolve(len))==0){
				continue;
			}
			board=loadFromFile(parameters[1]);
			if (board==NULL){
				continue;
			}
			err=solve(game, board);
			if (err==1){
				printf("board loading failed because of a collision between 2 or more fixed cells\n");
				continue;
			}
		}
		/* edit */
		else if (!strcmp(parameters[0],"edit")){
			Board* board;

			if ((ret=checkEdit(len))==0){
				continue;
			}
			if (ret==1){
				board=createEmptyBoard();
				edit(game, board);
				continue;
			}
			board=loadFromFile(parameters[1]);
			if (board==NULL){
				continue;
			}
			edit(game,board);
		}
		/* mark_errors */
		else if (!strcmp(parameters[0],"mark_errors")){
			if((ret=checkMarkErrors(parameters,len,game->mode))==0){
				continue;
			}
			markErrors(game,atoi(parameters[1]));
		}
		/* print_board */
		else if (!strcmp(parameters[0],"print_board")){
			if((ret=checkPrintBoard(len, game->mode))==0){
				continue;
			}
			print_board(game);
		}
		/* set */
		else if (!strcmp(parameters[0],"set")){
			if (game->mode==0){
				printError(MODE);
				currCommands(game->mode);
				continue;
			}
			if((ret=checkSet(parameters,len, game->curr_command->after_command_board->n, game->curr_command->after_command_board->m))==0){
				continue;
			}
			if (game->mode==2){
				if (game->curr_command->after_command_board->fixed_board[atoi(parameters[2])-1][atoi(parameters[1])-1]==1){
					printError(FIXEDCELL);
					continue;
				}
			}
			set(game,atoi(parameters[1]),atoi(parameters[2]),atoi(parameters[3]));
		}
		/* validate */
		else if (!strcmp(parameters[0],"validate")){
			if (game->mode==0){
				printError(MODE);
				currCommands(game->mode);
				continue;
			}
			if((ret=checkValidate(len,game->curr_command->after_command_board->if_erroneous))==0){
				continue;
			}
			validate(game);
		}
		/* guess */
		else if (!strcmp(parameters[0],"guess")){
			if (game->mode!=2){
				printError(MODE);
				currCommands(game->mode);
				continue;
			}
			if((ret=checkGuess(parameters,len,game->curr_command->after_command_board->if_erroneous))==0){
				continue;
			}
			guess(game,atof(parameters[1]));
		}
		/* generate */
		else if (!strcmp(parameters[0],"generate")){
			int amount;
			if (game->mode!=1){
				printError(MODE);
				currCommands(game->mode);
				continue;
			}
			if((ret=checkGenerate(parameters,len,game->curr_command->after_command_board->m,
					game->curr_command->after_command_board->n, game->curr_command->after_command_board->if_erroneous))==0){
				continue;
			}
			amount=emptyCells(game->curr_command->after_command_board);
			if (amount<atoi(parameters[1])){
				printError(EMPTYCELLS);
				printf("number of empty cells is: %d\n",amount);
				continue;
			}
			generate(game,atoi(parameters[1]),atoi(parameters[2]));
		}
		/* undo */
		else if (!strcmp(parameters[0],"undo")){
			if((ret=checkUndo(len, game->mode))==0){
				continue;
			}
			undo(game);
		}
		/* redo */
		else if (!strcmp(parameters[0],"redo")){
			if((ret=checkRedo(len, game->mode))==0){
				continue;
			}
			redo(game);
		}
		/* save */
		else if (!strcmp(parameters[0],"save")){
			if (game->mode==0){
				printError(MODE);
				currCommands(game->mode);
				continue;
			}
			if((ret=checkSave(len,game->mode,game->curr_command->after_command_board->if_erroneous))==0){
				continue;
			}
			if (game->mode==1 && isValideBoard(game->curr_command->after_command_board)!=1){
				printError(NOSOLUTION);
				continue;
			}
			save(game, parameters[1]);
		}
		/* hint */
		else if (!strcmp(parameters[0],"hint")){
			if (game->mode!=2){
				printError(MODE);
				currCommands(game->mode);
				continue;
			}
			if((ret=checkHint(parameters,len,game->curr_command->after_command_board->if_erroneous,
					game->curr_command->after_command_board->n, game->curr_command->after_command_board->m))==0){
				continue;
			}
			if (game->curr_command->after_command_board->fixed_board[atoi(parameters[2])-1][atoi(parameters[1])-1]==1){ /* fixed */
				printError(FIXEDCELL);
				continue;
			}
			if (game->curr_command->after_command_board->game_board[atoi(parameters[2])-1][atoi(parameters[1])-1][0]==0){ /*not empty*/
				printError(NOTEMPTY);
				continue;
			}
			hint(game,atoi(parameters[1]),atoi(parameters[2]));
		}
		/* guess_hint */
		else if (!strcmp(parameters[0],"guess_hint")){
			if (game->mode!=2){
				printError(MODE);
				currCommands(game->mode);
				continue;
			}
			if((ret=checkGuessHint(parameters,len, game->curr_command->after_command_board->if_erroneous,
					game->curr_command->after_command_board->n, game->curr_command->after_command_board->m))==0){
				continue;
			}
			if (game->curr_command->after_command_board->fixed_board[atoi(parameters[2])-1][atoi(parameters[1])-1]==1){ /* fixed */
				printError(FIXEDCELL);
				continue;
			}
			if (game->curr_command->after_command_board->game_board[atoi(parameters[2])-1][atoi(parameters[1])-1][0]==0){  /*not empty*/
				printError(NOTEMPTY);
				continue;
			}
			guess_hint(game,atoi(parameters[1]),atoi(parameters[2]));
		}
		/* num_solutions */
		else if (!strcmp(parameters[0],"num_solutions")){
			if (game->mode==0){
				printError(MODE);
				currCommands(game->mode);
				continue;
			}
			if((ret=checkNumSolutions(len,game->curr_command->after_command_board->if_erroneous))==0){
				continue;
			}
			num_solutions(game);
		}
		/* autofill */
		else if (!strcmp(parameters[0],"autofill")){
			if (game->mode!=2){
				printError(MODE);
				currCommands(game->mode);
				continue;
			}
			if((ret=checkAutofill(len,game->curr_command->after_command_board->if_erroneous))==0){
				continue;
			}
			autofill(game);
		}
		/* reset */
		else if (!strcmp(parameters[0],"reset")){
			if((ret=checkReset(len, game->mode))==0){
				continue;
			}
			reset(game);
		}
		/* exit */
		else if (!strcmp(parameters[0],"exit")){
			if((ret=checkExit(len))==0){
				continue;
			}
			exitProg(game);
			return;
		}
		/* invalid command */
		else {
			printError(COMMANDNAME);
			currCommands(game->mode);
			continue;
		}

	}

}
