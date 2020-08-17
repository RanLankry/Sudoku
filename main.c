
#include "Parser.h"

int main() {
	CommandList *game = createGame();                                                                                   /* Creates new game list */
    printf("Welcome to SUDOKU game!\n");
    getCommand(game);                                                                                                  /* call for getting the user input repeatedly */

    return 0;
}
