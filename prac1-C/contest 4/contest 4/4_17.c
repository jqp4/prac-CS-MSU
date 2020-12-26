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


#define bool int
#define True 1
#define False 0


const unsigned int N = 2000000;


void prints(char *a, int n){
    for (int i = 0; i < n; i++){
        printf("%c", a[i]);
    }
}


char *get_bifurcated_string(int *n){
    char *s;
    s = malloc(N * 2 * sizeof(char));
    *n = -1;
    do {
        *n += 1;
        s[*n] = getchar();
    } while (s[*n] != EOF && s[*n] != '\n');
    
    for (int i = 0; i < *n; i++){
        s[i + *n] = s[i];
    }
    return s;
}


bool string_comparison(char *s1, char *s2, int n){
    for (int i = 0; i < n; i++){
        if ((int)s1[i] < (int)s2[i]) {
            return True;
        }
        if ((int)s1[i] > (int)s2[i]) {
            return False;
        }
    }
    return False;
}



int main_old_1(void){
    int n;
    char *s = get_bifurcated_string(&n);
    char *min_s;
    min_s = &s[0];
    for (int i = 0; i < n; i++){
        static char *this_s;
        this_s = &s[i];
        if (string_comparison(this_s, min_s, n)){
            min_s = this_s;
        }
    }
    prints(min_s, n);
    free(s);
    return 0;
}


int main_old_2(void){
    int n;
    char *s = get_bifurcated_string(&n);
    
    unsigned int null_max_count = 0;
    int i = 0;
    while (s[i] == '0'){
        null_max_count++;
        i++;
    }
    if (i >= n){
        //printf("<str:[0]*n>\n");
        prints(s, n);
    } else {
        int delta_l = 0;
        i = n - 1;
        while (s[i] == '0'){
            null_max_count++;
            delta_l++;
            i--;
        }
        //printf("nmc = %d, dl = %d\n", null_max_count, delta_l);
        
        unsigned int null_count = 0;
        char *this_s;
        this_s = &s[0];
        char *min_s;
        min_s = &s[n - delta_l];
        
        for (int i = 0; i < n; i++){
            if ((int)s[i] == '0'){
                null_count++;
                if (null_count > n/2 + 1){
                    
                }
            } else {
                if (null_count == null_max_count){
                    if (string_comparison(this_s, min_s, n)){
                        min_s = this_s;
                    }
                } else {
                    if (null_count > null_max_count){
                        null_max_count = null_count;
                        min_s = this_s;
                    }
                }
                null_count = 0;
                this_s = &s[i + 1];
            }
        }
        prints(min_s, n);
    }
    free(s);
    return 0;
}


int main_old_3(void){
    int n;
    char *s = get_bifurcated_string(&n);
    unsigned int null_max_count = 0;
    int i = 0;
    while (s[i] == '0'){
        null_max_count++;
        i++;
    }
    if (i >= n){
        //printf("<str:[0]*n>\n");
        prints(s, n);
    } else {
        int delta_l = 0;
        i = n - 1;
        while (s[i] == '0'){
            null_max_count++;
            delta_l++;
            i--;
        }
        
        unsigned int null_count = 0;
        char *this_s;
        this_s = &s[0];
        char *min_s;
        min_s = &s[n - delta_l];
        
        for (int i = 0; i < n; i++){
            if ((int)s[i] == '0'){
                null_count++;
                if (null_count > n/2 + 1){
                    
                }
            } else {
                if (null_count == null_max_count){
                    if (string_comparison(this_s, min_s, n)){
                        min_s = this_s;
                    }
                } else {
                    if (null_count > null_max_count){
                        null_max_count = null_count;
                        min_s = this_s;
                    }
                }
                null_count = 0;
                this_s = &s[i + 1];
            }
        }
        prints(min_s, n);
    }
    free(s);
    return 0;
}


int main_old_4(void){
    int n;
    char *s = get_bifurcated_string(&n);
    unsigned int null_max_count = 0;
    int i = 0;
    while (s[i] == '0'){
        null_max_count++;
        i++;
    }
    if (i >= n){
        //printf("<str:[0]*n>\n");
        prints(s, n);
    } else {
        int delta_l = 0;
        i = n - 1;
        while (s[i] == '0'){
            null_max_count++;
            delta_l++;
            i--;
        }
        char *kl[n];
        int kln = 0;
        
        unsigned int null_count = 0;
        char *this_s;
        this_s = &s[0];
        char *min_s;
        min_s = &s[n - delta_l];
        
        for (int i = 0; i < n; i++){
            if ((int)s[i] == '0'){
                null_count++;
                if (null_count > n/2 + 1){
                    
                }
            } else {
                if (null_count == null_max_count){
                    kl[kln] = this_s;
                    kln++;
                } else {
                    if (null_count > null_max_count){
                        null_max_count = null_count;
                        min_s = this_s;
                        kln = 0;
                    }
                }
                null_count = 0;
                this_s = &s[i + 1];
            }
        }
        
        for (int j = 0; j < kln; j++){
            if (strcmp(kl[j], min_s) < 0){
                min_s = kl[j];
            }
        }
        
        prints(min_s, n);
    }
    free(s);
    return 0;
}



char *get_start_min_str(char *s, int n, unsigned int *null_max_count, bool *valid_code){
    *null_max_count = 0;
    char *min_s;
    min_s = &s[0];
    int i = 0;
    while (s[i] == '0'){
        *null_max_count += 1;
        i++;
    }
    if (i >= n){
        *valid_code = False;
    } else {
        *valid_code = True;
        int delta_l = 0;
        i = n - 1;
        while (s[i] == '0'){
            *null_max_count += 1;
            delta_l++;
            i--;
        }
        min_s = &s[n - delta_l];
    }
    return min_s;
}


int main(void){
    int n;
    char *s = get_bifurcated_string(&n);
    unsigned int null_max_count;
    bool valid_code;
    char *min_s = get_start_min_str(s, n, &null_max_count, &valid_code);
    
    if (valid_code){
        unsigned int null_count = 0;
        char *this_s;
        this_s = &s[0];
        char *kl[n];
        int kln = 0;
        for (int i = 0; i < n; i++){
            if ((int)s[i] == '0'){
                null_count++;
                if (null_count > n/2 + 1){
                    
                }
            } else {
                if (null_count == null_max_count){
                    kl[kln] = this_s;
                    kln++;
                } else {
                    if (null_count > null_max_count){
                        null_max_count = null_count;
                        min_s = this_s;
                        kl[0] = min_s;
                        kln = 1;
                    }
                }
                null_count = 0;
                this_s = &s[i + 1];
            }
        }
        
        for (int j = 0; j < kln; j++){
            if (strcmp(kl[j], min_s) < 0){
                min_s = kl[j];
            }
        }
    }
    
    prints(min_s, n);
    free(s);
    return 0;
}
