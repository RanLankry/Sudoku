/*
 * Board.c
 *
 *  Created on: Mar 15, 2020
 *      Author: DELL
 */
#include "Board.h"

Board* createNewBoard(int n, int m, int*** game_board, int** fixed_board){
	Board* b = (Board*)malloc(sizeof(Board));
	if(b==NULL){
		printf("terminating due to unexpected error\n");
		exit(EXIT_FAILURE);
	}
	b->n= n;
	b->m= m;
	b->game_board= game_board;
	b->solved_board= NULL;
	b->guess_board= NULL;
	b->fixed_board=fixed_board;
	b->err_board=NULL;
	getErrBoard(b);
	return b;
}

Board* createEmptyBoard(){
	int i;
	Board* b = (Board*)malloc(sizeof(Board));
	if(b==NULL){
		printf("terminating due to unexpected error\n");
		exit(EXIT_FAILURE);
	}
	b->n= DEFUALT_SIZE_OF_EMPTY_BOARD;
	b->m= DEFUALT_SIZE_OF_EMPTY_BOARD;
	b->game_board=create3DMatrix(DEFUALT_SIZE_OF_EMPTY_BOARD*DEFUALT_SIZE_OF_EMPTY_BOARD);
	for (i=0;i<b->n*b->m*b->n*b->m;i++){
		b->game_board[i/(b->n*b->m)][i%(b->n*b->m)][0]=1;
		}
	b->solved_board=NULL;
	b->guess_board= NULL;
	b->fixed_board=create2DMatrix(DEFUALT_SIZE_OF_EMPTY_BOARD*DEFUALT_SIZE_OF_EMPTY_BOARD);
	b->err_board= create2DMatrix(DEFUALT_SIZE_OF_EMPTY_BOARD*DEFUALT_SIZE_OF_EMPTY_BOARD);
	b->if_erroneous=0;
	return b;
}

Board* duplicateBoard(Board* b){
	Board* dup = (Board*)malloc(sizeof(Board));
	if(dup==NULL){
		printf("terminating due to unexpected error\n");
		exit(EXIT_FAILURE);
	}
	dup->n=b->n;
	dup->m=b->m;
	dup->game_board=dup3DMatrix(b->game_board,dup->n*dup->m);
	dup->guess_board=dup3DFMatrix(b->guess_board,dup->n*dup->m);
	dup->solved_board=dup3DMatrix(b->solved_board,dup->n*dup->m);
	dup->fixed_board=dup2DMatrix(b->fixed_board,dup->n*dup->m);
	dup->err_board=dup2DMatrix(b->err_board,dup->n*dup->m);
	dup->if_erroneous=b->if_erroneous;
	return dup;
}

void getErrBoard( Board* b){
	int** err_b;
	int i,size=b->n*b->m,val;
	b->if_erroneous=0;
	free2DMatrix(b->err_board,b->n*b->m);
	err_b=create2DMatrix(size);
	for(i=0;i<size*size;i++){
		val= getNumInCell(b,'g',i/size,i%size);
		if(val && isErrValForCell(b,i/size,i%size,val)){
			/*if cell is fixed it can't be also erroneus*/
			if(b->fixed_board==NULL ||( b->fixed_board!=NULL && !b->fixed_board[i/size][i%size])){
				err_b[i/size][i%size]=1;
				b->if_erroneous=1;
			}
		}
	}
	b->err_board=err_b;
}


void destroyBoard(Board* b){
	free3DMatrix(b->game_board,b->n*b->m);
	free3DMatrix(b->solved_board,b->n*b->m);
	free3DFMatrix(b->guess_board,b->n*b->m);
	free2DMatrix(b->fixed_board,b->n*b->m);
	free2DMatrix(b->err_board,b->n*b->m);
	free(b);
}

