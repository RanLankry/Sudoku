/*
 * LP.c
 *
 *  Created on: Apr 6, 2020
 *      Author: DELL
 */


#include "LP.h"


int isValideBoard(Board* b){
	int ret;
	if(b->solved_board!=NULL){
		return 1;
	}
	if((ret=LP(b,1))==2){
		return 1;
	}
	else if(ret==1){
		return 0;
	}
	return 2;
}

int generateGuessBoard(Board* b){
	int ret;
	if(b->guess_board!=NULL){
		return 1;
	}
	if((ret=LP(b,0))==2){
		return 1;
	}
	else if(ret==1){
		return 0;
	}
	return 2;
}


int guessSol(Board* b, float x){
	int ret,i,j,k,counter,val,bool_ch;
	float*** temp;
	float* cell_vals=(float*)calloc(b->n*b->m+1,sizeof(float));
	if(cell_vals==NULL){
		printf("terminating due to unexpected error\n");
		exit(EXIT_FAILURE);
	}
	bool_ch=0;
	if((ret=generateGuessBoard(b))!=1)
		return ret;
	temp=dup3DFMatrix(b->guess_board,b->n*b->m);
	for(i=0;i<b->n*b->m;i++){
		for(j=0;j<b->n*b->m;j++){
			if(b->game_board[i][j][0]){
				clearAllValues1D((int*)cell_vals,b->n*b->m+1);
				val=0;
				counter=0;
				for(k=1;k<b->n*b->m+1;k++){
					if(!isErrValForCell(b,i,j,k) && temp[i][j][k]>=x){
						cell_vals[k]=temp[i][j][k];
						counter++;
						val=k;
					}
				}
				if(counter==1){ /*there is only one legal and above threshold value*/
					setValue(b,i,j,val);
					bool_ch=1;
				}
				else if(counter>1){/*there are more than one legal and above threshold value*/
					setValue(b,i,j,chooseValueWithRand(cell_vals,b->n*b->m+1));
					bool_ch=1;
				}
			}
		}
	}
	free(cell_vals);
	if(bool_ch){
		b->guess_board=temp;
	}
	else{
		free3DFMatrix(temp,b->n*b->m);
	}
	return 1;
}


int generateNewBoard(Board* b,int x,int y){
	int round,ret;
	int*** game_b_temp= dup3DMatrix(b->game_board,b->n*b->m);
	round=0;
	while(round<1000){
		if(!fillXRandomly( b,x)){
			free3DMatrix(b->game_board,b->n*b->m);
			b->game_board=dup3DMatrix(game_b_temp,b->n*b->m);
			round++;
			continue;
		}
		if((ret=isValideBoard(b))==1){ /*there is a solution*/
			fillYRandomly( b,y);
			free3DMatrix(game_b_temp,b->n*b->m);
			return 0;
		}
		else if(ret==2){ /*LP failed*/
			free3DMatrix(b->game_board,b->n*b->m);
			b->game_board=game_b_temp;
			return 3;
		}
		/*there is no solution*/
		free3DMatrix(b->game_board,b->n*b->m);
		b->game_board=dup3DMatrix(game_b_temp,b->n*b->m);
		round++;

	}
	return 2;
}


