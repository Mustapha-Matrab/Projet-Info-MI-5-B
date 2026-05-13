#include <stdbool.h>
#include <stdio.h>

#include "save.h"

bool saveGame(const char *fileName, char **board, int rows, int cols, int playerCount, int currentPlayer)
{
    FILE *file = fopen(fileName, "w");

    if (file == NULL) {
        return false;
    }

    fprintf(file, "%d %d %d %d\n", rows, cols, playerCount, currentPlayer);

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            fputc(board[row][col], file);
        }
        fputc('\n', file);
    }

    fclose(file);
    return true;
}

bool loadGame(const char *fileName, char **board, int rows, int cols, int *playerCount, int *currentPlayer)
{
    FILE *file = fopen(fileName, "r");
    int savedRows;
    int savedCols;

    if (file == NULL) {
        return false;
    }

    if (fscanf(file, "%d %d %d %d\n", &savedRows, &savedCols, playerCount, currentPlayer) != 4) {
        fclose(file);
        return false;
    }

    if (savedRows != rows || savedCols != cols) {
        fclose(file);
        return false;
    }

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            int value = fgetc(file);

            if (value == EOF || value == '\n') {
                fclose(file);
                return false;
            }

            board[row][col] = (char)value;
        }

        fgetc(file);
    }

    fclose(file);
    return true;
}
