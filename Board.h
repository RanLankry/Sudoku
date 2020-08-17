/*
 * Board.h
 *
 *  Created on: Mar 14, 2020
 *      Author: DELL
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "AuxBoard.h"

/*
 *Board Summary:
 *
 *Module that includes structure that represents the current board game state
 *Struct Board- represents a sudoku board and it's state during the game
 *This module includes functions that manipulate boards
 *This is an auxiliary module for CommandList module
 *
 *createNewBoard- creates a new board using given values
 *createEmptyBoard- creates a 9*9 empty board
 *duplicateBoard- duplicates a given board
 *getErrBoard- finds and updates which cells are erroneous in a board
 *destroyBoard- destroys a board
 *print- prints a board according to the game mode and to mark_errors param
 *setValue- set value in a board
 *isErrValForCell- checks if a value is erroneous for a cell
 *emptyCells- calculate the number of empty cells in a board
 *checkDifferencesBetweenBoards- prints differences between 2 boards
 *getNumInCell- gets the numeric value for a cell
 *getSingleLegalValForCell- return the single legal value of a cell if exists
 *clearBoard- sets all board value to 0
 *
 */

#define DEFUALT_SIZE_OF_EMPTY_BOARD 3

/*
 * type used for representing a Sudoku board
 */
typedef struct sudoku_board_t{
	int n;
	int m;
	int*** game_board; /*for each cell X Y it maintains an array of (n*m)+1 integers represents the value in the cell*/
	int*** solved_board; /*will contain an ILP solution*/
	float*** guess_board; /*will contain LP solution*/
	int** fixed_board; /*for each cell X Y : 1 -> fixed. 0 -> not fixed*/
	int** err_board; /*for each cell X Y : 1 -> erroneous . 0 -> not erroneous*/
	int if_erroneous; /*a flag : 1 -> erroneous board . 0 -> not erroneous board*/

} Board;


/*
 *@param
 * m*n- the block size
 * game_board - 3D matrix represents the game board
 * fixed_board - 2D matrix represents the fixed cells
 *
 * @return
 * a new board that was build according to the params
 */
Board* createNewBoard(int n, int m, int*** game_board, int** fixed_board);

/*
 * @param
 * None.
 *
 * @return
 * a new empty 9x9 board
 *
 */
Board* createEmptyBoard();

/*
 * @param
 * b - a pointer to a board that will be duplicate
 *
 * @return
 * a pointer to the duplicated board
 *
 */
Board* duplicateBoard(Board* b);

/*
 * assigns 2D matrix that represents the errors board to err_board of b
 * assigns 1 - there is an error / 0 - no errors to if_erroneous of b
 *
 * @param
 * b - a pointer to a board
 *
 * @return
 * None.
 *
 */
void getErrBoard( Board* b);


/*
 * destroys a board by freeing memory allocations
 *
 * @param
 * b - a pointer to a board
 *
 * @return
 * None.
 *
 */
void destroyBoard(Board* b);


/*
 * prints the board according to mark_errors_param
 * If the game is in edit mode errors will be printed no matter what mark_errors_param value is
 *
 * @param
 * b - a pointer to a board
 * mode - 1 for edit\ 2 for solve
 * mark_errors_param(only matter in Solve)
 *
 * @return
 * None.
 *
 */

void print(Board* b,int mode, int mark_errors_param);

/*
 * sets game_board[row][col]=z
 * updates err_board and if_erroneous accordingly
 *
 * @param
 * b - a pointer to a board
 * row - row of a cell that will be set
 * col - column of a cell that will be set
 * z - the new value of the cell
 *
 * @return
 * None.
 *
 */
void setValue(Board* b, int row , int col , int z);

/*
 * @param
 * b - a pointer to a board
 * row - row of a cell that will be checked
 * col - column of a cell that will be checked
 * z - the value that will be checked
 *
 * @return
 * 1 - value is erroneous for cell [row][col] in the game_board
 * 0 - value is OK
 *
 */
int isErrValForCell(Board* b, int row , int col , int val);


/*
 * @param
 * board - a pointer to a board
 *
 * @return
 * amount of empty cells
 *
 */
int emptyCells(Board* board);

/*
 * prints the differences between them if exists
 *
 * @param
 * curr_b - a pointer to a board
 * before_b - a pointer to a board
 *
 * @return
 * None.
 *
 */
void checkDifferencesBetweenBoards(Board* curr_b,Board* before_b);


/*
 *
 * prints the differences between them if exists
 *
 * @param
 * b - a pointer to a board
 * game_or_solve_b - to get value from game_board('g') or solved_board('s')
 * row - row of a cell that will be checked
 * col - column of a cell that will be checked
 *
 * @return
 * value in cell [row][column]
 * -1 if the given board points to NULL
 *
 */
int getNumInCell(Board* b,char game_or_solve_b, int row, int column);



/*
 * @param
 * b - a pointer to a board
 * row - row of a cell that will be checked
 * col - column of a cell that will be checked
 *
 * @return
 * a single legal value for empty cell [row][column] if exists
 * 0 - otherwise
 *
 */
int getSingleLegalValForCell(Board* b, int row, int column);

/*
 * sets all cell values of game_board to 0
 *
 * @param
 * b - a pointer to a board
 *
 */
void clearBoard(Board* b);

#endif /* BOARD_H_ */