int LP(Board* b, int ILP){
	int N,num_of_vars,optimstatus,error=0;
	int*** signed_vars_for_LP;
	int* ind;
	double* sol,*val,*obj;
	char* vtype;
	GRBenv   *env   = NULL;
	GRBmodel *model = NULL;


	N=b->n*b->m;
	num_of_vars=0;
	signed_vars_for_LP= create3DMatrix(N);

	/*filling cells in signed_vars_for_LP with the index of them in the obj func*/
	num_of_vars= sign_leagal_vars(b,signed_vars_for_LP);

	/*there are no legal vars in game_board or there is an empty cell with no legal vars*/
	if(num_of_vars==-1){
		update_for_err(b, signed_vars_for_LP);
		return 1;
	}

	/*allocating memory for GRB params*/
	ind=(int*)calloc(N,sizeof(int));
	sol=(double*)calloc(num_of_vars,sizeof(double));
	val=(double*)calloc(N,sizeof(double));
	obj=(double*)calloc(num_of_vars,sizeof(double));
	vtype=(char*)calloc(num_of_vars,sizeof(char));
	if(ind==NULL || sol==NULL || val==NULL || obj==NULL || vtype==NULL){
		printf("terminating due to unexpected error\n");
		exit(EXIT_FAILURE);
	}


	/* Create environment - log file is LP.log */
	error = GRBloadenv(&env, "LP.log");
	if (error) {
		printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
		free_GRB_params(ind,sol,val,obj,vtype,env,model);
		update_for_err(b, signed_vars_for_LP);
		return 0;
	  }


	error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
	if (error) {
		printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		free_GRB_params(ind,sol,val,obj,vtype,env,model);
		update_for_err(b, signed_vars_for_LP);
		return 0;
	}

	/* Create an empty model named "LP" */
	error = GRBnewmodel(env, &model, "LP", 0, NULL, NULL, NULL, NULL, NULL);
	if (error) {
		printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
		free_GRB_params(ind,sol,val,obj,vtype,env,model);
		update_for_err(b, signed_vars_for_LP);
		return 0;
	}

	/* Add variables */

	/* coefficients  */
	if(ILP){
		setCoefsTo1(obj,num_of_vars);
		setVtypeToBin(vtype,num_of_vars);
	}
	else{
		setCoefsForLP(signed_vars_for_LP,b->n,b->m,obj);
		setVtypeToCont(vtype,num_of_vars);
	}

	/* add variables to model */
	error = GRBaddvars(model, num_of_vars, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype, NULL);
	if (error) {
		printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
		free_GRB_params(ind,sol,val,obj,vtype,env,model);
		update_for_err(b, signed_vars_for_LP);
		return 0;
	}

	/* Change objective sense to minimization */
	error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MINIMIZE);
	if (error) {
		  printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		  free_GRB_params(ind,sol,val,obj,vtype,env,model);
		  update_for_err(b, signed_vars_for_LP);
		  return 0;
	}


	/* update the model - to integrate new variables */

	error = GRBupdatemodel(model);
	if (error) {
		  printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
		  free_GRB_params(ind,sol,val,obj,vtype,env,model);
		  update_for_err(b, signed_vars_for_LP);
		  return 0;
	}

	/* Add constraint */

	error= AddCellConstraints(model,b->game_board,signed_vars_for_LP,ind,val,N);
	if (error) {
		free_GRB_params(ind,sol,val,obj,vtype,env,model);
		update_for_err(b, signed_vars_for_LP);
		return 0;
	}
	error= AddRowConstraints(model,b->game_board,signed_vars_for_LP,ind,val,N);
	if (error) {
		free_GRB_params(ind,sol,val,obj,vtype,env,model);
		update_for_err(b, signed_vars_for_LP);
		return 0;
	}
	error= AddColConstraints(model,b->game_board,signed_vars_for_LP,ind,val,N);
	if (error) {
		free_GRB_params(ind,sol,val,obj,vtype,env,model);
		update_for_err(b, signed_vars_for_LP);
		return 0;
	}
	error= AddBlockConstraints(model,signed_vars_for_LP,ind,val,b->n,b->m);
	if (error) {
		free_GRB_params(ind,sol,val,obj,vtype,env,model);
		update_for_err(b, signed_vars_for_LP);
		return 0;
	}

	if(!ILP){
		error= AddPositiveConstraints(model,b->game_board,signed_vars_for_LP,ind,val,b->n*b->m);
		if (error) {
			free_GRB_params(ind,sol,val,obj,vtype,env,model);
			update_for_err(b, signed_vars_for_LP);
			return 0;
		}
	}


	/* Optimize model  */
	error = GRBoptimize(model);
	if (error) {
		printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
		free_GRB_params(ind,sol,val,obj,vtype,env,model);
		update_for_err(b, signed_vars_for_LP);
		return 0;
	  }

	/* Write model to 'LP.lp' */
	error = GRBwrite(model, "LP.lp");
	if (error) {
		printf("ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
		free_GRB_params(ind,sol,val,obj,vtype,env,model);
		update_for_err(b, signed_vars_for_LP);
		return 0;
	}

	/* Get solution information */

	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error) {
		  printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
		  free_GRB_params(ind,sol,val,obj,vtype,env,model);
		  update_for_err(b, signed_vars_for_LP);
		  return 0;
	}

	printf("\nOptimization complete\n");

	/* solution found */
	if (optimstatus == GRB_OPTIMAL) {
		/* get the solution - the assignment to each variable */
		error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, num_of_vars, sol);
		if (error) {
			printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
			free_GRB_params(ind,sol,val,obj,vtype,env,model);
			update_for_err(b, signed_vars_for_LP);
			return 0;
		}
		update_for_success(b,signed_vars_for_LP,sol,ILP);
		free_GRB_params(ind,sol,val,obj,vtype,env,model);
		return 2;
	}

	/* no solution found */
	else if (optimstatus == GRB_INF_OR_UNBD || optimstatus == GRB_INFEASIBLE || optimstatus == GRB_UNBOUNDED) {
		update_for_err(b, signed_vars_for_LP);
		free_GRB_params(ind,sol,val,obj,vtype,env,model);
		return 1;
	}
	/* error or calculation stopped */
	else{
		printf("Optimization was stopped early\n");
		update_for_err(b, signed_vars_for_LP);
		free_GRB_params(ind,sol,val,obj,vtype,env,model);
		return 0;
	}



}

