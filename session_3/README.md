# Task 1:

```code
================================================================================
TASK 1: PARALLEL MERGE SORT (CUTOFF = 2048)
================================================================================
Size (N)   | Threads  | Time (s)     | Speedup    | Efficiency   | Verified  
--------------------------------------------------------------------------------
100000     | Serial   | 0.023000     | 1.00x      | 100.0%       | Baseline  
100000     | 1        | 0.021000     | 1.10     x | 109.5      % | PASS      
100000     | 2        | 0.013000     | 1.77     x | 88.5       % | PASS      
100000     | 4        | 0.009000     | 2.56     x | 63.9       % | PASS      
100000     | 8        | 0.008000     | 2.88     x | 35.9       % | PASS      
--------------------------------------------------------------------------------
1000000    | Serial   | 0.242000     | 1.00x      | 100.0%       | Baseline  
1000000    | 1        | 0.227000     | 1.07     x | 106.6      % | PASS      
1000000    | 2        | 0.121000     | 2.00     x | 100.0      % | PASS      
1000000    | 4        | 0.080000     | 3.03     x | 75.6       % | PASS      
1000000    | 8        | 0.055000     | 4.40     x | 55.0       % | PASS      
--------------------------------------------------------------------------------
10000000   | Serial   | 2.502000     | 1.00x      | 100.0%       | Baseline  
10000000   | 1        | 2.496000     | 1.00     x | 100.2      % | PASS      
10000000   | 2        | 1.274000     | 1.96     x | 98.2       % | PASS      
10000000   | 4        | 0.831000     | 3.01     x | 75.3       % | PASS      
10000000   | 8        | 0.561000     | 4.46     x | 55.7       % | PASS      
--------------------------------------------------------------------------------
```

Q1. What is the difference between OpenMP tasks and parallel for?   

#pragma omp parallel for is designed for structured data parallelism (loop-level parallelism) where the total number of iterations is known in advance prior to loop execution. The runtime partitions the iteration space across threads at the start of the loop.

#pragma omp task is designed for unstructured or irregular parallelism (such as recursive divide-and-conquer algorithms, tree traversals, or while loops) where work units are generated dynamically at runtime. Encountering a task construct packages the code block and its captured data environment (firstprivate/shared) into a task descriptor and places it into a task pool, allowing any available worker thread in the team to steal and execute it asynchronously.

Q2. Why is taskwait required before merging the two halves?   

The #pragma omp task directives are asynchronous; the thread encountering them merely queues the left and right recursive sorting calls and immediately proceeds to the next line of code. Because merge(a, tmp, left, mid, right) has a strict data dependency on both halves [left..mid] and [mid+1..right] being completely sorted, #pragma omp taskwait acts as a synchronization barrier for the current task's immediate children. Without taskwait, merge() would execute prematurely on unsorted or partially sorted data, causing race conditions and corrupted output.   

Q3. How does the cutoff threshold affect performance?   

The cutoff threshold controls task granularity.   If the cutoff is too small (e.g., < 64): The recursion tree spawns millions of tiny tasks. The overhead of allocating task structures, copying firstprivate variables, queuing, and task scheduling outweighs the tiny computation of sorting a few integers, severely degrading performance. If the cutoff is too large (e.g., > N/2): Very few tasks are created, starving worker threads of parallel work and underutilizing CPU cores. Optimal cutoff (e.g., 1024 to 4096): Balances sufficient task parallelism to keep all cores busy while keeping each leaf task large enough to amortize OpenMP runtime overhead and fit comfortably inside the L1/L2 cache.

Q4. Why can parallel merge sort become slower than serial merge sort for small arrays?   

For small arrays (such as $N = 10^5$ or smaller), the total serial execution time is only a few milliseconds. Entering a #pragma omp parallel region incurs thread fork/join latency, thread pool wake-up costs, task queue management, and synchronization overhead (#pragma omp single and #pragma omp taskwait). Additionally, multiple cores writing to adjacent segments of a and tmp can cause cache line bouncing and reduced cache locality compared to a single core executing sequentially in its local L1/L2 cache.   

Q5. How does task creation overhead affect scalability?   

