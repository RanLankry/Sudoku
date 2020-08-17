/*
 * AuxParser.c
 *
 *  Created on: Mar 15, 2020
 *      Author: DELL
 */
#include "AuxParser.h"


void printError(ERROR err){
	if (err==COMMANDLENGTH){
		printf("command length is too long\n");
	} else if (err==COMMANDNAME){
		printf("command name is not correct, please try again\n");
	} else if (err==MODE){
		printf("the command is not valid for the current MODE\n");
	} else if (err==MANYARGUMENTS){
		printf("too many arguments\n");
	}else if (err==FEWARGUMENTS){
		printf("too few arguments\n");
	} else if (err==PARAMETERSVALID){
		printf("parameters are not valid for the command\n");
	} else if (err==RANGE){
		printf("parameters are out of range\n");
	} else if (err==ERRONEOUS){
		printf("board is erroneous and not valid for the command\n");
	} else if (err==EMPTYCELLS){
		printf("there aren't enough empty cells for performing the command\n");
	} else if (err==NOSOLUTION){
		printf("command is not valid because there is no solution for the current board\n");
	} else if (err==FIXEDCELL){
		printf ("command isn't legal because cell is already fixed\n");
	} else if (err==NOTEMPTY){
		printf ("command is not legal because cell isn't empty\n");
	} else if (err==SAVEFILE){
		printf("saving the board was failed due to invalid filename\n");
	}
}

void currCommands(int mode){
	if (mode==0){
		printf("the commands which are available for the current MODE are: solve | edit | exit\n");
	} else if (mode==1){
		printf("the commands which are available for the current MODE are: solve | edit | print_board | set | validate | generate | "
				"undo | redo | save | num_solutions | reset | exit\n");
	} else {
		printf("the commands which are available for the current MODE are: solve | edit | mark_errors | print_board | "
				"set | validate | guess | undo | redo | save | hint | guess_hint | num_solutions | autofill | reset | exit\n");

	}
}

int getInput(char buf[], char finalbuf[]){
	int i;
	int tmpcounter=0;
	int finalcounter=0;
	int index=0;
	int curr=0;

	while(1){
		char c=fgetc(stdin);
		if(feof(stdin)){
			return 1;
		}
		if (c==10){
			break;
		}
		if (tmpcounter==MAX_SIZE){
			printError(COMMANDLENGTH);
			fflush(stdin);
			break;
		}
		buf[tmpcounter]=c;
		tmpcounter++;
	}
	/* finishing getting input, now we will normalize the start of the array */
	while(buf[curr]==32 || buf[curr]==9){
		curr++;
	}
	/* arranging the body of the buffer */
	while(curr<tmpcounter){
		while(curr<tmpcounter && buf[curr]!=32 && buf[curr]!=9){
			buf[index]=buf[curr];
			curr++; index++;
		}
		while(curr<tmpcounter && (buf[curr]==32 || buf[curr]==9)){
			curr++;
		}
		buf[index]=' ';
		index++;
	}
	/* a fix at the end and copying the buffer to the final buffer */
	finalcounter=index-1;
	for (i=0;i<finalcounter;i++){
		finalbuf[i]=buf[i];
	}
	return 0;
}

int isInteger(char* p){
	while (p[0]!='\0'){
		if (p[0]<'0' || p[0]>'9'){
			return 0;
		}
		p++;
	}
	return 1;
}

int isIntegerOrFloat(char* p){
	int point=0;
	while(p[0]!='\0'){
		if (((p[0]<'0') || p[0]>'9') && (p[0]!='.')){
			return 0;
		}
		if (p[0]=='.'){
			if (point==1){
				return 0;
			}
			point++;
		}
		p++;
	}
	return 1;
}

