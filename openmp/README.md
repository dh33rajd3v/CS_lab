# CS527 OpenMP Lab - Readme file

This folder contains five separate C programs corresponding to the five tasks in `CS527_Lab_tasks_OpenMP.pdf`. 
Each program compiles with GCC/OpenMP, verifies parallel results against a serial result, and prints timings. 

## 1. Compile


```bash
gcc -fopenmp -O2 -Wall vectoradd.c -o qn1
gcc -fopenmp -O2 -Wall sum_of_array.c -o qn2  
gcc -fopenmp -O2 -Wall max_min.c -o qn3
gcc -fopenmp -O2 -Wall mat_addition.c -o qn4
gcc -fopenmp -O2 -Wall mat_mul_naive.c -o q5
```

To run:

Ubuntu:
```bash
./qn1
./qn2
./qn3
./qn4
./qn5
```

Windows:
```bash
.\qn1.exe
.\qn2
.\qn3
.\qn4
.\qn5
```

## 2. Task 1 — Vector addition

Implementation: `task1_vecadd.c`.

- Serial: `c[i] = a[i] + b[i]`.
- Parallel: the same loop is preceded by `#pragma omp parallel for`.
- The program reports `T_serial`, `T_parallel`, and `speedup = T_serial / T_parallel` 

Result:

<!-- n = 100000, time for seriel = 0.000000000
no of threads = 1, time for parallel = 0.000999928, Speedup = 0.000000
no of threads = 2, time for parallel = 0.001000166, Speedup = 0.000000
no of threads = 4, time for parallel = 0.000999928, Speedup = 0.000000
no of threads = 8, time for parallel = 0.001000166, Speedup = 0.000000


n = 1000000, time for seriel = 0.008000135
no of threads = 1, time for parallel = 0.006000042, Speedup = 1.333347
no of threads = 2, time for parallel = 0.001999855, Speedup = 4.000358
no of threads = 4, time for parallel = 0.002000093, Speedup = 3.999881
no of threads = 8, time for parallel = 0.002999783, Speedup = 2.666905


n = 10000000, time for seriel = 0.042999983
no of threads = 1, time for parallel = 0.039000034, Speedup = 1.102563
no of threads = 2, time for parallel = 0.015000105, Speedup = 2.866645
no of threads = 4, time for parallel = 0.011999846, Speedup = 3.583378
no of threads = 8, time for parallel = 0.014000177, Speedup = 3.071388


n = 100000000, time for seriel = 0.411000013
no of threads = 1, time for parallel = 0.423999786, Speedup = 0.969340
no of threads = 2, time for parallel = 0.141999960, Speedup = 2.894367
no of threads = 4, time for parallel = 0.118999958, Speedup = 3.453783
no of threads = 8, time for parallel = 0.120000124, Speedup = 3.424997 -->

## Timing Tables (Thread Count vs. Execution Time)

### 1. Input Size: `n = 100000`

**Serial execution time:** `0.000000000` seconds

| Number of Threads | Parallel Execution Time (s) | Speedup |
|---:|---:|---:|
| 1 | 0.000999928 | 0.000000 |
| 2 | 0.001000166 | 0.000000 |
| 4 | 0.000999928 | 0.000000 |
| 8 | 0.001000166 | 0.000000 |

### 2. Input Size: `n = 1000000`

**Serial execution time:** `0.008000135` seconds

| Number of Threads | Parallel Execution Time (s) | Speedup |
|---:|---:|---:|
| 1 | 0.006000042 | 1.333347 |
| 2 | 0.001999855 | 4.000358 |
| 4 | 0.002000093 | 3.999881 |
| 8 | 0.002999783 | 2.666905 |

### 3. Input Size: `n = 10000000`

**Serial execution time:** `0.042999983` seconds

| Number of Threads | Parallel Execution Time (s) | Speedup |
|---:|---:|---:|
| 1 | 0.039000034 | 1.102563 |
| 2 | 0.015000105 | 2.866645 |
| 4 | 0.011999846 | 3.583378 |
| 8 | 0.014000177 | 3.071388 |

