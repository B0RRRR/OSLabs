#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matrix_utils.h"
#include "filter_worker.h"
#include "timer.h"
#include "matrix_filters.h"

static double run_filters_static(int M, int N, int K, int num_threads) {
    float **mat = create_matrix(M, N);
    float **erosion = create_matrix(M, N);
    float **dilation = create_matrix(M, N);
    fill_matrix(mat, M, N);

    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    ThreadWork *thread_data = malloc(num_threads * sizeof(ThreadWork));

    int rows_per_thread = M / num_threads;
    int extra = M % num_threads;
    int row_start = 0;

    double start = get_time_sec();

    for (int t = 0; t < num_threads; t++) {
        int row_end = row_start + rows_per_thread + (t < extra ? 1 : 0);
        thread_data[t].input = mat;
        thread_data[t].erosion_result = erosion;
        thread_data[t].dilation_result = dilation;
        thread_data[t].M = M;
        thread_data[t].N = N;
        thread_data[t].K = K;
        thread_data[t].start_row = row_start;
        thread_data[t].end_row = row_end;
        pthread_create(&threads[t], NULL, apply_filter_static, &thread_data[t]);
        row_start = row_end;
    }

    for (int t = 0; t < num_threads; t++)
        pthread_join(threads[t], NULL);

    double end = get_time_sec();

    free_matrix(mat, M);
    free_matrix(erosion, M);
    free_matrix(dilation, M);
    free(threads);
    free(thread_data);

    return end - start;
}

int run_matrix_filters(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Использование: %s M N K Tmax\n", argv[0]);
        return 1;
    }

    int M = atoi(argv[1]);
    int N = atoi(argv[2]);
    int K = atoi(argv[3]);
    int Tmax = atoi(argv[4]);

    FILE *f = fopen("results.csv", "w");
    if (!f) {
        perror("Ошибка создания файла results.csv");
        return 1;
    }
    fprintf(f, "threads,time\n");

    for (int threads = 1; threads <= Tmax; threads += 1) {
        double T = run_filters_static(M, N, K, threads);
        fprintf(f, "%d,%.6f\n", threads, T);
        printf("Потоков: %d, время: %.6f сек\n", threads, T);
    }

    fclose(f);
    printf("Результаты сохранены в results.csv\n");
    return 0;
}
