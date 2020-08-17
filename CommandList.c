/*
 * CommandList.c
 *
 *  Created on: Mar 15, 2020
 *      Author: DELL
 */
#include "CommandList.h"



CommandList* createGame(){
	CommandList* game= (CommandList*) malloc(sizeof(CommandList));
	if(game==NULL){
		printf("terminating due to unexpected error\n");
		exit(EXIT_FAILURE);
	}
	game->curr_command= NULL;
	game->head=game->curr_command;
	game->tail=game->curr_command;
	game->mode=INITMODE;
	game->mark_err=1;
	return game;
}

void  destroyGame(CommandList* game){
	clearList(game);
	free(game);
}

int solve(CommandList* game,Board* b){
	int i,ret;
	CommandNode* curr;
	Board* dup= duplicateBoard(b);
	/*clears all non-fixed cells*/
	for (i=0;i<((b->n)*(b->m))*((b->n)*(b->m));i++){
		if(!b->fixed_board[i/((b->n)*(b->m))][i%((b->n)*(b->m))]){
			clearAllValues1D(dup->game_board[i/((b->n)*(b->m))][i%((b->n)*(b->m))],(b->n)*(b->m)+1);
			dup->game_board[i/((b->n)*(b->m))][i%((b->n)*(b->m))][0]=1;
		}
	}
	free2DMatrix(dup->fixed_board,b->n*b->m);
	dup->fixed_board=NULL;
	getErrBoard(dup);
	if(!(ret=dup->if_erroneous)){
		game->mode=SOLVEMODE;
		curr= (CommandNode*)malloc(sizeof(CommandNode));
		if(curr==NULL){
			printf("terminating due to unexpected error\n");
			exit(EXIT_FAILURE);
		}
		curr->next=NULL;
		curr->prev=NULL;
		curr->after_command_board=b;
		clearList(game);
		insertCommand(game,curr);
		print_board(game);
		if(emptyCells(b)==0){
			if(!b->if_erroneous){
				printf("the puzzle was solved successfully\n");
				clearList(game);
				game->mode=INITMODE;
				game->mark_err=1;
			}
			else{
				printf("the completed puzzle is erroneous\n");
			}
		}
	}
	destroyBoard(dup);

	return ret;
}

void edit(CommandList* game, Board* b){
	CommandNode* curr;
	game->mode=EDITMODE;
	curr= (CommandNode*)malloc(sizeof(CommandNode));
	if(curr==NULL){
		printf("terminating due to unexpected error\n");
		exit(EXIT_FAILURE);
	}
	curr->next=NULL;
	curr->prev=NULL;
	/*freeing fixed board*/
	free2DMatrix(b->fixed_board,(b->n)*(b->m));
	b->fixed_board=NULL;
	getErrBoard(b);
	curr->after_command_board=b;
	clearList(game);
	insertCommand(game,curr);
	print_board(game);
}


void markErrors(CommandList* game, int x){
	game->mark_err=x;
}

void print_board(CommandList* game){
		print(game->curr_command->after_command_board,(int)game->mode, game->mark_err);
}

void set(CommandList* game, int x, int y, int z){
	CommandNode* newNode =(CommandNode*)malloc(sizeof(CommandNode));
	if(newNode==NULL){
		printf("terminating due to unexpected error\n");
		exit(EXIT_FAILURE);
	}
	newNode->next=NULL;
	newNode->prev=NULL;
	newNode->after_command_board= duplicateBoard(game->curr_command->after_command_board);
	setValue(newNode->after_command_board,y-1 ,x-1 ,z);
	insertCommand(game,newNode);
	print_board(game);
	if(emptyCells(game->curr_command->after_command_board)==0){
		if(!game->curr_command->after_command_board->if_erroneous){
			printf("the puzzle was solved successfully\n");
			clearList(game);
			game->mode=INITMODE;
			game->mark_err=1;
		}
		else{
			printf("the completed puzzle is erroneous\n");
		}
	}
}


void validate(CommandList* game){
	int ret;
	if((ret=isValideBoard(game->curr_command->after_command_board))==1){
		printf("board is solvable\n");
	}
	else if(ret==0){
		printf("board is not solvable\n");
	}
	else{
		printf("ILP execution failed\n");
	}
}

void guess(CommandList* game, float x){
	int ret;
	CommandNode* newNode =(CommandNode*)malloc(sizeof(CommandNode));
	if(newNode==NULL){
		printf("terminating due to unexpected error\n");
		exit(EXIT_FAILURE);
	}
	newNode->next=NULL;
	newNode->prev=NULL;
	newNode->after_command_board= duplicateBoard(game->curr_command->after_command_board);
	if((ret=guessSol(newNode->after_command_board,x))==1){
		insertCommand(game,newNode);
		print_board(game);
		if(emptyCells(game->curr_command->after_command_board)==0){
			if(!game->curr_command->after_command_board->if_erroneous){
				printf("the puzzle was solved successfully\n");
				clearList(game);
				game->mode=INITMODE;
				game->mark_err=1;
			}
			else{
				printf("the completed puzzle is erroneous\n");
			}
		}
	}
	else if(ret==0){
		destroyCommand(newNode);
		printf("board is not solvable\n");
	}
	else{
		destroyCommand(newNode);
		printf("LP execution failed\n");
	}

}

void generate(CommandList* game,int x, int y){
	int ret;
	CommandNode* newNode =(CommandNode*)malloc(sizeof(CommandNode));
	if(newNode==NULL){
		printf("terminating due to unexpected error\n");
		exit(EXIT_FAILURE);
	}
	newNode->next=NULL;
	newNode->prev=NULL;
	newNode->after_command_board= duplicateBoard(game->curr_command->after_command_board);
	if((ret=generateNewBoard(newNode->after_command_board,x,y))==2||ret==3){
		destroyCommand(newNode);
		printf("error in the puzzle generator\n");
	}
	else if(ret==0){
		insertCommand(game,newNode);
		print_board(game);
	}
}

