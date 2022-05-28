#include <assert.h>
#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int PROCESS_NUM;
int PROCESS_RANK;

void read_matrix(FILE *output, int size, double *p_matrix) {
  int readed_object_count = 0;
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      double num;
      readed_object_count = fread(&num, sizeof(double), 1, output);
      assert(readed_object_count == 1 && "Error read element in file");
      // printf("%lf ", num);
      p_matrix[j * size + i] = (double)num;
    }
    // printf("\n");
  }
}

void read_vector(FILE *output, int size, double *p_vector) {
  int readed_object_count;
  for (int i = 0; i < size; ++i) {
    double num;
    readed_object_count = fread(&num, sizeof(double), 1, output);
    assert(readed_object_count == 1 && "Error read element in file");
    p_vector[i] = (double)num;
  }
}

void initialize_slae(char *filename, int size, double **matrix_a, double **vector_x, double **vector_b) {
  if (PROCESS_RANK == 0) {
    *vector_x = (double *)malloc(size * sizeof(double));
    assert(*vector_x != NULL && "Malloc error");
    *vector_b = (double *)malloc(size * sizeof(double));
    assert(*vector_b != NULL && "Malloc error");
    *matrix_a = (double *)malloc(size * size * sizeof(double));
    assert(*matrix_a != NULL && "Malloc error");
    FILE *input = fopen(filename, "rb");
    if (!input) {
      printf("Error opening file \"%s\"\n", filename);
      free(*matrix_a);
      free(*vector_x);
      free(*vector_b);
      exit(1);
    }
    read_vector(input, size, *vector_x);
    read_matrix(input, size, *matrix_a);
    read_vector(input, size, *vector_b);
  }
}

void visualize_matrix(int size, int columns_count, double *process_columns) {
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < PROCESS_NUM; ++j) {
      if (j == PROCESS_RANK) {
        for (int k = 0; k < columns_count; ++k) {
          printf("%lf ", process_columns[k * size + i]);
          fflush(stdout);
        }
      }
    }
    printf("\n");
    fflush(stdout);
  }
}

void visualize_vector(int size, double *vector) {
  for (int j = 0; j < PROCESS_NUM; ++j) {
    if (j == PROCESS_RANK) {
      printf("PROCESS_RANK = %d;\n", PROCESS_RANK);
      for (int k = 0; k < size; ++k) {
        printf("%lf ", vector[k]);
        fflush(stdout);
      }
      printf("\nend\n");
      fflush(stdout);
    }
  }
}