Every OpenMP task requires dynamic memory allocation for its task descriptor, context capture, insertion into a thread's deque, and work-stealing synchronization locks across threads. Furthermore, in merge sort, the final top-level merge() step is performed sequentially by a single thread in O(N) time. By Amdahl's Law, both the serial merge bottleneck at the root of the recursion tree and the cumulative task management overhead limit strong scaling as thread count increases. 

# Task 2

```code
====================================================================================
TASK 2: LOOP SCHEDULING COMPARISON (STATIC vs DYNAMIC(100) vs GUIDED)
====================================================================================
Size (N)   | Schedule     | Threads  | Time (s)     | Speedup    | Efficiency  
------------------------------------------------------------------------------------
100000     | Serial       | 1        | 26.350000    | 1.00x      | 100.0%      
100000     | static       | 1        | 26.305000    | 1.00     x | 100.2      %
100000     | dynamic,100  | 1        | 26.347000    | 1.00     x | 100.0      %
100000     | guided       | 1        | 26.367000    | 1.00     x | 99.9       %
100000     | static       | 2        | 13.182000    | 2.00     x | 99.9       %
100000     | dynamic,100  | 2        | 13.211000    | 1.99     x | 99.7       %
100000     | guided       | 2        | 13.156000    | 2.00     x | 100.1      %
100000     | static       | 4        | 6.582000     | 4.00     x | 100.1      %
100000     | dynamic,100  | 4        | 6.587000     | 4.00     x | 100.0      %
100000     | guided       | 4        | 6.567000     | 4.01     x | 100.3      %
100000     | static       | 8        | 3.912000     | 6.74     x | 84.2       %
100000     | dynamic,100  | 8        | 3.653000     | 7.21     x | 90.2       %
100000     | guided       | 8        | 3.616000     | 7.29     x | 91.1       %
------------------------------------------------------------------------------------
1000000    | Serial       | 1        | 321.171000   | 1.00x      | 100.0%      
1000000    | static       | 1        | 284.539000   | 1.13     x | 112.9      %
1000000    | dynamic,100  | 1        | 285.669000   | 1.12     x | 112.4      %
1000000    | guided       | 1        | 270.693000   | 1.19     x | 118.6      %
1000000    | static       | 2        | 134.684000   | 2.38     x | 119.2      %
1000000    | dynamic,100  | 2        | 133.592000   | 2.40     x | 120.2      %
1000000    | guided       | 2        | 308.522000   | 1.04     x | 52.0       %
1000000    | static       | 4        | 134.728000   | 2.38     x | 59.6       %
1000000    | dynamic,100  | 4        | 159.366000   | 2.02     x | 50.4       %
1000000    | guided       | 4        | 131.398000   | 2.44     x | 61.1       %
1000000    | static       | 8        | 139.985000   | 2.29     x | 28.7       %
1000000    | dynamic,100  | 8        | 131.582000   | 2.44     x | 30.5       %
1000000    | guided       | 8        | 141.814000   | 2.26     x | 28.3       %
------------------------------------------------------------------------------------

```


Q1. What is the difference between static, dynamic, and guided scheduling?   

schedule(static, chunk): Iterations are divided into chunks of size chunk (or N/threads contiguous blocks if unspecified) and assigned to threads at compile/launch time in a round-robin fashion before the loop begins. It has near-zero runtime scheduling overhead.
schedule(dynamic, chunk): Iterations are placed into a shared work queue and broken into fixed-size chunks (default chunk = 1). Threads request a new chunk from the queue at runtime on a first-come, first-served basis as soon as they finish their current chunk.
schedule(guided, chunk): Similar to dynamic in that chunks are assigned dynamically at runtime, but the chunk size starts large and decreases exponentially (proportional to unassigned iterations num / threads) down to a minimum specified chunk size (default 1). Large initial chunks minimize scheduling overhead, while small trailing chunks provide fine-grained load balancing at the end of the loop.

Q2. Which scheduling strategy performs best for uniform workloads? What about uneven workloads?   

Uniform workloads: schedule(static) performs best because every iteration takes the same time, meaning pre-dividing N/p contiguous iterations achieves natural load balance with zero atomic queue contention and optimal spatial cache locality.

