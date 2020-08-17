/*
 * AuxBoard.h
 *
 *  Created on: Apr 6, 2020
 *      Author: DELL
 */

#ifndef AUXBOARD_H_
#define AUXBOARD_H_

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/*
 *AuxBoard Summary:
 *
 *Module that includes functions that manipulate Matrices (implemented by arrays)
 *This is an auxiliary module for Board module
 *
 *create2DMatrix- allocates memory for a 2D integers array
 *create3DMatrix- allocates memory for a 3D integers array
 *create3DFMatrix- allocates memory for a 3D floating point array
 *free2DMatrix- free a 2D integers array
 *free3DMatrix- free a 3D integers array
 *free3DFMatrix- free a 3D floating point array
 *dup2DMatrix- duplicates a 2D integers array
 *dup3DMatrix- duplicates a 3D integers array
 *dup3DFMatrix- duplicates a 3D floating point array
 *clearAllValues1D- initializing an integer array with 0
 *isLegalForRow- checks if a specific value is legal for a row
 *isLegalForColumn- checks if a specific value is legal for a column
 *isLegalForBlock- checks if a specific value is legal for a block
 *
 */

/*
 * @param
 * size- number of elements in a dimension of the array
 *
 * @return
 * a zero initialized 2D matrix (size*size) of integers
 */
int** create2DMatrix(int size);


/*
 * @param
 * size- number of elements in a dimension of the array
 *
 * @return
 * a zero initialized 3D matrix (size*size*(size+1)) of integers
 */
int*** create3DMatrix(int size);

/*
 * @param
 * size- number of elements in a dimension of the array
 *
 * @return
 * a zero initialized 3D matrix (size*size*(size+1)) of floating point numbers
 */
float*** create3DFMatrix(int size);

/*
 * free 2D array memory allocation
 *
 * @param
 * board- pointer to a 2D array
 * size- number of elements in a dimension of the array
 *
 * @return
 * None.
 *
 */
void free2DMatrix(int** board, int size);

/*
 * free 3D array memory allocation
 *
 * @param
 * board- pointer to a 3D array
 * size- number of elements in a dimension of the array
 *
 * @return
 * None.
 *
 */
void free3DMatrix(int*** board, int size);

/*
 * free 3D floating point array memory allocation
 *
 * @param
 * board- pointer to a 3D array
 * size- number of elements in a dimension of the array
 *
 * @return
 * None.
 *
 */
void free3DFMatrix(float*** board, int size);

/*
 *duplicates 2D array
 *
 * @param
 * board- pointer to a 2D array
 * size- number of elements in a dimension of the array
 *
 * @return
 * pointer to the 2D duplicated array
 *
 */
int** dup2DMatrix(int** board, int size);


/*
 *duplicates 3D array
 *
 * @param
 * board- pointer to a 3D array
 * size- number of elements in a dimension of the array
 *
 * @return
 * pointer to the 3D duplicated array
 *
 */
int*** dup3DMatrix(int*** board, int size);

/*
 *duplicates 3D floating point array
 *
 * @param
 * board- pointer to a 3DF array
 * size- number of elements in a dimension of the array
 *
 * @return
 * pointer to the 3D floating point duplicated array
 *
 */
float*** dup3DFMatrix(float*** board, int size);

/*
 * sets every value in an array to zero
 *
 * @param
 * array- pointer to a 1D array
 * size- number of elements in the array
 *
 * @return
 * None.
 *
 */
void clearAllValues1D(int* array, int size);

/*
 * @param
 * board- pointer to a 3D array
 * row- the column of val
 * column- the column of val
 * val- value that will be checked
 * size- number of elements in a dimension of the array
 *
 * @return
 * 1- if val is legal value for cell in the row
 * 0- otherwise
 *
 */
int isLegalForRow(int*** board,int row,int col,int val,int size);

/*
 * @param
 * board- pointer to a 3D array
 * row- the column of val
 * column- the column of val
 * val- value that will be checked
 * size- number of elements in a dimension of the array
 *
 * @return
 * 1- if val is legal value for cell in the column
 * 0- otherwise
 *
 */
int isLegalForColumn(int*** board,int row, int col,int val,int size);

/*
 * @param
 * board- pointer to a 3D array
 * row- the column of val
 * column- the column of val
 * val- value that will be checked
 * n,m- n*m is the number of elements in a dimension of the array
 *
 * @return
 * 1- if val is legal value for cell in the block
 * 0- otherwise
 *
 */
int isLegalForBlock(int*** board,int row,int col,int val,int n,int m);



#endif /* AUXBOARD_H_ */
