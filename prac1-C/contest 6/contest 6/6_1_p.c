//
//  main.c
//  contest 6
//
//  Created by  Gleb on 22.12.2019.
//  Copyright © 2019  Gleb. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>


int main(void) {
    FILE *fin  = fopen("input.txt",  "r");
    FILE *fout = fopen("output.txt", "w");
                
    char c;
    int W = 0, S = 0, P = 0;
    
    int in_paragraph = 0;
    int in_sentense = 0;
    int in_word = 0;
    
    int may_be_paragraph = 0;
    int may_be_wrap = 0;
    
    while((c = getc(fin)) != EOF){
        if (c != ' ' && c != '\n' && c != '.' && c != '-'){
            if (in_word == 0){
                in_word = 1;
                ++W;
            }
            if (in_sentense == 0){
                in_sentense = 1;
                ++S;
            }
            if (in_paragraph == 0){
                in_paragraph = 1;
                ++P;
            }
            may_be_paragraph = 0;
            may_be_wrap = 0;
        } else if (c == ' ') {
            in_word = 0;
        } else if (c == '.') {
            in_word = 0;
            in_sentense = 0;
        } else if (c == '-') {
            may_be_wrap = 1;
        } else if (c == '\n') {
            if (may_be_wrap == 0) {
                in_word = 0;
            };
            if (may_be_paragraph == 0) {
                may_be_paragraph = 1;
            } else if (may_be_paragraph == 1) {
                in_paragraph = 0;
            }
        }
    }
    
    fprintf(fout, "%d %d %d", W, S, P);
    fclose(fin);
    fclose(fout);
    return 0;
}

