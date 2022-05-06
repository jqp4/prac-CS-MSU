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