void distribute_matrix(int size, double *matrix_a, double **vector_b, int *columns_count, double **process_columns) {
  *columns_count = size / PROCESS_NUM;

  if (PROCESS_RANK < size % PROCESS_NUM) {
    *process_columns = (double *)malloc((*columns_count + 1) * size * sizeof(double));
    assert(*process_columns != NULL && "Malloc error");
  } else {
    *process_columns = (double *)malloc((*columns_count) * size * sizeof(double));
    assert(*process_columns != NULL && "Malloc error");
  }

  for (int i = 0; i < (*columns_count); ++i) {
    MPI_Scatter(matrix_a + i * PROCESS_NUM * size, size, MPI_DOUBLE, (*process_columns) + i * size, size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

  MPI_Comm new_comm;
  MPI_Comm_split(MPI_COMM_WORLD, PROCESS_RANK < size % PROCESS_NUM, PROCESS_RANK, &new_comm);
  if (PROCESS_RANK < size % PROCESS_NUM) {
    MPI_Scatter(matrix_a + PROCESS_NUM * (*columns_count) * size, size, MPI_DOUBLE, (*process_columns) + (*columns_count) * size, size, MPI_DOUBLE, 0, new_comm);
    (*columns_count) += 1;
  }
  MPI_Comm_free(&new_comm);

  if (size % PROCESS_NUM != 1 && PROCESS_NUM != 1) {
    int reciver_id = size % PROCESS_NUM ? (size % PROCESS_NUM) - 1 : PROCESS_NUM - 1;
    if (PROCESS_RANK == 0) {
      MPI_Send(*vector_b, size, MPI_DOUBLE, reciver_id, 0, MPI_COMM_WORLD);
    }
    if (PROCESS_RANK == reciver_id) {
      *vector_b = (double *)malloc(size * sizeof(double));
      assert(*vector_b != NULL && "Malloc error");
      MPI_Recv(*vector_b, size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
  }
}

void get_upper_triangular_form_iteration(int size, int k, int process, int columns_count, double *process_columns, double *vector_b, MPI_Comm comm) {
  int vector_b_process = size % PROCESS_NUM ? (size % PROCESS_NUM) - 1 : PROCESS_NUM - 1;
  int stride = k * PROCESS_NUM;
  double *vector_w = (double *)malloc((size - stride - process) * sizeof(double));
  if (PROCESS_RANK == process) {
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

  if (PROCESS_RANK == vector_b_process) {
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
  for (int k = 0; k < size / PROCESS_NUM; ++k) {
    for (int process = 0; process < PROCESS_NUM; ++process) {
      if (k * PROCESS_NUM + process == size - 1) {
        break;
      }
      get_upper_triangular_form_iteration(size, k, process, columns_count, process_columns, vector_b, MPI_COMM_WORLD);
    }
  }
  MPI_Comm new_comm;
  MPI_Comm_split(MPI_COMM_WORLD, PROCESS_RANK < size % PROCESS_NUM, PROCESS_RANK, &new_comm);
  if (PROCESS_RANK < size % PROCESS_NUM) {
    for (int process = 0; process < size % PROCESS_NUM - 1; ++process) {
      get_upper_triangular_form_iteration(size, (size / PROCESS_NUM), process, columns_count, process_columns, vector_b, new_comm);
    }
  }
  MPI_Comm_free(&new_comm);
}

void calculate_reverse_gauss_method_iteration(int size, int k, int process, int columns_count, double *process_columns, double *vector_b, double *vector_x, MPI_Comm comm) {
  int vector_b_process = size % PROCESS_NUM ? (size % PROCESS_NUM) - 1 : PROCESS_NUM - 1;
  double row_sum = 0.0;
  double process_row_sum = 0.0;
  int stride = k * PROCESS_NUM;
  for (int i = k; i < columns_count; ++i) {
    process_row_sum += vector_x[i] * process_columns[i * size + stride + process];
  }

  MPI_Reduce(&process_row_sum, &row_sum, 1, MPI_DOUBLE, MPI_SUM, process, comm);
  double a = process_columns[k * size + stride + process];
  if (process != vector_b_process) {
    if (PROCESS_RANK == vector_b_process) {
      MPI_Send(&(vector_b[stride + process]), 1, MPI_DOUBLE, process, 0, comm);
    }
    if (PROCESS_RANK == process) {
      double tmp_b_i = 0.0;
      MPI_Recv(&tmp_b_i, 1, MPI_DOUBLE, vector_b_process, 0, comm, MPI_STATUS_IGNORE);
      vector_x[k] = a != 0.0 ? (tmp_b_i - row_sum) / a : 0;
    }
  } else {
    if (PROCESS_RANK == process) {
      vector_x[k] = a != 0.0 ? (vector_b[stride + process] - row_sum) / a : 0;
    }
  }
}

void calculate_reverse_gauss_method(int size, int columns_count, double *process_columns, double *vector_b, double *vector_x) {
  MPI_Comm new_comm;
  MPI_Comm_split(MPI_COMM_WORLD, PROCESS_RANK < size % PROCESS_NUM, PROCESS_RANK, &new_comm);
  if (PROCESS_RANK < size % PROCESS_NUM) {
    for (int process = size % PROCESS_NUM - 1; process >= 0; --process) {
      calculate_reverse_gauss_method_iteration(size, (size / PROCESS_NUM), process, columns_count, process_columns, vector_b, vector_x, new_comm);
    }
  }

  MPI_Comm_free(&new_comm);
  for (int k = size / PROCESS_NUM - 1; k >= 0; --k) {
    for (int process = PROCESS_NUM - 1; process >= 0; --process) {
      calculate_reverse_gauss_method_iteration(size, k, process, columns_count, process_columns, vector_b, vector_x, MPI_COMM_WORLD);
    }
  }
}

double get_slae_residual_norm(int size, double *matrix_a, double *vector_x, double *vector_b) {
  double *vector_result;
  if (PROCESS_RANK == 0) {
    vector_result = (double *)malloc(size * sizeof(double));
    assert(vector_result != NULL && "Malloc error");
  }

  int columns_count = size / PROCESS_NUM;
  for (int i = 0; i < columns_count; ++i) {
    MPI_Gather(vector_x + i, 1, MPI_DOUBLE, vector_result + i * PROCESS_NUM, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

  MPI_Comm new_comm;
  MPI_Comm_split(MPI_COMM_WORLD, PROCESS_RANK < size % PROCESS_NUM, PROCESS_RANK, &new_comm);
  if (PROCESS_RANK < size % PROCESS_NUM) {
    MPI_Gather(vector_x + columns_count, 1, MPI_DOUBLE, vector_result + PROCESS_NUM * columns_count, 1, MPI_DOUBLE, 0, new_comm);
  }
  MPI_Comm_free(&new_comm);

  double norm = 0;
  if (PROCESS_RANK == 0) {
    for (int i = 0; i < size; ++i) {
      double row_sum = 0;
      for (int j = 0; j < size; ++j) {
        row_sum += matrix_a[j * size + i] * vector_result[j];
      }
      double diff = row_sum - vector_b[i];
      norm += diff * diff;
    }
    free(vector_result);
  }
  MPI_Bcast(&norm, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  return sqrt(norm);
}

double get_norm_of_solution_difference(int size, double *vector_x, double *vector_x_new) {
  double *vector_result;
  if (PROCESS_RANK == 0) {
    vector_result = (double *)malloc(size * sizeof(double));
    assert(vector_result != NULL && "Malloc error");
  }

  int columns_count = size / PROCESS_NUM;
  for (int i = 0; i < columns_count; ++i) {
    MPI_Gather(vector_x_new + i, 1, MPI_DOUBLE, vector_result + i * PROCESS_NUM, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

  MPI_Comm new_comm;
  MPI_Comm_split(MPI_COMM_WORLD, PROCESS_RANK < size % PROCESS_NUM, PROCESS_RANK, &new_comm);
  if (PROCESS_RANK < size % PROCESS_NUM) {
    MPI_Gather(vector_x_new + columns_count, 1, MPI_DOUBLE, vector_result + PROCESS_NUM * columns_count, 1, MPI_DOUBLE, 0, new_comm);
  }
  MPI_Comm_free(&new_comm);

  // if (PROCESS_RANK == 0) {
  //   printf("RESULT\n");
  //   visualize_vector(size, vector_result);
  // }
  // MPI_Barrier(MPI_COMM_WORLD);
  double norm = 0.0;
  if (PROCESS_RANK == 0) {
    for (int i = 0; i < size; ++i) {
      double diff = vector_x[i] - vector_result[i];
      norm += diff * diff;
    }
    free(vector_result);
  }
  // printf("Norm of solution difference: %g\n", sqrt(norm));
  // fflush(stdout);
  MPI_Bcast(&norm, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  // MPI_Barrier(MPI_COMM_WORLD);
  return sqrt(norm);
}

void terminate(int size, double *matrix_a, double *vector_x, double *vector_b, double *process_columns, double *vector_result) {
  if (PROCESS_RANK == 0) {
    free(matrix_a);
    free(vector_x);
    free(vector_b);
  }
  free(process_columns);
  free(vector_result);
  if (size % PROCESS_NUM != 1 && PROCESS_NUM != 1) {
    int reciver_id = size % PROCESS_NUM ? (size % PROCESS_NUM) - 1 : PROCESS_NUM - 1;
    if (PROCESS_RANK == reciver_id) {
      free(vector_b);
    }
  }
}

int main(int argc, char **argv) {
  if (argc != 3) {
    if (PROCESS_RANK == 0) {
      printf("Invalid number of command line arguments\n");
    }
    exit(1);
  }
  int error;
  error = MPI_Init(&argc, &argv);
  assert(error == MPI_SUCCESS && "MPI startup error!");
  error = MPI_Comm_size(MPI_COMM_WORLD, &PROCESS_NUM);
  assert(error == MPI_SUCCESS && "MPI_Comm_size error!");
  error = MPI_Comm_rank(MPI_COMM_WORLD, &PROCESS_RANK);
  assert(error == MPI_SUCCESS && "MPI_Comm_rank error!");

  int size = (int)atoi(argv[1]);
  double *matrix_a;
  double *vector_b;
  double *vector_x;
  int columns_count = size;
  double *process_columns;
  double *vector_result;

  initialize_slae(argv[2], size, &matrix_a, &vector_x, &vector_b);
  if (PROCESS_RANK == 0) {
    //   printf("INITIAL SLAE\n");
    //     for (int i = 0; i < size; ++i) {
    //       for (int j = 0; j < size; ++j) {
    //         printf("%lf ", matrix_a[i * size + j]);
    //       }
    //       printf("\n");
    //     }
    free(vector_x);
  }

  double timer_start, timer_stop;
  timer_start = MPI_Wtime();
  distribute_matrix(size, matrix_a, &vector_b, &columns_count, &process_columns);
  // if (PROCESS_RANK == 0) {
  //   printf("SLAE PROCESS_RANK == 0\n");
  //     for (int i = 0; i < columns_count; ++i) {
  //       for (int j = 0; j < size; ++j) {
  //         printf("%lf ", process_columns[i * size + j]);
  //       }
  //       printf("\n");
  //     }
  // }MPI_Barrier(MPI_COMM_WORLD);
  // if (PROCESS_RANK == 1) {
  //   printf("SLAE PROCESS_RANK == 1\n");
  //     for (int i = 0; i < columns_count; ++i) {
  //       for (int j = 0; j < size; ++j) {
  //         printf("%lf ", process_columns[i * size + j]);
  //       }
  //       printf("\n");
  //     }
  // }MPI_Barrier(MPI_COMM_WORLD);
  get_upper_triangular_form(size, columns_count, process_columns, vector_b);

  timer_stop = MPI_Wtime();
  double time_to_get_upper_triangular_form = timer_stop - timer_start;

  vector_result = (double *)malloc(columns_count * sizeof(double));
  assert(vector_result != NULL && "Malloc error");
  for (int i = 0; i < columns_count; ++i) {
    vector_result[i] = 0.0;
  }
  timer_start = MPI_Wtime();
  calculate_reverse_gauss_method(size, columns_count, process_columns, vector_b, vector_result);
  timer_stop = MPI_Wtime();
  double time_to_calculate_reverse_gauss_method = timer_stop - timer_start;

  if (PROCESS_RANK == 0) {
    free(matrix_a);
    free(vector_b);
  }

  initialize_slae(argv[2], size, &matrix_a, &vector_x, &vector_b);

  double slae_residual_norm = get_slae_residual_norm(size, matrix_a, vector_result, vector_b);

  double norm_of_solution_difference = get_norm_of_solution_difference(size, vector_x, vector_result);

  if (PROCESS_RANK == 0) {
    printf("Time to get upper triangular form (T1): %lf\n", time_to_get_upper_triangular_form);
    printf("Time to calculate reverse gauss method (T2): %lf\n", time_to_calculate_reverse_gauss_method);
    printf("Norm of solution difference: %g\n", norm_of_solution_difference);
    printf("Slae residual norm: %g\n", slae_residual_norm);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  terminate(size, matrix_a, vector_x, vector_b, process_columns, vector_result);
  MPI_Finalize();
  return 0;
}