void undo(CommandList* game){
	if(game->curr_command==game->head){
		printf("can not undo move - there are no previous moves\n");
	}
	else{
		game->curr_command=game->curr_command->prev;
		checkDifferencesBetweenBoards(game->curr_command->after_command_board,game->curr_command->next->after_command_board);
		print_board(game);
	}
}

void redo(CommandList* game){
	if(game->curr_command==game->tail){
		printf("can not redo move - there are no next moves\n");
	}
	else{
		game->curr_command=game->curr_command->next;
		checkDifferencesBetweenBoards(game->curr_command->after_command_board,game->curr_command->prev->after_command_board);
		print_board(game);
	}
}

void save(CommandList* game, char* path){
	int m=game->curr_command->after_command_board->m;
	int n=game->curr_command->after_command_board->n;
	int range=n*m;
	int row, column;
	int ret;
	char firstLine[4];
	int fd=open(path, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd==-1){
		printError(SAVEFILE);
		return;
	}
	firstLine[0]='0'+m; firstLine[1]=' '; firstLine[2]='0'+n; firstLine[3]='\n';
	ret=write(fd,firstLine,4);
	/* dealing with failing system call */
	if (ret==-1){
		return;
	}
	for (row=0; row<range;row++){
		char buf[256];
		int counter=0;
		for (column=0; column<range;column++){
			int valueOfCell=getNumInCell(game->curr_command->after_command_board,'g', row,column);
			if (valueOfCell==0){ /* cell is empty */
				buf[counter]='0'; counter++;
			} else { /* cell isn't empty */
				if (valueOfCell>=10){
					buf[counter]='0'+(valueOfCell/10); buf[counter+1]='0'+(valueOfCell%10);
					counter+=2;
				} else {
					buf[counter]='0'+valueOfCell;
					counter++;
				}
				if (game->mode==1){ /* we're on EDIT MODE */
					buf[counter]='.'; counter++;
				} else { /* we're on SOLVE MODE */
					if (game->curr_command->after_command_board->fixed_board[row][column]==1){
						buf[counter]='.'; counter++;
					}
				}
			}
			buf[counter]=' '; counter++;
		}
		buf[counter-1]='\n';
		if (row==range-1){
			counter--;
		}
		buf[counter]='\0';
		ret=write(fd, buf, counter);
		if (ret==-1){
			return;
		}
	}
	close(fd);
}


void hint(CommandList* game,int x, int y){
	int ret;
	if((ret=isValideBoard(game->curr_command->after_command_board))==1){
		printf("hint for cell <%2d,%2d> --> %2d\n",x,y,getNumInCell(game->curr_command->after_command_board,'s',y-1,x-1));
	}
	else if(ret==0){
		printf("board is not solvable\n");
	}
	else{
		printf("ILP execution failed\n");
	}
}

void guess_hint(CommandList* game,int x, int y){
	int i,ret;
	float score;
	int range= game->curr_command->after_command_board->m*game->curr_command->after_command_board->n+1;
	if((ret=generateGuessBoard(game->curr_command->after_command_board))==1){
		printf("possible values for cell <%2d,%2d> --> ",x,y);
		for(i=1;i<range;i++){
			score=game->curr_command->after_command_board->guess_board[y-1][x-1][i];
			if(score>0){
				printf("[%2d with score %2.2f] ",i,score);
			}
		}
		printf("\n");
	}
	else if(ret==0){
			printf("board is not solvable\n");
		}
	else{
			printf("LP execution failed\n");
		}
}

void num_solutions(CommandList* game){
	int ret=BackTracking(game->curr_command->after_command_board);
	if(ret!=0){
		printf("number of solutions --> %d\n ",ret);
	}
	else{
		printError(NOSOLUTION);
	}
}

void autofill(CommandList* game){
	int i,ret,range;
	CommandNode* newNode =(CommandNode*)malloc(sizeof(CommandNode));
	if(newNode==NULL){
		printf("terminating due to unexpected error\n");
		exit(EXIT_FAILURE);
	}
	newNode->next=NULL;
	newNode->prev=NULL;
	newNode->after_command_board= duplicateBoard(game->curr_command->after_command_board);
	range= newNode->after_command_board->m*newNode->after_command_board->n;
	for(i=0;i<range*range;i++){
		if(newNode->after_command_board->game_board[i/range][i%range][0]==1){
			if((ret=getSingleLegalValForCell(game->curr_command->after_command_board,i/range,i%range))){
				setValue(newNode->after_command_board, i/range , i%range , ret);
				printf("autofill cell <%2d,%2d> with %2d\n",(i%range)+1,(i/range)+1,ret);
			}
		}
	}
	printf("done autofill\n");
	insertCommand(game,newNode);
	print_board(game);
	if(emptyCells(game->curr_command->after_command_board)==0){
		if(!game->curr_command->after_command_board->if_erroneous){
			printf("the puzzle was solved successfully\n");
			clearList(game);
			game->mode=INITMODE;
			game->mark_err=1;
		}
		else{
			printf("the completed puzzle is erroneous\n");
		}
	}
}

void reset(CommandList* game){
	while(game->curr_command!=game->head){
		game->curr_command=game->curr_command->prev;
	}
	print_board(game);
}

void exitProg(CommandList* game){
	destroyGame(game);
	printf("exit the program.\n");
}




