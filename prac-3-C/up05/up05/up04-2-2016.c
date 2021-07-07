#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <time.h>

enum
{
    LEN = 3,
    BASE = 10,
    MIN3DIG = 100,
    MAX3DIG = 999
};

char *S = "-----x-w--wxr--r-xrw-rwx";

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        char *end;
        int x = (int)strtol(argv[i], &end, BASE);
        if (*end || x > MAX3DIG){
            fprintf(stderr, "invalid input\n");
            return 1;
        }
        int y[LEN] = {x / MIN3DIG, x / BASE % BASE, x % BASE};
        for (int i = 0; i < LEN; i++){
            for (int d = 0; d < LEN; d++) {
                putchar(S[y[i] * LEN + d]);
            }
        }
        putchar('\n');
    }
    return 0;
}















int main1(FILE *f, int n, int *a) {
    for (int i = 0; i < n; i++) {
        /*char *end;
        int x = (int)strtol(argv[i], &end, BASE);
        if (*end || x > MAX3DIG){
            fprintf(stderr, "invalid input\n");
            return 1;
        }*/
        int x = a[i];
        int y[LEN] = {x / MIN3DIG, x / BASE % BASE, x % BASE};
        for (int i = 0; i < LEN; i++){
            for (int d = 0; d < LEN; d++) {
                putchar(S[y[i] * LEN + d]);
            }
        }
        putchar('\n');
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

int main_(void){
    srand((unsigned)time(NULL));
    FILE *fg = fopen(filepath("file_gleb.txt"), "w");
    FILE *ft = fopen(filepath("file_true.txt"), "w");
    int n = 2;
    int a[n];
    for (int i = 0; i < n; i++){
        a[i] = rand() % 999;
    }
    a[0] = 644;
    a[1] = 755;
    
    main1(fg, n, a);
    //main2(ft, n, a);
    
    fclose(fg);
    fclose(ft);
    return 0;
}
