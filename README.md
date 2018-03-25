# MI-PDP Course
## Paralelní a distribuované programování
### CTU / ČVUT FIT

During the whole semester I was tackling with the `minimum bisection width` problem. This task is unfortunaly `NP-hard`, thus to solve it we have to generate all possible subsets and compute their bisection widths. Besides the naive approach, there are some heuristics allowing us to diminish the runtime.

### Task 1
This folder contains the serial version with a naive heuristic implementation. The heuristic is effectively prunning the state space using the `branch and bounds` method. This methods removes states which would never lead to a better solution. This version intensively uses a bunch of `g++` optimisation flags:

```
CF=-march=native
CF+=-Ofast
CF+=-flto -Wno-sign-compare -std=c++17 -Wall -pedantic 
CF+=-fopenmp -openmp
CF+=-fstrict-aliasing -mfpmath=sse -mavx 
CF+=-mpc32 -funsafe-math-optimizations -ffast-math 
CF+=-ftree-vectorize -fopt-info-vec
CF+=-falign-functions=1 -falign-jumps=1 -falign-loops=1 -falign-labels=1
CF+=-fassociative-math -freciprocal-math -fno-trapping-math  -funroll-loops -ftree-loop-im
CF+=-D_GLIBCXX_PARALLEL
CF+=-g
```  
### Task 2
This variant parallelizes algorithm using `OpenMP` and task paralellization. Recursive function precomputes tasks to a certain preset depth and put these tasks into the queue. OpenMP generate virtual tasks, binds them to working threads. Each of these threads takes a task from queue and starts computation.

### Task 3
The folder contains implementation of parallelized algorithm using `OpenMP` `parallel for` syntax. This enables to make a solution more efficient in comparison with a task-based approach.

### Task 4
Parallelization using `MPI` with a clustered approach. The single master and multiple slave processes are spawned. The master precomputes tasks a distributes them using a `MPI` library. To make the solution more efficient event the master takes its bit and works on a task.

### Report
This folder contains source files and the generated report as well. Each task described previouly is incorporated and mutually compared in terms of runtime and memory consumption.

