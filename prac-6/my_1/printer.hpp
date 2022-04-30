#pragma once

#include <iomanip>
#include <iostream>

/// вывод треугольной матрицы
void output(double** a, double* b, int n, bool showIter = false) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (showIter) {
        std::cout << "a[" << i << ", " << j << "] = ";
      }
      std::cout << std::setw(12) << a[i][j] << "   ";
    }
    std::cout << "| ";
    if (showIter) {
      std::cout << "b[" << i << "] = ";
    }
    std::cout << std::setw(12) << b[i] << std::endl;
  }
  std::cout << std::endl;
}

void outputAnswer(double* x, int n) {
  std::cout << "Korni sistemy: " << std::endl;
  for (int i = 0; i < n; i++) {
    std::cout << "x[" << i << "] = " << std::setw(12) << x[i] << " "
              << std::endl;
  }
  std::cout << std::endl;
}