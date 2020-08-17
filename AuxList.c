/*
 * AuxList.c
 *
 *  Created on: Mar 15, 2020
 *      Author: DELL
 */
#include "AuxList.h"

void insertCommand(CommandList* game, CommandNode* command){
	command->prev=game->curr_command;
	if(game->curr_command == NULL){
		game->curr_command=command;
		game->head=game->curr_command;
		game->tail=game->curr_command;
	}
	else{
		clearListFrom(game->curr_command->next);
		game->curr_command->next=command;
		game->curr_command=command;
		game->tail=command;
	}
}

void clearList(CommandList* game){
	CommandNode* temp;
	while(game->head!=game->tail){
		temp= game->head;
		game->head= game->head->next;
		destroyCommand(temp);
	}
	if(game->head!=NULL){
		destroyCommand(game->head);
		game->curr_command=NULL;
		game->head=NULL;
		game->tail=NULL;
	}
}

void clearListFrom(CommandNode* from){
	CommandNode* temp;
	while (from!=NULL){
		temp=from;
		from=from->next;
		destroyCommand(temp);
	}
}

void destroyCommand(CommandNode* command){
	destroyBoard(command->after_command_board);
	free(command);
}