### 4. Input Size: `n = 100000000`

**Serial execution time:** `0.411000013` seconds

| Number of Threads | Parallel Execution Time (s) | Speedup |
|---:|---:|---:|
| 1 | 0.423999786 | 0.969340 |
| 2 | 0.141999960 | 2.894367 |
| 4 | 0.118999958 | 3.453783 |
| 8 | 0.120000124 | 3.424997 |

### Answers to Task 1 questions

**At what size does parallelization start paying off?** from N=1000000, `T_parallel` is consistently below `T_serial`.

**Why does it not help for small N?** Creating/activating the OpenMP team, distributing loop iterations, synchronizing at the end, and timing itself can cost more than the few additions. The parallel loop also reads two arrays and writes one, so it quickly becomes limited by memory bandwidth rather than arithmetic.

**Does speedup grow linearly with thread count?** as thread count increases, speed up also is increasing, but not linearly.

## 3. Task 2 — Sum of array elements

Implementation: `task2_array_sum.c`.

The program first runs the deliberately broken version. It then benchmarks the correct reductionfor different N and different thread count. The input contains all ones, so the expected sum is N.

Output:

<!-- n = 10000000, sum seriel = 127808261
Sum seriel time 0.031000


no of threads = 1
________sum_________
Sum reduction = 127808261
Sum naive parallel = 127808261
________time_________
Sum reduction time 0.033000
Sum naive parallel time 0.020000
____Speed up_________
Speedup = -0.947000


no of threads = 2
________sum_________
Sum reduction = 127808261
Sum naive parallel = 70852719
________time_________
Sum reduction time 0.017000
Sum naive parallel time 0.026000
____Speed up_________
Speedup = -0.957000


no of threads = 4
________sum_________
Sum reduction = 127808261
Sum naive parallel = 40041949
________time_________
Sum reduction time 0.008000
Sum naive parallel time 0.026000
____Speed up_________
Speedup = -0.966000


no of threads = 8
________sum_________
Sum reduction = 127808261
Sum naive parallel = 21299283
________time_________
Sum reduction time 0.006000
Sum naive parallel time 0.029000
____Speed up_________
Speedup = -0.965000


n = 100000000, sum seriel = 1278342493
Sum seriel time 0.333000


no of threads = 1
________sum_________
Sum reduction = 1278342493
Sum naive parallel = 1278342493
________time_________
Sum reduction time 0.307000
Sum naive parallel time 0.154000
____Speed up_________
Speedup = -0.539000


no of threads = 2
________sum_________
Sum reduction = 1278342493
Sum naive parallel = 654453356
________time_________
Sum reduction time 0.152000
Sum naive parallel time 0.251000
____Speed up_________
Speedup = -0.597000


no of threads = 4
________sum_________
Sum reduction = 1278342493
Sum naive parallel = 360805477
________time_________
Sum reduction time 0.088000
Sum naive parallel time 0.259000
____Speed up_________
Speedup = -0.653000


no of threads = 8
________sum_________
Sum reduction = 1278342493
Sum naive parallel = 239771223
________time_________
Sum reduction time 0.048000
Sum naive parallel time 0.278000
____Speed up_________
Speedup = -0.674000


n = 200000000, sum seriel = 2556380582
Sum seriel time 0.681000


no of threads = 1
________sum_________
Sum reduction = 2556380582
Sum naive parallel = 2556380582
________time_________
Sum reduction time 0.623000
Sum naive parallel time 0.334000
____Speed up_________
Speedup = -0.043000


no of threads = 2
________sum_________
Sum reduction = 2556380582
Sum naive parallel = 1353035165
________time_________
Sum reduction time 0.297000
Sum naive parallel time 0.486000
____Speed up_________
Speedup = -0.217000


no of threads = 4
________sum_________
Sum reduction = 2556380582
Sum naive parallel = 666050257
________time_________
Sum reduction time 0.157000
Sum naive parallel time 0.522000
____Speed up_________
Speedup = -0.321000


no of threads = 8
________sum_________
Sum reduction = 2556380582
Sum naive parallel = 415376523
________time_________
Sum reduction time 0.077000
Sum naive parallel time 0.553000
____Speed up_________
Speedup = -0.370000 -->

