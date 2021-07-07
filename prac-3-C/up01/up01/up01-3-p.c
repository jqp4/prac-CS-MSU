#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

enum { MAX_N = 2000 };

int binpow(int a, int n, int N){
    int res = 1;
    while (n) {
        if (n & 1) {
            res *= a;
            res = res % N;
        }
        a *= a;
        a = a % N;
        n >>= 1;
    }
    return res;
}

int main3(int N, FILE *f){
    //int N;
    if (scanf("%d", &N) != 1) {
        fprintf(stderr, "invalid input\n");
        return 1;
    }
    if (N < 2 || N >= 2000){
        fprintf(stderr, "number out of range\n");
        return 2;
    }
    int reverse[2000];
    for (int i = 1; i < N; i++) {
        reverse[i] = binpow(i, N - 2, N);
    }
    for (int i = 0; i < N; i++) {
        for (int j = 1; j < N; j++) {
            //printf("%d ", i * reverse[j] % N);
            fprintf(f, "%d ", i * reverse[j] % N);
        }
        //putchar('\n');
        fprintf(f, "\n<[N %d] ", N);
    }
    return 0;
}

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

const char *root = "/Users/gleb/Projects/C/2_course/mz1/3/";
char buff[300];

char *filepath(char *filename){
    buff[0] = '\0';
    strcat(buff, root);
    strcat(buff, filename);
    return buff;
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

int main_(void){
    FILE *fg = fopen(filepath("file_gleb.txt"), "w");
    FILE *ft = fopen(filepath("file_true.txt"), "w");
    
    for (int N = 2; N < 309; N++){
        if (isSimple(N)){
            printf("%d ", N);
            fprintf(fg, "N = %d:\n", N);
            fprintf(ft, "N = %d:\n", N);
            main1(N, fg);
            main2(N, ft);
        }
    }
    
    fclose(fg);
    fclose(ft);
    return 0;
}


int main_gleb(void){
    short N, a, b, c, cmax, i;
    /*if (scanf("%hi", &N) != 1) {
        fprintf(stderr, "Ошибка ввода\n");
        return 1;
    }
    if (N >= 2000 || N <= 1){
        fprintf(stderr, "Введенные данные не входят в диапазон рабочих значений\n");
        return 2;
    }*/
    //for (int xxx = 300; xxx < 1000; xxx += 50){
    //int xxx = 2;
    //time_start();
    N = 5;
    const short r = 2;//xxx; //N > 100 ? N / 10 : N;
    //short m[r][N];
    //short *m = calloc(N * r, sizeof(*m));
     short m[r][N];
    for (i = 0; i < N; i += r) {
        cmax = i + r < N ? i + r : N;
        for (b = 0; b < N; b++) {
            for (a = 0; a < N; a++) {
                c = (a * b) % N;
                if (c >= i && c < cmax) {
                    m[c % r][a] = b;
                    //m[(c % r) * r + a] = b;
                }
            }
        }
        cmax -= i;
        for (c = 0; c < cmax; c++) {
            for (a = 1; a < N; a++) {
                printf("%hi ", m[c][a]);
                //printf("%hi ", m[c * r + a]);
            }
            printf("\n");
        }
    }
    //printf("xxx = %d Time: %ld\n", xxx, time_stop());
    return 0;
}
