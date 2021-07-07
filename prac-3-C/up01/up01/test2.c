#include <stdio.h>

/* SBC - Six Bit Code */

int ASCIItoSBC(int a){
    if (48 <= a && a <= 57){
        return a - 47;
    }
    if (65 <= a && a <= 90){
        return a - 28;
    }
    if (97 <= a && a <= 122){
        return a - 86;
    }
    return -1;
}

int SBCtoASCII(int a){
    if (1  <= a && a <= 10){
        return a + 47;
    }
    if (11 <= a && a <= 36){
        return a + 86;
    }
    if (37 <= a && a <= 62){
        return a + 28;
    }
    if (a == 0){
        return 64;
    }
    return 35;
}

int breakSBC(int a){
    a += a % 4 - a % 8;
    a += (a % 16 - a % 8) ? -8 : 8;
    return a;
}

int main1q3g(FILE *f){
    int a;
    while ((a = getchar()) != '$') {
        a = ASCIItoSBC(a);
        if (a != -1) {
            a = SBCtoASCII(breakSBC(a));
            fprintf(stdout, "%c", a);
        }
    }
    return 0;
}


#include <stdlib.h>
#include <string.h>
#include <math.h>

enum {
    NUM = 1,
    LOWER = NUM + '9' - '0' + 1,
    UPPER = LOWER + 'z' - 'a' + 1
};

void c_e_second(int *cmp){
    int c = *cmp;
    if (c == 0) {
        *cmp = '@';
    }
    if (c > 0 && c < LOWER) {
        *cmp = c + '0' - NUM;
    }
    if (c >= LOWER && c < UPPER) {
        *cmp = c + 'a' - LOWER;
    }
    if (c >= UPPER && c < 64) {
        *cmp = c + 'A' - UPPER;
    }
}

int change_encoding(int *cmp){
    int c = *cmp;
    int tmp = 0;
    if (c >= '0' && c <= '9') {
        *cmp = c - '0' + NUM;
        *cmp ^= 1 << 3;
        *cmp &= ~(1 << 2);
        tmp = 1;
    }
    if (c >= 'a' && c <= 'z') {
        *cmp = c - 'a' + LOWER;
        *cmp ^= 1 << 3;
        *cmp &= ~(1 << 2);
        tmp = 1;
    }
    if (c >= 'A' && c <= 'Z') {
        *cmp = c - 'A' + UPPER;
        *cmp ^= 1 << 3;
        *cmp &= ~(1 << 2);
        tmp = 1;
    }
    return tmp;
}

int check_char(char c){
    return (('0' <= c && '9' >= c) || ('a' <= c && 'z' >= c) || ('A' <= c && 'Z' >= c));
}

int main234q2g(FILE *f){
    int c;
    while ((c = getchar()) != '$') {
        if (change_encoding(&c)) {
            c_e_second(&c);
            //putchar(c);
            printf("%c", c);
        } else {
            continue;
        }
    }
    return 0;
}



const char *root = "/Users/gleb/Projects/C/2_course/mz1/3/";
char buff[300];

char *filepath(char *filename){
    buff[0] = '\0';
    strcat(buff, root);
    strcat(buff, filename);
    return buff;
}

int main(void){
    FILE *fg = fopen(filepath("file_gleb.txt"), "w");
    FILE *ft = fopen(filepath("file_true.txt"), "w");
    
    
    //https://stackoverflow.com/questions/32547384/can-i-access-stdin-without-keyboard
    
    ungetc ('$', stdin);
    for (int j = 0; j < 256; j++){
        ungetc (j, stdin);
    }
    int x = main1q3g(fg);
    printf("\n");
    
    
    
    ungetc ('$', stdin);
    for (int j = 0; j < 256; j++){
        ungetc (j, stdin);
    }
    int y = main234q2g(ft);
    printf("\n");
    
    
    
    
    fclose(fg);
    fclose(ft);
    return 0;
}
