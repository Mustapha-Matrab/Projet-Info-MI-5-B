#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "config.h"
#include "game.h"
#include "input.h"
#include "save.h"
#include "ui.h"

#ifdef USE_SDL
#include "sdl_ui.h"
#endif

#define ROTATE_LEFT 1
#define ROTATE_RIGHT 2
#define DRAW_RESULT -1
#define NO_WINNER 0

char getPlayerPiece(int playerIndex)
{
    /* Associe chaque joueur a un symbole sur le plateau. */
    const char pieces[MAX_PLAYERS] = {'X', 'O', 'A'};
    return pieces[playerIndex];
}

int getRandomRotationSize(void)
{
    /* La taille de rotation est choisie au hasard : 3x3 ou 5x5. */
    if (rand() % 2 == 0) {
        return 3;
    }

    return 5;
}

bool pivotContainsCell(int pivotRow, int pivotCol, int rotationSize, int row, int col)
{
    /* Verifie si une case appartient au carre centre sur le pivot.
       Pour 3x3, margin vaut 1. Pour 5x5, margin vaut 2. */
    int margin = rotationSize / 2;

    return row >= pivotRow - margin &&
        row <= pivotRow + margin &&
        col >= pivotCol - margin &&
        col <= pivotCol + margin;
}

void rotateSquare(char **board, int pivotRow, int pivotCol, int size, int direction)
{
    /* Rotation d'un carre 3x3 ou 5x5 autour du pivot.
       On copie d'abord le carre dans temporary pour ne pas ecraser
       des cases dont on a encore besoin pendant la rotation. */
    char temporary[5][5];
    int margin = size / 2;

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            temporary[row][col] = board[pivotRow - margin + row][pivotCol - margin + col];
        }
    }

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            /* Formules classiques de rotation d'une matrice carree. */
            if (direction == ROTATE_RIGHT) {
                board[pivotRow - margin + row][pivotCol - margin + col] = temporary[size - 1 - col][row];
            } else {
                board[pivotRow - margin + row][pivotCol - margin + col] = temporary[col][size - 1 - row];
            }
        }
    }
}

int hasFiveFromCell(char **board, int rows, int cols, int row, int col, int deltaRow, int deltaCol)
{
    /* Regarde si, en partant d'une case, on trouve 5 pieces identiques
       dans une direction donnee. Exemple : deltaRow=0 et deltaCol=1
       signifie "vers la droite". */
    char piece = board[row][col];

    for (int step = 1; step < CONNECT_COUNT; step++) {
        int nextRow = row + deltaRow * step;
        int nextCol = col + deltaCol * step;

        if (nextRow < 0 || nextRow >= rows || nextCol < 0 || nextCol >= cols) {
            return 0;
        }

        if (board[nextRow][nextCol] != piece) {
            return 0;
        }
    }

    return 1;
}

int playerHasWon(char **board, int rows, int cols, char piece)
{
    /* Teste toutes les cases du plateau pour un joueur donne.
       Pour chaque piece du joueur, on cherche un alignement dans 4 directions :
       droite, bas, diagonale bas-droite, diagonale bas-gauche. */
    const int directions[4][2] = {
        {0, 1},
        {1, 0},
        {1, 1},
        {1, -1}
    };

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (board[row][col] != piece) {
                continue;
            }

            for (int direction = 0; direction < 4; direction++) {
                if (hasFiveFromCell(board, rows, cols, row, col, directions[direction][0], directions[direction][1])) {
                    return 1;
                }
            }
        }
    }

    return 0;
}

int checkWinner(char **board, int rows, int cols, int playerCount)
{
    /* Verifie tous les joueurs.
       Si plusieurs joueurs gagnent apres la meme rotation, c'est une egalite. */
    int winner = NO_WINNER;
    int winnerCount = 0;

    for (int player = 0; player < playerCount; player++) {
        if (playerHasWon(board, rows, cols, getPlayerPiece(player))) {
            winner = player + 1;
            winnerCount++;
        }
    }

    if (winnerCount > 1) {
        return DRAW_RESULT;
    }

    return winner;
}

static int askValidColumn(char **board)
{
    /* Redemande une colonne tant que la colonne choisie est bloquee ou pleine. */
    int col;

    do {
        col = askColumn(COLS);
        if (!isColumnPlayable(board, ROWS, col)) {
            printf("This column is blocked or full. Choose another one.\n");
        }
    } while (!isColumnPlayable(board, ROWS, col));

    return col;
}

