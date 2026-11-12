#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define CHUNK_SIZE 100

void init_array(double *a, int n) {
    for (int i = 0; i < n; i++)
        a[i] = i * 0.001;
}

double run_serial(double *a, int n) {
    init_array(a, n);
    double start = omp_get_wtime();
    for (int i = 0; i < n; i++) {
        double x = a[i];
        int work = (i % 1000) * 100;
        for (int j = 0; j < work; j++)
            x = x * 2.0001 + 0.0001;
        a[i] = x;
    }
    return omp_get_wtime() - start;
}

double run_static(double *a, int n, int threads) {
    init_array(a, n);
    omp_set_num_threads(threads);
    double start = omp_get_wtime();

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < n; i++) {
        double x = a[i];
        int work = (i % 1000) * 100;
        for (int j = 0; j < work; j++)
            x = x * 2.0001 + 0.0001;
        a[i] = x;
    }
    return omp_get_wtime() - start;
}

double run_dynamic(double *a, int n, int threads) {
    init_array(a, n);
    omp_set_num_threads(threads);
    double start = omp_get_wtime();

    #pragma omp parallel for schedule(dynamic, CHUNK_SIZE)
    for (int i = 0; i < n; i++) {
        double x = a[i];
        int work = (i % 1000) * 100;
        for (int j = 0; j < work; j++)
            x = x * 2.0001 + 0.0001;
        a[i] = x;
    }
    return omp_get_wtime() - start;
}

double run_guided(double *a, int n, int threads) {
    init_array(a, n);
    omp_set_num_threads(threads);
    double start = omp_get_wtime();

    #pragma omp parallel for schedule(guided)
    for (int i = 0; i < n; i++) {
        double x = a[i];
        int work = (i % 1000) * 100;
        for (int j = 0; j < work; j++)
            x = x * 2.0001 + 0.0001;
        a[i] = x;
    }
    return omp_get_wtime() - start;
}

int main(void) {
    int sizes[] = {100000, 1000000, 10000000}; // 10^5, 10^6, 10^7
    int threads[] = {1, 2, 4, 8};

    printf("====================================================================================\n");
    printf("TASK 2: LOOP SCHEDULING COMPARISON (STATIC vs DYNAMIC(%d) vs GUIDED)\n", CHUNK_SIZE);
    printf("====================================================================================\n");
    printf("%-10s | %-12s | %-8s | %-12s | %-10s | %-12s\n", 
           "Size (N)", "Schedule", "Threads", "Time (s)", "Speedup", "Efficiency");
    printf("------------------------------------------------------------------------------------\n");

    for (int s = 0; s < 3; s++) {
        int n = sizes[s];
        double *a = (double *)malloc(n * sizeof(double));

        double t_serial = run_serial(a, n);
        printf("%-10d | %-12s | %-8s | %-12.6f | %-10s | %-12s\n", 
               n, "Serial", "1", t_serial, "1.00x", "100.0%");

        for (int t = 0; t < 4; t++) {
            int p = threads[t];

            double t_stat = run_static(a, n, p);
            printf("%-10d | %-12s | %-8d | %-12.6f | %-9.2fx | %-11.1f%%\n", 
                   n, "static", p, t_stat, t_serial / t_stat, ((t_serial / t_stat) / p) * 100.0);

            double t_dyn = run_dynamic(a, n, p);
            printf("%-10d | %-12s | %-8d | %-12.6f | %-9.2fx | %-11.1f%%\n", 
                   n, "dynamic,100", p, t_dyn, t_serial / t_dyn, ((t_serial / t_dyn) / p) * 100.0);

            double t_guid = run_guided(a, n, p);
            printf("%-10d | %-12s | %-8d | %-12.6f | %-9.2fx | %-11.1f%%\n", 
                   n, "guided", p, t_guid, t_serial / t_guid, ((t_serial / t_guid) / p) * 100.0);
        }
        printf("------------------------------------------------------------------------------------\n");
        free(a);
    }

    return 0;
}