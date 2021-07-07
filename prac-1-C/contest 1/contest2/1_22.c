//
//  main.c
//  contest 1 1.14
//
//  Created by  Gleb on 26/09/2019.
//  Copyright © 2019  Gleb. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>


void swap(int *a, int *b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}


int main(void) {
    int X, Y, L, l1, l2, N, n1, n2;
    scanf("%d%d", &X, &Y);
    X = abs(X);
    Y = abs(Y);
    if (Y > X) swap(&X, &Y);
    L = X;
    l1 = Y;
    n1 = 2 * l1;
    l2 = L - l1;
    if (l2 % 2 == 0) n2 = 2 * l2;
    else n2 = (l2 + 1) / 2 + (l2 - 1) * 3 / 2;
    N = n1 + n2;
    printf("%d", N);
    return 0;
}


