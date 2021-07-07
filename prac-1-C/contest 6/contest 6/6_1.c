//
//  main.c
//  contest 6
//
//  Created by  Gleb on 22.12.2019.
//  Copyright © 2019  Gleb. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool int
#define False 0
#define True 1
#define not !

#define STR_LEN 20001


int str_len(char *s){
    int len = 0;
    while (s[len] != '\0'){
        len++;
    }
    return len;
}



bool is_empty(char *s, int l){
    for (int i = 0; i < l; i++){
        if (s[i] != ' ' && s[i] != '\n'){
            return False;
        }
    }
    return True;
}


int get_word_count(char *s, int l){
    int wc = 0;
    bool word_flag = False;
    
    for (int i = 0; i < l; i++){
        if ((s[i] == ' ') || (s[i] == '.')){
            if (word_flag){
                wc++;
            }
            word_flag = False;
        } else if (s[i] == '-'){
            //wc--;              <-------
            //word_flag = False; <----------------- HERE
            //printf("w--\n");   <-------
        } else {
            word_flag = True;
            //printf("<%c>", s[i]);
        }
    }
    
    if (word_flag){
        //wc++;
    }
    return wc;
}


int get_sentence_count(char *s, int l){
    int sc = 0;
    for (int i = 0; i < l; i++){
        if (s[i] == '.'){
            sc++;
        }
    }
    return sc;
}


int main(void) {
    FILE *fin  = fopen("input.txt",  "r");
    FILE *fout = fopen("output.txt", "w");
    
    int W = 0, S = 0, P = 0;
    char *s = (char *)malloc(STR_LEN * sizeof(char));
    bool empty_flag = True;
    
    while(fgets(s, STR_LEN, fin) != NULL){
        int len = (int)strlen(s);
        //int len = str_len(s);
        //printf("%d\n", len);
        if (is_empty(s, len)){
            if (not empty_flag){ P++; }
            empty_flag = True;
        } else {
            empty_flag = False;
            W += get_word_count(s, len);
            S += get_sentence_count(s, len);
        }
        
        
        //printf("<%d>", get_word_count(s, len));
    }
    
    if (not empty_flag){ P++; }
    fprintf(fout, "%d %d %d", W, S, P);
    fclose(fin);
    fclose(fout);
    free(s);
    return 0;
}
