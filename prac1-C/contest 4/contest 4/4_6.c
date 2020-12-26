//
//  main.c
//  contest 4
//
//  Created by  Gleb on 07.12.2019.
//  Copyright © 2019  Gleb. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#define STR_SIZE 1000001
#define bool int
#define True 1
#define False 0



int lenstr(char *s){
    int n = 0;
    while (s[n] != '\0'){
        n++;
    }
    return n;
}


void showstr(char *s, int n){
    for (int i = 0; i < n; i++){
        printf("%c", s[i]);
    }
}


void skip_char(int n){
    char skip_chr = '\0';
    for(int i = 0; i < n; i++){
        skip_chr = getchar();
    }
    if(skip_chr){
        skip_chr = '\0';
    }
}


int input_int(void){
    int n;
    scanf("%d", &n);
    return n;
}


int get_space_amount(char *s){
    int space_count = 0;
    int len = lenstr(s);
    for (int i = 0; i < len; i++){
        if (s[i] == ' '){
            space_count++;
        }
    }
    return space_count;
}


char *input_str(int SIZE, int *len){
    char *s;
    s = malloc((SIZE + 1) * sizeof(char));
    *len = -1;
    do {
        *len += 1;
        s[*len] = getchar();
    } while (s[*len] != EOF && s[*len] != '\n');
    s[*len] = '\0';
    return s;
}


int *get_new_spaces(int delta_K, int space_amount){
    int aas = (int)(delta_K / space_amount); // amount additional spaces (div)
    int aasff = delta_K % space_amount; // amount additional spaces for firsts (mod)
    int *new_spaces = (int *)malloc(space_amount * sizeof(int));
    for (int i = 0; i < space_amount; i++){
        new_spaces[i] = 1 + aas + (i < aasff ? 1 : 0);
    }
    return new_spaces;
}


char *get_word(char *s, int i){
    int j = i;
    while (s[j] != ' ' && s[j] != '\0'){
        j++;
    }
    int len = j - i;
    char *word = (char *)malloc((len + 1) * sizeof(char));
    for (int g = 0; g < len; g++){
        word[g] = s[i + g];
    }
    word[len] = '\0';
    return word;
}



char **get_words(char *s, int word_amount){
    char **words = (char **)malloc(word_amount * sizeof(char *));
    
    for (int i = 0; i < word_amount; i++){
        words[i] = (char *)malloc(5 * sizeof(char));
        words[i][0] = 's';
        words[i][1] = '\0';
        //words[i] = get_word();
    }
    
    return words;
}


void add_spaces(char *s, int i, int n){
    int i_end = i + n;
    for (int j = i; j < i_end; j++){
        s[j] = ' ';
    }
}


int main(void){
    int len, K = input_int();
    skip_char(1);
    char *inps = input_str(K, &len);
    int space_amount = get_space_amount(inps);
    //char **words = get_words(inps, space_amount + 1);
    int *ns = get_new_spaces(K - len, space_amount);
    
    char *s = malloc((K + 1) * sizeof(char));
    
    int delta = 0, j = 0;
    for(int i = 0; i < len; i++){
        if (inps[i] == ' '){
            add_spaces(s, i + delta, ns[j]);
            delta += ns[j] - 1;
            j++;
        } else {
            s[i + delta] = inps[i];
        }
    }
    
    showstr(s, K);
    free(inps);
    free(ns);
    return 0;
}
