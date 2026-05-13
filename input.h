#ifndef INPUT_H
#define INPUT_H

int readInt(const char *message, int min, int max);
int askMenuChoice(void);
int askPlayerCount(void);
int askColumn(int cols);
int askPivotRow(int rows, int rotationSize);
int askPivotCol(int cols, int rotationSize);
int askRotationDirection(void);
int askSaveChoice(void);

#endif
