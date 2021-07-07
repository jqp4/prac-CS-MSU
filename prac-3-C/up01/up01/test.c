#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

enum { MAX_N = 2000 };

int main2(int inpN, FILE *f){
    int n = inpN;
    for (int i = 2; i * i < n; i++) {
        if (n % i == 0) {
            return 1;
        }
    }
    for (int i = 0; i < n - 1; i++) {
        //printf("0 ");
        fprintf(f, "0 ");
    }
    //printf("\n");
    fprintf(f, "\n<[N %d] ", n);
    int first_row[MAX_N];
    int prev_row[MAX_N];
    first_row[0] = 1;
    prev_row[0] = 1;
    for (int i = 1; i < n - 1; i++) {
        first_row[i] = (n - (n / (i + 1)) * first_row[n % (i + 1) - 1] % n) % n;
        prev_row[i] = first_row[i];
    }
    for(int i = 0; i < n - 1; i++) {
        //printf("%d ", first_row[i]);
        fprintf(f, "%d ", first_row[i]);
    }
    //printf("\n");
    fprintf(f, "\n<[N %d] ", n);
    int tmp;
    for(int i = 1; i < n - 1; i++) {
        for(int j = 0; j < n - 1; j++) {
            tmp = (first_row[j] + prev_row[j]) % n;
            prev_row[j] = tmp;
            //printf("%d ", tmp);
            fprintf(f, "%d ", tmp);
        }
        //printf("\n");
        fprintf(f, "\n<[N %d] ", n);
    }
    return 0;
}

int main1(int inpN, FILE *f){
    short N, a, b, c, cmax, i;
    //scanf("%hi", &N);
    N = inpN;
    const short r = N > 100 ? N / 10 : N;
    short m[r][N];
    for (i = 0; i < N; i += r) {
        cmax = i + r < N ? i + r : N;
        for (b = 0; b < N; b++) {
            for (a = 0; a < N; a++) {
                c = (a * b) % N;
                if (c >= i && c < cmax) {
                    m[c % r][a] = b;
                }
            }
        }
        cmax -= i;
        for (c = 0; c < cmax; c++) {
            for (a = 1; a < N; a++) {
                //printf("%hi ", m[c][a]);
                fprintf(f, "%hi ", m[c][a]);
            }
            //printf("\n");
            fprintf(f, "\n<[N %d] ", N);
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

int maintest_sf(void){
    FILE *S1, *S2;
    int x, y;
    printf("Введите число : ");
    scanf("%d", &x);
    S1 = fopen(filepath("s1.txt"), "w");
    fprintf(S1, "%d", x);
    fclose(S1);
    S1 = fopen(filepath("s1.txt"), "r");
    S2 = fopen(filepath("s2.txt"), "w");
    fscanf(S1, "%d", &y);
    y += 3;
    fclose(S1);
    fprintf(S2, "%d\n", y);
    fclose(S2);
    return 0;
}

int isSimple(int x){
    int imax = (int)sqrt(x) + 1;
    for (int i = 2; i < imax; i++){
        if (x % i == 0){
            return 0;
        }
    }
    return 1;
}

int main(void){
    FILE *fg = fopen(filepath("file_gleb.txt"), "w");
    FILE *ft = fopen(filepath("file_true.txt"), "w");
    
    for (int N = 2; N < 309; N++){
        if (isSimple(N)){
            printf("%d ", N);
            fprintf(fg, "N = %d:\n", N);
            fprintf(ft, "N = %d:\n", N);
            main11(N, fg);
            main2(N, ft);
        }
    }
    
    fclose(fg);
    fclose(ft);
    return 0;
}
