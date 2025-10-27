#include "matrix_utils.h"
#include <stdio.h>
#include <time.h>

float** create_matrix(int M, int N) {
    float **mat = malloc(M * sizeof(float*));
    for (int i = 0; i < M; i++)
        mat[i] = malloc(N * sizeof(float));
    return mat;
}

void fill_matrix(float** mat, int M, int N) {
    srand(time(NULL));
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            mat[i][j] = (float)(rand()) / RAND_MAX;
}

void free_matrix(float** mat, int M) {
    for (int i = 0; i < M; i++)
        free(mat[i]);
    free(mat);
}
