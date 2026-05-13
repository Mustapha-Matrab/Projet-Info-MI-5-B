#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "input.h"

int readInt(const char *message, int min, int max)
{
    char buffer[100];
    char *endPointer = NULL;
    long value;

    while (1) {
        printf("%s", message);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error. Try again.\n");
            clearerr(stdin);
            continue;
        }

        value = strtol(buffer, &endPointer, 10);

        if (endPointer == buffer) {
            printf("Invalid input. Try again.\n");
            continue;
        }

        while (*endPointer == ' ' || *endPointer == '\t') {
            endPointer++;
        }

        if (*endPointer != '\n' && *endPointer != '\0') {
            printf("Please enter only a number.\n");
            continue;
        }

        if (value < min || value > max) {
            printf("Choose a value between %d and %d.\n", min, max);
            continue;
        }

        return (int)value;
    }
}

int askMenuChoice(void)
{
    printf("\n1. New game\n");
    printf("2. Load game\n");
    printf("3. Quit\n");
    return readInt("Choose an option: ", 1, 3);
}

int askPlayerCount(void)
{
    return readInt("Number of players (2 or 3): ", MIN_PLAYERS, MAX_PLAYERS);
}

int askColumn(int cols)
{
    return readInt("Choose a column: ", 1, cols) - 1;
}

int askPivotRow(int rows, int rotationSize)
{
    int margin = rotationSize / 2;
    return readInt("Choose pivot row: ", margin + 1, rows - margin) - 1;
}

int askPivotCol(int cols, int rotationSize)
{
    int margin = rotationSize / 2;
    return readInt("Choose pivot column: ", margin + 1, cols - margin) - 1;
}

int askRotationDirection(void)
{
    printf("1. Left rotation\n");
    printf("2. Right rotation\n");
    return readInt("Choose rotation direction: ", 1, 2);
}

int askSaveChoice(void)
{
    printf("\n1. Continue\n");
    printf("2. Save and continue\n");
    printf("3. Save and quit\n");
    return readInt("Choose an option: ", 1, 3);
}
