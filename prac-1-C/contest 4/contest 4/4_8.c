//
//  main.c
//  contest 4
//
//  Created by  Gleb on 07.12.2019.
//  Copyright © 2019  Gleb. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STR_SIZE 101
#define bool int
#define not !
#define True 1
#define False 0



int lenstr(char *s){
    int n = 0;
    while (s[n] != '\0'){
        n++;
    }
    return n;
}


short int ctoi(char a){
    return (short int)a - 48;
}


bool contains(short int *m, int n, int x){
    for (int i = 0; i < n; i++){
        if (m[i] == x){
            return True;
        }
    }
    return False;
}



int main(void){
    char *s = (char *)malloc(STR_SIZE * sizeof(char));
    scanf("%s", s);
    int n = lenstr(s), k = 0;
    short int m[n], numbers[(int)pow(n - 2, 3) - n];
    for (int i = 0; i < n; i++){
        m[i] = ctoi(s[i]);
    }
    
    static short int x;
    for (int i = 0; i < n; i++){
        for (int j = i + 1; j < n; j++){
            for (int g = j + 1; g < n; g++){
                if (m[i] != 0 && i != j && j != g){
                    x = m[i] * 100 + m[j] * 10 + m[g];
                    if (not contains(numbers, k, x)){
                        numbers[k] = x;
                        k++;
                    }
                }
            }
        }
    }
    
    printf("%d", k);
    free(s);
    return 0;
}
