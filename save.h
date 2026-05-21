#ifndef SAVE_H
#define SAVE_H

#include <stdbool.h>

#include "config.h"

bool saveGame(const char *fileName, char **board, int rows, int cols, int playerCount, int currentPlayer,
    char playerNames[MAX_PLAYERS][MAX_NAME_LENGTH]);
bool loadGame(const char *fileName, char **board, int rows, int cols, int *playerCount, int *currentPlayer,
    char playerNames[MAX_PLAYERS][MAX_NAME_LENGTH]);

#endif
