//
//  main.c
//  contest 5
//
//  Created by  Gleb on 20.12.2019.
//  Copyright © 2019  Gleb. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>



struct complex{
    long int a;
    long int b;
};


void getcomplex(struct complex *x){
    scanf("%ld %ld", &x->a, &x->b);
}
    

void printcomplex(struct complex x){
    printf("%ld %ld ", x.a, x.b);
}


struct complex **get_square_complex_matrix(int n){
    struct complex **A = (struct complex **)malloc(n * sizeof(struct complex *));
    for (int i = 0; i < n; i++){
        A[i] = (struct complex *)malloc(n * sizeof(struct complex));
        for (int j = 0; j < n; j++){
            getcomplex(&A[i][j]);
        }
    }
    return A;
}


struct complex get_conjugate(struct complex x){
    struct complex x_;
    x_.a = x.a;
    x_.b = -x.b;
    return x_;
}


void print_square_complex_matrix(struct complex **A, int n){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            printcomplex(A[i][j]);
        }
        printf("\n");
    }
}


struct complex **get_ermite_conjugate_matrix(struct complex **A, int n){
    struct complex **Ah = (struct complex **)malloc(n * sizeof(struct complex *));
    for (int i = 0; i < n; i++){
        Ah[i] = (struct complex *)malloc(n * sizeof(struct complex));
        for (int j = 0; j < n; j++){
            Ah[i][j] = get_conjugate(A[j][i]);
        }
    }
    return Ah;
}



int main(void) {
    int n;
    scanf("%d", &n);
    struct complex **A = get_square_complex_matrix(n);
    struct complex **Ah = get_ermite_conjugate_matrix(A, n);
    print_square_complex_matrix(Ah, n);
    return 0;
}


/*
 3
 1 2 3 4 5 6
 7 8 9 10 11 12
 13 14 15 16 17 18
 */
