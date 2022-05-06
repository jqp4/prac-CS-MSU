CFLAGS = -qsmp=omp -Wall -Werror -fopenmp -lpthread

all: compile run


compile: 
	xlc $(CFLAGS) parallel_algorithm.c -o parallel_algorithm -lm
	xlc $(CFLAGS) slae_generation.c -o slae_generation -lm

run:
	./slae_generation 1000
	./parallel_algorithm 1000 4 slae

bsub:
	bsub < OpenMP_job.lsf
