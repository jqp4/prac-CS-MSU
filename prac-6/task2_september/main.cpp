#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <mpi.h>
#include <string>
#include <sys/time.h>

double double_rand(double fMin, double fMax) {
  double f = (double)rand() / RAND_MAX;
  return fMin + f * (fMax - fMin);
}

/// Функция генерации матрицы по процессам.
void init_matrix(double *matrix, int n, int k, int threads_count, int thread_num, double *vector_b, int *sendcounts, int cols) {
  double *buf, *newbuf;
  buf = new double[n];
  newbuf = new double[n];

  int *displs = new int[threads_count];
  for (int i = 0; i < n; ++i) {
    buf[i] = 0;
  }

  for (int i = 0; i < n; ++i) {
    if (thread_num == 0) {

      vector_b[i] = 0;
      for (int j = 0; j < n; ++j) {
        buf[j] = double_rand(-100, 100);
        vector_b[i] += buf[j] * (j % 2);
      }
      for (int j = 0; j < n; ++j) {
        int index1 = j % threads_count;
        int index2 = j / threads_count;
        int shift = index1 < n % threads_count ? index1 : n % threads_count;
        newbuf[index1 * (n / threads_count) + index2 + shift] = buf[j];
        // printf("%d -> %d\n",j, index1*threads_count + index2);
      }
    }

    displs[0] = 0;
    for (int l = 1; l < threads_count; l++) {
      displs[l] = displs[l - 1] + sendcounts[l - 1];
    }
    MPI_Scatterv(newbuf, sendcounts, displs, MPI_DOUBLE, matrix + cols * i, cols, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

  delete[] buf;
  delete[] newbuf;
  delete[] displs;
}

void print_matrix(double *matrix, int n, int m, int threads_count, int thread_num, int *sendcounts, int cols) {
  int *displs = new int[threads_count];
  double *buf = new double[n];
  double *newbuf = new double[n];
  displs[0] = 0;
  for (int i = 1; i < threads_count; i++)
    displs[i] = displs[i - 1] + sendcounts[i - 1];
  for (int i = 0; i < m; i++) {
    MPI_Gatherv(matrix + i * cols, cols, MPI_DOUBLE, buf, sendcounts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if (thread_num == 0) {
      for (int j = 0; j < n; ++j) {
        int index1 = j % threads_count;
        int index2 = j / threads_count;
        int shift = index1 < n % threads_count ? index1 : n % threads_count;
        newbuf[j] = buf[index1 * (n / threads_count) + index2 + shift];
      }
      for (int j = 0; j < n && j < m; j++)
        printf(" %10.3e", newbuf[j]);
      printf("\n");
    }
  }
  delete[] displs;
  delete[] buf;
  delete[] newbuf;
}

/// Вычисление нормы невязки
double calculate_residual_norm(double *a, double *x, double *b, int n, int threads_count, int thread_num, int *sendcounts) {
  long double top = 0.0, bottom = 0.0;
  int cols;
  if (thread_num + 1 > n % threads_count) cols = n / threads_count;
  else cols = n / threads_count + 1;
  long double glob_elem_of_ax_minus_b = 0;
  for (int i = 0; i < n; ++i) {
    long double elem_of_ax_minus_b = 0;
    for (int j = 0; j < cols; ++j) {
      elem_of_ax_minus_b += a[i * cols + j] * x[thread_num + threads_count * j];
    }
    glob_elem_of_ax_minus_b = 0;
    MPI_Reduce(&elem_of_ax_minus_b, &glob_elem_of_ax_minus_b, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    glob_elem_of_ax_minus_b -= b[i];
    top += glob_elem_of_ax_minus_b * glob_elem_of_ax_minus_b;
  }
  if (thread_num == 0)
    for (int i = 0; i < n; ++i) {
      bottom += b[i] * b[i];
    }

  top = sqrt(top);
  bottom = sqrt(bottom);
  MPI_Bcast(&bottom, 1, MPI_LONG_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&top, 1, MPI_LONG_DOUBLE, 0, MPI_COMM_WORLD);
  // if(thread_num == 0)
  //     printf("    %Lf %Lf\n", top, bottom);
  return (double)top / bottom;
}

void print_answer(double *x, int n, int m) {
  for (int i = 0; i < n && i < m; i++)
    printf(" %10.3e", x[i]);
  printf("\n");
}

double get_time() {
  struct timeval t;
  gettimeofday(&t, 0);
  return (t.tv_sec + t.tv_usec / 1000000.0) * 1000;
}

/// Вычисление точности решения
double calcualte_soulution_error(double *x, int n) {
  long double sum_of_diffs = 0.0;

  for (int i = 0; i < n; ++i) {
    double t = x[i] - (i % 2);
    sum_of_diffs += t * t;
  }

  return std::sqrt(sum_of_diffs);
}

// main solver
void solve_slae_by_reflection(double *matrix, double *b, double *x, int *sendcounts, int n, int thread_num, int threads_count, double *t1, double *t2) {
  // MPI_Barrier(MPI_COMM_WORLD);
  double t1_local = get_time();
  int cols; // кол-во столбцов для каждого процессе
  double norm_ak = 0, norm_xk = 0;
  double *xk = new double[n];
  if (thread_num + 1 > n % threads_count) cols = n / threads_count;
  else cols = n / threads_count + 1;

  // считаем R = U^(k) * U^(k-1) * ... * U^(1) * A
  //         y = U^(k) * U^(k-1) * ... * U^(1) * b
  for (int k = 0; k < n; ++k) {
    // k - глоабльный номер столбца по матрице
    // root - локальный номер столбца в разбиении
    // kk - номер разбиения

    int root = 0, kk = 0;
    root = k % threads_count;
    kk = k / threads_count;

    // далее считаем только в одном процессе
    if (thread_num == root) {
      // t - глобальный номер столбца (оптимизация с кропом матрицы)
      for (int t = k; t < n; t++)
        xk[t] = matrix[t * cols + kk]; // заполняем 1 столбец (с ведущими нулями для оптимизации)

      // считаем норму вектора столбца (в отчете: w^(k))
      norm_ak = 0;
      for (int i = k; i < n; ++i)
        norm_ak += xk[i] * xk[i];
      norm_ak = std::sqrt(norm_ak);

      // вычитаем из прервого значимого элемента столбца эту норму
      xk[k] -= norm_ak;
      norm_xk = 0;

      for (int i = k; i < n; ++i)
        norm_xk += xk[i] * xk[i];
      norm_xk = std::sqrt(norm_xk);
      for (int t = k; t < n; t++)
        xk[t] /= norm_xk;

      // теперь мы посчитали w^(k) = xk длины n
    }

    // рассылаем xk = w^(k) всем отсальным процессам
    // отсылаем только элементы начиная с k, потому что первые эелемены нули
    MPI_Bcast(&xk[k], n - k, MPI_DOUBLE, root, MPI_COMM_WORLD);

    // теперь перемножим U^(k) на А (в очередной раз) в отдельных процессах по несколько столбцов на проццесс.
    // однако U явно считать не будем, перемножим сразу на w
    // считаем от 0 до количества столбцов выделенных для данного процесса
    for (int i = 0; i < sendcounts[thread_num]; i++)
      // какое то условие пока не пон
      if (thread_num + i * threads_count >= k) {
        // умножаем j строку матрицы на w^(k)
        double dot_product = 0;
        for (int j = k; j < n; j++)
          dot_product += matrix[j * cols + i] * xk[j];
        for (int j = k; j < n; j++)
          matrix[j * cols + i] -= 2 * xk[j] * dot_product;
      }

    // матрица хроница по столбцам в разных процессах так что делиться результатом с остальными процессами не нужно

    // после этого сделаем тоже самое для вектора b (полностью)
    if (thread_num == 0) {
      double dot_product_b = 0;
      for (int j = k; j < n; j++) {
        dot_product_b += b[j] * xk[j];
      }
      for (int j = k; j < n; j++) {
        b[j] -= 2 * xk[j] * dot_product_b;
        // printf("%lf %Lf\n", b[j],xk[j]);
      }
    }
  }

  // время для этого
  t1_local = get_time() - t1_local;
  double t2_local = get_time();
  // gauss backward
  double t = 0, glob_t = 0;
  for (int k = n - 1; k >= 0; --k) {
    // инициализируем все что и в прошлом цикле
    int root = 0, kk = 0;
    if (thread_num == 0) t = b[k];
    else t = 0;
    root = k % threads_count;
    kk = k / threads_count;
    if (thread_num == root) kk = kk + 1;
    else kk = 0;

    // просто считаем по формуле
    for (int i = kk; i < sendcounts[thread_num]; i++)
      if (thread_num + i * threads_count >= k) {
        t -= matrix[k * cols + i] * x[thread_num + i * threads_count];
      }
    // составим сумму r_ij*x_j в glob_t по кусочкам t посчитанным в разных процессах
    // MPI_Reduce составляет сумму
    MPI_Reduce(&t, &glob_t, 1, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);
    // считаем по формуле (большая дробь) очередную компонентку искомого вектора х
    if (thread_num == root) {
      x[k] = glob_t / matrix[k * cols + kk - 1];
    }

    // делимся со всеми процессами этой компонентой вектора х
    MPI_Bcast(&x[k], 1, MPI_DOUBLE, root, MPI_COMM_WORLD);
  }
  t2_local = get_time() - t2_local;

  // считаем максимальное время расчетов среди процессов что бы получить время рассчета общего алгоритма
  MPI_Reduce(&t1_local, t1, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Reduce(&t2_local, t2, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
}

// argv[1] - n - matrix size
// argv[2] - m - max output
// argv[3] - k - generation f number
int main(int argc, char *argv[]) {
  int n, m, k, threads_count, thread_num, cols;
  double *matrix, *vector_b, *x;
  int *sendcounts;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &threads_count);
  MPI_Comm_rank(MPI_COMM_WORLD, &thread_num);

  // считывание данных
  if (thread_num == 0) {
    if (argc != 4) {
      printf("uncorrect use: 1.\n"
             "correct use: '%s n m k', where\n"
             "n - matrix size ,\nm - max output,\n"
             "k - generation f number,\n",
             argv[0]);
      MPI_Finalize();
      return -1;
    }

    if (sscanf(argv[1], "%d", &n) != 1 || n < 0) {
      printf("uncorrect use: 1.\n"
             "correct use: '%s n m k', where\n"
             "n - matrix size ,\nm - max output,\n"
             "k - generation f number,\n",
             argv[0]);
      MPI_Finalize();
      return -1;
    }
    if (sscanf(argv[2], "%d", &m) != 1 || m < 0) {
      printf("uncorrect use: 1.\n"
             "correct use: '%s n m k', where\n"
             "n - matrix size ,\nm - max output,\n"
             "k - generation f number,\n",
             argv[0]);
      MPI_Finalize();
      return -1;
    }
    if (sscanf(argv[3], "%d", &k) != 1 || k <= 0) {
      printf("uncorrect use: 1.\n"
             "correct use: '%s n m k', where\n"
             "n - matrix size ,\nm - max output,\n"
             "k - generation f number,\n",
             argv[0]);
      MPI_Finalize();
      return -1;
    }
  }

  // отправить всем процессам n m k
  MPI_Bcast(&n, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
  MPI_Bcast(&m, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
  MPI_Bcast(&k, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);

  if (n % threads_count == 0 || thread_num + 1 > n % threads_count) cols = n / threads_count;
  else cols = n / threads_count + 1;

  matrix = new double[cols * n];
  vector_b = new double[n];
  x = new double[n];
  sendcounts = new int[threads_count]; // тоже самое что и cols только в массиве
  for (int i = 0; i < threads_count; ++i) {
    sendcounts[i] = (i + 1 > n % threads_count) ? n / threads_count : n / threads_count + 1;
  }
  init_matrix(matrix, n, k, threads_count, thread_num, vector_b, sendcounts, cols);
  if (thread_num == 0) printf("\nMatrix:\n");
  print_matrix(matrix, n, m, threads_count, thread_num, sendcounts, cols);

  // дождать выполнения всех параллельных реализаций после генерации матрицы и b
  MPI_Barrier(MPI_COMM_WORLD);
  double t1, t2;

  // решить методом отражения
  solve_slae_by_reflection(matrix, vector_b, x, sendcounts, n, thread_num, threads_count, &t1, &t2);
  // останаваливаем расспаралеливание
  MPI_Barrier(MPI_COMM_WORLD);

  if (thread_num == 0) printf("\nMatrix after:\n");
  print_matrix(matrix, n, m, threads_count, thread_num, sendcounts, cols);

  if (thread_num == 0) {
    printf("\nAnswer:\n");
    print_answer(x, n, m);
  }

  init_matrix(matrix, n, k, threads_count, thread_num, vector_b, sendcounts, cols);

  double residual = calculate_residual_norm(matrix, x, vector_b, n, threads_count, thread_num, sendcounts);

  if (thread_num == 0) {
    double error = calcualte_soulution_error(x, n);
    int s = k;
    int p = threads_count;
    printf("==========================\n");
    printf("residual = %e\nsoulution error = %e\nt1 = %.2lf t2 = %.2lf\ns = %d n = %d m = %d p = %d\n", residual, error, t1, t2, s, n, m, p);
    printf("==========================\n");

    // std::ofstream table;
    // table.open("table1.csv", std::ios_base::app);
    // table << p << ';' << n << ';' << t1 << ';' << t2 << ';' << t1 + t2 << ';';
    // table << '\n';
    // table.close();
  }

  delete[] x;
  delete[] matrix;
  delete[] vector_b;
  delete[] sendcounts;
  MPI_Finalize();
  return 0;
}