# OpenMP Parallel Reduction vs. Naive Parallel Sum

## 1. Input Size: n = 10,000,000

- **Serial Sum:** 127,808,261
- **Serial Execution Time:** 0.031 seconds

### Timing Table

| Number of Threads | Reduction Time (s) | Naive Parallel Time (s) | Reduction Sum | Naive Parallel Sum | Speedup |
|---:|---:|---:|---:|---:|---:|
| 1 | 0.033 | 0.020 | 127808261 | 127808261 | -0.947 |
| 2 | 0.017 | 0.026 | 127808261 | 70852719 | -0.957 |
| 4 | 0.008 | 0.026 | 127808261 | 40041949 | -0.966 |
| 8 | 0.006 | 0.029 | 127808261 | 21299283 | -0.965 |

## 2. Input Size: n = 100,000,000

- **Serial Sum:** 1,278,342,493
- **Serial Execution Time:** 0.333 seconds

### Timing Table

| Number of Threads | Reduction Time (s) | Naive Parallel Time (s) | Reduction Sum | Naive Parallel Sum | Speedup |
|---:|---:|---:|---:|---:|---:|
| 1 | 0.307 | 0.154 | 1278342493 | 1278342493 | -0.539 |
| 2 | 0.152 | 0.251 | 1278342493 | 654453356 | -0.597 |
| 4 | 0.088 | 0.259 | 1278342493 | 360805477 | -0.653 |
| 8 | 0.048 | 0.278 | 1278342493 | 239771223 | -0.674 |

## 3. Input Size: n = 200,000,000

- **Serial Sum:** 2,556,380,582
- **Serial Execution Time:** 0.681 seconds

### Timing Table

| Number of Threads | Reduction Time (s) | Naive Parallel Time (s) | Reduction Sum | Naive Parallel Sum | Speedup |
|---:|---:|---:|---:|---:|---:|
| 1 | 0.623 | 0.334 | 2556380582 | 2556380582 | -0.043 |
| 2 | 0.297 | 0.486 | 2556380582 | 1353035165 | -0.217 |
| 4 | 0.157 | 0.522 | 2556380582 | 666050257 | -0.321 |
| 8 | 0.077 | 0.553 | 2556380582 | 415376523 | -0.370 |

## Observations

1. **Reduction performance:** Execution time decreases as the number of threads increases. For example, with `n = 200,000,000`, reduction time decreases from `0.623 s` with one thread to `0.077 s` with eight threads.
2. **Naive parallel sum:** The computed sum differs from the serial result when multiple threads are used, indicating a race condition caused by concurrent updates to a shared variable.
3. **Correctness:** The reduction implementation produces the correct sum for every tested thread count. The naive parallel implementation produces the correct result only with one thread in these experiments.
4. **Effect of input size:** Larger inputs make the benefits of parallel reduction more apparent because the computation can be distributed across multiple threads.
5. **Timing variability:** Small differences between serial and parallel execution times may occur because of thread-management overhead, scheduling, and system load.

### Answers to Task 2 questions

**Why is the naive version wrong/inconsistent?** `sum` is shared between threads. `sum += a[i]` is a read-modify-write sequence: multiple threads can read the same old sum and then overwrite one another's updates. This is a data race, so the program has undefined behavior. It may lose updates, return different values on different runs, or occasionally appear correct; a wrong value is not guaranteed on every execution.

**Would `critical` fix it? What about performance?**

 Yes, the below implementation can give correct answer:

```c
#pragma omp parallel for
for (long i = 0; i < n; ++i) {
    #pragma omp critical
    sum += a[i];
}
```
But every element must enter the same critical section, making updates effectively serialized and creating lock contention. It is generally much slower than a reduction. With `reduction(+:sum)`, every thread accumulates into a private partial sum and OpenMP combines those partial sums after the loop, avoiding a lock per element.

## 4. Task 3 — Maximum/minimum and their indices

Implementation: `task3_minmax.c`.

