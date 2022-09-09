CFLAGS = -std=c99 -Wall -Werror -O0 -lm
CPPFLAGS= -Wall -Werror

all: create_table compile submit clean_bin

compile: main.cpp 
	mpicxx $(CPPFLAGS) -c main.cpp
	mpicxx $(CPPFLAGS) main.o -o run

submit:
	bsub < ./lsf/task2job_15.09.50_300_1.lsf
	bsub < ./lsf/task2job_15.09.50_300_2.lsf
	bsub < ./lsf/task2job_15.09.50_300_4.lsf
	bsub < ./lsf/task2job_15.09.50_300_8.lsf
	bsub < ./lsf/task2job_15.09.50_1000_1.lsf
	bsub < ./lsf/task2job_15.09.50_1000_2.lsf
	bsub < ./lsf/task2job_15.09.50_1000_4.lsf
	bsub < ./lsf/task2job_15.09.50_1000_8.lsf
	bsub < ./lsf/task2job_15.09.50_3000_1.lsf
	bsub < ./lsf/task2job_15.09.50_3000_2.lsf
	bsub < ./lsf/task2job_15.09.50_3000_4.lsf
	bsub < ./lsf/task2job_15.09.50_3000_8.lsf
	bjobs -u all

submit1:
	mpiexec ./run 300

create_table:
	rm results.csv
	touch results.csv
	echo "threads_count;matrix_size;t1;t2;t_all;" >> results.csv


generate: slae_generation.c
	xlc $(CFLAGS) slae_generation.c -o slae_generation
	./slae_generation 300
	./slae_generation 1000
	./slae_generation 3000
	./slae_generation 6000

clean_bin:
	rm main.o
	rm run

clean:
	rm -rf slae_generation main

clean_err:
	rm -rf *.err

clean_out:
	rm -rf *.out
	