Uneven workloads: schedule(dynamic, chunk) or schedule(guided) generally performs best. For monotonically increasing/decreasing workloads or random heavy spikes, static suffers from severe load imbalance (some threads finish early and sit idle waiting at the implicit barrier while one thread is stuck with heavy iterations). dynamic and guided dynamically redistribute work to idle threads. (Note: In the specific sample code where work = (i % 1000) * 100, the workload is periodic every 1,000 iterations. When N >> 1000, each contiguous block of N/p iterations in static actually contains roughly the same number of 1,000-element periods, unless N/p is not a multiple of 1,000 or chunks of size <1000 are used! However, within any sub-period, dynamic, 100 actively balances the light 0..99 vs heavy 900..999 chunks).   

Q3. Why can dynamic scheduling introduce additional overhead?   
Every time a thread finishes a chunk in dynamic scheduling, it must query and update the shared iteration counter in the OpenMP runtime using atomic operations or locks. This causes synchronization overhead and contention among threads. Furthermore, because threads grab non-contiguous chunks out of order, spatial locality is degraded and false sharing can occur if adjacent chunks share a cache line.

Q4. How does the chunk size affect performance?   

Very small chunk size (e.g., chunk = 1): Maximizes load balancing granularity, but forces threads to perform N atomic updates on the shared work queue, causing massive synchronization overhead and false sharing on array writes (a[i]).Very large chunk size (e.g., chunk = N / p): Reduces scheduling overhead to a minimum, but degrades into static scheduling, losing the ability to balance uneven workloads.Intermediate chunk size (e.g., chunk = 100 or 256): Amortizes atomic queue access over 100 iterations and ensures each chunk spans multiple cache lines (preventing false sharing), while remaining small enough relative to N to keep all threads evenly loaded.

# Task 3

```code
====================================================================================================
TASK 3: MULTIPLE REDUCTIONS vs ATOMIC UPDATES (EVEN / ODD COUNT)
====================================================================================================
Size (N)   | Method     | Threads | Time (s)   | Speedup   | Efficiency | Even+Odd==N  | Match   
----------------------------------------------------------------------------------------------------
100000     | Serial     | 1       | 0.001000   | 1.00x     | 100.0%     | YES          | Baseline
100000     | Reduction  | 1       | 0.001000   | 1.00    x | 100.0    % | YES          | PASS    
100000     | Atomic     | 1       | 0.002000   | 0.50    x | 50.0     % | YES          | PASS    
100000     | Reduction  | 2       | 0.003000   | 0.33    x | 16.7     % | YES          | PASS    
100000     | Atomic     | 2       | 0.003000   | 0.33    x | 16.7     % | YES          | PASS    
100000     | Reduction  | 4       | 0.001000   | 1.00    x | 25.0     % | YES          | PASS    
100000     | Atomic     | 4       | 0.003000   | 0.33    x | 8.3      % | YES          | PASS    
100000     | Reduction  | 8       | 0.001000   | 1.00    x | 12.5     % | YES          | PASS    
100000     | Atomic     | 8       | 0.005000   | 0.20    x | 2.5      % | YES          | PASS    
----------------------------------------------------------------------------------------------------
1000000    | Serial     | 1       | 0.008000   | 1.00x     | 100.0%     | YES          | Baseline
1000000    | Reduction  | 1       | 0.010000   | 0.80    x | 80.0     % | YES          | PASS    
1000000    | Atomic     | 1       | 0.014000   | 0.57    x | 57.1     % | YES          | PASS    
1000000    | Reduction  | 2       | 0.005000   | 1.60    x | 80.0     % | YES          | PASS    
1000000    | Atomic     | 2       | 0.027000   | 0.30    x | 14.8     % | YES          | PASS    
1000000    | Reduction  | 4       | 0.003000   | 2.67    x | 66.7     % | YES          | PASS    
1000000    | Atomic     | 4       | 0.031000   | 0.26    x | 6.5      % | YES          | PASS    
1000000    | Reduction  | 8       | 0.003000   | 2.67    x | 33.3     % | YES          | PASS    
1000000    | Atomic     | 8       | 0.035000   | 0.23    x | 2.9      % | YES          | PASS    
----------------------------------------------------------------------------------------------------
10000000   | Serial     | 1       | 0.089000   | 1.00x     | 100.0%     | YES          | Baseline
10000000   | Reduction  | 1       | 0.102000   | 0.87    x | 87.3     % | YES          | PASS    
10000000   | Atomic     | 1       | 0.140000   | 0.64    x | 63.6     % | YES          | PASS    
10000000   | Reduction  | 2       | 0.050000   | 1.78    x | 89.0     % | YES          | PASS    
10000000   | Atomic     | 2       | 0.287000   | 0.31    x | 15.5     % | YES          | PASS    
10000000   | Reduction  | 4       | 0.026000   | 3.42    x | 85.6     % | YES          | PASS    
10000000   | Atomic     | 4       | 0.319000   | 0.28    x | 7.0      % | YES          | PASS    
10000000   | Reduction  | 8       | 0.016000   | 5.56    x | 69.5     % | YES          | PASS    
10000000   | Atomic     | 8       | 0.335000   | 0.27    x | 3.3      % | YES          | PASS    
----------------------------------------------------------------------------------------------------
100000000  | Serial     | 1       | 0.912000   | 1.00x     | 100.0%     | YES          | Baseline
100000000  | Reduction  | 1       | 1.007000   | 0.91    x | 90.6     % | YES          | PASS    
100000000  | Atomic     | 1       | 1.411000   | 0.65    x | 64.6     % | YES          | PASS    
100000000  | Reduction  | 2       | 0.501000   | 1.82    x | 91.0     % | YES          | PASS    
100000000  | Atomic     | 2       | 2.826000   | 0.32    x | 16.1     % | YES          | PASS    
100000000  | Reduction  | 4       | 0.256000   | 3.56    x | 89.1     % | YES          | PASS    
100000000  | Atomic     | 4       | 3.263000   | 0.28    x | 7.0      % | YES          | PASS    
100000000  | Reduction  | 8       | 0.157000   | 5.81    x | 72.6     % | YES          | PASS    
100000000  | Atomic     | 8       | 3.517000   | 0.26    x | 3.2      % | YES          | PASS    
----------------------------------------------------------------------------------------------------

```