The serial implementation establishes the reference max/min values and indices.Critical section method is used in parallel processing

The elementwise critical-section approach enters the critical section for every array element. It is extremely costly at large N.

result:

<!-- #### N=10000000 serial: max=2147450839 at 8212502; min=228 at 7680591
SERIEL FIND MAX time 0.015000
SERIEL FIND MIN time 0.012000
no of threads = 1, nmax val = 2147450839, min val = 228, max idx = 8212502, min idx = 7680591 
________time_________
PARALLEL FIND MAX time 0.221000
PARALLEL FIND MIN time 0.218000
no of threads = 2, nmax val = 2147450839, min val = 228, max idx = 8212502, min idx = 7680591 
________time_________
PARALLEL FIND MAX time 0.364000
PARALLEL FIND MIN time 0.387000
no of threads = 4, nmax val = 2147450839, min val = 228, max idx = 8212502, min idx = 7680591 
________time_________
PARALLEL FIND MAX time 0.374000
PARALLEL FIND MIN time 0.353000
no of threads = 8, nmax val = 2147450839, min val = 228, max idx = 8212502, min idx = 7680591 
________time_________
PARALLEL FIND MAX time 0.679000
PARALLEL FIND MIN time 0.696000


#### N=100000000 serial: max=2147450868 at 60269286; min=11 at 11070201
SERIEL FIND MAX time 0.127000
SERIEL FIND MIN time 0.118000
no of threads = 1, nmax val = 2147450868, min val = 11, max idx = 60269286, min idx = 11070201 
________time_________
PARALLEL FIND MAX time 2.206000
PARALLEL FIND MIN time 2.146000
no of threads = 2, nmax val = 2147450868, min val = 11, max idx = 60269286, min idx = 11070201 
________time_________
PARALLEL FIND MAX time 3.890000
PARALLEL FIND MIN time 3.692000
no of threads = 4, nmax val = 2147450868, min val = 11, max idx = 60269286, min idx = 11070201 
________time_________
PARALLEL FIND MAX time 3.654000
PARALLEL FIND MIN time 3.801000
no of threads = 8, nmax val = 2147450868, min val = 11, max idx = 60269286, min idx = 11070201 
________time_________
PARALLEL FIND MAX time 6.573000
PARALLEL FIND MIN time 6.554000


#### N=200000000 serial: max=2147450875 at 42522683; min=0 at 132241752
SERIEL FIND MAX time 0.247000
SERIEL FIND MIN time 0.239000
no of threads = 1, nmax val = 2147450875, min val = 0, max idx = 42522683, min idx = 132241752 
________time_________
PARALLEL FIND MAX time 4.456000
PARALLEL FIND MIN time 4.623000
no of threads = 2, nmax val = 2147450875, min val = 0, max idx = 42522683, min idx = 132241752 
________time_________
PARALLEL FIND MAX time 7.267000
PARALLEL FIND MIN time 7.276000
no of threads = 4, nmax val = 2147450875, min val = 0, max idx = 42522683, min idx = 132241752 
________time_________
PARALLEL FIND MAX time 7.653000
PARALLEL FIND MIN time 7.516000
no of threads = 8, nmax val = 2147450875, min val = 0, max idx = 42522683, min idx = 132241752 
________time_________
PARALLEL FIND MAX time 13.302000
PARALLEL FIND MIN time 13.060000 -->


## Timing Results: Serial vs. Parallel Execution

### 1. Input Size: N = 10,000,000

**Serial Results**
- Maximum: `2147450839` at index `8212502`
- Minimum: `228` at index `7680591`

| Thread Count | Find Maximum (s) | Find Minimum (s) |
|---:|---:|---:|
| Serial | 0.015000 | 0.012000 |
| 1 | 0.221000 | 0.218000 |
| 2 | 0.364000 | 0.387000 |
| 4 | 0.374000 | 0.353000 |
| 8 | 0.679000 | 0.696000 |

### 2. Input Size: N = 100,000,000

**Serial Results**
- Maximum: `2147450868` at index `60269286`
- Minimum: `11` at index `11070201`

