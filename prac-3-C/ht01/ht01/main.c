#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum
{
    BSIZE = 64,
    DEC = 10,
    FUNC = 2,
    CHAR_BIT = 8,
    LONG_BIT = 32,
};

union ud{
    double d;
    long i[2];
};

void double_to_bits(double val){
    /*unsigned idx;
    unsigned char arr[sizeof val];
    memcpy (arr, &val, sizeof val);
    for (idx = CHAR_BIT * sizeof val; idx-- ; ) {
        putc((arr[idx / CHAR_BIT] & (1u << (idx % CHAR_BIT))) ? '1' : '0', stdout);
    }*/
    
    unsigned char arr[sizeof val];
    memcpy (arr, &val, sizeof val);
    for (unsigned idx = CHAR_BIT * sizeof val; idx-- ; ) {
        putc((arr[idx / CHAR_BIT] & (1u << (idx % CHAR_BIT))) ? '1' : '0', stdout);
        //putchar((arr[idx / CHAR_BIT] & (1u << (idx % CHAR_BIT))) ? '1' : '0');
    }
    printf("\n");
    
    for (unsigned idx = sizeof val; idx--;) {
        printf("       %d", arr[idx] & 1);
    }
    printf("\n");
    
    unsigned long a[2];
    memcpy (a, &val, sizeof val);
    for (unsigned i = 2; i--;) {
        printf("        ");
        printf("        ");
        printf("        ");
        printf("       %d", (int)(a[i] & 1));
    }
    printf("\n");
}



int
main(int argc, char *argv[])
{
    void *dinLyb=dlopen(argv[1], RTLD_NOW);
    if (!(dinLyb)) {
        fprintf(stderr, "Error cant load dynlib\n");
        exit(1);
    }
    long mas[16] = {0};
    int iter_mas = 0;
    //union ud elem;
    double d;
    for (int i = 1; i < strlen(argv[FUNC + 1]); i++) {
        switch(argv[FUNC + 1][i]) {
            case 'd':
                sscanf(argv[FUNC + 1 + i], "%lf", &d);
                //double_to_bits(d);
                
                
                unsigned long arr[2];
                memcpy (arr, &d, sizeof d);
                mas[iter_mas++] = arr[0];
                mas[iter_mas++] = arr[1];
                
                //memcpy ((double *)(mas + iter_mas), &d, sizeof(d));
                break;
            case 's':
                mas[iter_mas++] = (long)argv[FUNC + i + 1];
                break;
            case 'i':
                sscanf(argv[FUNC + 1 + i], "%ld", &(mas[iter_mas++]));
                break;
        }
    }
    void *function = dlsym(dinLyb, argv[FUNC]);
    if (!function) {
        fprintf(stderr, "Error cant load funcion %s from %s\n", argv[FUNC], argv[1]);
        exit(1);
    }

    if (argv[FUNC+1][0] == 'v') {
        ((void (*)(long, long, long, long, long, long, long, long, long, long, long, long, long, long, long, long)) function)(mas[0], mas[1], mas[2], mas[3], mas[4], mas[5], mas[6], mas[7], mas[8], mas[9], mas[10], mas[11], mas[12], mas[13], mas[14], mas[15]);
    }
    return 0;
}
