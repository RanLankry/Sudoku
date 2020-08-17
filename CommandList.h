/*
 * CommandList.h
 *
 *  Created on: Mar 14, 2020
 *      Author: DELL
 */

#ifndef COMMANDLIST_H_
#define COMMANDLIST_H_
#include "AuxList.h"
#include "LP.h"
#include "BackTracking.h"

/*
 *CommandList Summary:
 *
 *Module that includes all possible commands that the user can use during a game
 *This module is a pipe between the Parser module and the game state
 *
 *createGame- creates a new game when entering the program
 *destroyGame- destroys a game
 *solve- entering to solve mode with loaded board
 *edit- entering to edit mode with loaded board or 9*9 empty board
 *markErrors- sets mark_error parameter
 *print_board- prints a board according to the format
 *set- set value in a cell
 *validate- checks if a current game board is solvable
 *guess- guesses a solution to a current game board using threshold
 *generate- generates new board
 *undo- undo command
 *redo- redo command
 *save- saves a current game board to a file
 *hint- gives a hint for a cell if exists
 *guess_hint- gives a probability list for values for a cell
 *num_solutions- calculates the number of different solutions for the current game board
 *autofill- autofill single legal value cells
 *reset- undo all commands that were done in the game
 *exitProg- exit the program cleanly
 *
 */


/*
 * this function is called only once - when entering the program
 *
 * @param
 * None.
 *
 * @return
 * an empty CommandList
 *
 */
CommandList* createGame();

/*
 * free all memory allocations that was made through the game
 *
 * @param
 * game - a pointer to a game
 *
 * @return
 * None.
 *
 */
void  destroyGame(CommandList* game);

/*
 * for all of this functions - we assume that all the required assumptions was checked by the parser
 */


/*
 * entering solve mode
 *
 * @param
 * game - a pointer to a game
 * b- a board that contains a legal loaded file
 *
 * @return
 * 1 - fixed_cell board is erroneous (solve fails)
 * 0 - otherwise
 *
 */
int solve(CommandList* game, Board* b);

/*
 * entering edit mode
 *
 * @param
 * game - a pointer to a game
 * b- a board that contains a legal loaded file
 *
 * @return
 * None.
 *
 */
void edit(CommandList* game, Board* b);

/*
 * sets mark_errors parameter
 *
 * @param
 * game - a pointer to a game
 *
 * @return
 * None.
 *
 */
void markErrors(CommandList* game, int x);

/*
 * prints the current game_board
 *
 * @param
 * game - a pointer to a game
 *
 * @return
 * None.
 *
 */
void print_board(CommandList* game);

/*
 * sets game_board[Y][X]=Z
 * prints the board
 *
 * @param
 * game - a pointer to a game
 * x - column index
 * y - line index
 * z - value
 *
 * @return
 * None.
 *
 */
void set(CommandList* game, int x, int y, int z);

/*
 * prints whether the board is solvable or not
 *
 * @param
 * game - a pointer to a game
 *
 * @return
 * None.
 *
 */
void validate(CommandList* game);

/*
 *
 * prints a guessed board according to threshold
 *
 * @param
 * game - a pointer to a game
 * x - a threshold for guessing
 *
 * @return
 * None.
 *
 */
void guess(CommandList* game, float x);


/*
 * generates new board according to x and y
 * prints it
 *
 * @param
 * game - a pointer to a game
 * x - the number of empty cells
 * y- the number of filled cells of the new board
 *
 * @return
 * None.
 *
 */
void generate(CommandList* game,int x, int y);

/*
 * undo to the previous move if exists and print the changes
 *
 * @param
 * game - a pointer to a game
 *
 * @return
 * None.
 *
 */
void undo(CommandList* game);

/*
 * redo to the next move if exists and print the changes
 *
 * @param
 * game - a pointer to a game
 *
 * @return
 * None.
 *
 */
void redo(CommandList* game);

/*
 *
 * saves the current game_board to the file according to limitations
 *
 * @param
 * game - a pointer to a game
 * path - a path to a file
 *
 * @return
 * None.
 *
 */
void save(CommandList* game, char* path);

/*
 * if the game_board is solvable, prints a hint for a cell[y][x]
 *
 * @param
 * game - a pointer to a game
 * x - column index
 * y - line index
 *
 * @return
 * None.
 *
 */
void hint(CommandList* game,int x, int y);

/*
 *
 * if the game_board is solvable, prints all legal values for the [y][x] cell with probability greater than 0
 *
 * @param
 * game - a pointer to a game
 * x - column index
 * y - line index
 *
 * @return
 * None.
 *
 */
void guess_hint(CommandList* game,int x, int y);

/*
 *if the board is solvable - use backtracking to find out number of solutions and print it
 *
 * @param
 * game - a pointer to a game
 *
 * @return
 * None.
 *
 */
void num_solutions(CommandList* game);

/*
 * fills the cells which have single legal value with it and prints the changes
 * @param
 * game - a pointer to a game
 *
 * @return
 * None.
 *
 */
void autofill(CommandList* game);

/*
 * undo all moves
 * prints the board at the end
 * @param
 * game - a pointer to a game
 *
 * @return
 * None.
 *
 */
void reset(CommandList* game);

/*
 * terminates the game and free all memory resources
 *
 * @param
 * game - a pointer to a game
 *
 * @return
 * None.
 *
 */
void exitProg(CommandList* game);

#endif /* COMMANDLIST_H_ */

