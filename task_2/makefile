# zadani: Paralelni verze OpenMP - task
# autor Martin Holoubek

CF=-march=native --std=c++17
# CF+=-DTHREADS=40
CF+=-DTHREADS=$(threads)
# CF+=-Ofast -flto -Wno-sign-compare -std=c++11 -Wall -pedantic 
CF+=-Ofast
CF+=-fopenmp -fstrict-aliasing -mfpmath=sse -mavx 
CF+=-mpc32 -funsafe-math-optimizations -ffast-math 
CF+=-ftree-vectorize -fopt-info-vec
CF+=-falign-functions=1 -falign-jumps=1 -falign-loops=1 -falign-labels=1
CF+=-fassociative-math -freciprocal-math -fno-trapping-math  -funroll-loops -ftree-loop-im
CF+=-D_GLIBCXX_PARALLEL
CF+=-g

all: main.cpp
	g++ $(CF) main.cpp -o pdp

