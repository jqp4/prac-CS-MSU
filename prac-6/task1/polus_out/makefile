CFLAGS = -qsmp=omp -Wall -Werror -fopenmp -lpthread -lm

all: compile submit

compile: 
	xlc $(CFLAGS) parallel_algorithm.c -o parallel_algorithm
	xlc $(CFLAGS) slae_generation.c -o slae_generation

run:
	./slae_generation 1000
	./parallel_algorithm 1000 4 slae

submit:
	./slae_generation 300
	./slae_generation 1000
	./slae_generation 3000
	./slae_generation 6000
	bsub < ./lsf/OMP_job_300_1.lsf
	bsub < ./lsf/OMP_job_300_2.lsf
	bsub < ./lsf/OMP_job_300_4.lsf
	bsub < ./lsf/OMP_job_300_8.lsf
	bsub < ./lsf/OMP_job_300_16.lsf
	bsub < ./lsf/OMP_job_300_32.lsf
	bjobs
