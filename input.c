#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "input.h"

int readInt(const char *message, int min, int max)
{
    /* Lecture securisee d'un entier.
       fgets lit une ligne complete, puis strtol convertit en nombre.
       Cela evite les problemes classiques de scanf avec les mauvaises entrees. */
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

        /* Si aucun chiffre n'a ete lu, l'entree est invalide. */
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

void askPlayerName(char playerName[MAX_NAME_LENGTH], int playerNumber)
{
    /* Demande le nom d'un joueur.
       On refuse un nom vide pour garder un affichage clair pendant la partie. */
    char *lineBreak;

    while (1) {
        printf("Name of player %d: ", playerNumber);

        if (fgets(playerName, MAX_NAME_LENGTH, stdin) == NULL) {
            printf("Input error. Try again.\n");
            clearerr(stdin);
            continue;
        }

        lineBreak = strchr(playerName, '\n');
        if (lineBreak != NULL) {
            *lineBreak = '\0';
        }

        if (strlen(playerName) > 0) {
            return;
        }

        printf("The name cannot be empty.\n");
    }
}

int askMenuChoice(void)
{
    /* Menu principal du programme. */
    printf("\n1. New game\n");
    printf("2. Load game\n");
    printf("3. Quit\n");
    return readInt("Choose an option: ", 1, 3);
}

int askPlayerCount(void)
{
    /* Le cahier des charges autorise 2 ou 3 joueurs. */
    return readInt("Number of players (2 or 3): ", MIN_PLAYERS, MAX_PLAYERS);
}

int askColumn(int cols)
{
    /* L'utilisateur voit les colonnes de 1 a 8,
       mais le tableau C utilise des indices de 0 a 7. */
    return readInt("Choose a column: ", 1, cols) - 1;
}

int askPivotRow(int rows, int rotationSize)
{
    /* Ancienne saisie possible du pivot :
       on empeche un pivot trop proche du bord. */
    int margin = rotationSize / 2;
    return readInt("Choose pivot row: ", margin + 1, rows - margin) - 1;
}

int askPivotCol(int cols, int rotationSize)
{
    /* Meme logique que pour la ligne du pivot, mais avec les colonnes. */
    int margin = rotationSize / 2;
    return readInt("Choose pivot column: ", margin + 1, cols - margin) - 1;
}

int askRotationDirection(void)
{
    /* Choix du sens de rotation de la zone carree. */
    printf("1. Left rotation\n");
    printf("2. Right rotation\n");
    return readInt("Choose rotation direction: ", 1, 2);
}

int askSaveChoice(void)
{
    /* A chaque fin de tour, le joueur peut sauvegarder la partie. */
    printf("\n1. Continue\n");
    printf("2. Save and continue\n");
    printf("3. Save and quit\n");
    return readInt("Choose an option: ", 1, 3);
}
