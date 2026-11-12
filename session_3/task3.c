#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void count_serial(int *a, long n, long *even_count, long *odd_count) {
    long even = 0, odd = 0;
    for (long i = 0; i < n; i++) {
        if (a[i] % 2 == 0)
            even++;
        else
            odd++;
    }
    *even_count = even;
    *odd_count = odd;
}

void count_reduction(int *a, long n, long *even_count, long *odd_count) {
    long even = 0, odd = 0;

    #pragma omp parallel for reduction(+:even, odd)
    for (long i = 0; i < n; i++) {
        if (a[i] % 2 == 0)
            even++;
        else
            odd++;
    }

    *even_count = even;
    *odd_count = odd;
}

void count_atomic(int *a, long n, long *even_count, long *odd_count) {
    long even = 0, odd = 0;

    #pragma omp parallel for shared(even, odd)
    for (long i = 0; i < n; i++) {
        if (a[i] % 2 == 0) {
            #pragma omp atomic
            even++;
        } else {
            #pragma omp atomic
            odd++;
        }
    }

    *even_count = even;
    *odd_count = odd;
}

int main(void) {
    long sizes[] = {100000L, 1000000L, 10000000L, 100000000L}; // 10^5, 10^6, 10^7, 10^8
    int threads[] = {1, 2, 4, 8};

    printf("====================================================================================================\n");
    printf("TASK 3: MULTIPLE REDUCTIONS vs ATOMIC UPDATES (EVEN / ODD COUNT)\n");
    printf("====================================================================================================\n");
    printf("%-10s | %-10s | %-7s | %-10s | %-9s | %-10s | %-12s | %-8s\n", 
           "Size (N)", "Method", "Threads", "Time (s)", "Speedup", "Efficiency", "Even+Odd==N", "Match");
    printf("----------------------------------------------------------------------------------------------------\n");

    for (int s = 0; s < 4; s++) {
        long n = sizes[s];
        int *a = (int *)malloc(n * sizeof(int));

        srand(42);
        for (long i = 0; i < n; i++) {
            a[i] = rand();
        }

        // 1. Serial Baseline
        long s_even = 0, s_odd = 0;
        double start_s = omp_get_wtime();
        count_serial(a, n, &s_even, &s_odd);
        double t_serial = omp_get_wtime() - start_s;

        printf("%-10ld | %-10s | %-7s | %-10.6f | %-9s | %-10s | %-12s | %-8s\n", 
               n, "Serial", "1", t_serial, "1.00x", "100.0%", 
               (s_even + s_odd == n) ? "YES" : "NO", "Baseline");

        // 2. Parallel Reduction & Atomic across threads
        for (int t = 0; t < 4; t++) {
            int p = threads[t];
            omp_set_num_threads(p);

            // Reduction
            long r_even = 0, r_odd = 0;
            double start_r = omp_get_wtime();
            count_reduction(a, n, &r_even, &r_odd);
            double t_red = omp_get_wtime() - start_r;
            int r_match = (r_even == s_even && r_odd == s_odd);

            printf("%-10ld | %-10s | %-7d | %-10.6f | %-8.2fx | %-9.1f%% | %-12s | %-8s\n", 
                   n, "Reduction", p, t_red, t_serial / t_red, ((t_serial / t_red) / p) * 100.0,
                   (r_even + r_odd == n) ? "YES" : "NO", r_match ? "PASS" : "FAIL");

            // Atomic
            long a_even = 0, a_odd = 0;
            double start_a = omp_get_wtime();
            count_atomic(a, n, &a_even, &a_odd);
            double t_atom = omp_get_wtime() - start_a;
            int a_match = (a_even == s_even && a_odd == s_odd);

            printf("%-10ld | %-10s | %-7d | %-10.6f | %-8.2fx | %-9.1f%% | %-12s | %-8s\n", 
                   n, "Atomic", p, t_atom, t_serial / t_atom, ((t_serial / t_atom) / p) * 100.0,
                   (a_even + a_odd == n) ? "YES" : "NO", a_match ? "PASS" : "FAIL");
        }
        printf("----------------------------------------------------------------------------------------------------\n");
        free(a);
    }

    return 0;
}