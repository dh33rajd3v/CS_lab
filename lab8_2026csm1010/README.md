## Task 1

### compile task 1:
```code
gcc -O2 -fopenmp -Wall task1.c -o task1
```

### Results of task 1
```code
Array Search (N = 2147483647)
Sequential: Found 999999 at index 3750 in 0.000000 s

Threads    Time (s)        Speedup      Efficiency   Verified  
1          2.294000        0.00        x 0.0        % PASS      
2          1.328000        0.00        x 0.0        % PASS      
4          0.669000        0.00        x 0.0        % PASS      
8          0.244000        0.00        x 0.0        % PASS  
```

## Task 2

### compile task 2:
```code
gcc -O2 -fopenmp -Wall task2.c -o task2
```

### Results of task 2
```code
Task 2: Odd-Even Parallel Bubble Sort (N = 20000)
Sequential Bubble Sort Time: 0.723000 s

Threads    Time (s)        Speedup      Efficiency   Verified  
1          0.372000        1.94        x 1.9        % PASS      
2          0.567000        1.28        x 0.6        % PASS      
4          0.915000        0.79        x 0.2        % PASS      
8          1.673000        0.43        x 0.1        % PASS  
```

## Task 3

### compile task 3:
```code
gcc -O2 -fopenmp -Wall task3.c -o task3
```

### Results of task 3
```code
Task 3: Histogram Calculation (N = 100000000)
Sequential Time: 0.087000 s

Threads    Time (s)        Speedup      Efficiency   Verified  
1          0.088000        0.99        x 1.0        % PASS      
2          0.031000        2.81        x 1.4        % PASS      
4          0.020000        4.35        x 1.1        % PASS      
8          0.015000        5.80        x 0.7        % PASS      
```

## Task 4

### compile task 4:
```code
gcc -O2 -fopenmp -Wall task4.c -o task4
```

### Results of task 4
```code
Task 4: In-Place 2D Matrix Transpose (4096x4096)
Sequential Time: 0.293000 s

Threads    Time (s)        Speedup      Efficiency   Verified  
1          0.294000        1.00        x 1.0        % PASS      
2          0.234000        1.25        x 0.6        % PASS      
4          0.152000        1.93        x 0.5        % PASS      
8          0.082000        3.57        x 0.4        % PASS      
```

## Task 5

### compile task 5:
```code
gcc -O2 -fopenmp -Wall task5.c -o task5
```

### Results of task 5
```code
Task 5: 2D Convolution (4096x4096)
Sequential Time: 0.132000 s

Threads    Time (s)        Speedup      Efficiency   Verified  
1          0.153000        0.86        x 0.9        % PASS      
2          0.062000        2.13        x 1.1        % PASS      
4          0.033000        4.00        x 1.0        % PASS      
8          0.026000        5.08        x 0.6        % PASS     
```