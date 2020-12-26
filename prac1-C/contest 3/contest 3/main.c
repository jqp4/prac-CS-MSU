//
//  main.c
//  contest 3
//
//  Created by  Gleb on 25.10.2019.
//  Copyright © 2019  Gleb. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



int main_(void){
    int a, b, c, m;
    /*scanf("%d", &a);
    scanf("%d", &b);
    scanf("%d", &c);
    scanf("%d", &m);
    printf("%d");*/
    a = 2;
    m = 7;
    b = 1;
    int n = 30;
    for (int i = 1; i <= n; i++){
        b = (b * a)%m;
        //printf("%3d : %d, %ld\n", i, b, (long int)pow(a, (i % m) + (i / m))%m);
        printf("%2d : %d\n", i, b);
    }
    printf("  b1 = %d\n", b);
    int delta = (n % m) + (int)(n / m);
    printf("  delta = %d\n", delta);
    b = 1;
    for (int i = 1; i <= n%delta; i++){
        b = (b * a)%m;
        //printf("%3d : %d, %ld\n", i, b, (long int)pow(a, (i % m) + (i / m))%m);
    }
    printf("  b2 = %d\n", b);
    
    return 0;
}


int main(void){
    int a, m, b;
    m = 7;
    
    printf("%d\n", 2 % m);
    printf("%d\n", 12 % m);
    printf("%d\n", 102 % m);
    printf("%d\n", 112 % m);
    printf("%d\n", 1112 % m);
    
    
    return 0;
}
