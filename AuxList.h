/*
 * AuxList.h
 *
 *  Created on: Mar 15, 2020
 *      Author: DELL
 */

#ifndef AUXLIST_H_
#define AUXLIST_H_
#include "AuxParser.h"

/*
 *AuxList Summary:
 *
 *Module that includes structures that represent the current game state
 *Struct CommandNode- represents a single command of the game that changed his state
 *Struct CommandList- a doubly linked list of command nodes that represents the entire game
 *This module includes functions that manipulate nodes and lists of nodes
 *This is an auxiliary module for CommandList module
 *
 *insertCommand- inserts a command node to the list
 *clearList- clears a command list
 *clearListFrom- clears a command list from a given node
 *destroyCommand- free all memory allocations for a given node
 *copyNode- duplicate a given node
 *
 */

/*
 * type used for representing the game mode
 */
typedef enum game_mode_t{
	INITMODE,
	EDITMODE,
	SOLVEMODE
} Game_MODE;


/*
 * type used for representing a single command node
 */
typedef struct command_node_t{
	struct command_node_t* next; /* a pointer to the next command node*/
	struct command_node_t* prev;/* a pointer to the previous command node*/
	Board* after_command_board; /* the board after this current command was completed*/

} CommandNode;


/*
 * type used for representing a commands list
 */
typedef struct commands_list_t{
	Game_MODE mode; /* the game mode (the initial value will be INIT)*/
	int mark_err; /* a flag for the mark errors parameter 1 -> on , 0 -> off(the default value will be 1)*/
	CommandNode* curr_command;
	CommandNode* head;
	CommandNode* tail;

} CommandList;


/*
 * adds a command node to the list
 *
 * @param
 * game- the list of commands of the current game
 * command- a command node which will be insert
 *
 * @return
 * None.
 *
 */
void insertCommand(CommandList* game, CommandNode* command);

/*
 * clears a list
 *
 * @param
 * game- the list of commands of the current game
 *
 * @return
 * None.
 *
 */
void clearList(CommandList* game);

/*
 *clears a list from a specific node
 *!func does not change the game's pointer(head,tail,curr_command)!
 *
 * @param
 * game- the list of commands of the current game
 *
 * @return
 * None.
 *
 */
void clearListFrom(CommandNode* from);

/*
 * destroys a node by freeing memory allocations
 *
 * @param
 * command - a node in the list that will be destroyed
 *
 * @return
 * None.
 *
 */
void destroyCommand(CommandNode* command);

/*
 * @param
 * command - a node in the list that will be copied
 *
 * @return
 *new node with the same values of the one in the input ,but next and prev are NULL
 *
 */
CommandNode* copyNode(CommandNode* command);


#endif /* AUXLIST_H_ */
