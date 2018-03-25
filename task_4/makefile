procs?=2
threads?=1

# OMPI_CXX=clang++

# CF=-march=native
CF=-march=sandybridge
CF+=-Ofast
# CF+=-O2
# CF+=-O0
CF+=-flto -Wno-sign-compare -std=c++17 -Wall -pedantic 
CF+=-fopenmp -openmp
CF+=-fstrict-aliasing -mfpmath=sse -mavx 
CF+=-mpc32 -funsafe-math-optimizations -ffast-math 
CF+=-ftree-vectorize -fopt-info-vec
CF+=-falign-functions=1 -falign-jumps=1 -falign-loops=1 -falign-labels=1
CF+=-fassociative-math -freciprocal-math -fno-trapping-math  -funroll-loops -ftree-loop-im
CF+=-D_GLIBCXX_PARALLEL
# CF+=-DDUMP_TASKS
# CF+=-DDUMP_COMBS
CF+=-g

all: compile

compile:
	mpic++ $(CF) main.cpp worker.cpp -o pdp

callgrind:
	mpirun -np ${procs} -show-progress valgrind --tool=callgrind ./pdp ./data/graph30_5.txt 8 $(threads)

cachegrind:
	mpirun -np ${procs} -show-progress valgrind --tool=cachegrind ./pdp ./data/graph30_5.txt 8 $(threads)
	
run:
	mpirun -np ${procs} ./pdp ./data/graph40_5.txt 16 $(threads)
