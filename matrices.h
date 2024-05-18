#include <stdio.h>
#include <stdlib.h>

#define NULLVALUE -1

typedef struct matrix *MatrixPtr;

MatrixPtr initMatrix(int rows, int columns);
MatrixPtr Sum(MatrixPtr a, MatrixPtr b);
MatrixPtr Diff(MatrixPtr a, MatrixPtr b);
MatrixPtr Mul(MatrixPtr a, MatrixPtr b);
int Det(MatrixPtr a);
void printMatrix(MatrixPtr matrix);