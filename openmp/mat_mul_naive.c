// #include <omp.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <stdint.h>
// #include <math.h>

// static void zero_matrix(double *c, size_t count)
// {
//     for (size_t p = 0; p < count; ++p) c[p] = 0.0;
// }

// static void matmul_serial(const double *a, const double *b, double *c, size_t n)
// {
//     for (size_t i = 0; i < n; ++i)
//         for (size_t j = 0; j < n; ++j) {
//             double sum = 0.0;
//             for (size_t k = 0; k < n; ++k)
//                 sum += a[i * n + k] * b[k * n + j];
//             c[i * n + j] = sum;
//         }
// }

// static void matmul_parallel_i(const double *a, const double *b, double *c, size_t n)
// {
//     #pragma omp parallel for
//     for (size_t i = 0; i < n; ++i)
//         for (size_t j = 0; j < n; ++j) {
//             double sum = 0.0;
//             for (size_t k = 0; k < n; ++k)
//                 sum += a[i * n + k] * b[k * n + j];
//             c[i * n + j] = sum;
//         }
// }

// static void matmul_parallel_ij(const double *a, const double *b, double *c, size_t n)
// {
//     #pragma omp parallel for collapse(2)
//     for (size_t i = 0; i < n; ++i)
//         for (size_t j = 0; j < n; ++j) {
//             double sum = 0.0;
//             for (size_t k = 0; k < n; ++k)
//                 sum += a[i * n + k] * b[k * n + j];
//             c[i * n + j] = sum;
//         }
// }

// // bonus part
// static void matmul_serial_ikj(const double *a, const double *b, double *c, size_t n)
// {
//     for (size_t i = 0; i < n; ++i)
//         for (size_t k = 0; k < n; ++k) {
//             const double aik = a[i * n + k];
//             for (size_t j = 0; j < n; ++j)
//                 c[i * n + j] += aik * b[k * n + j];
//         }
// }

// static void matmul_parallel_ikj(const double *a, const double *b, double *c, size_t n)
// {
//     #pragma omp parallel for schedule(static)
//     for (size_t i = 0; i < n; ++i)
//         for (size_t k = 0; k < n; ++k) {
//             const double aik = a[i * n + k];
//             for (size_t j = 0; j < n; ++j)
//                 c[i * n + j] += aik * b[k * n + j];
//         }
// }

// static int matrices_match(const double *x, const double *y, size_t count)
// {
//     const double tolerance = 1e-9;
//     for (size_t i = 0; i < count; ++i)
//         if (fabs(x[i] - y[i]) > tolerance) return 0;
//     return 1;
// }

// static void fill_matrices(double *a, double *b, size_t n)
// {
//     for (size_t i = 0; i < n; ++i) {
//         for (size_t j = 0; j < n; ++j) {
//             a[i * n + j] = (double)((i * 3U + j * 7U + 1U) % 17U);
//             b[i * n + j] = (double)((i * 5U + j * 11U + 2U) % 19U);
//         }
//     }
// }

// static double run_serial(const double *a, const double *b, double *c, size_t n)
// {
//     zero_matrix(c, n * n); /* initialization is excluded from the timed interval */
//     const double t0 = omp_get_wtime();
//     matmul_serial(a, b, c, n);
//     return omp_get_wtime() - t0;
// }

// int main(int argc, char **argv)
// {
//     const size_t sizes[] = {256U, 512U, 1024U};
//     const int threads[] = {1, 2, 4, 8};

//     const size_t num_sizes = sizeof(sizes) / sizeof(sizes[0]);
//     const size_t num_threads = sizeof(threads) / sizeof(threads[0]);

//     omp_set_dynamic(0);
    
//     puts("N,threads,T_serial_s,T_parallel_i_s,speedup_i,efficiency_i,T_collapse_ij_s,speedup_ij,efficiency_ij,verified");

//     for (size_t si = 0; si < num_sizes; ++si) {

//         const size_t n = sizes[si];
//         if (n > SIZE_MAX / n || n * n > SIZE_MAX / sizeof(double)) {
//             fprintf(stderr, "Matrix size overflow for N=%zu\n", n);
//             return EXIT_FAILURE;
//         }
//         const size_t count = n * n;

