#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void readMatrix(FILE *output, double *matrix, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      double num;
      fread(&num, sizeof(double), 1, output);
      matrix[i * n + j] = (long long)num;
    }
  }
}

void readVector(FILE *output, double *vector, int n) {
  for (int i = 0; i < n; i++) {
    double num;
    fread(&num, sizeof(double), 1, output);
    vector[i] = (double)num;
  }
}

void readSlae(int n, char *filename, double *matrix_A, double *vector_x, double *vector_b) {
  FILE *output = fopen(filename, "rb");

  if (!output) {
    free(matrix_A);
    free(vector_x);
    free(vector_b);
    exit(1);
  }

  readVector(output, vector_x, n);
  readMatrix(output, matrix_A, n);
  readVector(output, vector_b, n);
}

void getUpperTriangular(int n, int numThreads, double *matrix_A, double *vector_b) {
  double normVector_a = 0;
  double normVector_x = 0;
  double mult_x_b = 0;
  for (int k = 0; k < n - 1; k++) {
    double *vector_x = (double *)malloc((n - k) * sizeof(double));
    double *vector_x_a = (double *)malloc((n - k) * sizeof(double));
    normVector_a = 0;
    normVector_x = 0;
    mult_x_b = 0;
    int isEmptyVector = 1;
    #pragma omp parallel num_threads(numThreads)
    {
      #pragma omp for reduction(+ : norm_vector_a)
      for (int j = 0; j < n - k; j++) {
        const double a = matrix_A[(j + k) * n + k];
        if (a != 0.0) isEmptyVector = 0;
        normVector_a += a * a;
        vector_x[j] = a;
      }

      if (!isEmptyVector) {
        #pragma omp single
        { vector_x[0] -= sqrt(normVector_a); }

        #pragma omp for reduction(+ : norm_vector_x)
        for (int j = 0; j < n - k; j++) {
          normVector_x += pow(vector_x[j], 2);
        }
        #pragma omp for
        for (int j = 0; j < n - k; j++) {
          vector_x[j] /= sqrt(normVector_x);
        }
        #pragma omp barrier
        #pragma omp for
        for (int i = 0; i < n - k; i++) {
          vector_x_a[i] = 0;
          for (int j = 0; j < n - k; j++) {
            vector_x_a[i] += vector_x[j] * matrix_A[(i + k) + (j + k) * n];
          }
        }
        #pragma omp barrier
        #pragma omp for
        for (int i = 0; i < n - k; i++) {
          for (int j = 0; j < n - k; j++) {
            matrix_A[(i + k) * n + (j + k)] -= vector_x[i] * vector_x_a[j] * 2;
          }
        }
        #pragma omp barrier
        #pragma omp for reduction(+ : mult_x_b)
        for (int j = 0; j < n - k; j++) {
          mult_x_b += vector_x[j] * vector_b[j + k];
        }

        #pragma omp for
        for (int j = 0; j < n - k; j++) {
          vector_b[j + k] -= 2 * mult_x_b * vector_x[j];
        }
        #pragma omp barrier
      }
    }
    free(vector_x_a);
    free(vector_x);
  }
}

void computeReverseGauss(int n, int numThreads, double *matrix_A, double *vector_x, double *vector_b) {
  double sum = 0;
  for (int i = n - 1; i >= 0; i--) {
    sum = 0;
    #pragma omp parallel num_threads(numThreads)
    {
      #pragma omp for reduction(+ : sum)
      for (int j = i + 1; j < n; j++) {
        sum += matrix_A[i * n + j] * vector_x[j];
      }

      #pragma omp single
      {
        const double a = matrix_A[i * n + i];
        if (a != 0.0) vector_x[i] = (vector_b[i] - sum) / a;
        else vector_x[i] = 0;
      }
    }
  }
}

double getResidualNorm(int n, double *matrix_A, double *vector_x, double *vector_b) {
  double norm = 0;
  for (int i = 0; i < n; i++) {
    double row_sum = 0;
    for (int j = 0; j < n; j++) {
      row_sum += matrix_A[i * n + j] * vector_x[j];
    }
    norm += pow(row_sum - vector_b[i], 2);
  }
  return sqrt(norm);
}

double getNormDifference(int n, double *vector_a, double *vector_b) {
  double norm = 0;
  for (int i = 0; i < n; ++i) {
    norm += pow(vector_a[i] - vector_b[i], 2);
  }
  return sqrt(norm);
}

int main(int argc, char *argv[]) {
  int n = (int)atoi(argv[1]);
  int OMPNumThreads = (int)atoi(argv[2]);
  char *filename = argv[3];

  double *matrix_A;
  double *vector_x;
  double *vector_b;
  double *vector_res;

  double t1;
  double t2;
  double tmpTimestamp;

  // Выделяем место под все используемые вектора и матрицу. 
  // Заполняем матрицу коэффициентов и вектора a, b, x.
  matrix_A = (double *)malloc(n * n * sizeof(double));
  vector_b = (double *)malloc(n * sizeof(double));
  vector_x = (double *)malloc(n * sizeof(double));
  vector_res = (double *)malloc(n * sizeof(double));
  readSlae(n, filename, matrix_A, vector_x, vector_b);

  // Рассчитываем вернюю треугольную форму. Замеряем время.
  tmpTimestamp = omp_get_wtime();
  getUpperTriangular(n, OMPNumThreads, matrix_A, vector_b);
  t1 = omp_get_wtime() - tmpTimestamp;

  // Применяем метод обратного хода Гаусса для нахождения решения. Замеряем время.
  tmpTimestamp = omp_get_wtime();
  computeReverseGauss(n, OMPNumThreads, matrix_A, vector_res, vector_b);
  t2 = omp_get_wtime() - tmpTimestamp;

  // Заполняем матрицу коэффициентов и вектора a, b, x.
  // Рассчитываем норму невязки
  readSlae(n, filename, matrix_A, vector_x, vector_b);
  double residualNorm = getResidualNorm(n, matrix_A, vector_res, vector_b);
  double normDifference = getNormDifference(n, vector_res, vector_x);

  printf("[T1] Время приведения к треугольному виду: %lf\n", t1);
  printf("[T2] Время для обратного хода методом Гаусса: %lf\n", t2);
  printf("[T] Общее время выполнения: %lf\n", t1 + t2);
  printf("Норма разницы между полученным и точным решениями: %g\n", normDifference);
  printf("Норма невязки: %g\n", residualNorm);

  free(matrix_A);
  free(vector_x);
  free(vector_b);
  free(vector_res);
  return 0;
}