| Thread Count | Find Maximum (s) | Find Minimum (s) |
|---:|---:|---:|
| Serial | 0.127000 | 0.118000 |
| 1 | 2.206000 | 2.146000 |
| 2 | 3.890000 | 3.692000 |
| 4 | 3.654000 | 3.801000 |
| 8 | 6.573000 | 6.554000 |

### 3. Input Size: N = 200,000,000

**Serial Results**
- Maximum: `2147450875` at index `42522683`
- Minimum: `0` at index `132241752`

| Thread Count | Find Maximum (s) | Find Minimum (s) |
|---:|---:|---:|
| Serial | 0.247000 | 0.239000 |
| 1 | 4.456000 | 4.623000 |
| 2 | 7.267000 | 7.276000 |
| 4 | 7.653000 | 7.516000 |
| 8 | 13.302000 | 13.060000 |


### Answers to Task 3 questions

**Why not `reduction(max: best_idx)`?** The largest index is not necessarily the index of the largest value. The desired result is a pair `(value, index)` with a rule such as: larger value wins; if values tie, smaller index wins. A reduction over the index alone ignores the array values. A custom pair reduction would need to encode that pair-comparison rule; ordinary built-in `max` on an integer index does not do it.

**Critical vs. manual per-thread pair reduction?** In the sample-style critical approach, every iteration competes for one lock, so only one thread at a time can inspect/update the current shared pair. This creates heavy contention as N increases. In the manual approach, each thread scans its own assigned elements without a lock, writes one local result, and then only a small number of per-thread results are combined. The manual reduction is therefore expected to be faster for large N. Check the actual times from your machine; the critical timing in the default run is only for the specified representative setting.

## 5. Task 4 — Matrix addition

Implementation: `task4_matrix_add.c`. The program tests N = 8, N = 1024 and N = 4096 with thread counts 1, 2, 4, 8. It compares the serial result with both the outer-loop parallel version and the `collapse(2)` version.

Results:
Task 4: Matrix Additions

<!-- N = 8
Serial time: 0.000000000 seconds
no of threads = 1,time outer = 0.000000000, time collapse = 0.000000000
no of threads = 2,time outer = 0.000999928, time collapse = 0.000000000
no of threads = 4,time outer = 0.000999928, time collapse = 0.000000000
no of threads = 8,time outer = 0.001000166, time collapse = 0.000000000


N = 1024
Serial time: 0.004999876 seconds
no of threads = 1,time outer = 0.006000042, time collapse = 0.003000021
no of threads = 2,time outer = 0.002000093, time collapse = 0.001999855
no of threads = 4,time outer = 0.000999928, time collapse = 0.000999928
no of threads = 8,time outer = 0.000999928, time collapse = 0.001000166


N = 4096
Serial time: 0.073000193 seconds
no of threads = 1,time outer = 0.069999933, time collapse = 0.052999973
no of threads = 2,time outer = 0.028999805, time collapse = 0.028000116
no of threads = 4,time outer = 0.022000074, time collapse = 0.020000219
no of threads = 8,time outer = 0.020999908, time collapse = 0.019999981 -->

## Timing Tables (Thread Count vs. Execution Time)

### N = 8

**Serial Execution Time:** 0.000000000 seconds

| Number of Threads | Outer Loop Time (seconds) | Collapse Time (seconds) |
|------------------:|--------------------------:|------------------------:|
| 1                 | 0.000000000               | 0.000000000             |
| 2                 | 0.000999928               | 0.000000000             |
| 4                 | 0.000999928               | 0.000000000             |
| 8                 | 0.001000166               | 0.000000000             |

### N = 1024

**Serial Execution Time:** 0.004999876 seconds

| Number of Threads | Outer Loop Time (seconds) | Collapse Time (seconds) |
|------------------:|--------------------------:|------------------------:|
| 1                 | 0.006000042               | 0.003000021             |
| 2                 | 0.002000093               | 0.001999855             |
| 4                 | 0.000999928               | 0.000999928             |
| 8                 | 0.000999928               | 0.001000166             |

### N = 4096

**Serial Execution Time:** 0.073000193 seconds

