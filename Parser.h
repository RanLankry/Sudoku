/*
 * Parser.h
 *
 *  Created on: Mar 14, 2020
 *      Author: DELL
 */

#ifndef PARSER_H_
#define PARSER_H_

#define MAX_SIZE 256


#include "CommandList.h"

/*
 * AuxParser Summary:
 *
 * A module that responsible for parsing the user input.
 *
 */



/*
 * The function which is responsible for communicating with the user.
 * Parses each command, and print an error if input is incorrect for some reason, or calling the appropriate command
 *
 * @param
 * game - a pointer to CommandList object which represents a new game initialization
 *
 * @return
 * None.
 *
 */
void getCommand(CommandList* game);

#endif /* PARSER_H_ */
