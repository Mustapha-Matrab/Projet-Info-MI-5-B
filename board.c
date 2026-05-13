#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "config.h"

static bool isMovablePiece(char cell)
{
    return cell != '.' && cell != '#';
}

char **createBoard(int rows, int cols)
{
    char **board = malloc(rows * sizeof(char *));

    if (board == NULL) {
        return NULL;
    }

    for (int i = 0; i < rows; i++) {
        board[i] = malloc(cols * sizeof(char));

        if (board[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(board[j]);
            }
            free(board);
            return NULL;
        }
    }

    return board;
}

void freeBoard(char **board, int rows)
{
    if (board == NULL) {
        return;
    }

    for (int i = 0; i < rows; i++) {
        free(board[i]);
    }
    free(board);
}

void initializeBoard(char **board, int rows, int cols)
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j] = '.';
        }
    }

    board[0][0] = '#';
    board[0][cols - 1] = '#';
    board[rows - 1][0] = '#';
    board[rows - 1][cols - 1] = '#';
}

bool copyBoard(char **destination, char **source, int rows, int cols)
{
    if (destination == NULL || source == NULL) {
        return false;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            destination[i][j] = source[i][j];
        }
    }

    return true;
}

bool isColumnPlayable(char **board, int rows, int col)
{
    for (int i = 0; i < rows; i++) {
        if (board[i][col] == '.') {
            return true;
        }

        if (board[i][col] == '#') {
            return false;
        }
    }

    return false;
}

bool dropPiece(char **board, int rows, int col, char piece, int *placedRow)
{
    int targetRow = -1;

    for (int i = 0; i < rows; i++) {
        if (board[i][col] == '#') {
            break;
        }

        if (board[i][col] == '.') {
            targetRow = i;
        }
    }

    if (targetRow == -1) {
        return false;
    }

    board[targetRow][col] = piece;

    if (placedRow != NULL) {
        *placedRow = targetRow;
    }

    return true;
}

void applyGravity(char **board, int rows, int cols)
{
    for (int col = 0; col < cols; col++) {
        int writeRow = rows - 1;

        while (writeRow >= 0) {
            while (writeRow >= 0 && board[writeRow][col] == '#') {
                writeRow--;
            }

            if (writeRow < 0) {
                break;
            }

            int segmentBottom = writeRow;

            while (writeRow >= 0 && board[writeRow][col] != '#') {
                writeRow--;
            }

            int segmentTop = writeRow + 1;
            int target = segmentBottom;

            for (int row = segmentBottom; row >= segmentTop; row--) {
                if (isMovablePiece(board[row][col])) {
                    board[target][col] = board[row][col];
                    if (target != row) {
                        board[row][col] = '.';
                    }
                    target--;
                }
            }

            for (int row = target; row >= segmentTop; row--) {
                board[row][col] = '.';
            }
        }
    }
}

bool isBoardFull(char **board, int rows, int cols)
{
    for (int col = 0; col < cols; col++) {
        if (isColumnPlayable(board, rows, col)) {
            return false;
        }
    }

    return true;
}