Board* loadFromFile(char* path){
	Board* board;
	int*** game_board;
	int** fixed_board;
	int fd;
	int ret;
	int counter=0;
	int range;
	int point;
	int m=-1;
	int n=-1;
	char c;

	fd=open(path, O_RDONLY);
	if (fd==-1){
		printf("file opening failed, please try again\n");
		return NULL;
	}

	/* finding m*/
	while((ret=read(fd,&c,1))!=0){
		if (ret==-1){
			printf("file reading failed\n");
			return NULL;
		}

		if(c==' ' || c=='\n' || c=='\t'){
			continue;
		}
		if (c<'1' || c>'5'){
			break;
		} else {
			m=c-'0';
			ret=read(fd,&c,1);
			if(ret==-1){
				printf("file reading failed\n");
				return NULL;
			}
			if (ret==0 || (c!=' ' && c!='\t' && c!='\n')){
				m=-1; break;
			} else{
				break;
			}
		}
	}
	if (m==-1){
		printf("invalid input for number of rows in a block - should be between 1 to 5\n");
		close(fd);
		return NULL;
	}
	/* finding n*/
	while((ret=read(fd,&c,1))!=0){
		if(ret==-1){
			printf("file reading failed\n");
			return NULL;
		}
		if(c==' ' || c=='\n' || c=='\t'){
			continue;
		}
		if (c<'1' || c>'5'){
			break;
		} else {
			n=c-'0';
			ret=read(fd,&c,1);
			if(ret==-1){
				printf("file reading failed\n");
				return NULL;
			}
			if (ret==0 || (c!=' ' && c!='\t' && c!='\n')){
				n=-1; break;
			} else{
				break;
			}
		}
	}
	if (n==-1){
		printf("invalid input for number of columns in a block - should be between 1 to 5\n");
		close(fd);
		return NULL;
	}

	/* finding (n*m)*(n*m) inputs */
	range=(m*n)*(m*n);

	/* initializing fixed board */
	fixed_board=create2DMatrix(m*n);
	/* initializing game board */
	game_board=create3DMatrix(n*m);

	/* we've found n,m, now we'll make sure the rest of inputs is valid and its amount fits the board size */
	while((ret=read(fd,&c,1))!=0){
		int value;
		if (ret==-1){
			free3DMatrix(game_board, n*m);
			free2DMatrix(fixed_board, n*m);
			printf("file reading failed\n");
			close(fd);
			return NULL;
		}
		if(c==' ' || c=='\n' || c=='\t'){
			continue;
		}
		if (counter==range){ /* we've seen already enough inputs, but we identify that there more - it's invalid */
			free3DMatrix(game_board, n*m);
			free2DMatrix(fixed_board, n*m);
			printf("too many inputs were given in the file\n");
			close(fd);
			return NULL;
		}
		if (c<'0' || c>'9'){ /* next input isn't an integer, therefore board is not valid */
			free3DMatrix(game_board, n*m);
			free2DMatrix(fixed_board, n*m);
			printf("input should include positive integers only or positive integers followed by a dot\n");
			close(fd);
			return NULL;
		} else {
			value=c-'0';
			point=0;
			while((ret=read(fd, &c, 1))!=0 && c!=' ' && c!='\n' && c!='\t'){ /* we'll check if input >=10 or there is a '.' */
				if (ret==-1){
					free3DMatrix(game_board, n*m);
					free2DMatrix(fixed_board, n*m);
					printf("file reading failed\n");
					close(fd);
					return NULL;
				}
				if ((c<'0' || c>'9')&& c!='.'){ /* next char isn't integer or a dot, therefore board is not valid */
					free3DMatrix(game_board, n*m);
					free2DMatrix(fixed_board, n*m);
					printf("input should include positive integers only or positive integers followed by a dot\n");
					close(fd);
					return NULL;
				}
				else if (c=='.' && point==1){ /* we already dealt with a dot */
					free3DMatrix(game_board, n*m);
					free2DMatrix(fixed_board, n*m);
					printf("2 points in a row are invalid\n");
					close(fd);
					return NULL;
				}
				else if(c=='.' && point==0){ /* first time that we see a dot */
					point++;
					if (value!=0){ /*  we don't want to mark empty cell as fixed */
						fixed_board[counter/(m*n)][counter%(m*n)]=1;
					}
				} else {
					if (point==1){
						free3DMatrix(game_board, n*m);
						free2DMatrix(fixed_board, n*m);
						printf("input should include positive integers only or positive integers followed by a dot\n");
						close(fd);
						return NULL;
					} else { /* integer is >= 10 */
						value=value*10;
						value+=c-'0';
					}
				}
			}
		}
			if (value>(m*n)){ /* we've exceeded the range */
				free3DMatrix(game_board, n*m);
				free2DMatrix(fixed_board, n*m);
				printf("input values cannot exceed the range %d\n",m*n);
				close(fd);
				return NULL;
			}
			game_board[counter/(m*n)][counter%(m*n)][value]=1;
			counter++;
		}
	/* arrived to EOF without failing. the number of inputs it's legal or it's lower than required */
	if (counter==range){ /* input fits exactly! */
		board=createNewBoard(n,m,game_board,fixed_board);
		close(fd);
		return board;
	}
	/* otherwise there are fewer or more inputs than required */
	free3DMatrix(game_board, n*m);
	free2DMatrix(fixed_board, n*m);
	printf("too few inputs were given in the file\n");
	close(fd);
	return NULL;
}

