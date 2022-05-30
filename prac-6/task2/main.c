#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int PROC_NUM;
int PROC_RANK;

#define PROC_WITH_B (size % PROC_NUM ? (size % PROC_NUM) - 1 : PROC_NUM - 1)

void get_upper_triangular_form_iteration(int size, int k, int process, int columns_count, double *process_columns, double *vector_b, MPI_Comm comm) {
  // int vector_b_process =
  //     size % PROC_NUM ? (size % PROC_NUM) - 1 : PROC_NUM - 1;
  int stride = k * PROC_NUM;
  double *vector_w = (double *)malloc((size - stride - process) * sizeof(double));
  if (PROC_RANK == process) {
    double norm_a_i = 0.0;
    for (int i = 0; i < size - stride - process; ++i) {
      double a = process_columns[(k * size) + (stride + process) + i];
      vector_w[i] = a;
      norm_a_i += a * a;
    }
    norm_a_i = sqrt(norm_a_i);
    vector_w[0] -= norm_a_i;
    double norm_w = 0.0;
    for (int i = 0; i < size - stride - process; ++i) {
      double w = vector_w[i];
      norm_w += w * w;
    }
    norm_w = sqrt(norm_w);
    if (norm_w != 0.0) {
      for (int i = 0; i < size - stride - process; ++i) {
        vector_w[i] /= norm_w;
      }
    } else {
      for (int i = 0; i < size - stride - process; ++i) {
        vector_w[i] = 0.0;
      }
    }
  }
  MPI_Bcast(vector_w, size - stride - process, MPI_DOUBLE, process, comm);
  double *vector_w_a = (double *)malloc((columns_count - k) * sizeof(double));
  for (int i = 0; i < columns_count - k; ++i) {
    vector_w_a[i] = 0.0;
    for (int j = 0; j < size - stride - process; ++j) {
      vector_w_a[i] += vector_w[j] * process_columns[(k * size) + i * size + (j + stride + process)];
    }
  }

  for (int i = 0; i < size - stride - process; ++i) {
    for (int j = 0; j < columns_count - k; ++j) {
      process_columns[(k * size) + j * size + (i + stride + process)] -= 2 * vector_w[i] * vector_w_a[j];
    }
  }

  if (PROC_RANK == PROC_WITH_B) {
    double mult_w_b = 0.0;
    for (int i = 0; i < size - stride - process; ++i) {
      mult_w_b += vector_w[i] * vector_b[i + stride + process];
    }

    for (int i = 0; i < size - stride - process; ++i) {
      vector_b[i + stride + process] -= 2 * vector_w[i] * mult_w_b;
    }
  }
  free(vector_w_a);
  free(vector_w);
}

void get_upper_triangular_form(int size, int columns_count, double *process_columns, double *vector_b) {
  for (int k = 0; k < size / PROC_NUM; ++k) {
    for (int process = 0; process < PROC_NUM; ++process) {
      if (k * PROC_NUM + process == size - 1) {
        break;
      }
      get_upper_triangular_form_iteration(size, k, process, columns_count, process_columns, vector_b, MPI_COMM_WORLD);
    }
  }
  MPI_Comm new_comm;
  MPI_Comm_split(MPI_COMM_WORLD, PROC_RANK < size % PROC_NUM, PROC_RANK, &new_comm);
  if (PROC_RANK < size % PROC_NUM) {
    for (int process = 0; process < size % PROC_NUM - 1; ++process) {
      get_upper_triangular_form_iteration(size, (size / PROC_NUM), process, columns_count, process_columns, vector_b, new_comm);
    }
  }
  MPI_Comm_free(&new_comm);
}

