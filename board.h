#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

char **createBoard(int rows, int cols);
void freeBoard(char **board, int rows);
void initializeBoard(char **board, int rows, int cols);
bool copyBoard(char **destination, char **source, int rows, int cols);
bool isColumnPlayable(char **board, int rows, int col);
bool dropPiece(char **board, int rows, int col, char piece, int *placedRow);
void applyGravity(char **board, int rows, int cols);
bool isBoardFull(char **board, int rows, int cols);

#endif
