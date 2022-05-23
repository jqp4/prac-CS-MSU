CPPFLAGS = -std=c++11 -qsmp=omp -Wall -Werror -fopenmp -lpthread -lm

all: compile submit

compile: 
	xlc++_r $(CPPFLAGS) main.cpp -o main

submit:
	bsub < ./lsf/OMP_job_300_1.lsf
	bjobs
