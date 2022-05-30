#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int PROC_NUM;
int PROC_RANK;

#define PROC_WITH_B (size % PROC_NUM ? (size % PROC_NUM) - 1 : PROC_NUM - 1)

void genSlae(int size, int *cols_num, double **proc_cols, double **proc_x, double **vector_b) {
  *cols_num = size / PROC_NUM;
  if (PROC_RANK < size % PROC_NUM) (*cols_num) += 1;
  *proc_cols = (double *)malloc((*cols_num) * size * sizeof(double));
  *proc_x = (double *)malloc((*cols_num) * sizeof(double));
  double *proc_b = (double *)malloc(size * sizeof(double));

  // генерируем кусочки матрицы A и вектора x
  for (int i = 0; i < (*cols_num); i++) {
    (*proc_x)[i] = (double)(rand() % 200);
    for (int j = 0; j < size; ++j) {
      double num = (double)(rand() % 200);
      (*proc_cols)[i * size + j] = num;
    }
  }

  // считаем кусочек вектора b
  for (int i = 0; i < (*cols_num); i++) {
    for (int j = 0; j < size; ++j) {
      proc_b[j] += (*proc_cols)[i * size + j] * (*proc_x)[i];
    }
  }

  // Собираем вектор b
  MPI_Reduce(proc_b, *vector_b, size, MPI_DOUBLE, MPI_SUM, PROC_WITH_B, MPI_COMM_WORLD);
}

void getUpperTriangularIteration(int size, int k, int proc, int cols_num, double *proc_cols, double *vector_b, MPI_Comm comm) {
  // int vector_b_proc = size % PROC_NUM ? (size % PROC_NUM) - 1 : PROC_NUM - 1;
  int stride = k * PROC_NUM;
  int tmp_size = size - stride - proc;
  double *tmp = (double *)malloc(tmp_size * sizeof(double));

  if (PROC_RANK == proc) {
    double norm_a_i = 0;
    double a0a0;
    for (int i = 0; i < tmp_size; i++) {
      double a = proc_cols[(k * size) + stride + proc + i];
      if (i == 0) a0a0 = pow(a, 2);
      norm_a_i += pow(a, 2);
      tmp[i] = a;
    }

    // старый способ
    tmp[0] -= sqrt(norm_a_i);
    double tmp_norm_old = 0;
    for (int i = 0; i < tmp_size; i++) {
      tmp_norm_old += pow(tmp[i], 2);
      //   double stmpi = pow(i == 0 ? tmp[i] - sqrt(norm_a_i) : tmp[i], 2);
      //   tmp_norm_old += stmpi;
    }
    tmp_norm_old = sqrt(tmp_norm_old);

    double tmp_norm = tmp_norm_old;

    // новый способ
    // tmp[0] += sqrt(norm_a_i);
    // double w0w0 = pow(tmp[0], 2);
    // double tmp_norm = sqrt(norm_a_i - a0a0 + w0w0);
    // printf("w norm old = %lf\n", tmp_norm_old);
    // printf("w norm new = %lf\n", tmp_norm);
    // printf("a norm = %lf\n", sqrt(norm_a_i));
    // printf("a0a0 = %lf\n", a0a0);
    // printf("w0w0 = %lf\n\n", w0w0);

    if (tmp_norm != 0) {
      for (int i = 0; i < tmp_size; i++) {
        tmp[i] /= tmp_norm;
      }
    } else {
      for (int i = 0; i < tmp_size; i++) {
        tmp[i] = 0;
      }
    }
  }

  MPI_Bcast(tmp, tmp_size, MPI_DOUBLE, proc, comm);
  double *tmp_a = (double *)malloc((cols_num - k) * sizeof(double));
  for (int i = 0; i < cols_num - k; i++) {
    tmp_a[i] = 0;
    for (int j = 0; j < tmp_size; j++) {
      tmp_a[i] += tmp[j] * proc_cols[k * size + i * size + j + stride + proc];
    }
  }

  for (int i = 0; i < tmp_size; i++) {
    for (int j = 0; j < cols_num - k; j++) {
      proc_cols[k * size + j * size + i + stride + proc] -= 2 * tmp[i] * tmp_a[j];
    }
  }

  if (PROC_RANK == PROC_WITH_B) {
    double mult_w_b = 0;
    for (int i = 0; i < tmp_size; i++) {
      mult_w_b += tmp[i] * vector_b[i + stride + proc];
    }

    for (int i = 0; i < tmp_size; i++) {
      vector_b[i + stride + proc] -= 2 * tmp[i] * mult_w_b;
    }
  }

  free(tmp_a);
  free(tmp);
}