void calculate_reverse_gauss_method_iteration(int size, int k, int process, int columns_count, double *process_columns, double *vector_b, double *vector_x, MPI_Comm comm) {
  int vector_b_process = size % PROC_NUM ? (size % PROC_NUM) - 1 : PROC_NUM - 1;
  double row_sum = 0.0;
  double process_row_sum = 0.0;
  int stride = k * PROC_NUM;
  for (int i = k; i < columns_count; ++i) {
    process_row_sum += vector_x[i] * process_columns[i * size + stride + process];
  }

  MPI_Reduce(&process_row_sum, &row_sum, 1, MPI_DOUBLE, MPI_SUM, process, comm);
  double a = process_columns[k * size + stride + process];
  if (process != vector_b_process) {
    if (PROC_RANK == vector_b_process) {
      MPI_Send(&(vector_b[stride + process]), 1, MPI_DOUBLE, process, 0, comm);
    }
    if (PROC_RANK == process) {
      double tmp_b_i = 0.0;
      MPI_Recv(&tmp_b_i, 1, MPI_DOUBLE, vector_b_process, 0, comm, MPI_STATUS_IGNORE);
      vector_x[k] = a != 0.0 ? (tmp_b_i - row_sum) / a : 0;
    }
  } else {
    if (PROC_RANK == process) {
      vector_x[k] = a != 0.0 ? (vector_b[stride + process] - row_sum) / a : 0;
    }
  }
}

void calculate_reverse_gauss_method(int size, int columns_count, double *process_columns, double *vector_b, double *vector_x) {
  MPI_Comm new_comm;
  MPI_Comm_split(MPI_COMM_WORLD, PROC_RANK < size % PROC_NUM, PROC_RANK, &new_comm);
  if (PROC_RANK < size % PROC_NUM) {
    for (int process = size % PROC_NUM - 1; process >= 0; --process) {
      calculate_reverse_gauss_method_iteration(size, (size / PROC_NUM), process, columns_count, process_columns, vector_b, vector_x, new_comm);
    }
  }
  MPI_Comm_free(&new_comm);
  for (int k = size / PROC_NUM - 1; k >= 0; --k) {
    for (int process = PROC_NUM - 1; process >= 0; --process) {
      calculate_reverse_gauss_method_iteration(size, k, process, columns_count, process_columns, vector_b, vector_x, MPI_COMM_WORLD);
    }
  }
}

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
    for (int j = 0; j < size; ++j) {
      double num = (double)((rand() + PROC_RANK) % 200);
      (*proc_cols)[i * size + j] = num;
    }
  }
  MPI_Barrier(MPI_COMM_WORLD);

  // считаем кусочек вектора b
  for (int i = 0; i < (*cols_num); i++) {
    for (int j = 0; j < size; ++j) {
      proc_b[j] += (*proc_cols)[i * size + j] * (*proc_x)[i];
    }
  }

  // Собираем вектор b
  MPI_Reduce(proc_b, *vector_b, size, MPI_DOUBLE, MPI_SUM, PROC_WITH_B, MPI_COMM_WORLD);
  free(proc_b);
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

  double *vector_b_new;
  // if (PROC_RANK == PROC_WITH_B)
  vector_b_new = (double *)malloc(size * sizeof(double));

  MPI_Reduce(proc_b, vector_b_new, size, MPI_DOUBLE, MPI_SUM, PROC_WITH_B, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  free(proc_b);

  double norm = 0.0;
  if (PROC_RANK == PROC_WITH_B) {
    printf("\n[[");
    for (int i = 0; i < size; i++) {
      norm += pow(vector_b_new[i] - vector_b[i], 2);
      printf(" %lf ", vector_b_new[i]);
    }
    printf("]]\n");
    printf("\n<< %lf >>\n", norm);
  }

  MPI_Bcast(&norm, 1, MPI_DOUBLE, PROC_WITH_B, MPI_COMM_WORLD);
  if (PROC_RANK == PROC_WITH_B) {
    free(vector_b_new);
  }
  return sqrt(norm);
}

