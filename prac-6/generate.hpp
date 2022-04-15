#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double getDoubleRand(double fMin, double fMax) {
  double f = (double)rand() / RAND_MAX;
  return fMin + f * (fMax - fMin);
}

void generate(double** a, double* b, int n) {
  srand((unsigned)time(NULL));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      a[i][j] = getDoubleRand(0, 10);
    }
    b[i] = getDoubleRand(0, 10);
  }
}

#define lli long long int

lli rand64int(void) {
  lli a = 1;
  a *= rand();
  a *= rand();
  return a;
}

void checking_rand_function(void) {
  srand((unsigned)time(NULL));
  lli a = rand64int(), max = a, min = a;
  for (int i = 0; i < 30; i++) {
    a = rand64int();
    min = a < min ? a : min;
    max = a > max ? a : max;
    printf("%lld\n", a);
  }
  printf("< %lld >\n< %lld >\n", min, max);
}

void generate_non_increasing_array(lli* arr, int n) {
  //в таком методе повышается вероятность получить именно НЕ возрастающий массив
  srand((unsigned)time(NULL));
  arr[0] = rand64int();
  for (int i = 1; i < n; i++) {
    arr[i] = arr[i - 1] -
             (lli)rand() * (rand() % (1 << 16)) * (rand() % 4 == 0 ? 0 : 1);
  }
}

void generator_type_1(lli* arr, int n) {
  //если элементы массива упорядочены по возрастанию и массив уже упорядочен,
  //то, очевидно, все эллементы массива равны
  srand((unsigned)time(NULL));
  arr[0] = rand64int() * (rand() % 2 == 0 ? 0 : 1);
  for (int i = 1; i < n; i++) {
    arr[i] = arr[0];
  }
}

void generator_type_2(lli* arr, int n) {
  //упорядоченность по не возрастнию и есть упорядоченность в обратном порядке
  generate_non_increasing_array(arr, n);
}
