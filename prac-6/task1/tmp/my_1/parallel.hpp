#pragma once

#include "printer.hpp"

void ggg(double** a, double* x, int n, int iter) {
  // #pragma omp parallel for
  for (int i = iter; i < n; i++) {
    double k = 0;
    for (int j = iter; j < n; j++) {
      k += x[j - iter] * a[j][i];
    }
    k *= 2;
    for (int j = iter; j < n; j++) {
      a[j][i] -= k * x[j - iter];
    }
  }
}

void gause(double** a, double* b, double* x, int n) {
  // прямой ход
  for (int k = 0; k < n; k++) {
    for (int j = k + 1; j < n; j++) {
      double d = a[j][k] / a[k][k];
      for (int i = k; i <= n; i++) {
        a[j][i] = a[j][i] - d * a[k][i];
      }
      b[j] = b[j] - d * b[k];
    }
  }

  output(a, b, n);

  // обратный ход
  for (int k = n - 1; k >= 0; k--) {
    double d = 0;
    for (int j = k + 1; j < n; j++) {
      double s = a[k][j] * x[j];
      d = d + s;
    }
    x[k] = (b[k] - d) / a[k][k];
  }
}