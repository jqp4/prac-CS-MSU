#include <iostream>
#include "generate.hpp"
#include "parallel.hpp"
#include "printer.hpp"

int main() {
  int n = 5;  // cin >> n;
  double* x = new double[n];
  double* b = new double[n];
  double** a = new double*[n];
  for (int i = 0; i <= n; i++) {
    a[i] = new double[n];
  }

  generate(a, b, n);
  output(a, b, n);

  ggg(a, b, n, 2);
  output(a, b, n);

  outputAnswer(x, n);
  return 0;
}