#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "board.h"
#include "config.h"
#include "game.h"
#include "input.h"
#include "save.h"

int main(void)
{
    char **board = createBoard(ROWS, COLS);
    int playerCount = 2;
    int currentPlayer = 0;
    int menuChoice;

    if (board == NULL) {
        printf("Memory allocation error.\n");
        return 1;
    }

    srand((unsigned int)time(NULL));

    menuChoice = askMenuChoice();

    if (menuChoice == 1) {
        playerCount = askPlayerCount();
        initializeBoard(board, ROWS, COLS);
        playGame(board, playerCount, currentPlayer);
    } else if (menuChoice == 2) {
        if (loadGame(SAVE_FILE, board, ROWS, COLS, &playerCount, &currentPlayer)) {
            playGame(board, playerCount, currentPlayer);
        } else {
            printf("No valid save file found.\n");
        }
    } else {
        printf("Goodbye.\n");
    }

    freeBoard(board, ROWS);
    return 0;
}