//         double *a = (double *)malloc(count * sizeof(*a));
//         double *b = (double *)malloc(count * sizeof(*b));

//         double *serial = (double *)malloc(count * sizeof(*serial));
//         double *parallel = (double *)malloc(count * sizeof(*parallel));

//         if (!a || !b || !serial || !parallel) {
//             fprintf(stderr, "Allocation failed at N=%zu.\n", n);
//             free(a); free(b); free(serial); free(parallel);
//             return EXIT_FAILURE;
//         }
        
//         fill_matrices(a, b, n);


//         const double t_serial = run_serial(a, b, serial, n);

//         printf("\n\nn = %zu, tseriel = %.9f\n", n, t_serial);

//         for (size_t ti = 0; ti < num_threads; ++ti) {
//             omp_set_num_threads(threads[ti]);
//             zero_matrix(parallel, count);
//             double t0 = omp_get_wtime();
//             matmul_parallel_i(a, b, parallel, n);
//             const double t_i = omp_get_wtime() - t0;
//             if (!matrices_match(serial, parallel, count)) {
//                 fprintf(stderr, "Outer-i version mismatch at N=%zu, threads=%d\n", n, threads[ti]);
//                 free(a); free(b); free(serial); free(parallel); return EXIT_FAILURE;
//             }

//             zero_matrix(parallel, count);
//             t0 = omp_get_wtime();
//             matmul_parallel_ij(a, b, parallel, n);
//             const double t_ij = omp_get_wtime() - t0;
//             if (!matrices_match(serial, parallel, count)) {
//                 fprintf(stderr, "collapse(2) version mismatch at N=%zu, threads=%d\n", n, threads[ti]);
//                 free(a); free(b); free(serial); free(parallel); return EXIT_FAILURE;
//             }
//             const double speed_i = t_i > 0.0 ? t_serial / t_i : 0.0;
//             const double speed_ij = t_ij > 0.0 ? t_serial / t_ij : 0.0;
//             printf("%zu,%d,%.9f,%.9f,%.6f,%.6f,%.9f,%.6f,%.6f,yes\n",
//                    n, threads[ti], t_serial, t_i, speed_i, speed_i / threads[ti],
//                    t_ij, speed_ij, speed_ij / threads[ti]);
//         }

//         if (true) {
//             double *bonus_serial = (double *)malloc(count * sizeof(*bonus_serial));
//             if (!bonus_serial) {
//                 fprintf(stderr, "Allocation failed for bonus output.\n");
//                 free(a); free(b); free(serial); free(parallel); return EXIT_FAILURE;
//             }
//             zero_matrix(bonus_serial, count);
//             double tb0 = omp_get_wtime();
//             matmul_serial_ikj(a, b, bonus_serial, n);
//             const double tb_serial = omp_get_wtime() - tb0;
//             if (!matrices_match(serial, bonus_serial, count)) {
//                 fprintf(stderr, "Bonus i-k-j serial result mismatch at N=%zu\n", n);
//                 free(a); free(b); free(serial); free(parallel); free(bonus_serial); return EXIT_FAILURE;
//             }
//             puts("# Bonus i-k-j results: N,threads,T_serial_ikj_s,T_parallel_ikj_s,speedup,verified");
//             for (size_t ti = 0; ti < num_threads; ++ti) {
//                 omp_set_num_threads(threads[ti]);
//                 zero_matrix(parallel, count);
//                 const double t2 = omp_get_wtime();
//                 matmul_parallel_ikj(a, b, parallel, n);
//                 const double tp = omp_get_wtime() - t2;
//                 const int ok = matrices_match(bonus_serial, parallel, count);
//                 printf("# %zu,%d,%.9f,%.9f,%.6f,%s\n", n, threads[ti], tb_serial, tp,
//                        tp > 0.0 ? tb_serial / tp : 0.0, ok ? "yes" : "no");
//                 if (!ok) {
//                     free(a); free(b); free(serial); free(parallel); free(bonus_serial);
//                     return EXIT_FAILURE;
//                 }
//             }
//             free(bonus_serial);
//         }
//         free(a); free(b); free(serial); free(parallel);
//     }
//     return EXIT_SUCCESS;
// }

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>


static void zero_matrix(size_t n, double c[n][n])
{
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            c[i][j] = 0.0;
        }
    }
}