| Number of Threads | Outer Loop Time (seconds) | Collapse Time (seconds) |
|------------------:|--------------------------:|------------------------:|
| 1                 | 0.069999933               | 0.052999973             |
| 2                 | 0.028999805               | 0.028000116             |
| 4                 | 0.022000074               | 0.020000219             |
| 8                 | 0.020999908               | 0.019999981             |

### Answers to Task 4 questions

**Does `collapse(2)` help for small N?** With N = 8, Ideally, outer-loop-only parallelism exposes only 8 independent iterations (one per row), while `collapse(2)` exposes 8 x 8 = 64 element iterations. This can give more scheduling flexibility if the thread count exceeds N. However, the total job has only 64 additions, so parallel runtime overhead is likely to dominate; `collapse(2)` need not be faster in elapsed time. But in my results, it is actually performing better than parallel.

**For large N, does `collapse(2)` matter?** For N = 4096 there are 4096 independent rows, far more than the maximum 8 threads in this lab. The outer loop already offers enough work to keep all requested threads busy. `collapse(2)` method is slightly performing better in the results.

## 6. Task 5 — Naive matrix-matrix multiplication

Implementation: `task5_matmul.c`. The required i-j-k order is used for serial, parallel-i, and `collapse(2)` parallel-i-j implementations. All runs zero C before the timed multiplication; initialization is outside the reported interval. Floating-point outputs are verified with absolute tolerance 1e-9. Timings include N = 256, 512, 1024 and threads 1, 2, 4, 8; speedup and efficiency are printed for each parallel approach.

<!-- N = 256, serial time = 0.060999870 seconds


no of threads = 1,time only i parallel = 0.062000036, time both i and j parallel = 0.057999849
speedup with i parallel = 0.983868,
parallel efficiency with i parallel = 0.983868,
speedup with i and j parallel = 1.051725,
parallel efficiency with i and j parallel = 1.051725


no of threads = 2,time only i parallel = 0.032999992, time both i and j parallel = 0.030999899
speedup with i parallel = 1.848481,
parallel efficiency with i parallel = 0.924241,
speedup with i and j parallel = 1.967744,
parallel efficiency with i and j parallel = 0.983872


no of threads = 4,time only i parallel = 0.017999887, time both i and j parallel = 0.020999908
speedup with i parallel = 3.388903,
parallel efficiency with i parallel = 0.847226,
speedup with i and j parallel = 2.904768,
parallel efficiency with i and j parallel = 0.726192


no of threads = 8,time only i parallel = 0.016000032, time both i and j parallel = 0.013000011
speedup with i parallel = 3.812484,
parallel efficiency with i parallel = 0.476561,
speedup with i and j parallel = 4.692294,
parallel efficiency with i and j parallel = 0.586537

Bonus i-k-j results for N=256

no of threads = 1,time parallel = 0.055999994, speedup = 0.946428
no of threads = 2,time parallel = 0.029999971, speedup = 1.766667
no of threads = 4,time parallel = 0.015000105, speedup = 3.533307
no of threads = 8,time parallel = 0.014000177, speedup = 3.785664


N = 512, serial time = 0.612999916 seconds


no of threads = 1,time only i parallel = 0.611999989, time both i and j parallel = 0.642000198
speedup with i parallel = 1.001634,
parallel efficiency with i parallel = 1.001634,
speedup with i and j parallel = 0.954828,
parallel efficiency with i and j parallel = 0.954828


no of threads = 2,time only i parallel = 0.342999935, time both i and j parallel = 0.345999956
speedup with i parallel = 1.787172,
parallel efficiency with i parallel = 0.893586,
speedup with i and j parallel = 1.771676,
parallel efficiency with i and j parallel = 0.885838


no of threads = 4,time only i parallel = 0.167000055, time both i and j parallel = 0.147000074
speedup with i parallel = 3.670657,
parallel efficiency with i parallel = 0.917664,
speedup with i and j parallel = 4.170065,
parallel efficiency with i and j parallel = 1.042516


