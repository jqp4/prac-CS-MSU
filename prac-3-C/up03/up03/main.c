#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum
{
    FPERCENT = 100,
    FACTOR = 10000,
    RATE_ARGN = 1,
    MIN_ARGC = 2
};

int
main(int argc, char *argv[])
{
    if (argc < MIN_ARGC) {
        fprintf(stderr, "invalid input\n");
        return 1;
    }
    char *end;
    double rate = strtod(argv[RATE_ARGN], &end);
    if (argc - MIN_ARGC) {
        for (int i = MIN_ARGC; i < argc; i++) {
            double perc = strtod(argv[i], &end);
            rate = rate * (FPERCENT + perc) / FPERCENT;
            rate = round(rate * FACTOR) / FACTOR;
        }
    }
    fprintf(stdout, "%.4lf\n", rate);
    return 0;
}
