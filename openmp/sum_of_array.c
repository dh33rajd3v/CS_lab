#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

long long sum_serial(long *a, long n) {
    long long sum = 0;
    for (long i = 0; i < n; i++){
        sum += a[i];
    }
    return sum;
    }

long long sum_naive_parallel_BROKEN(long *a, long n) {
    long long sum = 0;
    #pragma omp parallel for
    for (long i = 0; i < n; i++){
        sum += a[i]; // data race,
    }
    return sum;
    }

long long sum_reduction(long *a, long n) {
    long long sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (long i = 0; i < n; i++){
        sum += a[i];
    }
    return sum;
}

int main(int argc, char *argv[]) 
{
    long full_sizes[] = {10000000, 100000000, 200000000};
    int threads[] = {1, 2, 4, 8};
    int num_sizes = sizeof(full_sizes) / sizeof(full_sizes[0]);
    int num_threads = sizeof(threads) / sizeof(threads[0]);


    for(int i=0; i<num_sizes; i++){
        long n=full_sizes[i];
        long *a = malloc(n * sizeof(long));
        
        for (long q = 0; q < n; q++) {
            a[q] = (long)rand() / 1234;
        }

        long long sum1, sum2, sum3;
        double t0, t1, t2;

        double t3 = omp_get_wtime();
        sum3 = sum_serial(a, n);
        double t4 = omp_get_wtime();

        printf("\n\nn = %ld, sum seriel = %lld\n", n, sum3);
        printf("Sum seriel time %lf\n", t4-t3);

        for(int j=0; j<num_threads; ++j){

            omp_set_num_threads(threads[j]);            

            t0 = omp_get_wtime();
            sum1 = sum_reduction(a, n);
            t1 = omp_get_wtime();
            sum2 = sum_naive_parallel_BROKEN(a, n);
            t2 = omp_get_wtime();
            

            printf("\n\nno of threads = %d", threads[j]);

            printf("\n________sum_________\nSum reduction = %lld\n",sum1);
            printf("Sum naive parallel = %lld\n",sum2);

            printf("________time_________\nSum reduction time %lf\n", t1-t0);
            printf("Sum naive parallel time %lf\n", t2-t1);

            printf("____Speed up_________\n");
            printf("Speedup = %6f\n", t2-t1/t1-t0);
            

        }
        free(a);
    }
    return 0;
}