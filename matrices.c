#include <math.h>
#include "matrices.h"

typedef struct matrix
{
    float **matrix;
    unsigned int rows;
    unsigned int columns;
} Matrix;

typedef Matrix *MatrixPtr;

MatrixPtr initMatrix(int rows, int columns)
{
    MatrixPtr matrix = (MatrixPtr)malloc(sizeof(Matrix));
    float **m = (float **)malloc(rows * sizeof(float *));
    matrix->rows = rows;
    matrix->columns = columns;
    for (int i = 0; i < rows; i++)
    {
        m[i] = (float *)malloc(columns * sizeof(float));
        for (int j = 0; j < columns; j++)
        {
            m[i][j] = (float)i * columns + j;
        }
    }
    matrix->matrix = m;
    return matrix;
}

MatrixPtr Sum(MatrixPtr a, MatrixPtr b)
{
    if (a->columns == b->columns && a->rows == b->rows)
    {
        MatrixPtr Sum = (MatrixPtr)malloc(sizeof(Matrix));
        Sum->columns = a->columns;
        Sum->rows = a->rows;
        float **s = (float **)malloc(Sum->rows * sizeof(float *));
        for (int i = 0; i < Sum->rows; i++)
        {
            s[i] = (float *)malloc(Sum->columns * sizeof(float));
            for (int j = 0; j < Sum->columns; j++)
            {
                s[i][j] = a->matrix[i][j] + b->matrix[i][j];
            }
        }
        Sum->matrix = s;
        return Sum;
    }
    else
    {
        return NULL;
    }
}

MatrixPtr Diff(MatrixPtr a, MatrixPtr b)
{
    if (a->columns == b->columns && a->rows == b->rows)
    {
        MatrixPtr Diff = (MatrixPtr)malloc(sizeof(Matrix));
        Diff->columns = a->columns;
        Diff->rows = a->rows;
        float **d = (float **)malloc(Diff->rows * sizeof(float *));
        for (int i = 0; i < Diff->rows; i++)
        {
            d[i] = (float *)malloc(Diff->columns * sizeof(float));
            for (int j = 0; j < Diff->columns; j++)
            {
                d[i][j] = a->matrix[i][j] - b->matrix[i][j];
            }
        }
        Diff->matrix = d;
        return Diff;
    }
    else
    {
        return NULL;
    }
}

MatrixPtr Mul(MatrixPtr a, MatrixPtr b)
{
    if (a->columns == b->rows)
    {
        MatrixPtr Mul = (MatrixPtr)malloc(sizeof(Matrix));
        Mul->rows = a->rows;
        Mul->columns = b->columns;
        float **m = (float **)malloc(Mul->rows * sizeof(float *));
        for (int i = 0; i < Mul->rows; i++)
        {
            m[i] = (float *)malloc(Mul->columns * sizeof(float));
            for (int j = 0; j < Mul->columns; j++)
            {
                m[i][j] = 0;
                for (int k = 0; k < a->columns; k++)
                {
                    m[i][j] += a->matrix[i][k] * b->matrix[k][j];
                }
            }
        }
        Mul->matrix = m;
        return Mul;
    }
    else
    {
        return NULL;
    }
}

MatrixPtr Cofactor(MatrixPtr a, int y)
{
    MatrixPtr c = (MatrixPtr)malloc(sizeof(Matrix));
    c->rows = a->rows - 1;
    c->columns = a->columns - 1;
    float **cofactor = (float **)malloc(c->rows * sizeof(float *));
    for (int i = 0; i < c->rows; i++)
    {
        cofactor[i] = (float *)malloc(c->columns * sizeof(float));
        for (int j = 0, k = 0; j < c->rows; k++)
        {
            if (k != y)
            {
                cofactor[i][j] = a->matrix[i + 1][k];
                j++;
            }
        }
    }
    c->matrix = cofactor;
    return c;
}

int Det(MatrixPtr a)
{
    if (a->rows != a->columns)
        return NULLVALUE;
    else if (a->rows == 2)
    {
        return (a->matrix[0][0] * a->matrix[1][1]) - (a->matrix[0][1] * a->matrix[1][0]);
    }
    else if (a->rows == 3)
    {
        return (a->matrix[0][0] * ((a->matrix[1][1] * a->matrix[2][2]) - (a->matrix[1][2] * a->matrix[2][1]))) - (a->matrix[0][1] * ((a->matrix[1][0] * a->matrix[2][2]) - (a->matrix[1][2] * a->matrix[2][0]))) + (a->matrix[0][2] * ((a->matrix[1][0] * a->matrix[2][1]) - (a->matrix[1][1] * a->matrix[2][0])));
    }
    else
    {
        int det = 0;
        for (int i = 0; i < a->columns; i++)
        {
            det += a->matrix[0][i] * pow(-1, i) * Det(Cofactor(a, i));
        }
        return det;
    }
}

void printMatrix(MatrixPtr matrix)
{
    if (matrix == NULL)
    {
        return;
    }
    else
    {
        printf("[\n");
        for (int i = 0; i < matrix->rows; i++)
        {
            for (int j = 0; j < matrix->columns; j++)
            {
                printf("%f ", matrix->matrix[i][j]);
            }
            printf("\n");
        }
        printf("], (%d, %d)\n", matrix->rows, matrix->columns);
    }
}