//
//  main.c
//  task2_1
//
//  Created by  Gleb on 07.02.2020.
//  Copyright © 2020  Gleb. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "generator.h"
//#include "someFunctions.h"
#include "sortings.h"



void show_sort_info(void (**generators) (lli *a, int n), char sort_type, int n){
    lli *a = (lli *)malloc(n * sizeof(lli));
    int swaps_count, comparisons_count;
    void (*get_sort_info) (lli *a, int n, int *sc, int *cc);
    if (sort_type == 'b'){ get_sort_info = get_bubble_sort_info;}
    else { get_sort_info = get_shell_sort_info; }
    printf("info for %s sort, n = %d:\n", sort_type == 'b' ? "Bubble" : "Shell", n);
    for (int i = 0; i < 4; i++){
        generators[i](a, n);
        get_sort_info(a, n, &swaps_count, &comparisons_count);
        printf("gen %d: %9d  comparisons, %9d  swaps\n", i + 1, comparisons_count, swaps_count);
    }
    printf("\n");
    free(a);
}


int main(void){
    void (**generators) (lli *a, int n) = (void *)malloc(4 * sizeof(void));
    generators[0] = generator_type_1;
    generators[1] = generator_type_2;
    generators[2] = generator_type_3;
    generators[3] = generator_type_4;
    char sort_types[] = {'b', 's'};
    for (int srt = 0; srt < 2; srt++){
        for (int i = 1; i <= 4; i++){ show_sort_info(generators, sort_types[srt], power(10, i)); }
        print_line();
    }
    free(generators);
    return 0;
}

/*
вариант : {2, 2, 1, 3}
I - 64-разрядные целые числа (long long int);
II - Числа упорядочиваются по невозрастанию;
III - 1) Метод «пузырька» (см. [3] 130-132; [4] 27-28; [5] 101-102);
      2) Метод Шелла (см. [3] 105-107; [4] 37-40; [5] 105-107).
*/
