/*
 * LP.h
 *
 *  Created on: Apr 6, 2020
 *      Author: DELL
 */

#ifndef LP_H_
#define LP_H_
#include "Board.h"
#include "gurobi_c.h"

/*
 *LP Summary:
 *
 *Module that includes the linear programing functions that using GUROBI library
 *This is an auxiliary module to CommandList module
 *This module also includes auxiliary functions that support the LP functions
 *
 *isValideBoard- using ILP for checking if a game_board is solvable
 *guessSol- using LP for guessing a solution to a game_board according to threshold
 *generateGuessBoard- using LP for getting scores for each legal value for each empty cell in game_board
 *generateNewBoard- using ILP to generate new game_board
 *LP- runs a GUROBI model for solving the problems above
 *sign_leagal_vars- (aux for LP) calculate the legal variable of a game_board
 *free_GRB_params- (aux for LP) free GUROBI parameters
 *update_for_err- (aux for LP)helps exit LP on error
 *setCoefsTo1- (aux for LP) sets coefficients for ILP
 *setCoefsForLP-(aux for LP) sets coefficients for LP
 *setVtypeToBin- (aux for LP) sets variable type for ILP
 *setVtypeToCont-(aux for LP) sets variable type for LP
 *AddCellConstraints- (aux for LP)adds cell constraints to the model
 *AddRowConstraints- (aux for LP)adds row constraints to the model
 *AddColConstraints-(aux for LP)adds column constraints to the model
 *AddBlockConstraints- (aux for LP)adds block constraints to the model
 *AddPositiveConstraints- (aux for LP)adds positive constraints to the model- LP only
 *update_for_success- (aux for LP)helps exit LP on success
 *chooseValueWithRand-(aux for guessSol) choosing value for a cell in a random way that considering its LP score
 *randInRange- generates a random float number in a range
 *randIntInRange- generates a random integer number in a range
 *max- find the muximum in an array
 *fillXRandomly- (aux for generateNewBoard)fills X random cells in game_board with random legal values
 *fillyRandomly- (aux for generateNewBoard)fills Y random cells in game_board with solved_board values
 *
 */


/*
 * using ILP to generate a solution to the board
 *
 * @param
 * b - a pointer to a board
 *
 * @return
 * 0 - there is no solution
 * 1 - there is a solution
 * 2 - there is error in ILP
 *
 */
int isValideBoard(Board* b);

/*
 * using LP and x to generate a guess to the board
 *
 * @param
 * b - a pointer to a board
 *
 * @return
 * 0 - there is no solution
 * 1 - there is a solution
 * 2 - there is error in LP
 *
 */
int guessSol(Board* b, float x);

/*
 * using LP to generate 3D matrix of scores for values in cells
 *
 * @param
 * b - a pointer to a board
 *
 * @return
 * 0 - there is no solution
 * 1 - there is a solution
 * 2 - there is error in LP
 *
 */
int generateGuessBoard(Board* b);

/*
 * choosing randomly X cells and filling them with randomly legal values
 * using ILP to generate a solution to the board
 * choosing randomly Y cells and clear the rest
 *
 * @param
 * b - a pointer to a board
 *
 * @return
 * 0 - there is solution
 * 2 - reached to 1000 iterations with no appropriate solution
 * 3 - there is error in LP
 *
 */
int generateNewBoard(Board* b,int x,int y);

/*
 * using I\LP to generate 3D matrix of scores for values in cells
 *
 * @param
 * b - a pointer to a board
 * ILP- 1 if we want an ILP sol / 0- if we want LP sol
 *
 * @return
 * 0 - there is error in LP
 * 1 - there is no solution
 * 2 - there is solution
 *
 */
int LP(Board* b, int ILP);

/*
 * going through all the cells in game_board of board and check if each cell represents a legal variable for LP objective func
 * if game_board[i][j][k] is a legal variable, sign_board[i][j][k] = the index of the variable in the objective func.
 * if game_board[i][j][k] is not a legal variable, sign_board[i][j][k] = -1.
 *
 * @param
 * b - a pointer to a board
 * sign_board - a pointer to zero initialized board
 *
 * @return
 * the number of legal variables in board
 * -1 if there is an empty cell in game_board which can't be filled with legal value
 *
 */
int sign_leagal_vars(Board* board, int *** sign_board);

/*
 * free the memory allocated to those pointers
 *
 * @param
 * ind - a pointer to ind array
 * sol -  a pointer to the sol array
 * val - a pointer to the val array
 * obj - a pointer to the obj array
 * vtype - a pointer to the vtype array
 * env - a pointer to the env
 * model - a pionter to the model
 *
 * @return
 * None.
 *
 */
void free_GRB_params(int *ind, double* sol, double* val, double* obj, char* vtype,GRBenv* env, GRBmodel* model);

/*
 * free the memory allocated to solved_board/ guess_board and to signed_vars_for_LP
 *
 * @param
 * b - a pointer to a board
 * signed_vars_for_LP - 3D integer array
 *
 * @return
 * None.
 *
 */
void update_for_err(Board* b,int*** signed_vars_for_LP);

/*
 * sets all coefs to 1
 *
 * @param
 * obj - a pointer to the variable's coefs array
 * num_of_vars- size of the array(number of variables)
 *
 * @return
 * None.
 *
 */
void setCoefsTo1(double * obj,int num_of_vars);

/*
 * sets coefs of the objective func for the probability solutions
 *
 * @param
 * signed_vars_for_LP - 3D integer array of legal values for LP
 * n,m - n*m size of block in the game
 * obj - a pointer to the variable's coefs array
 *
 *
 * @return
 * None.
 *
 */