static void askValidPivot(int rotationSize, int placedRow, int placedCol, int *pivotRow, int *pivotCol)
{
    /* Au lieu de demander ligne puis colonne, on calcule tous les pivots valides.
       Un pivot est valide si le carre de rotation reste dans le plateau
       ET si ce carre contient la nouvelle piece. */
    int pivotRows[ROWS * COLS];
    int pivotCols[ROWS * COLS];
    int pivotCount = 0;
    int margin = rotationSize / 2;
    int choice;

    /* On parcourt seulement les pivots qui ne font pas sortir le carre du plateau. */
    for (int row = margin; row < ROWS - margin; row++) {
        for (int col = margin; col < COLS - margin; col++) {
            if (pivotContainsCell(row, col, rotationSize, placedRow, placedCol)) {
                pivotRows[pivotCount] = row;
                pivotCols[pivotCount] = col;
                pivotCount++;
            }
        }
    }

    /* Les coordonnees affichees commencent a 1 pour etre plus naturelles. */
    printf("\nYour piece landed at row %d, column %d.\n", placedRow + 1, placedCol + 1);
    printf("Choose one of these valid pivots:\n");

    for (int i = 0; i < pivotCount; i++) {
        printf("%d. Row %d, column %d\n", i + 1, pivotRows[i] + 1, pivotCols[i] + 1);
    }

    choice = readInt("Choose a pivot number: ", 1, pivotCount) - 1;
    *pivotRow = pivotRows[choice];
    *pivotCol = pivotCols[choice];
}

static bool handleSaveMenu(char **board, int playerCount, int currentPlayer,
    char playerNames[MAX_PLAYERS][MAX_NAME_LENGTH])
{
    /* Menu de fin de tour : continuer, sauvegarder, ou sauvegarder et quitter. */
    int choice = askSaveChoice();

    if (choice == 2 || choice == 3) {
        if (saveGame(SAVE_FILE, board, ROWS, COLS, playerCount, currentPlayer, playerNames)) {
            printf("Game saved in %s.\n", SAVE_FILE);
        } else {
            printf("Save failed.\n");
        }
    }

    return choice == 3;
}

void playGame(char **board, int playerCount, int currentPlayer, char playerNames[MAX_PLAYERS][MAX_NAME_LENGTH])
{
    /* Boucle principale :
       1. afficher le plateau
       2. inserer une piece
       3. choisir un pivot et tourner une zone
       4. appliquer la gravite
       5. tester la victoire */
    int winner = NO_WINNER;
#ifdef USE_SDL
    /* La version SDL est optionnelle. Si elle echoue, le terminal reste utilisable. */
    bool useSdl = sdlInitialize();
#endif

    while (winner == NO_WINNER && !isBoardFull(board, ROWS, COLS)) {
        int rotationSize = getRandomRotationSize();
        int placedRow = 0;
        int placedCol = 0;
        int pivotRow = 0;
        int pivotCol = 0;
        int direction = 0;

        clearScreen();
        printTurnInfo(playerNames[currentPlayer], currentPlayer, rotationSize);
        printBoard(board, ROWS, COLS);
#ifdef USE_SDL
        if (useSdl) {
            sdlRenderBoard(board, playerNames[currentPlayer], currentPlayer, rotationSize);
        }
#endif

        /* Le joueur choisit une colonne, puis la piece tombe au plus bas. */
        placedCol = askValidColumn(board);
        dropPiece(board, ROWS, placedCol, getPlayerPiece(currentPlayer), &placedRow);

        clearScreen();
        printf("After piece insertion:\n");
        printBoard(board, ROWS, COLS);
#ifdef USE_SDL
        if (useSdl) {
            sdlRenderBoard(board, playerNames[currentPlayer], currentPlayer, rotationSize);
        }
#endif

        /* Le joueur choisit un pivot valide, puis le sens de rotation. */
        askValidPivot(rotationSize, placedRow, placedCol, &pivotRow, &pivotCol);
        direction = askRotationDirection();

        rotateSquare(board, pivotRow, pivotCol, rotationSize, direction);

        clearScreen();
        printf("After rotation:\n");
        printBoard(board, ROWS, COLS);
#ifdef USE_SDL
        if (useSdl) {
            sdlRenderBoard(board, playerNames[currentPlayer], currentPlayer, rotationSize);
        }
#endif
        pauseScreen(800);

        /* Apres une rotation, des pieces peuvent se retrouver en l'air :
           on applique donc la gravite sur tout le plateau. */
        applyGravity(board, ROWS, COLS);

        clearScreen();
        printf("After gravity:\n");
        printBoard(board, ROWS, COLS);
#ifdef USE_SDL
        if (useSdl) {
            sdlRenderBoard(board, playerNames[currentPlayer], currentPlayer, rotationSize);
        }
#endif

        /* La victoire est testee seulement apres rotation et gravite,
           comme demande dans le cahier des charges. */
        winner = checkWinner(board, ROWS, COLS, playerCount);

        if (winner == NO_WINNER) {
            currentPlayer = (currentPlayer + 1) % playerCount;
            if (handleSaveMenu(board, playerCount, currentPlayer, playerNames)) {
#ifdef USE_SDL
                if (useSdl) {
                    sdlClose();
                }
#endif
                return;
            }
        }
    }

    if (winner == DRAW_RESULT) {
        printf("\nDraw: several players aligned %d pieces.\n", CONNECT_COUNT);
    } else if (winner != NO_WINNER) {
        printf("\n%s wins!\n", playerNames[winner - 1]);
    } else {
        printf("\nThe board is full. Draw game.\n");
    }

#ifdef USE_SDL
    if (useSdl) {
        waitForEnter();
        sdlClose();
    }
#endif
}
