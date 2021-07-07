//
//  main.c
//  contest 5
//
//  Created by  Gleb on 20.12.2019.
//  Copyright © 2019  Gleb. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define bool int
#define False 0
#define True 1


int getfn(int n, int k, int f[n], int a[k]){
    int fn = 0;
    
    
    return fn;
}


int main(void){
    int k, n, p;
    scanf("%d %d %d", &k, &n, &p);
    int f[n], a[k];
    for (int i = 0; i < k; i++){ scanf("%d", &f[i]); }
    for (int i = 0; i < k; i++){ scanf("%d", &a[i]); }
    
    for (int i = k; i < n; i++){
        f[i] = getfn(n, k, f, a);
    }
    
    
    for (int i = 0; i < n; i++){
        printf("%d ", f[i]);
    }
    

    return 0;
}


/*
 3
 5
 100
 1 2 3
 4 5 6
 */