Q1. How can multiple variables be included in a reduction clause?   

Multiple variables sharing the same reduction operator can be listed as a comma-separated list inside a single reduction clause: reduction(+:even, odd). If different reduction operators are needed (e.g., + and max), multiple reduction clauses can be chained on the same directive (e.g., reduction(+:sum) reduction(max:max_val)). For each listed variable, OpenMP creates a private copy per thread initialized to the operator's identity value (0 for +), accumulates into the thread-private variables during the loop, and combines them into the global shared variable at the end of the parallel region.   

Q2. Why is the reduction approach generally faster than using atomic updates?   

Reduction: Each thread increments its own thread-private even and odd counters stored in CPU registers or local L1 cache with zero synchronization during the N loop iterations. Only O(p) atomic/tree-combine operations occur at the very end of the loop. 

Atomic (#pragma omp atomic): Every single iteration (N times) executes a hardware atomic instruction (such as lock add on x86) directly on the shared even and odd memory addresses. Because all threads hammer the exact same two variables (which also sit on the same cache line, causing severe cache coherency invalidation traffic / false and true sharing), the cores serialize their memory updates over the interconnect bus. Consequently, atomic updates often run 10x to 100x slower than serial code as thread counts increase.   

Q3. Why must the sum of the even and odd counts equal N?   

Every integer a[i] in the array is parity-classified into one of two mutually exclusive and collectively exhaustive sets: either a[i] = 0 mod 2 (even) or a[i] != 0 mod 2 (odd). Since the loop executes exactly N iterations and increments either even or odd by 1 in every iteration without race conditions (guaranteed by reduction or atomic), the invariant even_count + odd_count = N must strictly hold. If unprotected shared variables were used without reduction or atomic, lost updates from race conditions would cause even_count + odd_count < N.   

Q4. Does increasing the number of threads always improve performance?   No. Performance does not always scale linearly with thread count for three primary reasons:Memory Bandwidth Saturation: Counting even/odd integers has very low arithmetic intensity (only 1 modulo/bitwise operation per 4-byte integer load). Beyond 4 or 8 threads, the CPU cores saturate the main memory (DRAM) bus bandwidth, becoming memory-bound rather than compute-bound.Thread Overhead on Small N: For small arrays (N = 10^5), the cost of waking up 8 threads, partitioning the loop, and performing the final reduction tree takes longer than simply scanning 10^5 integers on 1 or 2 cores.Synchronization Contention: In the #pragma omp atomic implementation, adding more threads increases cache-line invalidations and bus lock contention, causing execution time to actively worsen as threads increase from 1 to 8.