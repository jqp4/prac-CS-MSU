//
//  main.c
//  contest 4
//
//  Created by  Gleb on 07.12.2019.
//  Copyright © 2019  Gleb. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#define STR_SIZE 256
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


bool balance_is_present(int *m, int n, int k){
    int s[2] = {0, 0};
    for (int i = 0; i < n - 1; i++){
        s[i % 2] += m[i >= k ? i + 1 : i];
    }
    return s[0] == s[1] ? True : False;
}



int main(void) {
    char s[STR_SIZE];
    scanf("%s", s);
    int len = len_str(s);
    int m[len];
    for (int i = 0; i < len; i++){
        m[i] = s[i] == 'A' ? 1 : 0;
    }
    for (int k = 0; k < len; k++){
        if (balance_is_present(m, len, k)){
            printf("%d ", k + 1);
        }
    }
    return 0;
}
