/*
 * AuxParser.h
 *
 *  Created on: Mar 15, 2020
 *      Author: DELL
 */

#ifndef AUXPARSER_H_
#define AUXPARSER_H_

#define MAX_SIZE 256

#include "Board.h"

/*
 * AuxParser Summary:
 *
 * A module that includes functions which are a support for dealing with the user's input.
 * This is an auxiliary module for Parser.
 *
 * PrintError - prints an appropriate error to the console based on invalid or incorrect user input
 * currCommands - prints the commands which are legal for the current MODE
 * getInput - function that receives the user's input and arranges it
 * isInteger - gets a string and determines if it represents an integer or not
 * isIntegerOrFloat - gets a string and determines if it represents a float number / integer or not
 * loadFromFile - gets a string representing a file name and parses its content to a valid SUDOKU board
 * checkSolve - Checking the "solve" command's parameters before calling it
 * checkEdit - Checking the "edit" command's parameters before calling it
 * checkMarkErrors - Checking the "mark_errors" command's parameters before calling it
 * checkPrintBoard - Checking the "print_board" command's parameters before calling it
 * checkSet - Checking the "set" command's parameters before calling it
 * checkValidate - Checking the "validate" command's parameters before calling it
 * checkGuess - Checking the "guess" command's parameters before calling it
 * checkGenerate - Checking the "generate" command's parameters before calling it
 * checkUndo - Checking the "undo" command's parameters before calling it
 * checkRedo - Checking the "redo" command's parameters before calling it
 *  checkSave - Checking the "save" command's parameters before calling it
 *  checkHint - Checking the "hint" command's parameters before calling it
 *  checkGuessHint - Checking the "guess_hint" command's parameters before calling it
 *  checkNumSolutions - Checking the "num_solutions" command's parameters before calling it
 *  checkAutofill - Checking the "autofill" command's parameters before calling it
 *  checkReset - Checking the "reset" command's parameters before calling it
 *  checkExit - Checking the "exit" command's parameters before calling it
 *
 */



/*
 * type used for representing the game errors
 */
typedef enum game_errors_t{
	COMMANDLENGTH, COMMANDNAME, MODE, MANYARGUMENTS,
	FEWARGUMENTS,PARAMETERSVALID, RANGE, ERRONEOUS,
	EMPTYCELLS, NOSOLUTION, FIXEDCELL, NOTEMPTY, SAVEFILE
}ERROR;



/*
 * A function that prints an appropriate error to the console based on invalid or incorrect user input
 *
 * @param
 * err - Error type object, representing the appropriate error
 *
 * @return
 * None.
 *
 */
void printError(ERROR err);

/*
 * A function that prints the commands which are legal for the current MODE
 *
 * @param
 * mode - The current MODE
 *
 * @return
 * None.
 * An appropriate message is printed to the user.
 *
 */
void currCommands(int mode);

/*
 * A function that receives the user's input and arranges the characters in a new buffer,
 * such that the new arrangement ignores extra white spaces, and the command is ready for processing.
 *
 * @param
 * buf - a character buffer representing the user input as is
 * finalbuf- an empty buffer which at the end of the functions will be conveniently arranged for processing
 *
 * @return
 * 1 - if EOF was given as an input
 * 0 - otherwise
 * finalbuf is arranged and ready for processing
 *
 */
int getInput(char buf[],char finalbuf[]);

/*
 * A function that gets a string and determines if it represents an integer or not.
 *
 * @param
 * p - A string representing a command parameter
 *
 * @return
 * 1 - if p is an integer
 * 0 - otherwise
 *
 */
int isInteger(char* p);

/*
 * A function that gets a string and determines if it represents a float number / integer or not.
 *
 * @param
 * p - A string representing a command parameter
 *
 * @return
 * 1 - if p is a float number or an integer
 * 0 - otherwise
 *
 */
int isIntegerOrFloat(char* p);

/*
 * A function that gets a string representing a file name and parses its content to a valid SUDOKU board.
 * The functions prints an appropriate error if the file loading fails whether the file loading failed due to
 * problem in the file or the file content.
 *
 * @param
 * path - A string representing a file name
 *
 * @return
 * if the file loading and parsing fails for any reason, the function returns NULL
 * otherwise, the functions return a pointer to an appropriate Board object which represents a SUDOKU board
 *
 */
Board* loadFromFile(char* path);

/*
 * Checking the "solve" command's parameters before calling it
 *
 * @param
 * len - The number of parameters that are given with the "solve" command
 *
 * @return
 * 1 - Parameters are correct
 * 0 - Parameters are incorrect and an appropriate error is printed
 *
 */
int checkSolve (int len);

/*
 * Checking the "edit" command's parameters before calling it
 *
 * @param
 * len - The number of parameters that are given with the "edit" command
 *
 * @return
 * 2 - Parameters are correct, a file name was given
 * 1 - Parameters are correct, no extra parameters to the "edit" command
 * 0 - Parameters are incorrect and an appropriate error is printed
 *
 */
int checkEdit (int len);

/*
 * Checking the "mark_errors" command's parameters before calling it
 *
 * @param
 * command - An array of string representing the command parameters
 * len - The number of parameters that are given with the "mark_errors" command
 * mode - the current MODE
 *
 * @return
 * 1 - Parameters are correct
 * 0 - Parameters are incorrect and an appropriate error is printed
 *
 */
int checkMarkErrors (char* command[], int len, int mode);

