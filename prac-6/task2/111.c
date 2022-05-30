void getUpperTriangularIteration(int size, int k, int proc, int cols_num, double *proc_cols, double *vector_b, MPI_Comm comm) {
  int vector_b_proc = size % proc_num ? (size % proc_num) - 1 : proc_num - 1;
  int stride = k * proc_num;
  int tmp_size = size - stride - proc;
  double *tmp = (double *)malloc(tmp_size * sizeof(double));

  if (proc_rank == proc) {
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
    tmp[0] += sqrt(norm_a_i);

    // новый способ
    tmp[0] += sqrt(norm_a_i);
    double w0w0 = pow(tmp[0], 2);
    double tmp_norm = sqrt(norm_a_i - a0a0 + w0w0);

    printf("w norm old = %lf\n", tmp_norm_old);
    printf("w norm new = %lf\n", tmp_norm);
    printf("a norm = %lf\n", sqrt(norm_a_i));
    printf("a0a0 = %lf\n", a0a0);
    printf("w0w0 = %lf\n\n", w0w0);

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

  if (PROC_RANK == vector_b_proc) {
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
