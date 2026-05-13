#ifndef SAVE_H
#define SAVE_H

#include <stdbool.h>

bool saveGame(const char *fileName, char **board, int rows, int cols, int playerCount, int currentPlayer);
bool loadGame(const char *fileName, char **board, int rows, int cols, int *playerCount, int *currentPlayer);

#endif
