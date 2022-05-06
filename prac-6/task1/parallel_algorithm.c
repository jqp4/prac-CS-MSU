#include <assert.h>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void visualize_slae(int n,
                    double* matrix_a,
                    double* vector_x,
                    double* vector_b) {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      printf("%7.2lf * %5.2lf", matrix_a[i * n + j], vector_x[j]);
      if (j < n - 1) {
        printf(" + ");
      }
    }
    printf(" = %7.2lf\n", vector_b[i]);
  }
}

void visualize_vector(int n, double* vector) {
  for (int i = 0; i < n; ++i) {
    printf("%7.2lf", vector[i]);
  }
  printf("\n");
}

void read_matrix(FILE* output, int size, double* p_matrix) {
  int readed_object_count = 0;
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      double num;
      readed_object_count = fread(&num, sizeof(double), 1, output);
      assert(readed_object_count == 1 && "Error read element in file");
      p_matrix[i * size + j] = (long long)num;
    }
  }
}

void read_vector(FILE* output, int size, double* p_vector) {
  int readed_object_count;
  for (int i = 0; i < size; ++i) {
    double num;
    readed_object_count = fread(&num, sizeof(double), 1, output);
    assert(readed_object_count == 1 && "Error read element in file");
    p_vector[i] = (double)num;
  }
}

void get_slae(int n,
              char* filename,
              double* matrix_a,
              double* vector_x,
              double* vector_b) {
  FILE* output = fopen(filename, "rb");
  if (!output) {
    printf("Error opening file \"%s\"\n", filename);
    free(matrix_a);
    free(vector_x);
    free(vector_b);
    exit(1);
  }
  read_vector(output, n, vector_x);
  read_matrix(output, n, matrix_a);
  read_vector(output, n, vector_b);
}

void get_upper_triangular_form(int n,
                               int num_threads,
                               double* matrix_a,
                               double* vector_b) {
  double norm_vector_a = 0;
  double norm_vector_x = 0;
  double mult_x_b = 0;
  for (int k = 0; k < n - 1; ++k) {
    double* vector_x = (double*)malloc((n - k) * sizeof(double));
    assert(vector_x != NULL && "Malloc error");
    double* vector_x_a = (double*)malloc((n - k) * sizeof(double));
    assert(vector_x_a != NULL && "Malloc error");
    norm_vector_a = 0;
    norm_vector_x = 0;
    mult_x_b = 0;
    int is_zeros_vector = 1;
#pragma omp parallel num_threads(num_threads)
    {
#pragma omp for reduction(+ : norm_vector_a)
      for (int j = 0; j < n - k; ++j) {
        double a = matrix_a[(j + k) * n + k];
        vector_x[j] = a;
        if (a != 0.0) {
          is_zeros_vector = 0;
        }
        norm_vector_a += a * a;
      }

      if (!is_zeros_vector) {
#pragma omp single
        { vector_x[0] -= sqrt(norm_vector_a); }

#pragma omp for reduction(+ : norm_vector_x)
        for (int j = 0; j < n - k; ++j) {
          double x = vector_x[j];
          norm_vector_x += x * x;
        }

#pragma omp for
        for (int j = 0; j < n - k; ++j) {
          vector_x[j] /= sqrt(norm_vector_x);
        }
#pragma omp barrier
#pragma omp for
        for (int i = 0; i < n - k; ++i) {
          vector_x_a[i] = 0;
          for (int j = 0; j < n - k; ++j) {
            vector_x_a[i] += vector_x[j] * matrix_a[(i + k) + n * (j + k)];
          }
        }
#pragma omp barrier
#pragma omp for
        for (int i = 0; i < n - k; ++i) {
          for (int j = 0; j < n - k; ++j) {
            matrix_a[(i + k) * n + (j + k)] -= 2 * vector_x[i] * vector_x_a[j];
          }
        }
#pragma omp barrier
#pragma omp for reduction(+ : mult_x_b)
        for (int j = 0; j < n - k; ++j) {
          mult_x_b += vector_x[j] * vector_b[j + k];
        }

#pragma omp for
        for (int j = 0; j < n - k; ++j) {
          vector_b[j + k] -= 2 * vector_x[j] * mult_x_b;
        }
#pragma omp barrier
      }
    }
    free(vector_x_a);
    free(vector_x);
  }
}

