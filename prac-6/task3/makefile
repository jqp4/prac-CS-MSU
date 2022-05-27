CPPFLAGS = -std=c++11 -qsmp=omp -Wall -Werror -fopenmp -lpthread -lm

all: compile submit

compile: 
	xlc++_r $(CPPFLAGS) main.cpp -o main

submit:
	bsub < ./lsf/OMP_job_10000_c2_t1.lsf
	bsub < ./lsf/OMP_job_10000_c2_t2.lsf
	bsub < ./lsf/OMP_job_10000_c2_t4.lsf
	bsub < ./lsf/OMP_job_10000_c4_t4.lsf
	bsub < ./lsf/OMP_job_10000_c4_t8.lsf
	bsub < ./lsf/OMP_job_10000_c8_t8.lsf
	bsub < ./lsf/OMP_job_10000_c8_t16.lsf
	bsub < ./lsf/OMP_job_10000_c16_t16.lsf
	bsub < ./lsf/OMP_job_100000_c2_t1.lsf
	bsub < ./lsf/OMP_job_100000_c2_t2.lsf
	bsub < ./lsf/OMP_job_100000_c2_t4.lsf
	bsub < ./lsf/OMP_job_100000_c4_t4.lsf
	bsub < ./lsf/OMP_job_100000_c4_t8.lsf
	bsub < ./lsf/OMP_job_100000_c8_t8.lsf
	bsub < ./lsf/OMP_job_100000_c8_t16.lsf
	bsub < ./lsf/OMP_job_100000_c16_t16.lsf
	bsub < ./lsf/OMP_job_1000000_c2_t1.lsf
	bsub < ./lsf/OMP_job_1000000_c2_t2.lsf
	bsub < ./lsf/OMP_job_1000000_c2_t4.lsf
	bsub < ./lsf/OMP_job_1000000_c4_t4.lsf
	bsub < ./lsf/OMP_job_1000000_c4_t8.lsf
	bsub < ./lsf/OMP_job_1000000_c8_t8.lsf
	bsub < ./lsf/OMP_job_1000000_c8_t16.lsf
	bsub < ./lsf/OMP_job_1000000_c16_t16.lsf
	bjobs
