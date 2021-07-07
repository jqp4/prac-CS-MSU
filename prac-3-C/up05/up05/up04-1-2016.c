#include <stdio.h>

int main0(void) {
    int n, w;
    if (fscanf(stdin, "%d%d", &n, &w) != 2){
        fprintf(stdout, "invalid input\n");
        return 1;
    }
    for (int x = 0; x < 1 << n; x++) {
        fprintf(stdout, "|%*d|%*x|", w, x, w, x);
        fprintf(stdout, "%*d|\n", w, x >> (n - 1) ? x - (1 << n) : x);
    }
    return 0;
}















#include <string.h>
#include <stdint.h>
#include <inttypes.h>

int main2(FILE *f, int ni, int wi) {
    uint32_t n = (uint32_t)ni;
    int w = wi;
    for (uint32_t i = 0; i < (1 << n); i++) {
        fprintf(f, "|%*"PRId32"", w, i);
        fprintf(f, "|%*"PRIu32"", w, i);
        uint32_t j = i;
        if (i >= (1 << (n - 1))) {
            j = (1 << (n - 1)) - i;
        }
        fprintf(f, "|%*"PRId32"|\n", w, j);
    }
    return 0;
}

int main1(FILE *f, int ni, int wi) {
    int n = ni, w = wi;
    for (int x = 0; x < 1 << n; x++) {
        fprintf(f, "|%*d|%*x|", w, x, w, x);
        fprintf(f, "%*d|\n", w, x >> (n - 1) ? x - (1 << n) : x);
    }
    return 0;
}

const char *root = "/Users/gleb/Projects/prac/up04/up04/";
char buff[200];

char *filepath(char *filename){
    buff[0] = '\0';
    strcat(buff, root);
    strcat(buff, filename);
    return buff;
}

int main(void){
    int n = 4, w = 6;
    FILE *fg = fopen(filepath("file_gleb.txt"), "w");
    FILE *ft = fopen(filepath("file_true.txt"), "w");
    
    main1(fg, n, w);
    main2(ft, n, w);
    
    fclose(fg);
    fclose(ft);
    return 0;
}
