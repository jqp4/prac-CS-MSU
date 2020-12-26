//
//  main.c
//  contest 5
//
//  Created by  Gleb on 20.12.2019.
//  Copyright © 2019  Gleb. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#define bool int
#define True 1
#define False 0
#define not !


struct matrix{
    int n, m;
    int **a;
};


struct matrix matrix__init__(int n, int m){
    struct matrix x;
    x.n = n;
    x.m = m;
    x.a = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++){
        x.a[i] = (int *)malloc(m * sizeof(int));
    }
    return x;
}


struct matrix matrix__deepcopy__(struct matrix x0){
    struct matrix x;
    x.n = x0.n;
    x.m = x0.m;
    x.a = (int **)malloc(x.n * sizeof(int *));
    for (int i = 0; i < x.n; i++){
        x.a[i] = (int *)malloc(x.m * sizeof(int));
        for (int i = 0; i < x.n; i++){
            
        }
    }
    return x;
}


void matrix__free__(struct matrix x){
    for (int i = 0; i < x.n; i++){
        free(x.a[i]);
    }
    free(x.a);
}


void get_colony(struct matrix *c, struct matrix v){
    for (int i = 0; i < c->n; i++){
        for (int j = 0; j < c->m; j++){
            c->a[i][j] = 0;
        }
    }
    for (int i = 0; i < v.n; i++){
        c->a[v.a[i][0]][v.a[i][1]] = 2;
    }
}


void input_viruses(struct matrix *v){
    for (int i = 0; i < v->n; i++){
        scanf("%d %d", &v->a[i][0], &v->a[i][1]);
        v->a[i][0]--;
        v->a[i][1]--;
    }
}


void show_colony(struct matrix c, int iter){
    printf("\nit %d :\n", iter);
    for (int i = 0; i < c.n; i++) {
        for (int j = 0; j < c.m; j++) {
            printf("%d ", c.a[i][j]);
        }
        printf("\n");
    }
}


bool is_near_virus(struct matrix c, int i, int j){
    //if (c.a[i][j] != 0) { return False; }
    if ((i - 1 >=  0) && (c.a[i - 1][j] == 2)){ return True; }
    if ((i + 1 < c.n) && (c.a[i + 1][j] == 2)){ return True; }
    if ((j - 1 >=  0) && (c.a[i][j - 1] == 2)){ return True; }
    if ((j + 1 < c.m) && (c.a[i][j + 1] == 2)){ return True; }
    return False;
}


void spread_viruses(struct matrix *c, int *noh){
    for (int i = 0; i < c->n; i++){
        for (int j = 0; j < c->m; j++){
            if (c->a[i][j] == 1){
                c->a[i][j] = 2;
            }
        }
    }
    for (int i = 0; i < c->n; i++){
        for (int j = 0; j < c->m; j++){
            if (c->a[i][j] == 0){
                if (is_near_virus(*c, i, j)){
                    c->a[i][j] = 1;
                    (*noh)--;
                }
            }
        }
    }
}


bool is_completely_infected(struct matrix c){
    for (int i = 0; i < c.n; i++){
        for (int j = 0; j < c.m; j++){
            if (c.a[i][j] == 0){
                return False;
            }
        }
    }
    return True;
}



int main(void) {
    int n, m, k;
    scanf("%d %d %d", &n, &m, &k);
    struct matrix colony = matrix__init__(n, m);
    struct matrix viruses = matrix__init__(k, 2);
    input_viruses(&viruses);
    get_colony(&colony, viruses);
    
    int it = 0, number_of_healthy = n * m - k;
    //while (not is_completely_infected(colony)){
    while (number_of_healthy > 0){
        it++;
        spread_viruses(&colony, &number_of_healthy);
        //show_colony(colony, it);
    }
    
    printf("%d", it);
    matrix__free__(colony);
    matrix__free__(viruses);
    return 0;
}