static void matmul_serial(
    size_t n,
    const double a[n][n],
    const double b[n][n],
    double c[n][n])
{
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            double sum = 0.0;

            for (size_t k = 0; k < n; ++k) {
                sum += a[i][k] * b[k][j];
            }

            c[i][j] = sum;
        }
    }
}


static void matmul_parallel_i(
    size_t n,
    const double a[n][n],
    const double b[n][n],
    double c[n][n])
{
    #pragma omp parallel for
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            double sum = 0.0;

            for (size_t k = 0; k < n; ++k) {
                sum += a[i][k] * b[k][j];
            }

            c[i][j] = sum;
        }
    }
}


static void matmul_parallel_ij(
    size_t n,
    const double a[n][n],
    const double b[n][n],
    double c[n][n])
{
    #pragma omp parallel for collapse(2)
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            double sum = 0.0;

            for (size_t k = 0; k < n; ++k) {
                sum += a[i][k] * b[k][j];
            }

            c[i][j] = sum;
        }
    }
}


static void matmul_serial_ikj(
    size_t n,
    const double a[n][n],
    const double b[n][n],
    double c[n][n])
{
    for (size_t i = 0; i < n; ++i) {
        for (size_t k = 0; k < n; ++k) {
            const double aik = a[i][k];

            for (size_t j = 0; j < n; ++j) {
                c[i][j] += aik * b[k][j];
            }
        }
    }
}


static void matmul_parallel_ikj(
    size_t n,
    const double a[n][n],
    const double b[n][n],
    double c[n][n])
{
    #pragma omp parallel for
    for (size_t i = 0; i < n; ++i) {
        for (size_t k = 0; k < n; ++k) {
            const double aik = a[i][k];

            for (size_t j = 0; j < n; ++j) {
                c[i][j] += aik * b[k][j];
            }
        }
    }
}


static int matrices_match(
    size_t n,
    const double x[n][n],
    const double y[n][n])
{
    const double tolerance = 1e-9;

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (fabs(x[i][j] - y[i][j]) > tolerance) {
                return 0;
            }
        }
    }

    return 1;
}


static void fill_matrices(
    size_t n,
    double a[n][n],
    double b[n][n])
{
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            a[i][j] = (double)((i * 3U + j * 7U + 1U) % 17U);
            b[i][j] = (double)((i * 5U + j * 11U + 2U) % 19U);
        }
    }
}


static double run_serial(
    size_t n,
    const double a[n][n],
    const double b[n][n],
    double c[n][n])
{

    zero_matrix(n, c);

    const double t0 = omp_get_wtime();

    matmul_serial(n, a, b, c);

    return omp_get_wtime() - t0;
}

