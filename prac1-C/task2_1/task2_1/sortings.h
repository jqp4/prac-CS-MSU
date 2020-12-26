//
//  sortings.h
//  task2_1
//
//  Created by  Gleb on 15.02.2020.
//  Copyright © 2020  Gleb. All rights reserved.
//

#ifndef sortings_h
#define sortings_h


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define lli long long int
#define bool int
#define False 0
#define True 1


void swap(lli *a, lli *b){
    lli tmp = *a;
    *a = *b;
    *b = tmp;
}


double bubble_sort_time(lli *arr, int n){
    clock_t time;
    time = clock();
    for (int i = 0; i < n - 1; i++){
        for (int j = 0; j < n - i - 1; j++){
            if (arr[j] > arr[j + 1]){
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
    time = clock() - time;
    return (double)time/CLOCKS_PER_SEC;
}


void get_bubble_sort_info(lli *arr, int n, int *swaps_count, int *comparisons_count){
    *swaps_count = 0;
    *comparisons_count = 0;
    for (int i = 0; i < n - 1; i++){
        for (int j = 0; j < n - i - 1; j++){
            (*comparisons_count)++;
            if (arr[j] > arr[j + 1]){
                swap(&arr[j], &arr[j + 1]);
                (*swaps_count)++;
            }
        }
    }
}


double shell_sort_time(lli *arr, int n){
    clock_t time;
    time = clock();
    int step, i, j;
    lli tmp;
    for (step = n / 2; step > 0; step /= 2){
        for (i = step; i < n; i++){
            for (j = i - step; j >= 0 && arr[j] > arr[j + step]; j -= step){
                tmp = arr[j];
                arr[j] = arr[j + step];
                arr[j + step] = tmp;
            }
        }
    }
    time = clock() - time;
    return (double)time/CLOCKS_PER_SEC;
}


void get_shell_sort_info(lli *arr, int n, int *swaps_count, int *comparisons_count){
    *swaps_count = 0;
    *comparisons_count = 0;
    int step, i, j;
    lli tmp;
    for (step = n / 2; step > 0; step /= 2){
        for (i = step; i < n; i++){
            for (j = i - step; j >= 0; j -= step){
                (*comparisons_count)++;
                if (arr[j] > arr[j + step]){
                    tmp = arr[j];
                    arr[j] = arr[j + step];
                    arr[j + step] = tmp;
                    (*swaps_count)++;
                }
            }
        }
    }
}




#endif /* sortings_h */