void print(Board* b,int mode, int mark_errors_param){
	int j,val,len_of_separator_row=(4*b->n*b->m)+b->m+1, i=0,dimen=b->n*b->m;
	char* separator_row=(char*)calloc(len_of_separator_row+2,sizeof(char));
	if(separator_row==NULL){
		printf("terminating due to unexpected error\n");
		exit(EXIT_FAILURE);
	}
	/*creating separator row*/
	while(i<len_of_separator_row){
		strcat(separator_row, "-");
		i++;
	}
	strcat(separator_row, "\n");
	/*prints board*/
	printf("%s",separator_row);
	for(i=0;i<dimen;i++){
		for(j=0;j<dimen;j++){
			if((j%b->n==0))
				printf("|");
			val=getNumInCell(b,'g',i,j);
			if(val)
				printf(" %2d",val);
			else
				printf("   ");
			if(b->err_board[i][j]){
				if(mode==1 ||(mode==2 && mark_errors_param))
						printf("*");
				else{
					printf(" ");
				}
			}
			else{
				if(mode==2 && b->fixed_board[i][j])
					printf(".");
				else
					printf(" ");
				}
			}
		printf("|\n");
		if((i+1)%b->m==0){
			printf("%s",separator_row);
		}
	}
	free(separator_row);

}



void setValue(Board* b, int row , int col , int z){
	int before=getNumInCell(b,'g',row,col);
	if (z!= before){
		b->game_board[row][col][before]=0;
		b->game_board[row][col][z]=1;
		free3DMatrix(b->solved_board,b->n*b->m);
		free3DFMatrix(b->guess_board,b->n*b->m);
		b->solved_board=NULL;
		b->guess_board=NULL;
		if(z!=0 || b->if_erroneous){
			getErrBoard(b);
		}
	}
}

int isErrValForCell(Board* b, int row , int col , int val){
	return (!isLegalForRow(b->game_board,row,col,val,b->n*b->m) || !isLegalForColumn(b->game_board,row,col,val,b->n*b->m)|| !isLegalForBlock(b->game_board,row,col,val,b->n,b->m));
}

int emptyCells(Board* board){
	int cnt=0;
	int row, column;
	int range=board->m*board->n;
	for (row=0;row<range;row++){
		for (column=0;column<range;column++){
			if (board->game_board[row][column][0]==1){
				cnt++;
			}
		}
	}
	return cnt;
}


void checkDifferencesBetweenBoards(Board* curr_b,Board* before_b){
	int i,curr,prev,counter=0,size=curr_b->n*curr_b->m;
	for(i=0;i<size*size;i++){
		if((curr=getNumInCell(curr_b,'g',i/size,i%size))!= (prev=getNumInCell(before_b,'g',i/size,i%size))){
			printf("cell <%2d,%2d> was changed from %2d to %2d \n",i%size+1,i/size+1, prev,curr);
			counter++;
		}
	}
	if(!counter){
		printf("board was not changed");
	}
}

int getNumInCell(Board* b,char game_or_solve_b, int row, int column){
	int i=0;
	int*** curr;
	if(game_or_solve_b=='g'){
		curr=b->game_board;
	}
	else{
		curr=b->solved_board;
	}
	if(curr!=NULL){
		while(i<(b->n*b->m)+1 && (!curr[row][column][i]))
			i++;
		return i;
	}
	return -1;

}

int getSingleLegalValForCell(Board* b, int row, int column){
	int i,ret=0;
	int range=(b->n*b->m)+1;
	for(i=1;i<range;i++){
		if(!isErrValForCell(b,row,column,i)){
			if(ret>0)
				return 0;
			ret=i;
		}
	}
	return ret;
}


void clearBoard(Board* b){
	int i,j,k;
	for(i=0;i<b->n*b->m;i++){
		for(j=0;j<b->n*b->m;j++){
			b->game_board[i][j][0]=1;
			for(k=1;k<b->n*b->m+1;k++){
				b->game_board[i][j][k]=0;
			}
		}
	}
}

