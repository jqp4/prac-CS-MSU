CFLAGS = -std=c99 -Wall -Werror -O0 -lm

all: compile_job submit

compile_job: 
	mpicc $(CFLAGS) main.c -o main

gen_slaes:
	xlc $(CFLAGS) slae_generation.c -o slae_generation
	./slae_generation 300
	./slae_generation 1000
	./slae_generation 3000
	./slae_generation 6000

submit:
	bsub < ./lsf/task_2_job_300_1.lsf
	bsub < ./lsf/task_2_job_300_2.lsf
	bsub < ./lsf/task_2_job_300_4.lsf
	bsub < ./lsf/task_2_job_300_8.lsf
	bsub < ./lsf/task_2_job_1000_1.lsf
	bsub < ./lsf/task_2_job_1000_2.lsf
	bsub < ./lsf/task_2_job_1000_4.lsf
	bsub < ./lsf/task_2_job_1000_8.lsf
	bjobs -u all