void calculate_reverse_gauss_method(int n,
                                    int num_threads,
                                    double* matrix_a,
                                    double* vector_x,
                                    double* vector_b) {
  double sum = 0;
  for (int i = n - 1; i >= 0; --i) {
    sum = 0;
    // #pragma omp parallel num_threads(num_threads)
    {
      // #pragma omp for reduction(+ : sum)
      for (int j = i + 1; j < n; ++j) {
        sum += matrix_a[i * n + j] * vector_x[j];
      }

      // #pragma omp single
      {
        double a = matrix_a[i * n + i];
        vector_x[i] = a != 0.0 ? (vector_b[i] - sum) / a : 0;
      }
    }
  }
}

double get_slae_residual_norm(int n,
                              double* matrix_a,
                              double* vector_x,
                              double* vector_b) {
  double norm = 0;
  for (int i = 0; i < n; ++i) {
    double row_sum = 0;
    for (int j = 0; j < n; ++j) {
      row_sum += matrix_a[i * n + j] * vector_x[j];
    }
    double diff = row_sum - vector_b[i];
    norm += diff * diff;
  }
  return sqrt(norm);
}

double get_norm_of_solution_difference(int n,
                                       double* vector_a,
                                       double* vector_b) {
  double norm = 0;
  for (int i = 0; i < n; ++i) {
    double diff = vector_a[i] - vector_b[i];
    norm += diff * diff;
  }
  return sqrt(norm);
}

void parallel_termination(double* matrix_a,
                          double* vector_x,
                          double* vector_b,
                          double* vector_result) {
  free(matrix_a);
  free(vector_x);
  free(vector_b);
  free(vector_result);
}

int main(int argc, char* argv[]) {
  srand(42);
  assert(argc == 4 && "Invalid command line arguments");
  int n = (int)atoi(argv[1]);
  assert(n > 0 && "Invalid matrix size");
  int num_threads = (int)atoi(argv[2]);
  assert(num_threads > 0 && "Invalid number of threads");

  double* matrix_a;
  double* vector_x;
  double* vector_b;

  // Заполняем матрицу коэффициентов, вектор
  // свободных членов и вектор x
  vector_x = (double*)malloc(n * sizeof(double));
  assert(vector_x != NULL && "Malloc error");
  vector_b = (double*)malloc(n * sizeof(double));
  assert(vector_b != NULL && "Malloc error");
  matrix_a = (double*)malloc(n * n * sizeof(double));
  assert(matrix_a != NULL && "Malloc error");
  get_slae(n, argv[3], matrix_a, vector_x, vector_b);

  // Вывод системы уравнений
  // visualize_slae(n, matrix_a, vector_x, vector_b);

  double timer_start, timer_stop;

  timer_start = omp_get_wtime();
  get_upper_triangular_form(n, num_threads, matrix_a, vector_b);
  timer_stop = omp_get_wtime();
  double time_to_get_upper_triangular_form = timer_stop - timer_start;

  // Вывод системы уравнений
  // visualize_slae(n, matrix_a, vector_x, vector_b);

  double* vector_result;
  vector_result = (double*)malloc(n * sizeof(double));
  assert(vector_result != NULL && "Malloc error");

  timer_start = omp_get_wtime();
  calculate_reverse_gauss_method(n, num_threads, matrix_a, vector_result,
                                 vector_b);
  timer_stop = omp_get_wtime();
  double time_to_calculate_reverse_gauss_method = timer_stop - timer_start;

  // //Вывод решения системы уравнений
  // printf("Vector result");
  // visualize_vector(n, vector_result);
  // // //реальный вектор x
  // printf("Vector x");
  // visualize_vector(n, vector_x);

  // Снова заполняем матрицу коэффициентов,
  // вектор свободных членов и вектор x для
  // подсчета нормы невязки
  get_slae(n, argv[3], matrix_a, vector_x, vector_b);
  double slae_residual_norm =
      get_slae_residual_norm(n, matrix_a, vector_result, vector_b);
  double norm_of_solution_difference =
      get_norm_of_solution_difference(n, vector_result, vector_x);

  printf("Time to get upper triangular form (T1): %lf\n",
         time_to_get_upper_triangular_form);
  printf("Time to calculate reverse gauss method (T2): %lf\n",
         time_to_calculate_reverse_gauss_method);
  printf("Norm of solution difference: %g\n", norm_of_solution_difference);
  printf("Slae residual norm: %g\n", slae_residual_norm);

  parallel_termination(matrix_a, vector_x, vector_b, vector_result);
  return 0;
}