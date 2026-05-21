#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "save.h"

bool saveGame(const char *fileName, char **board, int rows, int cols, int playerCount, int currentPlayer,
    char playerNames[MAX_PLAYERS][MAX_NAME_LENGTH])
{
    /* Sauvegarde simple dans un fichier texte :
       premiere ligne = dimensions, nombre de joueurs, joueur courant
       lignes suivantes = noms des joueurs
       puis le plateau. */
    FILE *file = fopen(fileName, "w");

    if (file == NULL) {
        return false;
    }

    fprintf(file, "%d %d %d %d\n", rows, cols, playerCount, currentPlayer);

    for (int player = 0; player < playerCount; player++) {
        fprintf(file, "%s\n", playerNames[player]);
    }

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            fputc(board[row][col], file);
        }
        fputc('\n', file);
    }

    fclose(file);
    return true;
}

bool loadGame(const char *fileName, char **board, int rows, int cols, int *playerCount, int *currentPlayer,
    char playerNames[MAX_PLAYERS][MAX_NAME_LENGTH])
{
    /* Charge une sauvegarde et verifie que les dimensions correspondent
       au plateau attendu par le programme. */
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

    if (savedRows != rows || savedCols != cols || *playerCount < MIN_PLAYERS || *playerCount > MAX_PLAYERS) {
        fclose(file);
        return false;
    }

    for (int player = 0; player < *playerCount; player++) {
        char *lineBreak;

        /* Lecture du nom du joueur, puis suppression du retour a la ligne. */
        if (fgets(playerNames[player], MAX_NAME_LENGTH, file) == NULL) {
            fclose(file);
            return false;
        }

        lineBreak = strchr(playerNames[player], '\n');
        if (lineBreak != NULL) {
            *lineBreak = '\0';
        }
    }

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            int value = fgetc(file);

            /* Si le fichier est coupe ou mal forme, le chargement echoue. */
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
