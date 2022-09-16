CFLAGS = -std=c99 -Wall -Werror -O0 -lm
CPPFLAGS= -Wall -Werror

all: create_table compile submit

compile: main.cpp 
	mpicxx $(CPPFLAGS) -c main.cpp
	mpicxx $(CPPFLAGS) main.o -o run
	rm main.o

submit:
	bsub < ./lsf/task2_job003_n300_p1.lsf
	bsub < ./lsf/task2_job003_n300_p2.lsf
	bsub < ./lsf/task2_job003_n300_p4.lsf
	bsub < ./lsf/task2_job003_n300_p8.lsf
	bsub < ./lsf/task2_job003_n300_p16.lsf
	bsub < ./lsf/task2_job003_n300_p32.lsf
	bsub < ./lsf/task2_job003_n1000_p1.lsf
	bsub < ./lsf/task2_job003_n1000_p2.lsf
	bsub < ./lsf/task2_job003_n1000_p4.lsf
	bsub < ./lsf/task2_job003_n1000_p8.lsf
	bsub < ./lsf/task2_job003_n1000_p16.lsf
	bsub < ./lsf/task2_job003_n1000_p32.lsf
	bsub < ./lsf/task2_job003_n3000_p1.lsf
	bsub < ./lsf/task2_job003_n3000_p2.lsf
	bsub < ./lsf/task2_job003_n3000_p4.lsf
	bsub < ./lsf/task2_job003_n3000_p8.lsf
	bsub < ./lsf/task2_job003_n3000_p16.lsf
	bsub < ./lsf/task2_job003_n3000_p32.lsf
	bsub < ./lsf/task2_job003_n6000_p1.lsf
	bsub < ./lsf/task2_job003_n6000_p2.lsf
	bsub < ./lsf/task2_job003_n6000_p4.lsf
	bsub < ./lsf/task2_job003_n6000_p8.lsf
	bsub < ./lsf/task2_job003_n6000_p16.lsf
	bsub < ./lsf/task2_job003_n6000_p32.lsf
	bjobs -u all

submit1:
	mpiexec ./run 300

create_table:
	rm results.csv
	touch results.csv
	echo "threads_count;matrix_size;t1;t2;t_all;mse;residual_norm;" >> results.csv


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
	