#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void read_matrix(FILE *output, double *matrix, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      double num;
      fread(&num, sizeof(double), 1, output);
      matrix[i * n + j] = (long long)num;
    }
  }
}

void read_vector(FILE *output, double *vector, int n) {
  for (int i = 0; i < n; i++) {
    double num;
    fread(&num, sizeof(double), 1, output);
    vector[i] = (double)num;
  }
}

void get_slae(int n, char *filename, double *matrix_a, double *vector_x, double *vector_b) {
  FILE *output = fopen(filename, "rb");
  if (!output) {
    free(matrix_a);
    free(vector_x);
    free(vector_b);
    exit(1);
  }

  read_vector(output, vector_x, n);
  read_matrix(output, matrix_a, n);
  read_vector(output, vector_b, n);
}

void get_upper_triangular_form(int n, int num_threads, double *matrix_a, double *vector_b) {
  double norm_vector_a = 0;
  double norm_vector_x = 0;
  double mult_x_b = 0;
  for (int k = 0; k < n - 1; k++) {
    double *vector_x = (double *)malloc((n - k) * sizeof(double));
    double *vector_x_a = (double *)malloc((n - k) * sizeof(double));
    norm_vector_a = 0;
    norm_vector_x = 0;
    mult_x_b = 0;
    int is_zeros_vector = 1;
    #pragma omp parallel num_threads(num_threads)
    {
      #pragma omp for reduction(+ : norm_vector_a)
      for (int j = 0; j < n - k; j++) {
        const double a = matrix_a[(j + k) * n + k];
        if (a != 0.0) is_zeros_vector = 0;
        norm_vector_a += a * a;
        vector_x[j] = a;
      }

      if (!is_zeros_vector) {
        #pragma omp single
        { vector_x[0] -= sqrt(norm_vector_a); }

        #pragma omp for reduction(+ : norm_vector_x)
        for (int j = 0; j < n - k; j++) {
          norm_vector_x += pow(vector_x[j], 2);
        }

        #pragma omp for
        for (int j = 0; j < n - k; j++) {
          vector_x[j] /= sqrt(norm_vector_x);
        }
        #pragma omp barrier
        #pragma omp for
        for (int i = 0; i < n - k; i++) {
          vector_x_a[i] = 0;
          for (int j = 0; j < n - k; j++) {
            vector_x_a[i] += vector_x[j] * matrix_a[(i + k) + n * (j + k)];
          }
        }
        #pragma omp barrier
        #pragma omp for
        for (int i = 0; i < n - k; i++) {
          for (int j = 0; j < n - k; j++) {
            matrix_a[(i + k) * n + (j + k)] -= 2 * vector_x[i] * vector_x_a[j];
          }
        }
        #pragma omp barrier
        #pragma omp for reduction(+ : mult_x_b)
        for (int j = 0; j < n - k; j++) {
          mult_x_b += vector_x[j] * vector_b[j + k];
        }

        #pragma omp for
        for (int j = 0; j < n - k; j++) {
          vector_b[j + k] -= 2 * vector_x[j] * mult_x_b;
        }
        #pragma omp barrier
      }
    }
    free(vector_x_a);
    free(vector_x);
  }
}

void calculate_reverse_gauss_method(int n, int num_threads, double *matrix_a, double *vector_x, double *vector_b) {
  double sum = 0;
  for (int i = n - 1; i >= 0; i--) {
    sum = 0;
    #pragma omp parallel num_threads(num_threads)
    {
      #pragma omp for reduction(+ : sum)
      for (int j = i + 1; j < n; j++) {
        sum += matrix_a[i * n + j] * vector_x[j];
      }

      #pragma omp single
      {
        const double a = matrix_a[i * n + i];
        if (a != 0.0) vector_x[i] = (vector_b[i] - sum) / a;
        else vector_x[i] = 0;
      }
    }
  }
}

double get_slae_residual_norm(int n, double *matrix_a, double *vector_x, double *vector_b) {
  double norm = 0;
  for (int i = 0; i < n; i++) {
    double row_sum = 0;
    for (int j = 0; j < n; j++) {
      row_sum += matrix_a[i * n + j] * vector_x[j];
    }
    norm += pow(row_sum - vector_b[i], 2);
  }
  return sqrt(norm);
}

double get_norm_of_solution_difference(int n, double *vector_a, double *vector_b) {
  double norm = 0;
  for (int i = 0; i < n; ++i) {
    norm += pow(vector_a[i] - vector_b[i], 2);
  }
  return sqrt(norm);
}

int main(int argc, char *argv[]) {
  srand(42);
  int n = (int)atoi(argv[1]);
  int num_threads = (int)atoi(argv[2]);
  char *filename = argv[3];

  double *matrix_a;
  double *vector_x;
  double *vector_b;
  double *vector_result;

  double time_to_get_upper_triangular_form;
  double time_to_calculate_reverse_gauss_method;
  double timer_start;

  // Выделяем место под все используемые вектора и матрицу. 
  // Заполняем матрицу коэффициентов и вектора a, b, x.
  matrix_a = (double *)malloc(n * n * sizeof(double));
  vector_b = (double *)malloc(n * sizeof(double));
  vector_x = (double *)malloc(n * sizeof(double));
  vector_result = (double *)malloc(n * sizeof(double));
  get_slae(n, filename, matrix_a, vector_x, vector_b);

  // Рассчитываем вернюю треугольную форму. Замеряем время.
  timer_start = omp_get_wtime();
  get_upper_triangular_form(n, num_threads, matrix_a, vector_b);
  time_to_get_upper_triangular_form = omp_get_wtime() - timer_start;

  // Применяем метод обратного хода Гаусса для нахождения решения. Замеряем время.
  timer_start = omp_get_wtime();
  calculate_reverse_gauss_method(n, num_threads, matrix_a, vector_result, vector_b);
  time_to_calculate_reverse_gauss_method = omp_get_wtime() - timer_start;

  // Заполняем матрицу коэффициентов и вектора a, b, x.
  // Рассчитываем норму невязки
  get_slae(n, filename, matrix_a, vector_x, vector_b);
  double slae_residual_norm = get_slae_residual_norm(n, matrix_a, vector_result, vector_b);
  double norm_of_solution_difference = get_norm_of_solution_difference(n, vector_result, vector_x);

  printf("[T1] Время приведения к треугольному виду: %lf\n", time_to_get_upper_triangular_form);
  printf("[T2] Время для обратного хода методом Гаусса: %lf\n", time_to_calculate_reverse_gauss_method);
  printf("[T] Общее время выполнения: %lf\n", time_to_get_upper_triangular_form + time_to_calculate_reverse_gauss_method);
  printf("Норма разницы между полученным и точным решениями: %g\n", norm_of_solution_difference);
  printf("Норма невязки: %g\n", slae_residual_norm);

  free(matrix_a);
  free(vector_x);
  free(vector_b);
  free(vector_result);
  return 0;
}