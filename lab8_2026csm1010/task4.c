// Compile: gcc -O2 -fopenmp -Wall task4_inplace_transpose.c -o task4_inplace_transpose
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 4096

static int A[N][N];
static int A_verify[N][N];

void init_matrix(int mat[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            mat[i][j] = i * N + j;
        }
    }
}

int main() {
    init_matrix(A_verify);

    double start_seq = omp_get_wtime();
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            int temp = A_verify[i][j];
            A_verify[i][j] = A_verify[j][i];
            A_verify[j][i] = temp;
        }
    }
    double time_seq = omp_get_wtime() - start_seq;

    printf("Task 4: In-Place 2D Matrix Transpose (%dx%d)\n", N, N);
    printf("Sequential Time: %.6f s\n\n", time_seq);
    printf("%-10s %-15s %-12s %-12s %-10s\n", "Threads", "Time (s)", "Speedup", "Efficiency", "Verified");

    int thread_counts[] = {1, 2, 4, 8};
    for (int t = 0; t < 4; t++) {
        int threads = thread_counts[t];
        init_matrix(A); 

        double start_par = omp_get_wtime();
        
        #pragma omp parallel for num_threads(threads) schedule(guided, 32)
        for (int i = 0; i < N; i++) {
            for (int j = i + 1; j < N; j++) {
                int temp = A[i][j];
                A[i][j] = A[j][i];
                A[j][i] = temp;
            }
        }
        double time_par = omp_get_wtime() - start_par;

        int ok = 1;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (A[i][j] != A_verify[i][j]) {
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