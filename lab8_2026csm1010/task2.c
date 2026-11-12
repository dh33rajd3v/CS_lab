#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define N 20000

void bubble_sort_seq(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

void odd_even_sort_par(int *arr, int n, int threads) {
    #pragma omp parallel num_threads(threads) default(none) shared(arr, n)
    {
        for (int phase = 0; phase < n; phase++) {
            if (phase % 2 == 0) {
                // Even phase: implicit barrier at end of omp for synchronizes threads
                #pragma omp for
                for (int i = 1; i < n; i += 2) {
                    if (arr[i - 1] > arr[i]) {
                        int tmp = arr[i - 1];
                        arr[i - 1] = arr[i];
                        arr[i] = tmp;
                    }
                }
            } else {
                // Odd phase: implicit barrier at end of omp for synchronizes threads
                #pragma omp for
                for (int i = 1; i < n - 1; i += 2) {
                    if (arr[i] > arr[i + 1]) {
                        int tmp = arr[i + 1];
                        arr[i + 1] = arr[i];
                        arr[i] = tmp;
                    }
                }
            }
        }
    }
}


int verify_sorted(int *a, int *b, int n) {
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

int main() {
    int *orig = (int *)malloc(N * sizeof(int));
    int *seq_arr = (int *)malloc(N * sizeof(int));
    int *par_arr = (int *)malloc(N * sizeof(int));
    if (!orig || !seq_arr || !par_arr) return 1;

    srand(42);
    for (int i = 0; i < N; i++) {
        orig[i] = rand() % 100000;
    }

    memcpy(seq_arr, orig, N * sizeof(int));
    double start_seq = omp_get_wtime();
    bubble_sort_seq(seq_arr, N);
    double time_seq = omp_get_wtime() - start_seq;

    printf("Task 2: Odd-Even Parallel Bubble Sort (N = %d)\n", N);
    printf("Sequential Bubble Sort Time: %.6f s\n\n", time_seq);
    printf("%-10s %-15s %-12s %-12s %-10s\n", "Threads", "Time (s)", "Speedup", "Efficiency", "Verified");

    int thread_counts[] = {1, 2, 4, 8};
    for (int t = 0; t < 4; t++) {
        int threads = thread_counts[t];
        memcpy(par_arr, orig, N * sizeof(int));

        double start_par = omp_get_wtime();
        odd_even_sort_par(par_arr, N, threads);
        double time_par = omp_get_wtime() - start_par;

        double speedup = time_seq / time_par;
        double efficiency = (speedup / threads);
        int ok = verify_sorted(seq_arr, par_arr, N);

        printf("%-10d %-15.6f %-12.2fx %-11.1f%% %-10s\n",
               threads, time_par, speedup, efficiency, ok ? "PASS" : "FAIL");
    }

    free(orig);
    free(seq_arr);
    free(par_arr);
    return 0;
}