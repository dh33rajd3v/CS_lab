#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void vec_add_serial(double *a, double *b, double *c, long n) {
    for (long i = 0; i < n; i++){
        c[i] = a[i] + b[i];
    }
}
void vec_add_parallel(double *a, double *b, double *c, long n) {
    #pragma omp parallel for
    for (long i = 0; i < n; i++){
        c[i] = a[i] + b[i];
    }
}

 int arrays_equal(double *x, double *y, long n)
{
    for (long i = 0; i < n; ++i)
        if (x[i] != y[i])
            return 0;
    return 1;
}

double timed_serial(double *a, double *b, double *c, long n)
{
    double t0 = omp_get_wtime();
    vec_add_serial(a, b, c, n);
    return omp_get_wtime() - t0;
}

double timed_parallel(double *a, double *b, double *c, long n, int threads)
{
    omp_set_num_threads(threads);
    double t0 = omp_get_wtime();
    vec_add_parallel(a, b, c, n);
    return omp_get_wtime() - t0;
}

int main(int argc, char *argv[]) 
{
    // long n = 94665727;
    long full_sizes[] = {100000, 1000000, 10000000, 100000000};
    int threads[] = {1, 2, 4, 8};
    int num_sizes = sizeof(full_sizes) / sizeof(full_sizes[0]);
    int num_threads = sizeof(threads) / sizeof(threads[0]);

    for(int i=0; i<num_sizes; i++){
        long n=full_sizes[i];
        double *a = malloc(n * sizeof(double));
        double *b = malloc(n * sizeof(double));
        double *c_serial = malloc(n * sizeof(double));
        double *c_parallel = malloc(n * sizeof(double));

        for (long i = 0; i < n; ++i) {
                a[i] = (double)(i % 1000U);
                b[i] = (double)((i * 7U) % 1000U);
            }

        double t_serial = timed_serial(a, b, c_serial, n);

        printf("\n\nn = %zu, time for seriel = %.9f\n", n, t_serial);

        for (int ti = 0; ti < num_threads; ++ti) {
            double t_parallel = timed_parallel(a, b, c_parallel, n, threads[ti]);
            int ok = arrays_equal(c_serial, c_parallel, n);
            if (!ok) {
                fprintf(stderr, "Correctness check failed for N=%zu, threads=%d\n", n, threads[ti]);
                free(a); free(b); free(c_serial); free(c_parallel);
                return EXIT_FAILURE;
            }
            printf("no of threads = %d, time for parallel = %.9f, ", threads[ti], t_parallel);
            printf("Speedup = %6f\n", t_serial/t_parallel);
        }
        free(a); free(b); free(c_serial); free(c_parallel);
    }
    return EXIT_SUCCESS;
}