int sign_leagal_vars(Board* board,int *** sign_board){
	int i,j,k,N,can_fill,counter=0;
	N=board->n*board->m;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			sign_board[i][j][0]=-1;
			if(board->game_board[i][j][0]){ /*if cell is empty*/
				can_fill=0;
				for(k=1;k<N+1;k++){
					if(isErrValForCell(board,i ,j ,k)){
						sign_board[i][j][k]=-1;
					}
					else{
						sign_board[i][j][k]=counter;
						can_fill=1;
						counter++;
					}
				}
				if(!can_fill)/*there is no legal val for the cell*/
					return -1;
			}
			else{ /*cell is not empty*/
				for(k=1;k<N+1;k++){
					sign_board[i][j][k]=-1;
				}

			}

		}
	}
	return counter;
}

void free_GRB_params(int *ind, double* sol, double* val, double* obj, char* vtype,GRBenv* env, GRBmodel* model){
	free(ind);
	free(sol);
	free(val);
	free(obj);
	free(vtype);
	free(env);
	free(model);
}

void update_for_err(Board* b,int*** signed_vars_for_LP){
	free3DFMatrix(b->guess_board,b->n*b->m);
	free3DMatrix(b->solved_board,b->n*b->m);
	b->solved_board=NULL;
	b->guess_board=NULL;
	free3DMatrix(signed_vars_for_LP,b->n*b->m);
}


void setCoefsTo1(double * obj,int num_of_vars){
	int i;
	for(i=0;i<num_of_vars;i++){
		obj[i]=1;
	}
}

void setCoefsForLP(int *** signed_vars_for_LP, int n, int m,double * obj){
	int i,j,k,v;
	for(i=0;i<n*m;i++){
		for(j=0;j<n*m;j++){
			for(k=1;k<n*m+1;k++){
				if((v=signed_vars_for_LP[i][j][k])!=-1){
					obj[v]=1+(rand()%(n*m));
				}
			}
		}
	}
}


void setVtypeToBin(char * vtype,int num_of_vars){
	int i;
	for(i=0;i<num_of_vars;i++){
		vtype[i]=GRB_BINARY;
	}
}

void setVtypeToCont(char * vtype,int num_of_vars){
	int i;
	for(i=0;i<num_of_vars;i++){
		vtype[i]=GRB_CONTINUOUS;
	}
}

int AddCellConstraints(GRBmodel* model,int*** game_board,int*** signed_vars_for_LP,int* ind,double* val,int N){
	int i,j,k,vars_in_const;
	char const_name[50];
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			const_name[0]='\0';
			vars_in_const=0;
			if(!game_board[i][j][0])/*if cell is not empty - continue*/
				continue;
			for(k=1;k<N+1;k++){ /*if cell is empty*/
				if(signed_vars_for_LP[i][j][k]!=-1){ /*if [i][j][k] is legal variable*/
					ind[vars_in_const]=signed_vars_for_LP[i][j][k];
					val[vars_in_const]=1;
					vars_in_const++;
				}
			}
			if(sprintf(const_name,"Cell [%2d][%2d] constraint\n",i,j)<0){
				return 1;
			}
			if(GRBaddconstr(model, vars_in_const, ind, val, GRB_EQUAL, 1.0, const_name)){
				return 1;
			}
		}
	}
	return 0;

}

