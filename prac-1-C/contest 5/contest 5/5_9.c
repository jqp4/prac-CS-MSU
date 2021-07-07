//
//  main.c
//  contest 5
//
//  Created by  Gleb on 20.12.2019.
//  Copyright © 2019  Gleb. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>


int *get_viruses(int size, int k, int m){
    int *v = (int *)malloc(size * sizeof(int));
    int x, y;
    for (int i = 0; i < k; i++){
        scanf("%d %d", &x, &y);
        v[i] = (y - 1) * m + (x - 1);
    }
    return v;
}


int *get_colony(int size, int *v, int k){
    int *c = (int *)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++){
        c[i] = 0;
    }
    for (int i = 0; i < k; i++){
        c[v[i]] = 1;
    }
    return c;
}


void show_colony(int *c, int n, int m, int it){
    if (it >= 0){
        printf("\niter %d :\n", it);
    } else {
        printf("\n");
    }
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            printf("%d ", c[i * m + j]);
        }
        printf("\n");
    }
}


void set_new_viruses(int size, int *v[size], int *vc, int i0, int *c[size], int m){
    int v0 = (*v)[i0], nv;
    int cv0 = (*c)[v0];
    int new_vs[4];
    
    new_vs[0] = ((v0 + 1) % m == 0) ? -1 : v0 + 1;
    new_vs[1] = ((  v0  ) % m == 0) ? -1 : v0 - 1;
    new_vs[2] = ((v0 + m) >= size ) ? -1 : v0 + m;
    new_vs[3] = ((v0 - m) <   0   ) ? -1 : v0 - m;
    
    for (int i = 0; i < 4; i++){
        nv = new_vs[i];
        if (nv != -1){
            if ((*c)[nv] == 0){
                (*c)[nv] = cv0 + 1;
                (*v)[*vc] = nv;
                (*vc)++;
            }
        }
    }
}


int get_array_max(int *a, int n){
    int max = a[0];
    for (int i = 1; i < n; i++){
        if (a[i] > max){
            max = a[i];
        }
    }
    return max;
}



int main(void){
    int n, m, k;
    scanf("%d %d %d", &n, &m, &k);
    int size = n * m, v_count = k;
    int *v = get_viruses(size, k, m);
    int *c = get_colony(size, v, k);
    
    for (int i = 0; v_count < size; i++){
        set_new_viruses(size, &v, &v_count, i, &c, m);
    }
    
    printf("%d", get_array_max(c, size) - 1);
    free(c);
    free(v);
    return 0;
}
