CFLAGS = -std=c99 -Wall -Werror -O0 -lm
CPPFLAGS=  -Wall -Werror

all: compile submit1

compile: main.cpp 
	mpicxx $(CPPFLAGS) -c main.cpp
	mpicxx $(CPPFLAGS) main.o -o main

submit:
	bsub < ./lsf/task2job_15.09.50_300_1.lsf
	bsub < ./lsf/task2job_15.09.50_300_2.lsf
	bsub < ./lsf/task2job_15.09.50_300_4.lsf
	bsub < ./lsf/task2job_15.09.50_300_8.lsf
	bsub < ./lsf/task2job_15.09.50_300_16.lsf
	bsub < ./lsf/task2job_15.09.50_300_32.lsf
	bjobs -u all

submit1:
	./main 300 3 1

generate: slae_generation.c
	xlc $(CFLAGS) slae_generation.c -o slae_generation
	./slae_generation 300
	./slae_generation 1000
	./slae_generation 3000
	./slae_generation 6000

clean:
	rm -rf slae_generation main

clean_err:
	rm -rf *.err

clean_out:
	rm -rf *.out
	