void triagonalize_it(int size, int k, int proc, int local_size, double *local_matrix, double *vector_b, MPI_Comm comm) {
  int shift = k * PROC_NUM;
  double *tmp = (double *)malloc((size - shift - proc) * sizeof(double));

  if (PROC_RANK == proc) {
    double norm_a_i = 0.0;
    double a0a0;
    for (int i = 0; i < size - shift - proc; i++) {
      double a = local_matrix[(k * size) + (shift + proc) + i];
      tmp[i] = a;
      norm_a_i += pow(a, 2);
      if (i == 0) a0a0 = a * a;
    }
    double norm_a = sqrt(norm_a_i);
    tmp[0] += norm_a;
    // double tmp_norm = 0.0;
    // for (int i = 0; i < size - shift - proc; i++) {
    // double w = tmp[i];
    // tmp_norm += w * w;
    //}
    double w0w0 = tmp[0] * tmp[0];
    double tmp_norm = sqrt(norm_a_i - a0a0 + w0w0);
    // tmp_norm = sqrt(tmp_norm);
    // printf("norm a %lf\n", norm_a);
    // printf("norm w %lf\n", tmp_norm);
    // printf("norm x %lf\n", norm_x);

    if (tmp_norm != 0.0) {
      for (int i = 0; i < size - shift - proc; i++) {
        tmp[i] /= tmp_norm;
      }
    } else {
      for (int i = 0; i < size - shift - proc; i++) {
        tmp[i] = 0.0;
      }
    }
  }
}

void getUpperTriangular(int size, int cols_num, double *proc_cols, double *vector_b) {
  for (int k = 0; k < size / PROC_NUM; k++) {
    for (int proc = 0; proc < PROC_NUM; ++proc) {
      if (k * PROC_NUM + proc == size - 1) break;

      getUpperTriangularIteration(size, k, proc, cols_num, proc_cols, vector_b, MPI_COMM_WORLD);
    }
  }

  MPI_Comm new_comm;
  MPI_Comm_split(MPI_COMM_WORLD, PROC_RANK < size % PROC_NUM, PROC_RANK, &new_comm);
  if (PROC_RANK < size % PROC_NUM) {
    for (int proc = 0; proc < size % PROC_NUM - 1; ++proc) {
      getUpperTriangularIteration(size, (size / PROC_NUM), proc, cols_num, proc_cols, vector_b, new_comm);
    }
  }

  MPI_Comm_free(&new_comm);
}

void computeReverseGaussIteration(int size, int k, int proc, int cols_num, double *proc_cols, double *vector_b, double *vector_x, MPI_Comm comm) {
  // int vector_b_proc = size % PROC_NUM ? (size % PROC_NUM) - 1 : PROC_NUM - 1;
  double row_sum = 0;
  double proc_row_sum = 0;
  int stride = k * PROC_NUM;

  for (int i = k; i < cols_num; i++) {
    proc_row_sum += vector_x[i] * proc_cols[i * size + stride + proc];
  }

  MPI_Reduce(&proc_row_sum, &row_sum, 1, MPI_DOUBLE, MPI_SUM, proc, comm);
  double a = proc_cols[k * size + stride + proc];
  if (proc != PROC_WITH_B) {
    if (PROC_RANK == PROC_WITH_B) {
      MPI_Send(&(vector_b[stride + proc]), 1, MPI_DOUBLE, proc, 0, comm);
    }
    if (PROC_RANK == proc) {
      double tmp_b_i = 0;
      MPI_Recv(&tmp_b_i, 1, MPI_DOUBLE, PROC_WITH_B, 0, comm, MPI_STATUS_IGNORE);

      if (a == 0) vector_x[k] = 0;
      else vector_x[k] = (tmp_b_i - row_sum) / a;
    }
  } else {
    if (PROC_RANK == proc) {
      if (a == 0) vector_x[k] = 0;
      else vector_x[k] = (vector_b[stride + proc] - row_sum) / a;
    }
  }
}

