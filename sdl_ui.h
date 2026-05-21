#ifndef SDL_UI_H
#define SDL_UI_H

#include <stdbool.h>

bool sdlInitialize(void);
void sdlClose(void);
void sdlRenderBoard(char **board, const char *playerName, int currentPlayer, int rotationSize);

#endif
