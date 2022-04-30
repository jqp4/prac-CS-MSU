CFLAGS = -std=c99 -Wall -Werror -O0 -fopenmp -lpthread

all: 
	gcc $(CFLAGS) parallel_algorithm.c -o parallel_algorithm -lm -fsanitize=address
	gcc $(CFLAGS) slae_generation.c -o slae_generation -lm -fsanitize=address
	./slae_generation 1000
	./parallel_algorithm 1000 4 slae

parallel_algorithm:
	rm -rf c_test_p parallel_algorithm
	gcc $(CFLAGS) parallel_algorithm.c -o parallel_algorithm -lm -fsanitize=address

clean:
	rm -rf parallel_algorithm slae_generation

slae_generation:
	rm -rf slae slae_generation
	gcc $(CFLAGS) slae_generation.c -o slae_generation -lm -fsanitize=address

test: clean slae_generation parallel_algorithm
	./slae_generation 1000
	./parallel_algorithm 1000 4 slae