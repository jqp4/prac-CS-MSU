#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

// https://seminar.ruro.live/ejudge1/0073

enum
{
    DECBASE = 10,
    COUNT_AN = 1,
    SEED_AN = 4,
    HIGH_AN = 3,
    LOW_AN = 2,
    TARGC = 5,
};


int main435(int argc, const char *argv[]) {
    /*if (argc != TARGC) {
        fprintf(stderr, "invalid input\n");
        return 1;
    }
    char *end;
    unsigned count = (unsigned)strtoul(argv[COUNT_AN], &end, DECBASE);
    unsigned seed = (unsigned)strtoul(argv[SEED_AN], &end, DECBASE);
    int high = (int)strtol(argv[HIGH_AN], &end, DECBASE);
    int low = (int)strtol(argv[LOW_AN], &end, DECBASE);
    srand(seed);
    
    while (count--) {
        double rx = (double)rand() / RAND_MAX;
        int x = rx * (high - low) + low;
        printf("%d\n", x);
    }*/
    
    
    
    char *end;
    int low = (int)strtol(argv[LOW_AN], &end, DECBASE);
    int high = (int)strtol(argv[HIGH_AN], &end, DECBASE);
    srand((unsigned)strtoul(argv[SEED_AN], &end, DECBASE));
    for (unsigned long i = strtoul(argv[1], &end, DECBASE); i--;) {
        printf("%d\n", (int)((double)rand() / RAND_MAX  * (high - low) + low));
    }
    
    
    
    
    return 0;
}
