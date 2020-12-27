#include <stdio.h>
#include <stdlib.h>

enum
{
    OBASE = 10
};

int main(int argc, char *argv[]){
    long long sum_pos = 0;
    long long sum_neg = 0;
    for (int i = 1; i < argc; i++) {
        char *end;
        long long x = strtoll(argv[i], &end, OBASE);
        if (*end){
            fprintf(stderr, "invalid input\n");
            return 1;
        } else if (x > 0){
            sum_pos += x;
        } else {
            sum_neg += x;
        }
    }
    fprintf(stdout, "%lld\n%lld\n", sum_pos, sum_neg);
    return 0;
}
