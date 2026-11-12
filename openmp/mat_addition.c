// #include <omp.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <stdint.h>

// static void matrix_add_serial(const double *a, const double *b, double *c, size_t n)
// {
//     for (size_t i = 0; i < n; ++i)
//         for (size_t j = 0; j < n; ++j) {
//             const size_t p = i * n + j;
//             c[p] = a[p] + b[p];
//         }
// }

// static void matrix_add_outer(const double *a, const double *b, double *c, size_t n)
// {
//     #pragma omp parallel for
//     for (size_t i = 0; i < n; ++i)
//         for (size_t j = 0; j < n; ++j) {
//             const size_t p = i * n + j;
//             c[p] = a[p] + b[p];
//         }
// }

// static void matrix_add_collapse(const double *a, const double *b, double *c, size_t n)
// {
//     #pragma omp parallel for collapse(2)
//     for (size_t i = 0; i < n; ++i)
//         for (size_t j = 0; j < n; ++j) {
//             const size_t p = i * n + j;
//             c[p] = a[p] + b[p];
//         }
// }

// static int arrays_equal(const double *x, const double *y, size_t count)
// {
//     for (size_t i = 0; i < count; ++i)
//         if (x[i] != y[i]) return 0;
//     return 1;
// }

// static double time_serial(const double *a, const double *b, double *c, size_t n)
// {
//     const int repeats = n <= 256U ? 5 : 1;
//     double total = 0.0;
//     for (int r = 0; r < repeats; ++r) {
//         const double t0 = omp_get_wtime();
//         matrix_add_serial(a, b, c, n);
//         total += omp_get_wtime() - t0;
//     }
//     return total / repeats;
// }

// int main(int argc, char **argv)
// {
//     const size_t sizes[] = {1024U, 4096U};
//     const int threads[] = {1, 2, 4, 8};
//     const size_t num_sizes = sizeof(sizes) / sizeof(sizes[0]);
//     const size_t num_threads = sizeof(threads) / sizeof(threads[0]);

//     omp_set_dynamic(0);
//     puts("Task 4: matrix addition; matrices are flattened row-major arrays");
//     // puts("N,threads,T_serial_s,T_outer_s,T_collapse_s,speedup_outer,speedup_collapse,verified");

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
//             fprintf(stderr, "Allocation failed for N=%zu (four matrices require about %.1f MB).\n",
//                     n, (double)count * sizeof(double) * 4.0 / (1024.0 * 1024.0));
//             free(a); free(b); free(serial); free(parallel);
//             return EXIT_FAILURE;
//         }

//         for (size_t i = 0; i < count; ++i) {
//             a[i] = (double)((i * 3U + 7U) % 97U);
//             b[i] = (double)((i * 5U + 11U) % 89U);
//         }

//         const double t_serial = time_serial(a, b, serial, n);


//         printf("\n\nn = %zu, tseriel = %.9f\n", n, t_serial);

//         for (size_t ti = 0; ti < num_threads; ++ti) {
//             omp_set_num_threads(threads[ti]);

//             const double t0 = omp_get_wtime();
//             matrix_add_outer(a, b, parallel, n);
//             const double t_outer = omp_get_wtime() - t0;

//             if (!arrays_equal(serial, parallel, count)) {
//                 fprintf(stderr, "Outer-loop result mismatch at N=%zu, threads=%d\n", n, threads[ti]);
//                 free(a); free(b); free(serial); free(parallel); return EXIT_FAILURE;
//             }

//             const double t1 = omp_get_wtime();
//             matrix_add_collapse(a, b, parallel, n);
//             const double t_collapse = omp_get_wtime() - t1;

//             if (!arrays_equal(serial, parallel, count)) {
//                 fprintf(stderr, "collapse(2) result mismatch at N=%zu, threads=%d\n", n, threads[ti]);
//                 free(a); free(b); free(serial); free(parallel); return EXIT_FAILURE;
//             }

//             printf("no of threads = %d,time outer = %.9f, time collapse = %.9f\n", threads[ti], t_outer, t_collapse);
//         }
//         free(a); free(b); free(serial); free(parallel);
//     }
//     return EXIT_SUCCESS;
// }

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static void matrix_add_serial(
    size_t n,
    double a[n][n],
    double b[n][n],
    double c[n][n])
{
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
}

static void matrix_add_outer(
    size_t n,
    double a[n][n],
    double b[n][n],
    double c[n][n])
{
    #pragma omp parallel for
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
}