/*
 * Checking the "print_board" command's parameters before calling it
 *
 * @param
 * len - The number of parameters that are given with the "print_board" command
 * mode - the current MODE
 *
 * @return
 * 1 - Parameters are correct
 * 0 - Parameters are incorrect and an appropriate error is printed
 *
 */
int checkPrintBoard (int len,int mode);

/*
 * Checking the "set" command's parameters before calling it
 *
 * @param
 * command - an array of string representing the command's parameters
 * len - The number of parameters that are given with the "set" command
 * n,m - representing the block sizes
 *
 * @return
 * 1 - Parameters are correct
 * 0 - Parameters are incorrect and an appropriate error is printed
 *
 */
int checkSet (char* command[], int len, int n, int m);

/*
 * Checking the "validate" command's parameters before calling it
 *
 * @param
 * len - The number of parameters that are given with the "validate" command
 * err - 1 if the board is erroneous, 0 otherwise
 *
 * @return
 * 1 - Parameters are correct
 * 0 - Parameters are incorrect and an appropriate error is printed
 *
 */
int checkValidate (int len, int err);

/*
 * Checking the "guess" command's parameters before calling it
 *
 * @param
 * command - An array of strings representing the command's parameters
 * len - The number of parameters that are given with the "guess" command
 * err - 1 if the board is erroneous, 0 otherwise
 *
 * @return
 * 1 - Parameters are correct
 * 0 - Parameters are incorrect and an appropriate error is printed
 *
 */
int checkGuess (char* command[], int len, int err);

/*
 * Checking the "generate" command's parameters before calling it
 *
 * @param
 * command - An array of strings representing the command's parameters
 * len - The number of parameters that are given with the "generate" command
 * m,n - Sizes of the board's block
 * err - 1 if the board is erroneous, 0 otherwise
 *
 * @return
 * 1 - Parameters are correct
 * 0 - Parameters are incorrect and an appropriate error is printed
 *
 */
int checkGenerate (char* command[], int len, int m, int n, int err);

/*
 * Checking the "undo" command's parameters before calling it
 *
 * @param
 * len - The number of parameters that are given with the "undo" command
 * mode - The current MODE
 *
 * @return
 * 1 - Parameters are correct
 * 0 - Parameters are incorrect and an appropriate error is printed
 *
 */
int checkUndo (int len, int mode);

/*
 * Checking the "redo" command's parameters before calling it
 *
 * @param
 * len - The number of parameters that are given with the "redo" command
 * mode - The current MODE
 *
 * @return
 * 1 - Parameters are correct
 * 0 - Parameters are incorrect and an appropriate error is printed
 *
 */
int checkRedo (int len, int mode);

/*
 * Checking the "save" command's parameters before calling it
 *
 * @param
 * len - The number of parameters that are given with the "save" command
 * mode - The current MODE
 * err - 1 if the board is erroneous, 0 otherwise
 *
 * @return
 * 1 - Parameters are correct
 * 0 - Parameters are incorrect and an appropriate error is printed
 *
 */
int checkSave (int len, int mode, int err);

/*
 * Checking the "hint" command's parameters before calling it
 *
 * @param
 * command - An array of strings representing the command's parameters
 * len - The number of parameters that are given with the "hint" command
 * err - 1 if the board is erroneous, 0 otherwise
 * n,m - Sizes of the board's block
 *
 * @return
 * 1 - Parameters are correct
 * 0 - Parameters are incorrect and an appropriate error is printed
 *
 */
int checkHint (char* command[], int len, int err, int n, int m);

/*
 * Checking the "guess_hint" command's parameters before calling it
 *
 * @param
 * command - An array of strings representing the command's parameters
 * len - The number of parameters that are given with the "guess_hint" command
 * err - 1 if the board is erroneous, 0 otherwise
 * n,m - Sizes of the board's block
 *
 * @return
 * 1 - Parameters are correct
 * 0 - Parameters are incorrect and an appropriate error is printed
 *
 */
int checkGuessHint (char* command[], int len, int err, int n, int m);

/*
 * Checking the "num_solutions" command's parameters before calling it
 *
 * @param
 * len - The number of parameters that are given with the "num_solutions" command
 * err - 1 if the board is erroneous, 0 otherwise
 *
 * @return
 * 1 - Parameters are correct
 * 0 - Parameters are incorrect and an appropriate error is printed
 *
 */
int checkNumSolutions (int len, int err);

/*
 * Checking the "autofill" command's parameters before calling it
 *
 * @param
 * len - The number of parameters that are given with the "autofill" command
 * err - 1 if the board is erroneous, 0 otherwise
 *
 * @return
 * 1 - Parameters are correct
 * 0 - Parameters are incorrect and an appropriate error is printed
 *
 */
int checkAutofill (int len, int err);

/*
 * Checking the "reset" command's parameters before calling it
 *
 * @param
 * len - The number of parameters that are given with the "reset" command
 * mode - The current MODE
 *
 * @return
 * 1 - Parameters are correct
 * 0 - Parameters are incorrect and an appropriate error is printed
 *
 */
int checkReset (int len, int mode);

/*
 * Checking the "exit" command's parameters before calling it
 *
 * @param
 * len - The number of parameters that are given with the "exit" command
 *
 * @return
 * 1 - Parameters are correct
 * 0 - Parameters are incorrect and an appropriate error is printed
 *
 */
int checkExit (int len);


#endif /* AUXPARSER_H_ */
