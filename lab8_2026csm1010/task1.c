#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 2147483647
#define TARGET_INDEX 3750
#define TARGET_VAL 999999

int main() {
    // int N;
    // printf("Enter the no of array elements");
    // scanf("%d", &N);
    int *arr = (int *)malloc(N * sizeof(int));
    if (!arr) return 1;

    for (int i = 0; i < N; i++) {
        arr[i] = i % 1000;
    }

    arr[TARGET_INDEX] = TARGET_VAL;

    // 1. Sequential Search

    double start_seq = omp_get_wtime();
    int seq_index = -1;
    for (int i = 0; i < N; i++) {
        if (arr[i] == TARGET_VAL) {
            seq_index = i;
            break;
        }
    }
    double time_seq = omp_get_wtime() - start_seq;

    printf("Array Search (N = %d)\n", N);
    printf("Sequential: Found %d at index %d in %.6f s\n\n", TARGET_VAL, seq_index, time_seq);
    printf("%-10s %-15s %-12s %-12s %-10s\n", "Threads", "Time (s)", "Speedup", "Efficiency", "Verified");

    int thread_counts[] = {1, 2, 4, 8};
    for (int t = 0; t < 4; t++) {
        int threads = thread_counts[t];
        int par_index = -1;

        double start_par = omp_get_wtime();
        #pragma omp parallel for num_threads(threads) shared(arr, par_index)
        for (int i = 0; i < N; i++) {
            if (arr[i] == TARGET_VAL) {
                #pragma omp critical
                {
                    if (par_index == -1 || i < par_index) {
                        par_index = i;
                    }
                }
            }
        }

        double time_par = omp_get_wtime() - start_par;
        double speedup = time_seq / time_par;
        double efficiency = (speedup / threads);

        printf("%-10d %-15.6f %-12.2fx %-11.1f%% %-10s\n",
               threads, time_par, speedup, efficiency,
               (par_index == seq_index) ? "PASS" : "FAIL");
    }

    free(arr);
    return 0;
}