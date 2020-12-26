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







int main(void) {
    FILE *fin  = fopen("input.txt",  "r");
    FILE *fout = fopen("output.txt", "w");
    //FILE *fin  = fopen("/Users/gleb/Projects/C/contest 6/contest 6/input.txt",  "r");
    //FILE *fout = fopen("/Users/gleb/Projects/C/contest 6/contest 6/output.txt", "w");
    
    int n, m, k = 1;
    fscanf(fin, "%d %d", &n, &m);
    for (int i = 0; i < m; i++){
        fscanf(fin, "%d", &k);
    }
    for (int i = k; i <= n; i++){
        fprintf(fout, "%d ", i);
    }
    for (int i = 1; i < k; i++){
        fprintf(fout, "%d ", i);
    }
    
    
    
    
    
    
    fclose(fin);
    fclose(fout);
    return 0;
}
