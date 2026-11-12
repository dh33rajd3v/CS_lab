#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define CUTOFF 2048

void merge(int *a, int *tmp, int left, int mid, int right) {
    int i = left, j = mid + 1, k = left;

    while (i <= mid && j <= right) {
        if (a[i] <= a[j])
            tmp[k++] = a[i++];
        else
            tmp[k++] = a[j++];
    }
    while (i <= mid)
        tmp[k++] = a[i++];
    while (j <= right)
        tmp[k++] = a[j++];

    for (i = left; i <= right; i++)
        a[i] = tmp[i];
}

void merge_sort_serial(int *a, int *tmp, int left, int right) {
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;
    merge_sort_serial(a, tmp, left, mid);
    merge_sort_serial(a, tmp, mid + 1, right);
    merge(a, tmp, left, mid, right);
}

void merge_sort_parallel(int *a, int *tmp, int left, int right) {
    if (left >= right)
        return;

    // Fallback to serial sort for small subarrays to avoid task overhead
    if (right - left < CUTOFF) {
        merge_sort_serial(a, tmp, left, right);
        return;
    }

    int mid = left + (right - left) / 2;

    #pragma omp task shared(a, tmp) firstprivate(left, mid)
    merge_sort_parallel(a, tmp, left, mid);

    #pragma omp task shared(a, tmp) firstprivate(mid, right)
    merge_sort_parallel(a, tmp, mid + 1, right);

    #pragma omp taskwait
    merge(a, tmp, left, mid, right);
}

int verify_sorted_and_equal(int *serial_arr, int *parallel_arr, int n) {
    for (int i = 0; i < n; i++) {
        if (serial_arr[i] != parallel_arr[i]) return 0;
        if (i > 0 && parallel_arr[i] < parallel_arr[i - 1]) return 0;
    }
    return 1;
}

int main(void) {
    int sizes[] = {100000, 1000000, 10000000}; // 10^5, 10^6, 10^7
    int threads[] = {1, 2, 4, 8};

    printf("================================================================================\n");
    printf("TASK 1: PARALLEL MERGE SORT (CUTOFF = %d)\n", CUTOFF);
    printf("================================================================================\n");
    printf("%-10s | %-8s | %-12s | %-10s | %-12s | %-10s\n", 
           "Size (N)", "Threads", "Time (s)", "Speedup", "Efficiency", "Verified");
    printf("--------------------------------------------------------------------------------\n");

    for (int s = 0; s < 3; s++) {
        int n = sizes[s];
        int *orig = (int *)malloc(n * sizeof(int));
        int *a_serial = (int *)malloc(n * sizeof(int));
        int *a_parallel = (int *)malloc(n * sizeof(int));
        int *tmp = (int *)malloc(n * sizeof(int));

        srand(42);
        for (int i = 0; i < n; i++) {
            orig[i] = rand();
        }

        // 1. Run Serial Baseline
        memcpy(a_serial, orig, n * sizeof(int));
        double start_s = omp_get_wtime();
        merge_sort_serial(a_serial, tmp, 0, n - 1);
        double time_serial = omp_get_wtime() - start_s;

        printf("%-10d | %-8s | %-12.6f | %-10s | %-12s | %-10s\n", 
               n, "Serial", time_serial, "1.00x", "100.0%", "Baseline");

        // 2. Run Parallel across thread counts
        for (int t = 0; t < 4; t++) {
            int p = threads[t];
            memcpy(a_parallel, orig, n * sizeof(int));
            omp_set_num_threads(p);

            double start_p = omp_get_wtime();
            #pragma omp parallel
            {
                #pragma omp single
                merge_sort_parallel(a_parallel, tmp, 0, n - 1);
            }
            double time_p = omp_get_wtime() - start_p;

            double speedup = time_serial / time_p;
            double efficiency = (speedup / p) * 100.0;
            int ok = verify_sorted_and_equal(a_serial, a_parallel, n);

            printf("%-10d | %-8d | %-12.6f | %-9.2fx | %-11.1f%% | %-10s\n", 
                   n, p, time_p, speedup, efficiency, ok ? "PASS" : "FAIL");
        }
        printf("--------------------------------------------------------------------------------\n");

        free(orig);
        free(a_serial);
        free(a_parallel);
        free(tmp);
    }

    return 0;
}