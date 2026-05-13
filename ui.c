#include <stdio.h>
#include <unistd.h>

#include "ui.h"

#define RESET "\033[0m"
#define RED "\033[1;31m"
#define BLUE "\033[1;34m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define CYAN "\033[1;36m"

void clearScreen(void)
{
    printf("\033[H\033[J");
}

static void printCell(char cell)
{
    if (cell == 'X') {
        printf(RED "X " RESET);
    } else if (cell == 'O') {
        printf(BLUE "O " RESET);
    } else if (cell == 'A') {
        printf(GREEN "A " RESET);
    } else if (cell == '#') {
        printf(YELLOW "# " RESET);
    } else {
        printf(". ");
    }
}

void printBoard(char **board, int rows, int cols)
{
    printf("\n   ");
    for (int col = 0; col < cols; col++) {
        printf("%d ", col + 1);
    }
    printf("\n");

    for (int row = 0; row < rows; row++) {
        printf("%d |", row + 1);
        for (int col = 0; col < cols; col++) {
            printCell(board[row][col]);
        }
        printf("|\n");
    }

    printf("   ");
    for (int col = 0; col < cols; col++) {
        printf("--");
    }
    printf("\n");
}

void printTurnInfo(int currentPlayer, int rotationSize)
{
    printf(CYAN "\nPlayer %d turn" RESET "\n", currentPlayer + 1);
    printf("Rotation square size: %dx%d\n", rotationSize, rotationSize);
}

void waitForEnter(void)
{
    char buffer[8];

    printf("\nPress ENTER to continue...");
    fgets(buffer, sizeof(buffer), stdin);
}

void pauseScreen(int milliseconds)
{
    usleep(milliseconds * 1000);
}