int AddRowConstraints(GRBmodel* model,int*** game_board,int*** signed_vars_for_LP,int* ind,double* val,int N){
	int i,j,k,vars_in_const;
	char const_name[50];
	for(i=0;i<N;i++){
		for(k=1;k<N+1;k++){
			const_name[0]='\0';
			vars_in_const=0;
			for(j=0;j<N;j++){
				if(!game_board[i][j][0])/*if cell is not empty - continue*/
					continue;
				if(signed_vars_for_LP[i][j][k]!=-1){
					ind[vars_in_const]=signed_vars_for_LP[i][j][k];
					val[vars_in_const]=1;
					vars_in_const++;
				}
			}
			if(!vars_in_const)/*if there are no legal variable for constraint - continue*/
				continue;
			if(sprintf(const_name,"value %2d for row %2d constraint\n",k,i)<0){
				return 1;
			}

			if(GRBaddconstr(model, vars_in_const, ind, val, GRB_EQUAL, 1.0, const_name)){
				return 1;
			}
		}
	}
	return 0;
}

int AddColConstraints(GRBmodel* model,int*** game_board,int*** signed_vars_for_LP,int* ind,double* val,int N){
	int i,j,k,vars_in_const;
	char const_name[50];
	for(j=0;j<N;j++){
		for(k=1;k<N+1;k++){
			const_name[0]='\0';
			vars_in_const=0;
			for(i=0;i<N;i++){
				if(!game_board[i][j][0])/*if cell is not empty - continue*/
					continue;
				if(signed_vars_for_LP[i][j][k]!=-1){
					ind[vars_in_const]=signed_vars_for_LP[i][j][k];
					val[vars_in_const]=1;
					vars_in_const++;
				}
			}
			if(!vars_in_const)/*if there are no legal variable for constraint - continue*/
				continue;
			if(sprintf(const_name,"value %2d for column %2d constraint\n",k,j)<0){
				return 1;
			}
			if(GRBaddconstr(model, vars_in_const, ind, val, GRB_EQUAL, 1.0, const_name)){
				return 1;
			}
		}
	}
	return 0;
}

int AddBlockConstraints(GRBmodel* model,int*** signed_vars_for_LP,int* ind,double* val,int n,int m){
	int i,j,k,l,v,vars_in_const;
	char const_name[50];
	for(i=0;i<n;i++){
		for(j=0;j<m;j++){
			for(v=1;v<n*m+1;v++){
				vars_in_const=0;
				const_name[0]='\0';
				for(k=0;k<m;k++){
					for(l=0;l<n;l++){
						if(signed_vars_for_LP[i*m+k][j*n+l][v]!=-1){
							ind[vars_in_const]=signed_vars_for_LP[i*m+k][j*n+l][v];
							val[vars_in_const]=1;
							vars_in_const++;
						}
					}
				}
				if(!vars_in_const)
					continue;
				if(sprintf(const_name,"value %2d for Block <%2d,%2d> constraint\n",v,i,j)<0){
					return 1;
				}
				if(GRBaddconstr(model, vars_in_const, ind, val, GRB_EQUAL, 1.0, const_name)){
					return 1;
				}
			}
		}
	}
	return 0;
}

int AddPositiveConstraints(GRBmodel* model,int*** game_board,int*** signed_vars_for_LP,int* ind,double* val,int N){
		int i,j,k;
		char const_name[100];
		for( i=0;i<N;i++){
			for(j=0;j<N;j++){
				if(!game_board[i][j][0])
					continue;
				for(k=1;k<N+1;k++){
					const_name[0]='\0';
					if(signed_vars_for_LP[i][j][k]!=-1){
						ind[0]=signed_vars_for_LP[i][j][k];
						val[0]=1;
						if(sprintf(const_name,"non negative for value %2d for cell <%2d,%2d> constraint\n",k,i,j)<0){

							return 1;
						}
						if(GRBaddconstr(model, 1, ind, val, GRB_GREATER_EQUAL, 0.0, const_name)){
							return 1;
						}
					}
				}
			}

		}
		return 0;
}


