#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const char *root = "/Users/gleb/Projects/prac/up03/up03/";
char buff[200];

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


int main(void){
    FILE *fg = fopen(filepath("file_gleb.txt"), "w");
    FILE *ft = fopen(filepath("file_true.txt"), "w");
    
    fprintf(fg, "N = %d:\n", N);
    
    fclose(fg);
    fclose(ft);
    return 0;
}
