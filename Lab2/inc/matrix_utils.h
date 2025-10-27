#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include <stdlib.h>

float** create_matrix(int M, int N);
void fill_matrix(float** mat, int M, int N);
void free_matrix(float** mat, int M);

#endif
