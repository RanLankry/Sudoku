/*
 * BackTracking.h
 *
 *  Created on: 7 באפר 2020
 *      Author: ASUS
 */

#ifndef BACKTRACKING_H_
#define BACKTRACKING_H_

#include "Board.h"

/*
 * BackTracking Summary:
 *
 * A module that responsible for running the backtracking algorithm iteratively, using a structure which represents a stack.
 *
 * struct Stack - type used for representing a stack which is used for the backtracking algorithm
 * isLegalForRow2D - checks if a value is legal according to the rest of the values in the row
 * isLegalForColumn2D - checks if a value is legal according to the rest of the values in the column
 * isLegalForBlock2D - checks if a value is legal according to the rest of the values in the block
 * isLegal2D - checks if a value is legal using 3 sub functions that check the row, column and block
 * createStack - initializes a stack for the backtracking algorithm
 * destroyStack - frees a Stack object memory after finishing the backtracking algorithm
 * BackTracking - runs the backtracking algorithm on the desired board
 *
 *
 */


/*
 * type used for representing a stack which is used for the backtracking algorithm.
 */
typedef struct Stack{
	int maxSize;
	int size;
	int* values;
	int* indices;
} Stack;

/*
 * A function that checks if a value placing is legal in a given 2D board according to the values of its row.
 *
 * @param
 * board - the current 2D board
 * row - the required row
 * val - the required value
 * size - the row size
 *
 *
 * @return
 * 1 if placing is legal
 * 0 otherwise
 *
 */
int isLegalForRow2D(int** board,int row,int val,int size);

/*
 * A function that checks if a value placing is legal in a given 2D board according to the values of its column.
 *
 * @param
 * board - the current 2D board
 * col - the required column
 * val - the required value
 * size - the column size
 *
 *
 * @return
 * 1 if placing is legal
 * 0 otherwise
 *
 */
int isLegalForColumn2D(int** board,int col,int val,int size);


/*
 * A function that checks if a value placing is legal in a given 2D board according to the values in its block.
 *
 * @param
 * board - the current 2D board
 * row - the required row
 * col - the required column
 * val - the required value
 * n,m - the dimensions the board's block
 *
 *
 * @return
 * 1 if placing is legal
 * 0 otherwise
 *
 */
int isLegalForBlock2D(int** board,int row,int col,int val,int n,int m);


/*
 * A function that checks if a value placing is legal by calling the 3 auxiliary functions which check the value's row, column and block.
 *
 * @param
 * board - the current 2D board
 * row - the required row
 * col - the required column
 * val - the required value
 * n,m - the dimensions the board's block
 *
 *
 * @return
 * 1 if placing is legal
 * 0 otherwise
 *
 */
int isLegal2D(int ** board, int row, int col, int val, int n, int m);


/*
 * A function that initialize a stack for the backtracking algorithm
 *
 * @param
 * n,m - The block sizes of the board which backtracked for finding its solutions
 *
 * @return
 * Pointer to a Stack object, which used for backtracking
 *
 */
Stack* createStack(int n, int m);

/*
 * A function that frees a Stack object memory after finishing the backtracking algorithm or exiting due to an error.
 *
 * @param
 * stack - A pointer to Stack object.
 *
 * @return
 * None. freeing the stack memory.
 *
 */
void destroyStack(Stack* stack);

/*
 * The function that runs the backtracking algorithm on the desired board.
 *
 * @param
 * board - A pointer to a Board object, which represent the board that we want to find its solutions.
 *
 * @return
 * number of solutions of the current board. number is equal or greater than 0.
 *
 */
int BackTracking(Board* board);

#endif /* BACKTRACKING_H_ */
