#include "filter_worker.h"
#include <float.h>

static inline float min_in_3x3(float **mat, int M, int N, int i, int j) {
    float min_val = FLT_MAX;
    for (int di = -1; di <= 1; di++)
        for (int dj = -1; dj <= 1; dj++) {
            int ni = i + di;
            int nj = j + dj;
            if (ni >= 0 && ni < M && nj >= 0 && nj < N)
                if (mat[ni][nj] < min_val)
                    min_val = mat[ni][nj];
        }
    return min_val;
}

static inline float max_in_3x3(float **mat, int M, int N, int i, int j) {
    float max_val = -FLT_MAX;
    for (int di = -1; di <= 1; di++)
        for (int dj = -1; dj <= 1; dj++) {
            int ni = i + di;
            int nj = j + dj;
            if (ni >= 0 && ni < M && nj >= 0 && nj < N)
                if (mat[ni][nj] > max_val)
                    max_val = mat[ni][nj];
        }
    return max_val;
}

// Теперь каждый поток получает свои строки
void* apply_filter_static(void* arg) {
    ThreadWork *data = (ThreadWork*)arg;

    for (int rep = 0; rep < data->K; rep++) {
        for (int i = data->start_row; i < data->end_row; i++)
            for (int j = 0; j < data->N; j++) {
                data->erosion_result[i][j] = min_in_3x3(data->input, data->M, data->N, i, j);
                data->dilation_result[i][j] = max_in_3x3(data->input, data->M, data->N, i, j);
            }
    }
    return NULL;
}
