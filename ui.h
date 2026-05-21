#ifndef UI_H
#define UI_H

void clearScreen(void);
void printBoard(char **board, int rows, int cols);
void printTurnInfo(const char *playerName, int currentPlayer, int rotationSize);
void waitForEnter(void);
void pauseScreen(int milliseconds);

#endif
