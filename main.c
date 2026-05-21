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
    /* Creation du plateau en memoire dynamique.
       Le plateau est un tableau de lignes, et chaque ligne contient des cases. */
    char **board = createBoard(ROWS, COLS);
    char playerNames[MAX_PLAYERS][MAX_NAME_LENGTH];
    int playerCount = 2;
    int currentPlayer = 0;
    int menuChoice;

    if (board == NULL) {
        printf("Memory allocation error.\n");
        return 1;
    }

    srand((unsigned int)time(NULL));

    /* Le menu permet de choisir entre une nouvelle partie,
       une sauvegarde existante, ou quitter le programme. */
    menuChoice = askMenuChoice();

    if (menuChoice == 1) {
        /* Nouvelle partie : on demande le nombre de joueurs et leurs noms. */
        playerCount = askPlayerCount();
        for (int player = 0; player < playerCount; player++) {
            askPlayerName(playerNames[player], player + 1);
        }
        initializeBoard(board, ROWS, COLS);
        playGame(board, playerCount, currentPlayer, playerNames);
    } else if (menuChoice == 2) {
        /* Chargement : le fichier contient le plateau, le joueur actuel
           et les noms des joueurs. */
        if (loadGame(SAVE_FILE, board, ROWS, COLS, &playerCount, &currentPlayer, playerNames)) {
            playGame(board, playerCount, currentPlayer, playerNames);
        } else {
            printf("No valid save file found.\n");
        }
    } else {
        printf("Goodbye.\n");
    }

    /* Toujours liberer la memoire avant de quitter le programme. */
    freeBoard(board, ROWS);
    return 0;
}
