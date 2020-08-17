/*
 * AuxBoard.c
 *
 *  Created on: Apr 6, 2020
 *      Author: DELL
 */

#include "AuxBoard.h"


int** create2DMatrix(int size){
	int i;
	int** mat=(int**)calloc(size, sizeof(int*));
	if(mat==NULL){
		printf("terminating due to unexpected error\n");
		exit(EXIT_FAILURE);
	}
	for (i=0; i<size;i++){
		if((mat[i]=(int*)calloc(size,sizeof(int)))==NULL){
			printf("terminating due to unexpected error\n");
			exit(EXIT_FAILURE);
		}
	}
	return mat;
}

int*** create3DMatrix(int size){
	int i;
	int*** mat=(int***)calloc(size, sizeof(int*));
	if(mat==NULL){
		printf("terminating due to unexpected error\n");
		exit(EXIT_FAILURE);
	}
	for (i=0; i<size;i++){
		if((mat[i]=(int**)calloc(size,sizeof(int*)))==NULL){
			printf("terminating due to unexpected error\n");
			exit(EXIT_FAILURE);
		}
	}
	for (i=0;i<size*size;i++){
		if((mat[i/size][i%size]=(int*)calloc(size+1,sizeof(int)))==NULL){
			printf("terminating due to unexpected error\n");
			exit(EXIT_FAILURE);
		}
	}
	return mat;
}

float*** create3DFMatrix(int size){
	int i;
	float*** mat=(float***)calloc(size, sizeof(float*));
	if(mat==NULL){
		printf("terminating due to unexpected error\n");
		exit(EXIT_FAILURE);
	}
	for (i=0; i<size;i++){
		if((mat[i]=(float**)calloc(size,sizeof(float*)))==NULL){
			printf("terminating due to unexpected error\n");
			exit(EXIT_FAILURE);
		}
	}
	for (i=0;i<size*size;i++){
		if((mat[i/size][i%size]=(float*)calloc(size+1,sizeof(float)))==NULL){
			printf("terminating due to unexpected error\n");
			exit(EXIT_FAILURE);
		}
	}
	return mat;
}


void free2DMatrix(int** board, int size){
	int i;
	if(board!=NULL){
		for (i=0; i<size;i++){
			free(board[i]);
		}
		free(board);
	}
}

void free3DMatrix(int*** board, int size){
	int i,j;
	if(board!=NULL){
		for (i=0;i<size;i++){
			for (j=0;j<size;j++){
				free(board[i][j]);
			}
			free(board[i]);
		}
		free(board);
	}
}

void free3DFMatrix(float*** board, int size){
	free3DMatrix((int***)board,size);
}

int** dup2DMatrix(int** board, int size){
	int i;
	int** new;
	if(board==NULL) return NULL;
	new=create2DMatrix(size);
	for (i=0;i<size*size;i++){
		new[i/size][i%size]=board[i/size][i%size];
	}
	return new;
}

int*** dup3DMatrix(int*** board, int size){
	int i,j;
	int*** new;
	if(board==NULL) return NULL;
	new=create3DMatrix(size);
	for (i=0;i<size*size;i++){
		for(j=0;j<size+1;j++){
			new[i/size][i%size][j]=board[i/size][i%size][j];
		}
	}
	return new;
}

float*** dup3DFMatrix(float*** board, int size){
	int i,j;
	float*** new;
	if(board==NULL) return NULL;
	new=(float***)create3DMatrix(size);
	for (i=0;i<size*size;i++){
		for(j=0;j<size+1;j++){
			new[i/size][i%size][j]=board[i/size][i%size][j];
		}
	}
	return new;
}

void clearAllValues1D(int* array, int size){
	int i;
	for(i=0; i<size; i++){
		array[i]=0;
	}
}

int isLegalForRow(int*** board,int row,int col,int val,int size){
	int i=0;
	while(i<size){
		if(i!= col && board[row][i][val])
			return 0;
		i++;
	}
	return 1;
}

int isLegalForColumn(int*** board,int row,int col,int val,int size){
	int i=0;
	while(i<size){
		if(i!= row && board[i][col][val])
			return 0;
		i++;
	}
	return 1;
}

int isLegalForBlock(int*** board,int row,int col,int val,int n,int m){
	int i,j;
	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			if( (((row/m)*m+i)!= row || (((col/n)*n)+j)!= col) && board[((row/m)*m)+i][((col/n)*n)+j][val])
				return 0;
		}
	}
	return 1;
}





