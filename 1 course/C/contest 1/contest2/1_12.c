//
//  main.c
//  contest2
//
//  Created by  Gleb on 26/09/2019.
//  Copyright © 2019  Gleb. All rights reserved.
//

#include <stdio.h>


void ouput_maxs(int max[3]){
    for (int i = 0; i < 3; i++){
        printf("%d ", max[i]);
    }
}


void swap(int *a, int *b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}


void start_normalization(int *max){
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 2 - i; j++){
            if (max[j] < max[j + 1]){
                swap(&max[j], &max[j + 1]);
            }
        }
    }
}


void input_maxs(int *max){
    for (int i = 0; i < 3; i++){
        scanf("%d", &max[i]);
    }
    start_normalization(max);
}



void normalization(int *max, int x){
    if (x >= max[0]){
        max[2] = max[1];
        max[1] = max[0];
        max[0] = x;
    } else if (x >= max[1]){
        max[2] = max[1];
        max[1] = x;
    } else if (x >= max[2]){
        max[2] = x;
    }
}

int main(void) {
    int x, max[3], n;
    scanf("%d", &n);
    input_maxs(max);
    for (int i = 0; i < n - 3; i++){
        scanf("%d", &x);
        normalization(max, x);
    }
    ouput_maxs(max);
    return 0;
}
