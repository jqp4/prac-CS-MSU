//
//  main.c
//  contest 5
//
//  Created by  Gleb on 20.12.2019.
//  Copyright © 2019  Gleb. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>


long long gettrack(long **m, int n){
    long long track = 0;
    for (int i = 0; i < n; i++){
        track += m[i][i];
    }
    return track;
}


long **getmatrix(int n){
    long **A = (long **)malloc(n * sizeof(long *));
    for (int i = 0; i < n; i++){
        A[i] = (long *)malloc(n * sizeof(long));
        for (int j = 0; j < n; j++){
            scanf("%ld", &A[i][j]);
        }
    }
    return A;
}


void printmatrix(long **m, int n){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            printf("%ld ", m[i][j]);
        }
        printf("\n");
    }
}


void free_maxtrix_array(long ***M, int *N, int n){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < N[i]; j++){
            free(M[i][j]);
        }
    }
}



int main(void) {
    int n;
    scanf("%d", &n);
    long **M[n];
    int N[n];
    for (int i = 0; i < n; i++){
        scanf("%d", &N[i]);
        M[i] = getmatrix(N[i]);
    }
    
    int i_max = 0;
    long long max_track = gettrack(M[i_max], N[i_max]);
    for (int i = 1; i < n; i++){
        long long i_track = gettrack(M[i], N[i]);
        if (i_track > max_track){
            max_track = i_track;
            i_max = i;
        }
    }
    
    printmatrix(M[i_max], N[i_max]);
    free_maxtrix_array(M, N, n);
    return 0;
}
