#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#include "config.h"

char getPlayerPiece(int playerIndex);
int getRandomRotationSize(void);
bool pivotContainsCell(int pivotRow, int pivotCol, int rotationSize, int row, int col);
void rotateSquare(char **board, int pivotRow, int pivotCol, int size, int direction);
int checkWinner(char **board, int rows, int cols, int playerCount);
void playGame(char **board, int playerCount, int currentPlayer, char playerNames[MAX_PLAYERS][MAX_NAME_LENGTH]);

#endif
