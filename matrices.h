#include <stdio.h>
#include <stdlib.h>

typedef struct matrix *MatrixPtr;

MatrixPtr initMatrix(int rows, int columns);
MatrixPtr Sum(MatrixPtr matrix, MatrixPtr b);
MatrixPtr Diff(MatrixPtr matrix, MatrixPtr b);
MatrixPtr Mul(MatrixPtr matrix, MatrixPtr b);
MatrixPtr Square(MatrixPtr matrix);
MatrixPtr Reduce(MatrixPtr matrix);
int Rank(MatrixPtr matrix);
int Det(MatrixPtr matrix);
size_t isTriangularI(MatrixPtr matrix);
size_t isTriangularS(MatrixPtr matrix);
size_t isTriangular(MatrixPtr matrix);
void printMatrix(MatrixPtr matrix);