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


#define max_size 64
#define bool int
#define False 0
#define True 1



bool is_simple(int x){
    int n = (int)sqrt(x);
    for (int i = 2; i <= n; i++){
        if (x % i == 0){
            return False;
        }
    }
    return True;
}



int main(void){
    int n;
    scanf("%d", &n);
    int last[4] = {1, 3, 7, 9};
    int f[n][max_size], ns[n];
    ns[0] = 4;
    f[0][0] = 2;
    f[0][1] = 3;
    f[0][2] = 5;
    f[0][3] = 7;
    
    for (int i = 1; i < n; i++){
        for (int j = 0; j <= ns[i - 1]; j++){
            for (int g = 0; g < 4; g++){
                f[i][j * 4 + g] = f[i - 1][j] * 10 + last[g];
            }
        }
        int m = ns[i - 1] * 4;
        ns[i] = 0;
        for (int j = 0; j < m; j++){
            if (is_simple(f[i][j])){
                f[i][ns[i]] = f[i][j];
                ns[i]++;
            }
        }
    }
    
    for (int i = 0; i < ns[n - 1]; i++){
        printf("%d ", f[n - 1][i]);
    }
    return 0;
}
