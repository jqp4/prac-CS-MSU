#include <iostream>
#include "generate.hpp"
#include "printer.hpp"

int main() {
  int n = 2;  // cin >> n;
  double* x = new double[n];
  double* b = new double[n];
  double** a = new double*[n];
  for (int i = 0; i <= n; i++) {
    a[i] = new double[n];
  }

  generate(a, b, n);
  output(a, b, n);

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

  outputAnswer(x, n);
  return 0;
}