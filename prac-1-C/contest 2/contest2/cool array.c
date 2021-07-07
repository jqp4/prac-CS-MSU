//
//  main.c
//  contest 1 1.14
//
//  Created by  Gleb on 26/09/2019.
//  Copyright © 2019  Gleb. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>



int *input(int *n){
    scanf("%d", n);
    int *a;
    a = malloc((*n + 1) * sizeof(int));
    *a = *n;
    a++;
    for (int i = 0; i < *n; i++){
        scanf("%d", &a[i]);
    }
    return a;
}


void my_free(int *a){
    free(--a);
}


void output_(int *a){
    int n = *(a - 1);
    for (int i = 0; i < n; i++){
        printf("%d ", a[i]);
    }
    printf("\n");
}


void output(int *a){
    int n = sizeof(a) / sizeof(int);
    for (int i = 0; i < n; i++){
        printf("%d ", a[i]);
    }
    printf("\n");
}


int max(int a, int b){
    if (a > b) return a;
    else return b;
}


int main(void){
    int n, *a = input(&n);
    int m, *b = input(&m);
    int ans[n + m], l = 0;
    
    for (int i = 0; i < max(n, m); i++){
        if (i < n) {
            ans[l] = a[i];
            l++;
        }
        if (i < m) {
            ans[l] = b[i];
            l++;
        }
    }
    
    output(ans);
    my_free(a);
    my_free(b);
    
    return 0;
}
