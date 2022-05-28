CFLAGS = -qsmp=omp -Wall -Werror -fopenmp -lpthread -lm

all: compile submit

compile: 
	xlc $(CFLAGS) parallel_algorithm.c -o parallel_algorithm
	xlc $(CFLAGS) slae_generation.c -o slae_generation

run:
	./slae_generation 1000
	./parallel_algorithm 1000 4 slae

submit:
	bsub < ./lsf/OMP_job_300_1.lsf
	bsub < ./lsf/OMP_job_300_2.lsf
	bsub < ./lsf/OMP_job_300_4.lsf
	bsub < ./lsf/OMP_job_300_8.lsf
	bsub < ./lsf/OMP_job_300_16.lsf
	bsub < ./lsf/OMP_job_300_32.lsf
	bsub < ./lsf/OMP_job_1000_1.lsf
	bsub < ./lsf/OMP_job_1000_2.lsf
	bsub < ./lsf/OMP_job_1000_4.lsf
	bsub < ./lsf/OMP_job_1000_8.lsf
	bsub < ./lsf/OMP_job_1000_16.lsf
	bsub < ./lsf/OMP_job_1000_32.lsf
	bsub < ./lsf/OMP_job_3000_1.lsf
	bsub < ./lsf/OMP_job_3000_2.lsf
	bsub < ./lsf/OMP_job_3000_4.lsf
	bsub < ./lsf/OMP_job_3000_8.lsf
	bsub < ./lsf/OMP_job_3000_16.lsf
	bsub < ./lsf/OMP_job_3000_32.lsf
	bsub < ./lsf/OMP_job_6000_1.lsf
	bsub < ./lsf/OMP_job_6000_2.lsf
	bsub < ./lsf/OMP_job_6000_4.lsf
	bsub < ./lsf/OMP_job_6000_8.lsf
	bsub < ./lsf/OMP_job_6000_16.lsf
	bsub < ./lsf/OMP_job_6000_32.lsf
	bjobs