static void matrix_add_collapse(
    size_t n,
    double a[n][n],
    double b[n][n],
    double c[n][n])
{
    #pragma omp parallel for collapse(2)
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
}

static int matrices_equal(
    size_t n,
    double x[n][n],
    double y[n][n])
{
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (x[i][j] != y[i][j]) {
                return 0;
            }
        }
    }

    return 1;
}

static double time_serial(
    size_t n,
    double a[n][n],
    double b[n][n],
    double c[n][n])
{
    double start = omp_get_wtime();

    matrix_add_serial(n, a, b, c);

    return omp_get_wtime() - start;
}

static double time_outer(
    size_t n,
    double a[n][n],
    double b[n][n],
    double c[n][n])
{
    double start = omp_get_wtime();

    matrix_add_outer(n, a, b, c);

    return omp_get_wtime() - start;
}

static double time_collapse(
    size_t n,
    double a[n][n],
    double b[n][n],
    double c[n][n])
{
    double start = omp_get_wtime();

    matrix_add_collapse(n, a, b, c);
    

    return omp_get_wtime() - start;
}

int main(void)
{
    const size_t sizes[] = {128U, 1024U, 4096U};
    const int threads[] = {1, 2, 4, 8};

    const size_t num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    const size_t num_threads = sizeof(threads) / sizeof(threads[0]);

    omp_set_dynamic(0);

    printf("Task 4: Matrix Additions\n");

    for (size_t si = 0; si < num_sizes; ++si) {

        const size_t n = sizes[si];

        if (n == 0 ||
            n > SIZE_MAX / n ||
            n * n > SIZE_MAX / sizeof(double)) {

            fprintf(stderr, "Invalid or overflowing matrix size: %zu\n", n);
            return EXIT_FAILURE;
        }

        const size_t count = n * n;


        double (*a)[n] = malloc(n * sizeof(*a));
        double (*b)[n] = malloc(n * sizeof(*b));
        double (*serial)[n] = malloc(n * sizeof(*serial));
        double (*parallel)[n] = malloc(n * sizeof(*parallel));

        if (a == NULL || b == NULL ||
            serial == NULL || parallel == NULL) {

            fprintf(stderr,
                    "Memory allocation failed for N=%zu. "
                    "Four matrices require approximately %.1f MiB.\n",
                    n,
                    (double)count * sizeof(double) * 4.0 /
                    (1024.0 * 1024.0));

            free(a);
            free(b);
            free(serial);
            free(parallel);

            return EXIT_FAILURE;
        }

        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                a[i][j] = (double)((i * 3U + j * 7U + 7U) % 97U);
                b[i][j] = (double)((i * 5U + j * 11U + 11U) % 89U);
            }
        }


        const double t_serial =
            time_serial(n, a, b, serial);

        printf("\n\nN = %zu\n", n);
        printf("Serial time: %.9f seconds\n", t_serial);

        // printf("\nThreads | Outer-loop (s) | Collapse(2) (s) | Verification\n");
        // printf("----------------------------------------------------------\n");

        for (size_t ti = 0; ti < num_threads; ++ti) {

            const int num_threads_to_use = threads[ti];

            omp_set_num_threads(num_threads_to_use);

            const double t_outer =
                time_outer(n, a, b, parallel);

            if (!matrices_equal(n, serial, parallel)) {
                fprintf(stderr,
                        "Outer-loop result mismatch: N=%zu, threads=%d\n",
                        n, num_threads_to_use);

                free(a);
                free(b);
                free(serial);
                free(parallel);

                return EXIT_FAILURE;
            }

            const double t_collapse =
                time_collapse(n, a, b, parallel);

            if (!matrices_equal(n, serial, parallel)) {
                fprintf(stderr,
                        "collapse(2) result mismatch: N=%zu, threads=%d\n",
                        n, num_threads_to_use);

                free(a);
                free(b);
                free(serial);
                free(parallel);

                return EXIT_FAILURE;
            }
            printf("no of threads = %d,time outer = %.9f, time collapse = %.9f\n", threads[ti], t_outer, t_collapse);
            // printf("%7d | %14.9f | %15.9f | PASS\n",
            //        num_threads_to_use,
            //        t_outer,
            //        t_collapse);
        }

        free(a);
        free(b);
        free(serial);
        free(parallel);
    }

    return EXIT_SUCCESS;
}