no of threads = 8,time only i parallel = 0.114000082, time both i and j parallel = 0.098000050
speedup with i parallel = 5.377188,
parallel efficiency with i parallel = 0.672149,
speedup with i and j parallel = 6.255098,
parallel efficiency with i and j parallel = 0.781887

Bonus i-k-j results for N=512

no of threads = 1,time parallel = 0.436000109, speedup = 1.022936
no of threads = 2,time parallel = 0.219000101, speedup = 2.036529
no of threads = 4,time parallel = 0.116999865, speedup = 3.811971
no of threads = 8,time parallel = 0.078999996, speedup = 5.645571


N = 1024, serial time = 5.378000021 seconds


no of threads = 1,time only i parallel = 4.894999981, time both i and j parallel = 4.735999823
speedup with i parallel = 1.098672,
parallel efficiency with i parallel = 1.098672,
speedup with i and j parallel = 1.135557,
parallel efficiency with i and j parallel = 1.135557


no of threads = 2,time only i parallel = 2.333000183, time both i and j parallel = 2.349999905
speedup with i parallel = 2.305186,
parallel efficiency with i parallel = 1.152593,
speedup with i and j parallel = 2.288511,
parallel efficiency with i and j parallel = 1.144255


no of threads = 4,time only i parallel = 1.170000076, time both i and j parallel = 1.164000034
speedup with i parallel = 4.596581,
parallel efficiency with i parallel = 1.149145,
speedup with i and j parallel = 4.620275,
parallel efficiency with i and j parallel = 1.155069


no of threads = 8,time only i parallel = 0.730999947, time both i and j parallel = 0.754999876
speedup with i parallel = 7.357046,
parallel efficiency with i parallel = 0.919631,
speedup with i and j parallel = 7.123180,
parallel efficiency with i and j parallel = 0.890398

Bonus i-k-j results for N=1024

no of threads = 1,time parallel = 3.380999804, speedup = 1.006507
no of threads = 2,time parallel = 1.696000099, speedup = 2.006486
no of threads = 4,time parallel = 0.873000145, speedup = 3.898052
no of threads = 8,time parallel = 0.534999847, speedup = 6.360750 -->

# OpenMP Matrix Multiplication Performance Results

## N = 256

**Serial execution time:** `0.060999870` seconds

### Performance: Parallelizing `i` vs. Parallelizing Both `i` and `j`

| Threads | Time (i parallel) (s) | Speedup (i) | Efficiency (i) | Time (i, j parallel) (s) | Speedup (i, j) | Efficiency (i, j) |
|--------:|----------------------:|------------:|---------------:|-------------------------:|---------------:|------------------:|
| 1 | 0.062000036 | 0.983868 | 0.983868 | 0.057999849 | 1.051725 | 1.051725 |
| 2 | 0.032999992 | 1.848481 | 0.924241 | 0.030999899 | 1.967744 | 0.983872 |
| 4 | 0.017999887 | 3.388903 | 0.847226 | 0.020999908 | 2.904768 | 0.726192 |
| 8 | 0.016000032 | 3.812484 | 0.476561 | 0.013000011 | 4.692294 | 0.586537 |

### Bonus: `i-k-j` Loop Ordering

| Threads | Parallel Execution Time (s) | Speedup |
|--------:|----------------------------:|--------:|
| 1 | 0.055999994 | 0.946428 |
| 2 | 0.029999971 | 1.766667 |
| 4 | 0.015000105 | 3.533307 |
| 8 | 0.014000177 | 3.785664 |

---

## N = 512

**Serial execution time:** `0.612999916` seconds

### Performance: Parallelizing `i` vs. Parallelizing Both `i` and `j`

| Threads | Time (i parallel) (s) | Speedup (i) | Efficiency (i) | Time (i, j parallel) (s) | Speedup (i, j) | Efficiency (i, j) |
|--------:|----------------------:|------------:|---------------:|-------------------------:|---------------:|------------------:|
| 1 | 0.611999989 | 1.001634 | 1.001634 | 0.642000198 | 0.954828 | 0.954828 |
| 2 | 0.342999935 | 1.787172 | 0.893586 | 0.345999956 | 1.771676 | 0.885838 |
| 4 | 0.167000055 | 3.670657 | 0.917664 | 0.147000074 | 4.170065 | 1.042516 |
| 8 | 0.114000082 | 5.377188 | 0.672149 | 0.098000050 | 6.255098 | 0.781887 |

