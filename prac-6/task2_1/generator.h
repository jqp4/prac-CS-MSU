//
//  generator.h
//  task2_1
//
//  Created by  Gleb on 07.02.2020.
//  Copyright © 2020  Gleb. All rights reserved.
//

#ifndef generator_h
#define generator_h


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define lli long long int



lli rand64int(void){
    lli a = 1;
    a *= rand();
    a *= rand();
    return a;
}


void checking_rand_function(void){
    srand((unsigned)time(NULL));
    lli a = rand64int(), max = a, min = a;
    for (int i = 0; i < 30; i++){
        a = rand64int();
        min = a < min ? a : min;
        max = a > max ? a : max;
        printf("%lld\n", a);
    }
    printf("< %lld >\n< %lld >\n", min, max);
}


void generate_non_increasing_array(lli *arr, int n){
    //в таком методе повышается вероятность получить именно НЕ возрастающий массив
    srand((unsigned)time(NULL));
    arr[0] = rand64int();
    for (int i = 1; i < n; i++){
        arr[i] = arr[i - 1] - (lli)rand() * (rand() % (1 << 16)) * (rand() % 4 == 0 ? 0 : 1);
    }
}


void generator_type_1(lli *arr, int n){
    //если элементы массива упорядочены по возрастанию и массив уже упорядочен, то, очевидно, все эллементы массива равны
    srand((unsigned)time(NULL));
    arr[0] = rand64int() * (rand() % 2 == 0 ? 0 : 1);
    for (int i = 1; i < n; i++){
        arr[i] = arr[0];
    }
}


void generator_type_2(lli *arr, int n){
    //упорядоченность по не возрастнию и есть упорядоченность в обратном порядке
    generate_non_increasing_array(arr, n);
}


void generator_type_3(lli *arr, int n){
    generate_non_increasing_array(arr, n);
}


void generator_type_4(lli *arr, int n){
    generate_non_increasing_array(arr, n);
}






#endif /* generator_h */
