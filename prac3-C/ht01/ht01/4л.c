#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum 
{
    BSIZE = 64, 
    DEC = 10,
    FUNC = 2, 

};

union u1{
    char *s;
    int i;
};

union u2{
    double d;
    int i[2];
};

int
main(int argc, char *argv[])
{
    void *dinLyb=dlopen(argv[1], RTLD_NOW);
    if (!(dinLyb)) {
        fprintf(stderr, "Error cant load dynlib\n");
        exit(1);
    }
    int mas[16] = {0};
    int iter_mas = 0;
    union u1 elem1;
    union u2 elem2;
    for (int i = 1; i < strlen(argv[FUNC + 1]); i++) {
        switch(argv[FUNC + 1][i]) {
            case 'd':
                //union u2 elem;
                sscanf(argv[FUNC + 1 + i], "%lf", &(elem2.d));
                mas[iter_mas++] = elem2.i[0];
                mas[iter_mas++] = elem2.i[1];
                // printf("%lf\n", elem2.d);
                break;
            case 's':
                //union u1 elem;
                elem1.s = argv[FUNC + i + 1];
                // printf("%s\n", elem1.s);
                mas[iter_mas++] = elem1.i;
                break;
            case 'i':
                //union u1 elem;
                sscanf(argv[FUNC + 1 + i], "%d", &(elem1.i));
                // printf("%d\n", elem1.i);
                mas[iter_mas++] = elem1.i;
                break;
        }
    }
    void *function = dlsym(dinLyb, argv[FUNC]);
    if (!function) {
        fprintf(stderr, "Error cant load funcion %s from %s\n", argv[FUNC], argv[1]);
        exit(1);
    }

    if (argv[FUNC+1][0] == 'v') {
        ((void (*)(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int)) function)(mas[0], mas[1], mas[2], mas[3], mas[4], mas[5], mas[6], mas[7], mas[8], mas[9], mas[10], mas[11], mas[12], mas[13], mas[14], mas[15]);
    }
    return 0;
}