double getSolutionDiff(int cols_num, double *vector_x, double *vector_x_new) {
  // double *vector_res;
  // int cols_num = size / PROC_NUM;
  // if (PROC_RANK == 0) vector_res = (double *)malloc(size * sizeof(double));

  // for (int i = 0; i < cols_num; i++) {
  //   MPI_Gather(vector_x_new + i, 1, MPI_DOUBLE, vector_res + i * PROC_NUM, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  // }

  // MPI_Comm new_comm;
  // MPI_Comm_split(MPI_COMM_WORLD, PROC_RANK < size % PROC_NUM, PROC_RANK, &new_comm);
  // if (PROC_RANK < size % PROC_NUM) MPI_Gather(vector_x_new + cols_num, 1, MPI_DOUBLE, vector_res + PROC_NUM * cols_num, 1, MPI_DOUBLE, 0, new_comm);
  // MPI_Comm_free(&new_comm);

  // double norm = 0.0;
  // if (PROC_RANK == 0) {
  //   for (int i = 0; i < size; i++) {
  //     norm += pow(vector_x[i] - vector_res[i], 2);
  //   }
  //   free(vector_res);
  // }
  double res_norm = 0.0;
  double norm = 0.0;
  for (int i = 0; i < cols_num; i++) {
    norm += pow(vector_x[i] - vector_x_new[i], 2);
  }
  MPI_Reduce(&norm, &res_norm, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Bcast(&res_norm, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  return sqrt(norm);
}

void mpifree(int size, double *vector_x, double *vector_b, double *proc_cols, double *vector_res) {
  // proc_cols, proc_x, vector_b, proc_cols, vector_res
  free(vector_x);
  free(proc_cols);
  free(vector_res);
  if (size % PROC_NUM != 1 && PROC_NUM != 1) {
    // int reciver_id = size % PROC_NUM ? (size % PROC_NUM) - 1 : PROC_NUM - 1;
    if (PROC_RANK == PROC_WITH_B) free(vector_b);
  }
}

int main(int argc, char **argv) {
  int size = (int)atoi(argv[1]);
  // size = 5;
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

  // srand(42);
  srand((unsigned int)time(NULL) + PROC_RANK);
  genSlae(size, &cols_num, &proc_cols, &proc_x, &vector_b);

  // for (int j = 0; j < PROC_NUM; j++) {
  //   if (PROC_RANK == j) {
  //     printf("PROC_RANK = %d, cols_num size = %d\n", PROC_RANK, cols_num * size);
  //   printf("Матрица:\n");
  //   fflush(stdout);
  //   for (int k = 0; k < cols_num; k++) {
  //     for (int i = 0; i < size; i++) {
  //       printf("%lf  ", proc_cols[k*size + i]);
  //       fflush(stdout);
  //     }
  //     printf("\n");
  //     fflush(stdout);
  //   }
  //   printf("Вектор x:\n");
  //   fflush(stdout);
  //   for (int k = 0; k < cols_num; k++) {
  //     printf("%lf  ", proc_x[k]);
  //     fflush(stdout);
  //   }
  //   printf("\n");
  //   fflush(stdout);
  //   }
  //   MPI_Barrier(MPI_COMM_WORLD);
  // }
  // MPI_Barrier(MPI_COMM_WORLD);
  // if (PROC_RANK == PROC_WITH_B) {
  //   printf("\nВектор B:\n");
  //   fflush(stdout);
  //   for (int k = 0; k < size; k++) {
  //     printf("%lf  ", vector_b[k]);
  //     fflush(stdout);
  //   }
  //   printf("\n");
  //   fflush(stdout);
  // }
  // MPI_Barrier(MPI_COMM_WORLD);

  // for (int i = 0; i < size; i++) {
  //   // int k = 0;
  //   // while (k < cols_num) {
  //   for (int k = 0; k < cols_num; k++) {
  //     for (int j = 0; j < PROC_NUM; j++) {
  //       if (PROC_RANK == j && k < cols_num) {
  //         printf("%lf  ", proc_cols[k*size + i]);
  //         fflush(stdout);
  //       }
  //       MPI_Barrier(MPI_COMM_WORLD);
  //     }
  //   }
  //   MPI_Barrier(MPI_COMM_WORLD);
  //   if (PROC_RANK == 0) {
  //     printf("\n");
  //     fflush(stdout);
  //   }
  //   MPI_Barrier(MPI_COMM_WORLD);
  // }
  // MPI_Barrier(MPI_COMM_WORLD);

  // printf("process #%d, positoin %d\n", PROC_RANK, 1);

  // ------------------------------
  // MPI_Barrier(MPI_COMM_WORLD);
  // // printf("[");
  // // for (int col = 0; col < cols_num; col++) {
  // //   for (int i = 0; i < size; i++) {
  // //     printf(" %lf ", proc_cols[col * size + i]);
  // //   }
  // // }
  // // printf("]\n");
  // // MPI_Barrier(MPI_COMM_WORLD);
  // // if (PROC_RANK == PROC_WITH_B) {
  // //   printf("\n\nВектор b: <");
  // //   for (int i = 0; i < size; i++) {
  // //     printf(" %lf ", vector_b[i]);
  // //   }
  // //   printf(">\n");
  // // }
  // // MPI_Barrier(MPI_COMM_WORLD);
  // // ------------------------------

  timer_start = MPI_Wtime();
  get_upper_triangular_form(size, cols_num, proc_cols, vector_b);
  T1 = MPI_Wtime() - timer_start;
  // for (int j = 0; j < PROC_NUM; j++) {
  //   if (PROC_RANK == j) {
  //     printf("PROC_RANK = %d, cols_num size = %d\n", PROC_RANK, cols_num * size);
  //   printf("Матрица:\n");
  //   fflush(stdout);
  //   for (int k = 0; k < cols_num; k++) {
  //     for (int i = 0; i < size; i++) {
  //       printf("%lf  ", proc_cols[k*size + i]);
  //       fflush(stdout);
  //     }
  //     printf("\n");
  //     fflush(stdout);
  //   }
  //   printf("Вектор x:\n");
  //   fflush(stdout);
  //   for (int k = 0; k < cols_num; k++) {
  //     printf("%lf  ", proc_x[k]);
  //     fflush(stdout);
  //   }
  //   printf("\n");
  //   fflush(stdout);
  //   }
  //   MPI_Barrier(MPI_COMM_WORLD);
  // }
  // MPI_Barrier(MPI_COMM_WORLD);
  // if (PROC_RANK == PROC_WITH_B) {
  //   printf("\nВектор B:\n");
  //   fflush(stdout);
  //   for (int k = 0; k < size; k++) {
  //     printf("%lf  ", vector_b[k]);
  //     fflush(stdout);
  //   }
  //   printf("\n");
  //   fflush(stdout);
  // }
  // MPI_Barrier(MPI_COMM_WORLD);

  // printf("process #%d, positoin %d\n", PROC_RANK, 2);

  vector_res = (double *)malloc(cols_num * sizeof(double));
  for (int i = 0; i < cols_num; i++) {
    vector_res[i] = 0;
  }

  // printf("process #%d, positoin %d\n", PROC_RANK, 3);

  timer_start = MPI_Wtime();
  calculate_reverse_gauss_method(size, cols_num, proc_cols, vector_b, vector_res);
  T2 = MPI_Wtime() - timer_start;

  // printf("process #%d, positoin %d\n", PROC_RANK, 4);

  double residual_norm = getResidualNorm(size, cols_num, proc_cols, vector_res, vector_b);
  // // printf("process #%d, positoin %d\n", PROC_RANK, 11);
  double solution_diff = getSolutionDiff(cols_num, proc_x, vector_res);
  // // printf("process #%d, positoin %d\n", PROC_RANK, 12);

  if (PROC_RANK == 0) {
    printf("T1, T2, точность, норма невязки:\n\n");
    printf("%lf\n", T1);
    printf("%lf\n", T2);
    printf("%g\n", solution_diff);
    printf("%g\n", residual_norm);
  }

  MPI_Barrier(MPI_COMM_WORLD);
  mpifree(size, proc_x, vector_b, proc_cols, vector_res);
  MPI_Finalize();
  return 0;
}
