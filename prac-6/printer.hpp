#pragma once

#include <iomanip>
#include <iostream>

/// вывод треугольной матрицы
void output(double** a, double* b, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      std::cout << "a[" << i << ", " << j << "] = " << std::setw(8) << a[i][j]
                << "   ";
    }
    std::cout << "| b[" << i << "] = " << std::setw(8) << b[i] << std::endl;
  }
  std::cout << std::endl;
}

void outputAnswer(double* x, int n) {
  std::cout << "Korni sistemy: " << std::endl;
  for (int i = 0; i < n; i++) {
    std::cout << "x[" << i << "] = " << x[i] << " " << std::endl;
  }
}