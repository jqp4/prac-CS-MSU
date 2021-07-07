#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char S[] = "rwxrwxrwx";

enum
{
    OCTBASE = 8,
    LEN = sizeof(S) - 2,
};

int main(int argc, char *argv[]){
    for (int i = 1; i < argc; i++) {
        char *end;
        int x = (int)strtol(argv[i], &end, OCTBASE);
        for (int bit = LEN; bit + 1; bit--) {
            putchar(x & (1 << bit) ? S[LEN - bit] : '-');
        }
        putchar('\n');
    }
    return 0;
}









/*int main0(int argc, char *argv[]){
    for (int i = 1; i < argc; i++) {
        char *end;
        int x = (int)strtol(argv[i], &end, BASE);
        for (int bit = BIT3; bit + 1; bit--) {
            putchar(x & (1 << bit) ? S[BIT3 - bit] : '-');
        }
        putchar('\n');
    }
    return 0;
}*/
