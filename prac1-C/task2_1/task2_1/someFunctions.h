//
//  someFunctions.h
//  task2_1
//
//  Created by  Gleb on 15.02.2020.
//  Copyright © 2020  Gleb. All rights reserved.
//

#ifndef someFunctions_h
#define someFunctions_h


#include <stdio.h>
#include <stdlib.h>

#define lli long long int
#define bool int
#define False 0
#define True 1



void show_array(lli *arr, int n, int d, bool line_break){
    //показывает элементы массива 'a' с переодичностью 'd'
    char c = line_break ? '\n' : ' ';
    for (int i = 0; i < n; i += d){
        if (line_break){ printf("# %4d : ", i); }
        printf("%lld%c", arr[i], c);
    }
    printf("\n");
}


void show_fla(lli *arr, int n){
    //показывает первой и последний элементы массива
    printf("%lld\n%lld\n", arr[0], arr[n - 1]);
}


void deepcopy(lli *copyarr, lli *arr, int n){
    //полноcтью копирует массив 'arr' в массив 'copyarr'
    for (int i = 0; i < n; i++){
        copyarr[i] = arr[i];
    }
}


void print_line(void){
    for (int i = 0; i < 47; i++){
        printf("-");
    }
    printf("\n");
}


int power(int a1, int n){
    int a = a1;
    for (int i = 1; i < n; i++){
        a *= a1;
    }
    return a;
}

#endif /* someFunctions_h */
