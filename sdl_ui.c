#include <stdbool.h>
#include <stdio.h>

#include <SDL.h>

#include "config.h"
#include "sdl_ui.h"

#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 560
#define CELL_SIZE 64
#define BOARD_LEFT 104
#define BOARD_TOP 96

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static void setColor(Uint8 red, Uint8 green, Uint8 blue)
{
    /* Choisit la couleur utilisee par le renderer SDL. */
    SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
}

static void fillCircle(int centerX, int centerY, int radius)
{
    /* SDL2 ne fournit pas directement de cercle plein.
       On le dessine point par point avec l'equation x*x + y*y <= r*r. */
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) {
                SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
            }
        }
    }
}

static void drawCellBackground(int row, int col)
{
    /* Dessine une case de la grille : fond sombre + contour. */
    SDL_Rect cell = {
        BOARD_LEFT + col * CELL_SIZE,
        BOARD_TOP + row * CELL_SIZE,
        CELL_SIZE,
        CELL_SIZE
    };

    setColor(35, 43, 55);
    SDL_RenderFillRect(renderer, &cell);

    setColor(80, 96, 120);
    SDL_RenderDrawRect(renderer, &cell);
}

static void drawPiece(char cell, int row, int col)
{
    /* Dessine le contenu d'une case :
       cercle colore pour les joueurs, carre jaune pour les blocs. */
    int centerX = BOARD_LEFT + col * CELL_SIZE + CELL_SIZE / 2;
    int centerY = BOARD_TOP + row * CELL_SIZE + CELL_SIZE / 2;
    SDL_Rect block = {
        centerX - 20,
        centerY - 20,
        40,
        40
    };

    if (cell == '.') {
        return;
    }

    if (cell == 'X') {
        setColor(220, 65, 65);
        fillCircle(centerX, centerY, 23);
    } else if (cell == 'O') {
        setColor(75, 130, 235);
        fillCircle(centerX, centerY, 23);
    } else if (cell == 'A') {
        setColor(75, 190, 110);
        fillCircle(centerX, centerY, 23);
    } else if (cell == '#') {
        setColor(235, 190, 65);
        SDL_RenderFillRect(renderer, &block);
    }
}

static void updateWindowTitle(const char *playerName, int currentPlayer, int rotationSize)
{
    /* Le titre de la fenetre indique qui joue et la taille de rotation. */
    char title[120];

    snprintf(title, sizeof(title), "CY-Connect - %s's turn - Player %d - Rotation %dx%d",
        playerName, currentPlayer + 1, rotationSize, rotationSize);
    SDL_SetWindowTitle(window, title);
}

bool sdlInitialize(void)
{
    /* Initialisation de SDL, creation de la fenetre et du renderer. */
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("CY-Connect", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        printf("SDL window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        printf("SDL renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    return true;
}

void sdlClose(void)
{
    /* Fermeture propre des ressources SDL. */
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }

    if (window != NULL) {
        SDL_DestroyWindow(window);
        window = NULL;
    }

    SDL_Quit();
}

void sdlRenderBoard(char **board, const char *playerName, int currentPlayer, int rotationSize)
{
    /* Redessine toute la fenetre a partir du plateau actuel.
       La logique du jeu reste dans game.c ; SDL ne fait qu'afficher. */
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            sdlClose();
            return;
        }
    }

    if (renderer == NULL || window == NULL) {
        return;
    }

    updateWindowTitle(playerName, currentPlayer, rotationSize);

    setColor(18, 24, 33);
    SDL_RenderClear(renderer);

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            drawCellBackground(row, col);
            drawPiece(board[row][col], row, col);
        }
    }

    SDL_RenderPresent(renderer);
}
