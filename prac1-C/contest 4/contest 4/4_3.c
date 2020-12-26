//
//  main.c
//  contest 4
//
//  Created by  Gleb on 07.12.2019.
//  Copyright © 2019  Gleb. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_SIZE 10001
#define bool int
#define True 1
#define False 0



int len_str(char *s){
    for(int n = 0; n < STR_SIZE; n++){
        if (s[n] == '\0'){
            return n;
        }
    }
    return -1;
}


int input_int(int *n){
    scanf("%d", n);
    return *n;
}


bool is_period(char *s, int t){
    int len = len_str(s);
    for (int i = t; i < len; i++){
        if (s[i] != s[i - t]){
            return False;
        }
    }
    return True;
}




int main(void) {
    int n = input_int(&n);
    char s[STR_SIZE];
    scanf("%s", s);
    int len = len_str(s);
    for (int t = 1; t <= len; t++){
        if (is_period(s, t)){
            printf("%d ", t);
        }
    }
    return 0;
}