void setCoefsForLP(int *** signed_vars_for_LP, int n, int m,double * obj);

/*
 * sets type to GRB_BINARY
 *
 * @param
 * vtype- a pointer to the variable's type array
 * num_of_vars- size of the array(number of variables)
 *
 * @return
 * None.
 *
 */
void setVtypeToBin(char * vtype,int num_of_vars);

/*
 * sets type to GRB_CONTINUOUS
 *
 * @param
 * vtype- a pointer to the variable's type array
 * num_of_vars- size of the array(number of variables)
 *
 * @return
 * None.
 *
 */
void setVtypeToCont(char * vtype,int num_of_vars);

/*
 * the func adds all cell constraints to the model
 * according to the game board and to the indices from signed_vars_for_LP board
 * Its prints the right error message if occurs
 *
 * @param
 * model - a pointer to current GRBmodel
 * game_board - a pointer to current game board
 * signed_vars_for_LP - a pointer to the sign board
 * ind - a pointer to the constraint indices array
 * val- a pointer to the constraint's values array
 * N - the size of a constraint
 *
 * @return
 * 0 -success
 * 1 - error
 *
 */
int AddCellConstraints(GRBmodel* model,int*** game_board,int*** signed_vars_for_LP,int* ind,double* val,int N);


/*
 * the func adds all row constraints to the model
 * according to the game board and to the indices from signed_vars_for_LP board
 * Its prints the right error message if occurs
 *
 * @param
 * model - a pointer to current GRBmodel
 * game_board - a pointer to current game board
 * signed_vars_for_LP - a pointer to the sign board
 * ind - a pointer to the constraint indices array
 * val- a pointer to the constraint's values array
 * N - the size of a constraint
 *
 * @return
 * 0 -success
 * 1 - error
 *
 */
int AddRowConstraints(GRBmodel* model,int*** game_board,int*** signed_vars_for_LP,int* ind,double* val,int N);

/*
 * the func adds all column constraints to the model
 * according to the game board and to the indices from signed_vars_for_LP board
 * Its prints the right error message if occurs
 *
 * @param
 * model - a pointer to current GRBmodel
 * game_board - a pointer to current game board
 * signed_vars_for_LP - a pointer to the sign board
 * ind - a pointer to the constraint indices array
 * val- a pointer to the constraint's values array
 * N - the size of a constraint
 *
 * @return
 * 0 -success
 * 1 - error
 *
 */
int AddColConstraints(GRBmodel* model,int*** game_board,int*** signed_vars_for_LP,int* ind,double* val,int N);

/*
 * the func adds all block constraints to the model
 * according to the game board and to the indices from signed_vars_for_LP board
 * Its prints the right error message if occurs
 *
 * @param
 * model - a pointer to current GRBmodel
 * signed_vars_for_LP - a pointer to the sign board
 * ind - a pointer to the constraint indices array
 * val- a pointer to the constraint's values array
 * n,m - n*m the size of a constraint
 *
 * @return
 * 0 -success
 * 1 - error
 *
 */
int AddBlockConstraints(GRBmodel* model,int*** signed_vars_for_LP,int* ind,double* val,int n,int m);


/*
 * the func adds all positive constraints to the model
 * according to the game board and to the indices from signed_vars_for_LP board
 * Its prints the right error message if occurs
 *
 * @param
 * model - a pointer to current GRBmodel
 * game_board - a pointer to current game board
 * signed_vars_for_LP - a pointer to the sign board
 * ind - a pointer to the constraint indices array
 * val- a pointer to the constraint's values array
 * N - the size of a constraint
 *
 * @return
 * 0 -success
 * 1 - error
 *
 */
int AddPositiveConstraints(GRBmodel* model,int*** game_board,int*** signed_vars_for_LP,int* ind,double* val,int N);

/*
 * if ILP- update solved_board according to sol and signed_vars_for_LP
 * else update guess_board
 * after the updates - free signed_vars_for_LP
 *
 * @param
 * b - a pointer to a board
 * signed_vars_for_LP - a pointer to the sign board
 * sol - a pointer to array of solutions
 * ILP - 1- update solved_board/ 0- update guess_board
 *
 * @return
 * 0 -success
 * 1 - error
 *
 */
void update_for_success(Board* b,int *** signed_vars_for_LP,double* sol,int ILP);

/*
 * @param
 * cell_vals - array of values
 * size - the size of the array
 *
 * @return
 * integer k that represents a random choice between the numbers(with consideration of their scores)
 *
 */
int chooseValueWithRand(float* cell_vals,int size);

/*
 * @param
 * range- max value for range
 *
 * @return
 * a random number between 0 to range
 *
 */
double randInRange( float range);

/*
 * @param
 * range- max value for range
 *
 * @return
 * a random integer number between 0 to range
 *
 */
int randIntInRange( int range);

/*
 * @param
 * array - array of values
 *
 * @return
 * the index of the max element
 *
 */
int max(float* array,int size);

/*
 * fills x cells with random legal vals
 *
 * @param
 * b - a pointer to a board
 * x - number of empty cells we want to fill
 *
 * @return
 * 0 -fail
 * 1- success
 *
 */
int fillXRandomly(Board* b,int x);

/*
 * clears game board accept of y randomly chosen cells
 *
 * @param
 * b - a pointer to a board
 * y - number of empty cells we want to fill
 *
 * @return
 * None.
 *
 */
void fillYRandomly(Board* b,int y);

#endif /* LP_H_ */