void update_for_success(Board* b,int *** signed_vars_for_LP,double* sol,int ILP){
	int i,j,k,N=b->n*b->m;
	if(ILP){
		free3DMatrix(b->solved_board,N);
		b->solved_board=create3DMatrix(N);
	}
	else{
		free3DFMatrix(b->guess_board,N);
		b->guess_board=create3DFMatrix(N);
	}
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			for(k=1;k<N+1;k++){
				if(signed_vars_for_LP[i][j][k]!=-1){
					if(ILP){
						b->solved_board[i][j][k]=(int)sol[signed_vars_for_LP[i][j][k]];
					}
					else{
						b->guess_board[i][j][k]=(float)sol[signed_vars_for_LP[i][j][k]];
					}
				}
				else{
					if(ILP){
						b->solved_board[i][j][k]=b->game_board[i][j][k];
					}
					else{
						b->guess_board[i][j][k]=(float)b->game_board[i][j][k];}
				}
			}
		}
	}

	free3DMatrix(signed_vars_for_LP,N);
}

int chooseValueWithRand(float* cell_vals,int size){
	int i,maxi;
	float sum=0;
	double x;
	for(i=0;i<size;i++){
		sum+=cell_vals[i];
	}
	x=randInRange(sum);
	do{
	maxi=max(cell_vals,size);
	sum=sum-cell_vals[maxi];
	if(sum<x){
		return maxi;
    	}
	cell_vals[maxi]=0;
	}
	while(maxi!=0);
	return 0;

}

int max(float* array,int size){
	int max,i;
	float val=0;
	max=0;
	for(i=0;i<size;i++){
		if(array[i]>val){
			max=i;
			val=array[i];
		}
	}
	return max;

}


double randInRange( float range)
{
  return ((double)rand() / (double)RAND_MAX)*range ;
}

int randIntInRange( int range){
	return  rand() / (RAND_MAX / (range ) + 1);
}

int fillXRandomly(Board* b,int x){
	int i ,counter,counter2,N,r1,r2,t;
	int* legal_vals,*empty_cells;
	N=b->n*b->m;
	empty_cells=(int*)calloc(N*N,sizeof(int));
	legal_vals=(int*)calloc(N,sizeof(int));
	if(empty_cells==NULL || legal_vals==NULL){
		printf("terminating due to unexpected error\n");
		exit(EXIT_FAILURE);
	}
	counter=0;
	for(i=0;i<N*N;i++){
		if(b->game_board[i/N][i%N][0]){
			empty_cells[counter]=i;
			counter++;
		}
	}
	t=0;
	while(t!=x){
		r1=randIntInRange(counter);
		if(empty_cells[r1]!=-1){
			counter2=0;
			for(i=1;i<N+1;i++){
				if(!isErrValForCell(b,empty_cells[r1]/N,empty_cells[r1]%N,i)){
					legal_vals[counter2]=i;
					counter2++;
				}
			}
			if(counter2==0){ /*there is no legal value for cell*/
				free(legal_vals);
				free(empty_cells);
				return 0;
			}

			r2=randIntInRange(counter2);
			setValue(b,empty_cells[r1]/N,empty_cells[r1]%N,legal_vals[r2]);
			empty_cells[r1]=-1;
			t++;
		}
	}
	/*done filling X cells*/

	free(legal_vals);
	free(empty_cells);
	return 1;

}

void fillYRandomly(Board* b,int y){
	int i,r,num;
	clearBoard(b);
	for(i=0;i<y;i++){
		r=randIntInRange(b->n*b->m*b->n*b->m);
		while (!(b->game_board[r/(b->n*b->m)][r%(b->n*b->m)][0])){
			r=randIntInRange(b->n*b->m*b->n*b->m);
		}
		num=getNumInCell(b,'s',r/(b->n*b->m),r%(b->n*b->m));
		b->game_board[r/(b->n*b->m)][r%(b->n*b->m)][0]=0;
		b->game_board[r/(b->n*b->m)][r%(b->n*b->m)][num]=1;
	}

}