### Bonus: `i-k-j` Loop Ordering

| Threads | Parallel Execution Time (s) | Speedup |
|--------:|----------------------------:|--------:|
| 1 | 0.436000109 | 1.022936 |
| 2 | 0.219000101 | 2.036529 |
| 4 | 0.116999865 | 3.811971 |
| 8 | 0.078999996 | 5.645571 |

---

## N = 1024

**Serial execution time:** `5.378000021` seconds

### Performance: Parallelizing `i` vs. Parallelizing Both `i` and `j`

| Threads | Time (i parallel) (s) | Speedup (i) | Efficiency (i) | Time (i, j parallel) (s) | Speedup (i, j) | Efficiency (i, j) |
|--------:|----------------------:|------------:|---------------:|-------------------------:|---------------:|------------------:|
| 1 | 4.894999981 | 1.098672 | 1.098672 | 4.735999823 | 1.135557 | 1.135557 |
| 2 | 2.333000183 | 2.305186 | 1.152593 | 2.349999905 | 2.288511 | 1.144255 |
| 4 | 1.170000076 | 4.596581 | 1.149145 | 1.164000034 | 4.620275 | 1.155069 |
| 8 | 0.730999947 | 7.357046 | 0.919631 | 0.754999876 | 7.123180 | 0.890398 |

### Bonus: `i-k-j` Loop Ordering

| Threads | Parallel Execution Time (s) | Speedup |
|--------:|----------------------------:|--------:|
| 1 | 3.380999804 | 1.006507 |
| 2 | 1.696000099 | 2.006486 |
| 4 | 0.873000145 | 3.898052 |
| 8 | 0.534999847 | 6.360750 |

---

### Answers to Task 5 questions

### Why is parallelizing the i loop safe?
Each thread receives different values of i, so each thread works on a different row of the 2D output matrix C. For a given row, the thread computes each element C[i][j] by iterating through k sequentially and accumulating the products A[i][k] * B[k][j] into a local variable sum. It then stores the result in C[i][j]. Since no two threads write to the same element of C, there is no data race.

### What if the k loop is parallelized with a plain parallel for?
For a fixed pair (i, j), all iterations of k contribute to the same output element C[i][j]. If multiple threads execute C[i][j] += A[i][k] * B[k][j] concurrently, they may read the same old value of C[i][j] and overwrite each other's updates. This creates a data race and can produce an incorrect sum. To parallelize k safely, each thread needs a private partial sum, followed by a proper reduction or combination of the partial sums.

### When does collapse(2) matter?
Using #pragma omp parallel for on the outer i loop exposes N independent iterations, one for each row of C. Using #pragma omp parallel for collapse(2) combines the i and j loops, exposing N^2 independent iterations, one for each output element C[i][j].

collapse(2) matters most when N is small relative to the number of threads, because the outer loop alone might not provide enough independent iterations to keep every thread busy. In this lab, N = 256, 512, 1024, while the maximum thread count is 8. The outer loop already provides plenty of independent rows, so collapse(2) is not guaranteed to improve performance. It may still affect scheduling overhead and cache behaviour.

### Bonus: i-k-j loop order
With 2D matrices stored in row-major order, the i-j-k loop order accesses A[i][k] sequentially as k changes, but accesses B[k][j] down a column, with a stride of approximately N elements between consecutive accesses.

In the i-k-j loop order, j is the innermost loop. Consequently, B[k][j] and C[i][j] are accessed consecutively across their rows, improving spatial locality. The value A[i][k] is loaded once and reused across the inner j loop. This can improve cache performance and opportunities for compiler vectorization.

The i-k-j version is also safe to parallelize over i: each thread updates only its assigned row of C, while the k and j loops execute sequentially for that row. However, actual performance improvements depend on the hardware and compiler, so verify correctness and compare the measured timings rather than assuming one loop order is always faster.