/*
 * BackTracking.c
 *
 *  Created on: 7 באפר 2020
 *      Author: ASUS
 */

#include "BackTracking.h"

int isLegalForRow2D(int** board,int row,int val,int size){
	int i=0;
	while(i<size){
		if(board[row][i]==val)
			return 0;
		i++;
	}
	return 1;
}

int isLegalForColumn2D(int** board,int col,int val,int size){
	int i=0;
	while(i<size){
		if(board[i][col]==val)
			return 0;
		i++;
	}
	return 1;
}

int isLegalForBlock2D(int** board,int row,int col,int val,int n,int m){
	int i,j;
	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			if(board[((row/m)*m)+i][((col/n)*n)+j]==val)
				return 0;
		}
	}
	return 1;
}

int isLegal2D(int ** board, int row, int col, int val, int n, int m){
	if (isLegalForRow2D(board, row, val, n*m) && isLegalForColumn2D(board, col, val, n*m) &&
			isLegalForBlock2D(board,row,col,val,n,m)){
		return 1;
	}
	return 0;
}


Stack* createStack(int n, int m){
	Stack* stack = (Stack*)malloc(sizeof(Stack));
	if(stack==NULL){
		printf("terminating due to unexpected error\n");
		exit(EXIT_FAILURE);
	}
	stack->maxSize=(n*m)*(n*m);
	stack->size=-1;
	stack->values=(int*)calloc(stack->maxSize,sizeof(int));
	stack->indices=(int*)calloc(stack->maxSize,sizeof(int));
	if(stack->indices==NULL || stack->values==NULL){
		printf("terminating due to unexpected error\n");
		exit(EXIT_FAILURE);
	}

	return stack;
}


void destroyStack(Stack* s){
	free(s->indices);
	free(s->values);
}

int BackTracking(Board* board){
	Stack* stack=createStack(board->n,board->m);
	int range=(board->m)*(board->n);
	int** gameBoard;
	int row,column;
	int solCounter=0;
	int currIndex=0;
	int maxIndex=stack->maxSize;
	int val;
	int direction;
	int firstIndex;

	gameBoard=create2DMatrix(range);
	for (row=0;row<range;row++){
		for (column=0;column<range;column++){
			gameBoard[row][column]=getNumInCell(board,'g',row,column);
		}
	}
	/* initializing first object in the stack */
	while(currIndex<maxIndex && gameBoard[currIndex/range][currIndex%range]!=0){
		currIndex++;
	}
	val=1;
	while(val<=range && isLegal2D(gameBoard,currIndex/range,currIndex%range,val,board->n,board->m)==0){
		val++;
	}
	if (val>range){
		return 0;
	}
	firstIndex=currIndex;
	gameBoard[currIndex/range][currIndex%range]=val;
	stack->size=stack->size+1;
	stack->values[stack->size]=val;
	stack->indices[stack->size]=currIndex;
	currIndex++;

	/* backtracking algorithm */
	direction=1;
	while(gameBoard[firstIndex/range][firstIndex%range]!=0){
		/* we found a solution */
		if (currIndex==maxIndex){
			solCounter++;
			currIndex=stack->indices[stack->size];
			stack->values[stack->size]=0;
			stack->indices[stack->size]=0;
			stack->size--;

		}
		if (direction==1){
			if (board->game_board[currIndex/range][currIndex%range][0]==0){ /*not empty cell*/
				currIndex++;
				continue;
			}

			val=gameBoard[currIndex/range][currIndex%range]+1;
			while((val<=range)&& isLegal2D(gameBoard,(currIndex/range),(currIndex%range),val,board->n,board->m)==0){
				val++;
			}
			if (val<=range){
				gameBoard[currIndex/range][currIndex%range]=val;
				stack->size++;
				stack->values[stack->size]=val;
				stack->indices[stack->size]=currIndex;
				currIndex++;
			} else {
				gameBoard[currIndex/range][currIndex%range]=0;
				currIndex=stack->indices[stack->size];
				direction=0;
				continue;

			}
		}
		if(direction==0){
			val=stack->values[stack->size];
			stack->values[stack->size]=0;
			stack->indices[stack->size]=0;
			stack->size--;
			direction=1;
			continue;

		}
	}
	destroyStack(stack);
	free2DMatrix(gameBoard, range);
	return solCounter;
}
