//
//  main.c
//  contest 4
//
//  Created by  Gleb on 07.12.2019.
//  Copyright © 2019  Gleb. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#define MAX_STR_SIZE 80


int lenstr(char *s){
    int n = 0;
    while (s[n] != '\0'){
        n++;
    }
    return n;
}


void showstr(char *s, int n){
    for (int i = 0; i < n; i++){
        printf("%c", s[i]);
    }
}


int getint(int *n){
    scanf("%d", n);
    return *n;
}


char *get_bifurcated_string(void){
    char *s = (char *)malloc((MAX_STR_SIZE + 1) * sizeof(char));
    scanf("%s", s);
    int len = lenstr(s);
    s = (char *)realloc(s, (2 * len + 1) * sizeof(char));
    for (int i = 0; i < len; i++){
        s[len + i] = s[i];
    }
    s[2 * len] = '\0';
    return s;
}



int main(void){
    int N = getint(&N);
    char *s = get_bifurcated_string();
    int l = (int)(lenstr(s) / 2);
    showstr(s + (l != 0 ? N % l : 0), l);
    free(s);
    return 0;
}
