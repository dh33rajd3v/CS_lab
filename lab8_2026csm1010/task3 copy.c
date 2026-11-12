#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 100000000
#define BINS 10

int main() {
    int *arr = (int *)malloc(N * sizeof(int));
    if (!arr) return 1;

    srand(42);
    for (int i = 0; i < N; i++) {
        arr[i] = i % BINS;
    }

    int seq_hist[BINS] = {0};
    double start_seq = omp_get_wtime();
    for (int i = 0; i < N; i++) {
        seq_hist[arr[i]]++;
    }
    double time_seq = omp_get_wtime() - start_seq;

    printf("Task 3: Histogram Calculation (N = %d)\n", N);
    printf("Sequential Time: %.6f s\n\n", time_seq);
    printf("%-10s %-15s %-12s %-12s %-10s\n", "Threads", "Time (s)", "Speedup", "Efficiency", "Verified");

    int thread_counts[] = {1, 2, 4, 8};
    for (int t = 0; t < 4; t++) {
        int threads = thread_counts[t];
        int par_hist[BINS] = {0};

        double start_par = omp_get_wtime();

        #pragma omp parallel num_threads(threads)
        {
            int local_hist[BINS] = {0};

            #pragma omp for nowait
            for (int i = 0; i < N; i++) {
                local_hist[arr[i]]++;
            }

            #pragma omp critical
            {
                for (int b = 0; b < BINS; b++) {
                    par_hist[b] += local_hist[b];
                }
            }
        }
        double time_par = omp_get_wtime() - start_par;

        int ok = 1;
        for (int b = 0; b < BINS; b++) {
            if (par_hist[b] != seq_hist[b]) ok = 0;
        }

        double speedup = time_seq / time_par;
        double efficiency = (speedup / threads);

        printf("%-10d %-15.6f %-12.2fx %-11.1f%% %-10s\n",
               threads, time_par, speedup, efficiency, ok ? "PASS" : "FAIL");
    }

    printf("\nVerified Bin Frequencies:\n");
    for (int b = 0; b < BINS; b++) {
        printf("Bin %d: %d\n", b, seq_hist[b]);
    }

    free(arr);
    return 0;
}