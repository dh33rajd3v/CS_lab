#include <stdio.h>
#include <omp.h>

#define N 4096

static int img[N][N];
static int O_seq[N][N];
static int O_par[N][N];

int main() {
    int K[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            img[i][j] = (i + j) % 256;
        }
    }

    double start_seq = omp_get_wtime();
    for (int i = 1; i < N - 1; i++) {
        for (int j = 1; j < N - 1; j++) {
            int sum = 0;
            for (int ki = -1; ki <= 1; ki++) {
                for (int kj = -1; kj <= 1; kj++) {
                    sum += img[i + ki][j + kj] * K[ki + 1][kj + 1];
                }
            }
            O_seq[i][j] = sum / 9;
        }
    }
    double time_seq = omp_get_wtime() - start_seq;

    printf("Task 5: 2D Convolution (%dx%d)\n", N, N);
    printf("Sequential Time: %.6f s\n\n", time_seq);
    printf("%-10s %-15s %-12s %-12s %-10s\n", "Threads", "Time (s)", "Speedup", "Efficiency", "Verified");

    int thread_counts[] = {1, 2, 4, 8};
    for (int t = 0; t < 4; t++) {
        int threads = thread_counts[t];

        double start_par = omp_get_wtime();
        #pragma omp parallel for collapse(2) num_threads(threads)
        for (int i = 1; i < N - 1; i++) {
            for (int j = 1; j < N - 1; j++) {
                int sum = 0;
                for (int ki = -1; ki <= 1; ki++) {
                    for (int kj = -1; kj <= 1; kj++) {
                        sum += img[i + ki][j + kj] * K[ki + 1][kj + 1];
                    }
                }
                O_par[i][j] = sum / 9;
            }
        }
        double time_par = omp_get_wtime() - start_par;

        int ok = 1;
        for (int i = 1; i < N - 1; i++) {
            for (int j = 1; j < N - 1; j++) {
                if (O_par[i][j] != O_seq[i][j]) {
                    ok = 0;
                    break;
                }
            }
            if (!ok) break;
        }

        double speedup = time_seq / time_par;
        double efficiency = (speedup / threads);

        printf("%-10d %-15.6f %-12.2fx %-11.1f%% %-10s\n",
               threads, time_par, speedup, efficiency, ok ? "PASS" : "FAIL");
    }

    return 0;
}