void computeReverseGauss(int size, int cols_num, double *proc_cols, double *vector_b, double *vector_x) {
  MPI_Comm new_comm;
  MPI_Comm_split(MPI_COMM_WORLD, PROC_RANK < size % PROC_NUM, PROC_RANK, &new_comm);
  if (PROC_RANK < size % PROC_NUM) {
    for (int proc = size % PROC_NUM - 1; proc >= 0; --proc) {
      computeReverseGaussIteration(size, (size / PROC_NUM), proc, cols_num, proc_cols, vector_b, vector_x, new_comm);
    }
  }

  MPI_Comm_free(&new_comm);
  for (int k = size / PROC_NUM - 1; k >= 0; --k) {
    for (int proc = PROC_NUM - 1; proc >= 0; --proc) {
      computeReverseGaussIteration(size, k, proc, cols_num, proc_cols, vector_b, vector_x, MPI_COMM_WORLD);
    }
  }
}

double getResidualNorm(int size, int cols_num, double *proc_cols, double *vector_x, double *vector_b) {
  double norm = 0;
  double *vector_res;
  double *all_row_sums;
  // int cols_num = size / PROC_NUM;
  // if (PROC_RANK < size % PROC_NUM) cols_num += 1;
  if (PROC_RANK == 0) vector_res = (double *)malloc(size * sizeof(double));

  // printf("process #%d, positoin %d\n", PROC_RANK, 5);

  for (int i = 0; i < cols_num; i++) {
    MPI_Gather(vector_x + i, 1, MPI_DOUBLE, vector_res + i * PROC_NUM, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

  // printf("process #%d, positoin %d\n", PROC_RANK, 6);

  MPI_Comm new_comm;
  MPI_Comm_split(MPI_COMM_WORLD, PROC_RANK < size % PROC_NUM, PROC_RANK, &new_comm);
  if (PROC_RANK < size % PROC_NUM) MPI_Gather(vector_x + cols_num, 1, MPI_DOUBLE, vector_res + PROC_NUM * cols_num, 1, MPI_DOUBLE, 0, new_comm);
  MPI_Comm_free(&new_comm);

  printf("process #%d, positoin %d\n", PROC_RANK, 7);

  // if (PROC_RANK == 0) {
  //   for (int i = 0; i < size; i++) {
  //     double row_sum = 0;
  //     for (int j = 0; j < size; j++) {
  //       row_sum += matrix_a[j * size + i] * vector_res[j];
  //     }
  //     norm += pow(row_sum - vector_b[i], 2);
  //   }
  //   free(vector_res);
  // }

  if (PROC_RANK == 0) all_row_sums = (double *)malloc(size * sizeof(double));
  double *row_sums = (double *)malloc(size * sizeof(double));

  // for (int i = 0; i < size; i++) {
  //   for (int j = 0; j < cols_num; ++j) {
  //     row_sums[i] += proc_cols[j * size + i] * vector_res[j];
  //   }
  // }

  for (int i = 0; i < cols_num; i++) {
    for (int j = 0; j < size; j++) {
      row_sums[i] += proc_cols[i * size + j] * vector_res[i];
    }
  }

  printf("process #%d, positoin %d\n", PROC_RANK, 8);

  MPI_Reduce(row_sums, all_row_sums, size, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  printf("process #%d, positoin %d\n", PROC_RANK, 9);

  if (PROC_RANK == 0) {
    for (int i = 0; i < size; i++) {
      norm += pow(all_row_sums[i] - vector_b[i], 2);
    }
  }

  printf("process #%d, positoin %d\n", PROC_RANK, 10);

  MPI_Bcast(&norm, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  return sqrt(norm);
}

double getSolutionDiff(int size, double *vector_x, double *vector_x_new) {
  double *vector_res;
  int cols_num = size / PROC_NUM;
  if (PROC_RANK == 0) vector_res = (double *)malloc(size * sizeof(double));

  for (int i = 0; i < cols_num; i++) {
    MPI_Gather(vector_x_new + i, 1, MPI_DOUBLE, vector_res + i * PROC_NUM, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

  MPI_Comm new_comm;
  MPI_Comm_split(MPI_COMM_WORLD, PROC_RANK < size % PROC_NUM, PROC_RANK, &new_comm);
  if (PROC_RANK < size % PROC_NUM) MPI_Gather(vector_x_new + cols_num, 1, MPI_DOUBLE, vector_res + PROC_NUM * cols_num, 1, MPI_DOUBLE, 0, new_comm);
  MPI_Comm_free(&new_comm);

  double norm = 0.0;
  if (PROC_RANK == 0) {
    for (int i = 0; i < cols_num; i++) {
      norm += pow(vector_x[i] - vector_res[i], 2);
    }
    free(vector_res);
  }

  MPI_Bcast(&norm, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  return sqrt(norm);
}

// void mpifree(int size, double *matrix_a, double *vector_x, double *vector_b, double *proc_cols, double *vector_res) {
//   if (PROC_RANK == 0) {
//     free(matrix_a);
//     free(vector_x);
//     free(vector_b);
//   }
//   free(proc_cols);
//   free(vector_res);
//   if (size % PROC_NUM != 1 && PROC_NUM != 1) {
//     // int reciver_id = size % PROC_NUM ? (size % PROC_NUM) - 1 : PROC_NUM - 1;
//     if (PROC_RANK == PROC_WITH_B) free(vector_b);
//   }
// }

int main(int argc, char **argv) {
  int size = (int)atoi(argv[1]);
  // int size = 5;
  int cols_num = size;
  // double *matrix_a;
  // double *vector_b;
  // double *vector_x;
  double *proc_x;
  double *vector_b;
  double *proc_cols;
  double *vector_res;
  double timer_start;
  double T1, T2;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &PROC_NUM);
  MPI_Comm_rank(MPI_COMM_WORLD, &PROC_RANK);

  // readSlae(argv[2], size, &matrix_a, &vector_x, &vector_b);
  // if (PROC_RANK == 0) free(vector_x);
  // shareMatrix(size, matrix_a, &vector_b, &cols_num, &proc_cols);
  if (PROC_RANK == PROC_WITH_B) vector_b = (double *)malloc(size * sizeof(double));

  srand((unsigned int)time(NULL) + PROC_RANK);
  genSlae(size, &cols_num, &proc_cols, &proc_x, &vector_b);

  // printf("Матрица:\n");
  // for (int i = 0; i < size; i++) {
  //   int k = 0;
  //   // while (k < cols_num) {
  //   for (int k = 0; k < cols_num; k++) {
  //     for (int j = 0; j < PROC_NUM; j++) {
  //       if (PROC_RANK == j) {
  //         printf("%lf  ", proc_cols[k * size + i]);
  //       }
  //     }
  //   }
  //   printf("\n");
  // }

  // ------------------------------
  MPI_Barrier(MPI_COMM_WORLD);
  // printf("[");
  // for (int col = 0; col < cols_num; col++) {
  //   for (int i = 0; i < size; i++) {
  //     printf(" %lf ", proc_cols[col * size + i]);
  //   }
  // }
  // printf("]\n");
  // MPI_Barrier(MPI_COMM_WORLD);
  // if (PROC_RANK == PROC_WITH_B) {
  //   printf("\n\nВектор b: <");
  //   for (int i = 0; i < size; i++) {
  //     printf(" %lf ", vector_b[i]);
  //   }
  //   printf(">\n");
  // }
  // MPI_Barrier(MPI_COMM_WORLD);
  // ------------------------------

  timer_start = MPI_Wtime();
  getUpperTriangular(size, cols_num, proc_cols, vector_b);
  T1 = MPI_Wtime() - timer_start;

  // printf("process #%d, positoin %d\n", PROC_RANK, 2);

  vector_res = (double *)malloc(cols_num * sizeof(double));
  for (int i = 0; i < cols_num; i++) {
    vector_res[i] = 0;
  }

  // printf("process #%d, positoin %d\n", PROC_RANK, 3);

  timer_start = MPI_Wtime();
  computeReverseGauss(size, cols_num, proc_cols, vector_b, vector_res);
  T2 = MPI_Wtime() - timer_start;

  // printf("process #%d, positoin %d\n", PROC_RANK, 4);

  // if (PROC_RANK == 0) {
  //   free(proc_cols);
  //   free(vector_b);
  // }

  // readSlae(argv[2], size, &proc_cols, &proc_x, &vector_b);
  double residual_norm = getResidualNorm(size, cols_num, proc_cols, vector_res, vector_b);
  printf("process #%d, positoin %d\n", PROC_RANK, 11);
  double solution_diff = getSolutionDiff(size, proc_x, vector_res);
  printf("process #%d, positoin %d\n", PROC_RANK, 12);

  if (PROC_RANK == 0) {
    printf("T1, T2, точность, норма невязки:\n\n");
    printf("%lf\n", T1);
    printf("%lf\n", T2);
    printf("%g\n", solution_diff);
    printf("%g\n", residual_norm);
  }

  MPI_Barrier(MPI_COMM_WORLD);
  // mpifree(size, proc_cols, proc_x, vector_b, proc_cols, vector_res);
  MPI_Finalize();
  return 0;
}