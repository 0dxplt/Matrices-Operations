#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

// Assuming Matrix is defined somewhere as:
typedef struct
{
    int rows;
    int columns;
    float **matrix;
} Matrix, *MatrixPtr;

// Function to calculate the optimal block size
int calculateOptimalBlockSize(int matrixSize)
{
    // You can fine-tune this formula based on your cache size and performance testing
    // A common heuristic is to use the square root of the matrix size divided by the number of processors
    int numThreads = omp_get_max_threads();
    int blockSize = (int)sqrt((double)(matrixSize / numThreads));
    return blockSize > 0 ? blockSize : 1; // Ensure block size is at least 1
}

MatrixPtr Mul(MatrixPtr a, MatrixPtr b)
{
    // Ensure the matrices can be multiplied
    if (a->columns != b->rows)
    {
        return NULL;
    }

    // Allocate memory for the result matrix structure
    MatrixPtr result = (MatrixPtr)malloc(sizeof(Matrix));
    if (!result)
    {
        return NULL;
    }

    result->rows = a->rows;
    result->columns = b->columns;

    // Allocate memory for the result matrix
    result->matrix = (float **)malloc(result->rows * sizeof(float *));
    if (!result->matrix)
    {
        free(result);
        return NULL;
    }

    // Initialize result matrix to zeros
    for (int i = 0; i < result->rows; i++)
    {
        result->matrix[i] = (float *)calloc(result->columns, sizeof(float));
        if (!result->matrix[i])
        {
            // Clean up allocated memory
            for (int j = 0; j < i; j++)
            {
                free(result->matrix[j]);
            }
            free(result->matrix);
            free(result);
            return NULL;
        }
    }

    // Transpose matrix b for better memory access patterns
    MatrixPtr bTranspose = (MatrixPtr)malloc(sizeof(Matrix));
    if (!bTranspose)
    {
        // Clean up allocated memory
        for (int i = 0; i < result->rows; i++)
        {
            free(result->matrix[i]);
        }
        free(result->matrix);
        free(result);
        return NULL;
    }

    bTranspose->rows = b->columns;
    bTranspose->columns = b->rows;
    bTranspose->matrix = (float **)malloc(bTranspose->rows * sizeof(float *));
    if (!bTranspose->matrix)
    {
        // Clean up allocated memory
        for (int i = 0; i < result->rows; i++)
        {
            free(result->matrix[i]);
        }
        free(result->matrix);
        free(result);
        free(bTranspose);
        return NULL;
    }

    for (int i = 0; i < bTranspose->rows; i++)
    {
        bTranspose->matrix[i] = (float *)malloc(bTranspose->columns * sizeof(float));
        if (!bTranspose->matrix[i])
        {
            // Clean up allocated memory
            for (int j = 0; j < i; j++)
            {
                free(bTranspose->matrix[j]);
            }
            free(bTranspose->matrix);
            free(bTranspose);
            // Clean up allocated memory for result matrix
            for (int j = 0; j < result->rows; j++)
            {
                free(result->matrix[j]);
            }
            free(result->matrix);
            free(result);
            return NULL;
        }
    }

    // Transpose matrix b
    for (int i = 0; i < b->rows; i++)
    {
        for (int j = 0; j < b->columns; j++)
        {
            bTranspose->matrix[j][i] = b->matrix[i][j];
        }
    }

// Perform matrix multiplication
#pragma omp parallel for
    for (int i = 0; i < result->rows; i++)
    {
        for (int j = 0; j < result->columns; j++)
        {
            float sum = 0.0;
            for (int k = 0; k < a->columns; k++)
            {
                sum += a->matrix[i][k] * bTranspose->matrix[j][k];
            }
            result->matrix[i][j] = sum;
        }
    }

    // Clean up allocated memory for transpose matrix b
    for (int i = 0; i < bTranspose->rows; i++)
    {
        free(bTranspose->matrix[i]);
    }
    free(bTranspose->matrix);
    free(bTranspose);

    return result;
}

// Example usage
// Example usage
int main()
{
    // Example matrices a and b should be initialized here
    // For demonstration, initializing two matrices a and b manually

    // Matrix a (2x3)
    Matrix a;
    a.rows = 10000;
    a.columns = 10000;
    a.matrix = (float **)malloc(a.rows * sizeof(float *));
    for (int i = 0; i < a.rows; i++)
    {
        a.matrix[i] = (float *)malloc(a.columns * sizeof(float));
        for (int j = 0; j < a.columns; j++)
        {
            a.matrix[i][j] = (i + 1) * (j + 1);
        }
    }

    // Matrix b (3x2)
    Matrix b;
    b.rows = 10000;
    b.columns = 10000;
    b.matrix = (float **)malloc(b.rows * sizeof(float *));
    for (int i = 0; i < b.rows; i++)
    {
        b.matrix[i] = (float *)malloc(b.columns * sizeof(float));
        for (int j = 0; j < b.columns; j++)
        {
            b.matrix[i][j] = (i + 1) + (j + 1);
        }
    }

    // Measure time before calling Mul function
    double start_time = omp_get_wtime();

    // Multiply matrices
    MatrixPtr result = Mul(&a, &b);

    // Measure time after calling Mul function
    double end_time = omp_get_wtime();

    // Print elapsed time
    printf("Time taken for matrix multiplication: %f seconds\n", end_time - start_time);

    if (result)
    {
        // Free result matrix
        free(result->matrix[0]); // Free contiguous data block
        free(result->matrix);    // Free row pointers
        free(result);
    }

    // Free matrix a
    for (int i = 0; i < a.rows; i++)
    {
        free(a.matrix[i]);
    }
    free(a.matrix);

    // Free matrix b
    for (int i = 0; i < b.rows; i++)
    {
        free(b.matrix[i]);
    }
    free(b.matrix);

    return 0;
}