int main(void)
{
    const size_t sizes[] = {256U, 512U, 1024U};
    const int threads[] = {1, 2, 4, 8};

    const size_t num_sizes =
        sizeof(sizes) / sizeof(sizes[0]);

    const size_t num_threads =
        sizeof(threads) / sizeof(threads[0]);

    omp_set_dynamic(0);

    puts("Matrix Multiplication");

    for (size_t si = 0; si < num_sizes; ++si) {
        const size_t n = sizes[si];

        if (n == 0 ||
            n > SIZE_MAX / n ||
            n * n > SIZE_MAX / sizeof(double)) {

            fprintf(stderr,
                    "Matrix size overflow for N=%zu\n", n);

            return EXIT_FAILURE;
        }

        double (*a)[n] = malloc(n * sizeof(*a));
        double (*b)[n] = malloc(n * sizeof(*b));
        double (*serial)[n] = malloc(n * sizeof(*serial));
        double (*parallel)[n] = malloc(n * sizeof(*parallel));

        if (a == NULL || b == NULL ||
            serial == NULL || parallel == NULL) {

            fprintf(stderr,
                    "Memory allocation failed for N=%zu\n", n);

            free(a);
            free(b);
            free(serial);
            free(parallel);

            return EXIT_FAILURE;
        }

        fill_matrices(n, a, b);

        const double t_serial =
            run_serial(n, a, b, serial);

        printf("\n\n\nN = %zu, serial time = %.9f seconds\n",
               n, t_serial);

        for (size_t ti = 0; ti < num_threads; ++ti) {
            const int num_threads_to_use = threads[ti];

            omp_set_num_threads(num_threads_to_use);

            zero_matrix(n, parallel);

            double t0 = omp_get_wtime();

            matmul_parallel_i(n, a, b, parallel);

            const double t_i = omp_get_wtime() - t0;

            if (!matrices_match(n, serial, parallel)) {
                fprintf(stderr,
                        "Outer-i mismatch at N=%zu, threads=%d\n",
                        n, num_threads_to_use);

                free(a);
                free(b);
                free(serial);
                free(parallel);

                return EXIT_FAILURE;
            }

            zero_matrix(n, parallel);

            t0 = omp_get_wtime();

            matmul_parallel_ij(n, a, b, parallel);

            const double t_ij = omp_get_wtime() - t0;

            if (!matrices_match(n, serial, parallel)) {
                fprintf(stderr,
                        "collapse(2) mismatch at N=%zu, threads=%d\n",
                        n, num_threads_to_use);

                free(a);
                free(b);
                free(serial);
                free(parallel);

                return EXIT_FAILURE;
            }

            const double speed_i =
                (t_i > 0.0) ? t_serial / t_i : 0.0;

            const double speed_ij =
                (t_ij > 0.0) ? t_serial / t_ij : 0.0;

            const double efficiency_i =
                speed_i / num_threads_to_use;

            const double efficiency_ij =
                speed_ij / num_threads_to_use;

            printf("\n\nno of threads = %d,time only i parallel = %.9f, time both i and j parallel = %.9f\n", threads[ti], t_i, t_ij);

            printf("speedup with i parallel = %.6f,\nparallel efficiency with i parallel = %.6f,"
                   "\nspeedup with i and j parallel = %.6f,\nparallel efficiency with i and j parallel = %.6f\n",
                   speed_i,
                   efficiency_i,
                   speed_ij,
                   efficiency_ij);
        }

        double (*bonus_serial)[n] =
            malloc(n * sizeof(*bonus_serial));

        if (bonus_serial == NULL) {
            fprintf(stderr,
                    "Bonus allocation failed for N=%zu\n", n);

            free(a);
            free(b);
            free(serial);
            free(parallel);

            return EXIT_FAILURE;
        }

        zero_matrix(n, bonus_serial);

        double tb0 = omp_get_wtime();

        matmul_serial_ikj(n, a, b, bonus_serial);

        const double tb_serial = omp_get_wtime() - tb0;

        if (!matrices_match(n, serial, bonus_serial)) {
            fprintf(stderr,
                    "Serial i-k-j mismatch at N=%zu\n", n);

            free(a);
            free(b);
            free(serial);
            free(parallel);
            free(bonus_serial);

            return EXIT_FAILURE;
        }


        printf("\nBonus i-k-j results for N=%zu\n", n);
        // puts("N,threads,T_serial_ikj_s,T_parallel_ikj_s,"
        //      "speedup,verified");

        for (size_t ti = 0; ti < num_threads; ++ti) {
            const int num_threads_to_use = threads[ti];

            omp_set_num_threads(num_threads_to_use);

            zero_matrix(n, parallel);

            const double tb1 = omp_get_wtime();

            matmul_parallel_ikj(n, a, b, parallel);

            const double tp = omp_get_wtime() - tb1;

            const int ok =
                matrices_match(n, bonus_serial, parallel);

            const double speedup =
                (tp > 0.0) ? tb_serial / tp : 0.0;

            // printf("%zu,%d,%.9f,%.9f,%.6f,%s\n",
            //        n,
            //        num_threads_to_use,
            //        tb_serial,
            //        tp,
            //        speedup,
            //        ok ? "yes" : "no");


            printf("\nno of threads = %d,time parallel = %.9f, speedup = %6f", threads[ti], tp, speedup);

            if (!ok) {
                fprintf(stderr,
                        "Parallel i-k-j mismatch at N=%zu, "
                        "threads=%d\n",
                        n, num_threads_to_use);

                free(a);
                free(b);
                free(serial);
                free(parallel);
                free(bonus_serial);

                return EXIT_FAILURE;
            }
        }

        free(a);
        free(b);
        free(serial);
        free(parallel);
        free(bonus_serial);
    }

    return EXIT_SUCCESS;
}