int checkSolve (int len){
	if (len>2){
		printError(MANYARGUMENTS);
		printf("solve command should include exactly 1 more parameter which is a file name\n");
		return 0;
	}
	if (len<2){
		printError(FEWARGUMENTS);
		printf("solve command should include exactly 1 more parameter which is a file name\n");
		return 0;
	}
	return 1;
}
int checkEdit (int len){
	if (len>2){
		printError(MANYARGUMENTS);
		printf("edit command should include 1 more parameter which is a file name, or no parameters for a empty 9x9 board\n");
		return 0;
	}
	if (len==2){
		return 2;
	}
	return 1;
}
int checkMarkErrors (char* command[], int len, int mode){
	int x;
	if (mode!=2){
		printError(MODE);
		currCommands(mode);
		return 0;
	}
	if (len<2){
		printError(FEWARGUMENTS);
		printf("mark_error command should contain exactly 1 extra parameter which is '1' for displaying error values or '0' for ignoring them\n");
		return 0;
	}
	if (len>2){
		printError(MANYARGUMENTS);
		printf("mark_error command should contain exactly 1 extra parameter which is '1' for displaying error values or '0' for ignoring them\n");
		return 0;
	}
	if (isInteger(command[1])==0){
		printError(PARAMETERSVALID);
		printf("extra parameter should be '0' or '1' only\n");
		return 0;
	}
	x=atoi(command[1]);
	if (x<0 || x>1){
		printError(RANGE);
		printf("extra parameter should be '0' or '1' only\n");
		return 0;
	}
	return 1;
}
int checkPrintBoard (int len ,int mode){
	if (mode==0){
		printError(MODE);
		currCommands(mode);
		return 0;
	}
	if (len>1){
		printError(MANYARGUMENTS);
		printf("print_board command shouldn't contain any extra parameters\n");
		return 0;
	}
	return 1;
}
int checkValidate (int len, int err){

	if (len>1){
		printError(MANYARGUMENTS);
		printf("validate command shouldn't contain any extra parameters\n");
		return 0;
	}
	if (err==1){
		printError(ERRONEOUS);
		return 0;
	}
	return 1;
}
int checkSet (char* command[], int len, int n, int m){
	int x,y,z;
	int range=n*m;

	if (len<4){
		printError(FEWARGUMENTS);
		printf("set command should contain exactly 3 extra parameters X Y Z such that:\nX - column index\n"
				"Y - row index\nZ - desired value\n");
		return 0;
	}
	if (len>4){
		printError(MANYARGUMENTS);
		printf("set command should contain exactly 3 extra parameters X Y Z such that:\nX - column index\n"
				"Y - row index\nZ - desired value\n");
		return 0;
	}
	if (isInteger(command[1])==0 || isInteger(command[2])==0 || isInteger(command[3])==0){
		printError(PARAMETERSVALID);
		printf("1 or more parameters aren't integers\n");
		return 0;
	}
	x=atoi(command[1]); y=atoi(command[2]); z=atoi(command[3]);

	if (x<1 || x>range || y<1 || y>range || z<0 || z>range){
		printError(RANGE);
		printf("1st and 2nd parameters should be integers between %d to %d, 3rd parameters should be integer between %d to %d\n",
				1,range, 0,range);
		return 0;
	}
	return 1;
}
int checkGuess (char* command[], int len, int err){
	float param;

	if (len<2){
		printError(FEWARGUMENTS);
		printf("guess command should contain exactly 1 extra parameter which is a float representing the threshold\n");
		return 0;
	}
	if (len>2){
		printError(MANYARGUMENTS);
		printf("guess command should contain exactly 1 extra parameter which is a float representing the threshold\n");
		return 0;
	}
	if(isIntegerOrFloat(command[1])==0){
		printError(PARAMETERSVALID);
		printf("the extra parameter should be a float number between 0 to 1\n");
		return 0;
	}
	param=atof(command[1]);

	if(param<0 || param>1){
		printError(RANGE);
		printf("the extra parameter should be a float number between 0 to 1\n");
		return 0;
	}
	if (err==1){
		printError(ERRONEOUS);
		return 0;
	}
	return 1;
}
int checkGenerate (char* command[], int len, int m, int n, int err){
	int x,y;
	int range=(m*n)*(m*n);

	if (len<3){
		printError(FEWARGUMENTS);
		printf("generate command should contain exactly 2 extra parameters X Y such that:\n"
				"X - amount of cells to fill\nY - amount of cells to clear\n");
		return 0;
	}
	if (len>3){
		printError(MANYARGUMENTS);
		printf("generate command should contain exactly 2 extra parameters X Y such that:\n"
						"X - amount of cells to fill\nY - amount of cells to clear\n");
		return 0;
	}
	if (isInteger(command[1])==0 ||isInteger(command[2])==0){
		printError(PARAMETERSVALID);
		printf("1 or more parameters are not integers\n");
		return 0;
	}
	x=atoi(command[1]); y=atoi(command[2]);

	if (x<0 || x>range || y<1 || y>range){
		printError(RANGE);
		printf("1st parameter should be an integer between 0 to %d, 2nd parameter should be an integer"
				"between 1 to %d\n",range,range);
		return 0;
	}
	if (err==1){
		printError(ERRONEOUS);
		return 0;
	}

	return 1;
}
int checkUndo (int len ,int mode){
	if (mode==0){
		printError(MODE);
		currCommands(mode);
		return 0;
	}
	if (len>1){
		printError(MANYARGUMENTS);
		printf("undo command shouldn't contain any extra parameters\n");
		return 0;
	}
	return 1;
}
int checkRedo (int len ,int mode){
	if (mode==0){
		printError(MODE);
		currCommands(mode);
		return 0;
	}
	if (len>1){
		printError(MANYARGUMENTS);
		printf("redo command shouldn't contain any extra parameters\n");
		return 0;
	}
	return 1;
}
int checkSave (int len, int mode, int err){

	if (len<2){
		printError(FEWARGUMENTS);
		printf("save command should contain exactly 1 extra parameter which is the target file name\n");
		return 0;
	}
	if (len>2){
		printError(MANYARGUMENTS);
		printf("save command should contain exactly 1 extra parameter which is the target file name\n");
		return 0;
	}
	if (mode==1 && err==1){
		printError(ERRONEOUS);
		return 0;
	}
	return 1;
}
int checkHint(char* command[], int len, int err, int n, int m){
	int x,y;
	int range=n*m;

	if (len<3){
		printError(FEWARGUMENTS);
		printf("hint command should contain exactly 2 extra parameters X Y such that:\n"
				"X - column index\nY - row index\n");
		return 0;
	}
	if (len>3){
		printError(MANYARGUMENTS);
		printf("hint command should contain exactly 2 extra parameters X Y such that:\n"
				"X - column index\nY - row index\n");
		return 0;
	}
	if (isInteger(command[1])==0 || isInteger(command[2])==0){
		printError(PARAMETERSVALID);
		printf("1 or more parameters aren't integers\n");
		return 0;
	}
	x=atoi(command[1]); y=atoi(command[2]);

	if (x<1 || y<1 || x>range || y>range){
		printError(RANGE);
		printf("both parameters should be integers between 1 to %d\n",range);
		return 0;
	}
	if (err==1){
		printError(ERRONEOUS);
		return 0;
	}
	return 1;
}
int checkGuessHint (char* command[], int len, int err, int n, int m){
	int x,y;
	int range=n*m;

	if (len<3){
		printError(FEWARGUMENTS);
		printf("guess_hint command should contain exactly 2 extra parameters X Y such that:\n"
				"X - column index\nY - row index\n");
		return 0;
	}
	if (len>3){
		printError(MANYARGUMENTS);
		printf("guess_hint command should contain exactly 2 extra parameters X Y such that:\n"
				"X - column index\nY - row index\n");
		return 0;
	}
	if (isInteger(command[1])==0 || isInteger(command[2])==0){
		printError(PARAMETERSVALID);
		printf("1 or more parameters aren't integers\n");
		return 0;
	}
	x=atoi(command[1]); y=atoi(command[2]);

	if (x<1 || y<1 || x>range || y>range){
		printError(RANGE);
		printf("both parameters should be integers between 1 to %d\n",range);
		return 0;
	}
	if (err==1){
		printError(ERRONEOUS);
		return 0;
	}
	return 1;
}
int checkNumSolutions (int len, int err){
	if (len>1){
		printError(MANYARGUMENTS);
		printf("num_solutions command shouldn't contain any extra parameters\n");
		return 0;
	}
	if (err==1){
		printError(ERRONEOUS);
		return 0;
	}
	return 1;
}
int checkAutofill (int len, int err){
	if (len>1){
		printError(MANYARGUMENTS);
		printf("autofill command shouldn't contain any extra parameters\n");
		return 0;
	}
	if (err==1){
		printError(ERRONEOUS);
		return 0;
	}
	return 1;
}
int checkReset(int len, int mode){
	if (mode==0){
		printError(MODE);
		currCommands(mode);
		return 0;
	}
	if (len>1){
		printError(MANYARGUMENTS);
		printf("reset command shouldn't contain any extra parameters\n");
		return 0;
	}
	return 1;
}
int checkExit (int len){
	if (len>1){
		printError(MANYARGUMENTS);
		printf("exit command shouldn't contain any extra parameters\n");
		return 0;
	}
	return 1;
}
