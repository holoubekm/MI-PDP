
CF=-march=sandybridge
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

all: main.cpp
	g++ $(CF) main.cpp -o pdp
