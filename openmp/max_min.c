#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void find_max_idx_seriel(int *a, long n, int *max_val, long *max_idx) {
    int best_val = a[0];
    long best_idx = 0;
    for (long i = 1; i < n; i++) {
        {
            if (a[i] > best_val) {
                best_val = a[i];
                best_idx = i;
            }
        }
    }
    *max_val = best_val;
    *max_idx = best_idx;
}

void find_min_idx_seriel(int *a, long n, int *min_val, long *min_idx) {
    int best_val = a[0];
    long best_idx = 0;
    for (long i = 1; i < n; i++) {
        {
            if (a[i] < best_val) {
                best_val = a[i];
                best_idx = i;
            }
        }
    }
    *min_val = best_val;
    *min_idx = best_idx;
}

void find_max_idx_critical(int *a, long n, int *max_val, long *max_idx) {
    int best_val = a[0];
    long best_idx = 0;
    #pragma omp parallel for
    for (long i = 1; i < n; i++) {
        #pragma omp critical
        {
            if (a[i] > best_val) {
                best_val = a[i];
                best_idx = i;
            }
        }
    }
    *max_val = best_val;
    *max_idx = best_idx;
}

void find_min_idx_critical(int *a, long n, int *min_val, long *min_idx) {
    int best_val = a[0];
    long best_idx = 0;
    #pragma omp parallel for
    for (long i = 1; i < n; i++) {
        #pragma omp critical
        {
            if (a[i] < best_val) {
                best_val = a[i];
                best_idx = i;
            }
        }
    }
    *min_val = best_val;
    *min_idx = best_idx;
}

int main(int argc, char *argv[]) 
{
    long full_sizes[] = {10000000, 100000000, 200000000};
    int threads[] = {1, 2, 4, 8};
    int num_sizes = sizeof(full_sizes) / sizeof(full_sizes[0]);
    int num_threads = sizeof(threads) / sizeof(threads[0]);

    double t1, t2, t3, t4, t5, t6;

    for(int i=0; i<num_sizes; i++){
        long n=full_sizes[i];
        int *a = malloc(n * sizeof(int));
    
        for (long q = 0; q < n; q++) {
            a[q] = ((rand() & 0x7FFF) << 16) | (rand() & 0xFFFF);
        }

        int min_val, max_val;
        long min_idx, max_idx;

        int min_val_seriel, max_val_seriel;
        long min_idx_seriel, max_idx_seriel;


        t1 = omp_get_wtime();
        find_max_idx_seriel(a, n, &max_val_seriel, &max_idx_seriel);
        t2 = omp_get_wtime();
        find_min_idx_seriel(a, n, &min_val_seriel, &min_idx_seriel);
        t3 = omp_get_wtime();

        printf("\n\n#### N=%ld serial: max=%d at %ld; min=%d at %ld\n", n,
               max_val_seriel, max_idx_seriel, min_val_seriel, min_idx_seriel);
        printf("SERIEL FIND MAX time %lf\n", t2-t1);
        printf("SERIEL FIND MIN time %lf\n", t3-t2);

        for(int nt=0; nt<num_threads; nt++){

            omp_set_num_threads(threads[nt]);    

            t4 = omp_get_wtime();
            find_max_idx_critical(a, n, &max_val, &max_idx);
            t5 = omp_get_wtime();
            find_min_idx_critical(a, n, &min_val, &min_idx);
            t6 = omp_get_wtime();


            // for (long i = 0; i < n; i++) {
            //     printf("%d\t", a[i]);
            // }

            printf("no of threads = %d, nmax val = %d, min val = %d, max idx = %ld, min idx = %ld \n", threads[nt],max_val, min_val, max_idx, min_idx);

            printf("________time_________\n");
            printf("PARALLEL FIND MAX time %lf\n", t5-t4);
            printf("PARALLEL FIND MIN time %lf\n", t6-t5);

        }

        


        free(a);
    }
        return 0;
}