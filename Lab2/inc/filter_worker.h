#ifndef FILTER_WORKER_H
#define FILTER_WORKER_H

#include <pthread.h>

typedef struct {
    float **input;
    float **erosion_result;
    float **dilation_result;
    int M, N;
    int K;
    int start_row, end_row;
} ThreadWork;

void* apply_filter_static(void* arg);

#endif
