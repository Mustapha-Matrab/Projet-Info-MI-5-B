#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "config.h"
#include "game.h"
#include "input.h"
#include "save.h"
#include "ui.h"

#define ROTATE_LEFT 1
#define ROTATE_RIGHT 2
#define DRAW_RESULT -1
#define NO_WINNER 0

char getPlayerPiece(int playerIndex)
{
    const char pieces[MAX_PLAYERS] = {'X', 'O', 'A'};
    return pieces[playerIndex];
}

int getRandomRotationSize(void)
{
    if (rand() % 2 == 0) {
        return 3;
    }

    return 5;
}

bool pivotContainsCell(int pivotRow, int pivotCol, int rotationSize, int row, int col)
{
    int margin = rotationSize / 2;

    return row >= pivotRow - margin &&
        row <= pivotRow + margin &&
        col >= pivotCol - margin &&
        col <= pivotCol + margin;
}

void rotateSquare(char **board, int pivotRow, int pivotCol, int size, int direction)
{
    char temporary[5][5];
    int margin = size / 2;

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            temporary[row][col] = board[pivotRow - margin + row][pivotCol - margin + col];
        }
    }

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (direction == ROTATE_RIGHT) {
                board[pivotRow - margin + row][pivotCol - margin + col] = temporary[size - 1 - col][row];
            } else {
                board[pivotRow - margin + row][pivotCol - margin + col] = temporary[col][size - 1 - row];
            }
        }
    }
}

static bool hasFiveFromCell(char **board, int rows, int cols, int row, int col, int deltaRow, int deltaCol)
{
    char piece = board[row][col];

    for (int step = 1; step < CONNECT_COUNT; step++) {
        int nextRow = row + deltaRow * step;
        int nextCol = col + deltaCol * step;

        if (nextRow < 0 || nextRow >= rows || nextCol < 0 || nextCol >= cols) {
            return false;
        }

        if (board[nextRow][nextCol] != piece) {
            return false;
        }
    }

    return true;
}

static bool playerHasWon(char **board, int rows, int cols, char piece)
{
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
                    return true;
                }
            }
        }
    }

    return false;
}

int checkWinner(char **board, int rows, int cols, int playerCount)
{
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
    do {
        *pivotRow = askPivotRow(ROWS, rotationSize);
        *pivotCol = askPivotCol(COLS, rotationSize);

        if (!pivotContainsCell(*pivotRow, *pivotCol, rotationSize, placedRow, placedCol)) {
            printf("The rotation square must contain the new piece.\n");
        }
    } while (!pivotContainsCell(*pivotRow, *pivotCol, rotationSize, placedRow, placedCol));
}

static bool handleSaveMenu(char **board, int playerCount, int currentPlayer)
{
    int choice = askSaveChoice();

    if (choice == 2 || choice == 3) {
        if (saveGame(SAVE_FILE, board, ROWS, COLS, playerCount, currentPlayer)) {
            printf("Game saved in %s.\n", SAVE_FILE);
        } else {
            printf("Save failed.\n");
        }
    }

    return choice == 3;
}

void playGame(char **board, int playerCount, int currentPlayer)
{
    int winner = NO_WINNER;

    while (winner == NO_WINNER && !isBoardFull(board, ROWS, COLS)) {
        int rotationSize = getRandomRotationSize();
        int placedRow = 0;
        int placedCol = 0;
        int pivotRow = 0;
        int pivotCol = 0;
        int direction = 0;

        clearScreen();
        printTurnInfo(currentPlayer, rotationSize);
        printBoard(board, ROWS, COLS);

        placedCol = askValidColumn(board);
        dropPiece(board, ROWS, placedCol, getPlayerPiece(currentPlayer), &placedRow);

        clearScreen();
        printf("After piece insertion:\n");
        printBoard(board, ROWS, COLS);

        askValidPivot(rotationSize, placedRow, placedCol, &pivotRow, &pivotCol);
        direction = askRotationDirection();

        rotateSquare(board, pivotRow, pivotCol, rotationSize, direction);

        clearScreen();
        printf("After rotation:\n");
        printBoard(board, ROWS, COLS);
        pauseScreen(800);

        applyGravity(board, ROWS, COLS);

        clearScreen();
        printf("After gravity:\n");
        printBoard(board, ROWS, COLS);

        winner = checkWinner(board, ROWS, COLS, playerCount);

        if (winner == NO_WINNER) {
            currentPlayer = (currentPlayer + 1) % playerCount;
            if (handleSaveMenu(board, playerCount, currentPlayer)) {
                return;
            }
        }
    }

    if (winner == DRAW_RESULT) {
        printf("\nDraw: several players aligned %d pieces.\n", CONNECT_COUNT);
    } else if (winner != NO_WINNER) {
        printf("\nPlayer %d wins!\n", winner);
    } else {
        printf("\nThe board is full. Draw game.\n");
    }
}
