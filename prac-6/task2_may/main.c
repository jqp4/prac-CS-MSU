#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int PROC_NUM;
int PROC_RANK;
int PROC_WITH_B;

void genSlae(int size, int *cols_num, double **proc_cols, double **proc_x, double **vector_b) {
  *cols_num = size / PROC_NUM;
  if (PROC_RANK < size % PROC_NUM) (*cols_num) += 1;
  *proc_cols = (double *)malloc((*cols_num) * size * sizeof(double));
  *proc_x = (double *)malloc((*cols_num) * sizeof(double));
  double *proc_b = (double *)malloc(size * sizeof(double));

  // генерируем кусочки матрицы A и вектора x
  for (int i = 0; i < (*cols_num); i++) {
    (*proc_x)[i] = (double)((rand() + PROC_RANK) % 200);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  for (int i = 0; i < (*cols_num); i++) {
    for (int j = 0; j < size; j++) {
      double num = (double)((rand() + PROC_RANK) % 200);
      (*proc_cols)[i * size + j] = num;
    }
  }
  MPI_Barrier(MPI_COMM_WORLD);

  // считаем кусочек вектора b
  for (int i = 0; i < (*cols_num); i++) {
    for (int j = 0; j < size; j++) {
      proc_b[j] += (*proc_cols)[i * size + j] * (*proc_x)[i];
    }
  }

  // Собираем вектор b
  MPI_Reduce(proc_b, *vector_b, size, MPI_DOUBLE, MPI_SUM, PROC_WITH_B, MPI_COMM_WORLD);
  free(proc_b);
}

void getTriangularFormJob(int size, int k, int process, int col_num, double *proc_cols, double *vector_b, MPI_Comm comm) {
  const int stride = k * PROC_NUM;
  const double w_size = size - stride - process;
  double *w = (double *)malloc((w_size) * sizeof(double));
  if (PROC_RANK == process) {
    double a0a0;
    double norm_a_i = 0;
    for (int i = 0; i < w_size; i++) {
      const double a = proc_cols[(k * size) + (stride + process) + i];
      if (i == 0) a0a0 = pow(a, 2);
      norm_a_i += pow(a, 2);
      w[i] = a;
    }

    w[0] += sqrt(norm_a_i);
    const double w0w0 = pow(w[0], 2);
    const double norm_w = sqrt(norm_a_i - a0a0 + w0w0);

    for (int i = 0; i < w_size; i++) {
      w[i] /= norm_w;
    }
  }

  MPI_Bcast(w, w_size, MPI_DOUBLE, process, comm);
  double *w_a = (double *)malloc((col_num - k) * sizeof(double));
  for (int i = 0; i < col_num - k; i++) {
    w_a[i] = 0;
    for (int j = 0; j < w_size; j++) {
      w_a[i] += w[j] * proc_cols[(k * size) + i * size + (j + stride + process)];
    }
  }

  for (int i = 0; i < w_size; i++) {
    for (int j = 0; j < col_num - k; j++) {
      proc_cols[(k * size) + j * size + (i + stride + process)] -= 2 * w[i] * w_a[j];
    }
  }

  if (PROC_RANK == PROC_WITH_B) {
    double mult_w_b = 0;
    for (int i = 0; i < w_size; i++) {
      mult_w_b += w[i] * vector_b[i + stride + process];
    }

    for (int i = 0; i < w_size; i++) {
      vector_b[i + stride + process] -= 2 * w[i] * mult_w_b;
    }
  }
  free(w_a);
  free(w);
}

void getTriangularForm(int size, int col_num, double *proc_cols, double *vector_b) {
  for (int k = 0; k < size / PROC_NUM; ++k) {
    for (int process = 0; process < PROC_NUM; process++) {
      if (k * PROC_NUM + process == size - 1) break;
      getTriangularFormJob(size, k, process, col_num, proc_cols, vector_b, MPI_COMM_WORLD);
    }
  }

  MPI_Comm comm;
  MPI_Comm_split(MPI_COMM_WORLD, PROC_RANK < size % PROC_NUM, PROC_RANK, &comm);
  if (PROC_RANK < size % PROC_NUM) {
    for (int process = 0; process < size % PROC_NUM - 1; process++) {
      getTriangularFormJob(size, (size / PROC_NUM), process, col_num, proc_cols, vector_b, comm);
    }
  }

  MPI_Comm_free(&comm);
}

void calculateReverseGaussJob(int size, int k, int process, int col_num, double *proc_cols, double *vector_b, double *vector_x, MPI_Comm comm) {
  double row_sum = 0.0;
  double process_row_sum = 0.0;
  const int stride = k * PROC_NUM;
  for (int i = k; i < col_num; i++) {
    process_row_sum += vector_x[i] * proc_cols[i * size + stride + process];
  }

  MPI_Reduce(&process_row_sum, &row_sum, 1, MPI_DOUBLE, MPI_SUM, process, comm);
  const double a = proc_cols[k * size + stride + process];
  if (process != PROC_WITH_B) {
    if (PROC_RANK == PROC_WITH_B) {
      MPI_Send(&(vector_b[stride + process]), 1, MPI_DOUBLE, process, 0, comm);
    }
    if (PROC_RANK == process) {
      double tmp_b_i = 0;
      MPI_Recv(&tmp_b_i, 1, MPI_DOUBLE, PROC_WITH_B, 0, comm, MPI_STATUS_IGNORE);
      vector_x[k] = a != 0 ? (tmp_b_i - row_sum) / a : 0;
    }
  } else {
    if (PROC_RANK == process) {
      vector_x[k] = a != 0 ? (vector_b[stride + process] - row_sum) / a : 0;
    }
  }
}

void calculateReverseGauss(int size, int col_num, double *proc_cols, double *vector_b, double *vector_x) {
  MPI_Comm comm;
  MPI_Comm_split(MPI_COMM_WORLD, PROC_RANK < size % PROC_NUM, PROC_RANK, &comm);
  if (PROC_RANK < size % PROC_NUM) {
    for (int process = size % PROC_NUM - 1; process >= 0; --process) {
      calculateReverseGaussJob(size, (size / PROC_NUM), process, col_num, proc_cols, vector_b, vector_x, comm);
    }
  }

  MPI_Comm_free(&comm);
  for (int k = size / PROC_NUM - 1; k >= 0; --k) {
    for (int process = PROC_NUM - 1; process >= 0; --process) {
      calculateReverseGaussJob(size, k, process, col_num, proc_cols, vector_b, vector_x, MPI_COMM_WORLD);
    }
  }
}

double getResidualNorm(int size, int cols_num, double *proc_cols, double *vector_x, double *vector_b) {
  double *proc_b = (double *)malloc(size * sizeof(double));
  for (int i = 0; i < size; i++) {
    proc_b[i] = 0.0;
  }

  for (int i = 0; i < cols_num; i++) {
    for (int j = 0; j < size; j++) {
      proc_b[j] += proc_cols[i * size + j] * vector_x[i];
    }
  }

  double *vector_b_new = (double *)malloc(size * sizeof(double));
  MPI_Reduce(proc_b, vector_b_new, size, MPI_DOUBLE, MPI_SUM, PROC_WITH_B, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  free(proc_b);

  double norm = 0;
  if (PROC_RANK == PROC_WITH_B) {
    for (int i = 0; i < size; i++) {
      norm += pow(vector_b_new[i] - vector_b[i], 2);
    }
  }

  MPI_Bcast(&norm, 1, MPI_DOUBLE, PROC_WITH_B, MPI_COMM_WORLD);
  if (PROC_RANK == PROC_WITH_B) free(vector_b_new);

  return sqrt(norm);
}

double getSolutionDiff(int cols_num, double *vector_x, double *vector_x_new) {
  double norm = 0;
  double res_norm = 0;
  for (int i = 0; i < cols_num; i++) {
    norm += pow(vector_x[i] - vector_x_new[i], 2);
  }
  MPI_Reduce(&norm, &res_norm, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Bcast(&res_norm, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  return sqrt(norm);
}

void mpiFree(int size, double *vector_x, double *vector_b, double *proc_cols, double *vector_res) {
  free(vector_x);
  free(proc_cols);
  free(vector_res);
  if (size % PROC_NUM != 1 && PROC_NUM != 1) {
    if (PROC_RANK == PROC_WITH_B) free(vector_b);
  }
}

int main(int argc, char **argv) {
  int size = (int)atoi(argv[1]);
  int cols_num = size;
  double *proc_x;
  double *vector_b;
  double *proc_cols;
  double *vector_res;
  double timestamp;
  double T1, T2;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &PROC_NUM);
  MPI_Comm_rank(MPI_COMM_WORLD, &PROC_RANK);

  PROC_WITH_B = (size % PROC_NUM ? (size % PROC_NUM) - 1 : PROC_NUM - 1);
  if (PROC_RANK == PROC_WITH_B) vector_b = (double *)malloc(size * sizeof(double));

  srand((unsigned int)time(NULL) + PROC_RANK);
  genSlae(size, &cols_num, &proc_cols, &proc_x, &vector_b);

  timestamp = MPI_Wtime();
  getTriangularForm(size, cols_num, proc_cols, vector_b);
  T1 = MPI_Wtime() - timestamp;

  vector_res = (double *)malloc(cols_num * sizeof(double));
  for (int i = 0; i < cols_num; i++) {
    vector_res[i] = 0;
  }

  timestamp = MPI_Wtime();
  calculateReverseGauss(size, cols_num, proc_cols, vector_b, vector_res);
  T2 = MPI_Wtime() - timestamp;

  double residual_norm = getResidualNorm(size, cols_num, proc_cols, vector_res, vector_b);
  double solution_diff = getSolutionDiff(cols_num, proc_x, vector_res);

  if (PROC_RANK == 0) {
    printf("T1, T2, точность, норма невязки:\n\n");
    printf("%lf\n", T1);
    printf("%lf\n", T2);
    printf("%g\n", solution_diff);
    printf("%g\n", residual_norm);
  }

  MPI_Barrier(MPI_COMM_WORLD);
  mpiFree(size, proc_x, vector_b, proc_cols, vector_res);
  MPI_Finalize();
  return 0;
}
