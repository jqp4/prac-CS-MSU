#include <stdint.h>
#include <stdio.h>

enum { ARGC = 3 };

int main(void) {
    int n, w;
    unsigned s;
    if (fscanf(stdin, "%d%u%d", &n, &s, &w) != ARGC){
        fprintf(stdout, "invalid input\n");
        return 1;
    }
    for (uint_fast64_t x = 0; x < 1 << n; x += s) {
        fprintf(stdout, "|%*llo|%*llu|", w, x, w, x);
        uint_fast64_t sign = x >> (n - 1);
        int_least64_t y = (int_least64_t)(x - (sign << (n - 1)));
        y *= (sign ? -1 : 1);
        fprintf(stdout, "%*lld|\n", w, y);

    }
    return 0;
}
