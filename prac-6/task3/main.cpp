#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <omp.h>
#include <tuple>
#include <vector>
#define ld long double
#define _USE_MATH_DEFINES
using std::abs;
using std::tie;
using std::vector;

ld solution_diff(int n, const vector<ld> &a, const vector<ld> &b) {
  ld norm = 0;
  for (int i = 0; i < n; i++)
    norm += std::pow(a[i] - b[i], 2);

  return sqrt(norm);
}

vector<ld> line_paqb(int n, ld p, const vector<ld> &a, ld q, const vector<ld> &b) {
  vector<ld> c(n);
  for (int i = 0; i < n; i++)
    c[i] = p * a[i] + q * b[i];

  return c;
}

vector<ld> omp_paqb(int num_threads, int n, ld p, const vector<ld> &a, ld q, const vector<ld> &b) {
  vector<ld> c(n);
  #pragma omp parallel num_threads(num_threads)
  {
    #pragma omp for
    for (int i = 0; i < n; i++)
      c[i] = p * a[i] + q * b[i];
  }

  return c;
}

ld line_mult_VV(int n, const vector<ld> &a, const vector<ld> &b) {
  ld c = 0.0;
  for (int i = 0; i < n; i++)
    c += a[i] * b[i];

  return c;
}

ld omp_mult_VV(int num_threads, int n, const vector<ld> &a, const vector<ld> &b) {
  ld c = 0.0;
  #pragma omp parallel num_threads(num_threads)
  {
    #pragma omp for reduction(+ : c)
    for (int i = 0; i < n; i++)
      c += a[i] * b[i];
  }

  return c;
}

vector<ld> line_mult_MV(int n, int m, const vector<int> &col, const vector<ld> &val, const vector<ld> &b) {
  vector<ld> c(n);
  for (int i = 0; i < n; i++) {
    ld sum = 0.0;
    for (int j = 0; j < m; j++)
      sum += val[i * m + j] * b[col[i * m + j]];
    c[i] = sum;
  }

  return c;
}

vector<ld> omp_mult_MV(int num_threads, int n, int m, const vector<int> &col, const vector<ld> &val, const vector<ld> &b) {
  vector<ld> c(n);
  #pragma omp parallel num_threads(num_threads)
  {
    #pragma omp for
    for (int i = 0; i < n; i++) {
      ld sum = 0.0;
      for (int j = 0; j < m; j++)
        sum += val[i * m + j] * b[col[i * m + j]];
      c[i] = sum;
    }
  }

  return c;
}

void omp_funcs_test(int num_threads, int n, int m, const vector<int> &col, const vector<ld> &val, const vector<ld> &b, const vector<ld> &x) {
  const ld eps = 1.0e-7;
  vector<ld> consistent_res = line_mult_MV(n, m, col, val, x);
  vector<ld> omp_res = omp_mult_MV(num_threads, n, m, col, val, x);
  ld norm_of_solution_difference = solution_diff(n, consistent_res, omp_res);
  if (norm_of_solution_difference > eps) exit(0);

  ld dot_consistent_res = line_mult_VV(n, b, x);
  ld dot_omp_res = omp_mult_VV(num_threads, n, b, x);
  if (abs(dot_consistent_res - dot_omp_res) > eps) exit(0);

  vector<ld> paqb_consistent_res = line_paqb(n, 1.0, b, 1.0, x);
  vector<ld> paqb_omp_res = omp_paqb(num_threads, n, 1.0, b, 1.0, x);
  ld paqb_diff = solution_diff(n, paqb_consistent_res, paqb_omp_res);
  if (paqb_diff > eps) exit(0);
}

std::tuple<int, int, int> split_number(int n) {
  int n_x, n_y, n_z;
  switch (n) {
  case 10000:
    tie(n_x, n_y, n_z) = std::tuple<int, int, int>(50, 20, 10);
    break;
  case 100000:
    tie(n_x, n_y, n_z) = std::tuple<int, int, int>(50, 50, 40);
    break;
  case 1000000:
    tie(n_x, n_y, n_z) = std::tuple<int, int, int>(100, 100, 100);
    break;
  default:
    exit(0);
  }

  return tie(n_x, n_y, n_z);
}

std::tuple<vector<int>, vector<ld>, vector<ld>, vector<ld>> ellpack_matrix(int n, int m) {
  vector<int> col(n * m);
  vector<ld> val(n * m);
  int n_x, n_y, n_z;

  tie(n_x, n_y, n_z) = split_number(n);
  vector<vector<vector<ld>>> cartesian_grid(n_z, vector<vector<ld>>(n_y, vector<ld>(n_x)));
  for (int k = 0; k < n_z; k++)
    for (int j = 0; j < n_y; j++)
      for (int i = 0; i < n_x; i++)
        cartesian_grid[k][j][i] = k * n_x * n_y + j * n_x + i;

  auto lambda = [m, &col, &val, &cartesian_grid](int i, int j, int k, int col_i, int matrix_i) {
    int matrix_j = cartesian_grid[k][j][i];
    ld number = std::cos(matrix_i * matrix_j + M_PI);
    col[matrix_i * m + col_i] = matrix_j;
    val[matrix_i * m + col_i] = number;
    return number;
  };

  for (int k = 0; k < n_z; k++) {
    for (int j = 0; j < n_y; j++) {
      for (int i = 0; i < n_x; i++) {
        ld sum = 0.0;
        int column_i = 0;
        int matrix_i = cartesian_grid[k][j][i];
        if (k > 0) {
          sum += abs(lambda(i, j, k - 1, column_i, matrix_i));
          column_i++;
        }
        if (k < n_z - 1) {
          sum += abs(lambda(i, j, k + 1, column_i, matrix_i));
          column_i++;
        }
        if (j > 0) {
          sum += abs(lambda(i, j - 1, k, column_i, matrix_i));
          column_i++;
        }
        if (j < n_y - 1) {
          sum += abs(lambda(i, j + 1, k, column_i, matrix_i));
          column_i++;
        }
        if (i > 0) {
          sum += abs(lambda(i - 1, j, k, column_i, matrix_i));
          column_i++;
        }
        if (i < n_x - 1) {
          sum += abs(lambda(i + 1, j, k, column_i, matrix_i));
          column_i++;
        }
        col[matrix_i * m + column_i] = matrix_i;
        val[matrix_i * m + column_i] = 1.5 * sum;
      }
    }
  }

  vector<ld> x(n);
  for (int i = 0; i < n; i++)
    x[i] = (ld)(rand() % 10 - 5);
  vector<ld> b = line_mult_MV(n, m, col, val, x);
  return tie(col, val, b, x);
}

vector<ld> cg_solver(int num_threads, int n, int m, const vector<int> &col, const vector<ld> &val, const vector<ld> &b) {
  const ld eps = 1.0e-7;
  ld timer_start;
  ld spmv_total_time = 0.0;
  ld paqb_total_time = 0.0;
  ld dot_total_time = 0.0;

  vector<ld> x(n, 0);
  timer_start = omp_get_wtime();
  vector<ld> spmv_vec = omp_mult_MV(num_threads, n, m, col, val, x);
  spmv_total_time += omp_get_wtime() - timer_start;

  vector<ld> omp_paqb_res(n);
  timer_start = omp_get_wtime();
  omp_paqb_res = omp_paqb(num_threads, n, 1, b, -1, spmv_vec);
  paqb_total_time += omp_get_wtime() - timer_start;

  int iter_count = 1;
  int convergence = 1;
  int max_iter = 10000;
  vector<ld> p(n, 0);
  vector<ld> q(n, 0);
  vector<ld> z(n, 0);
  ld cur_f = 0;

  do {
    z = omp_paqb_res;
    timer_start = omp_get_wtime();
    ld f = cur_f;
    cur_f = omp_mult_VV(num_threads, n, omp_paqb_res, z);
    dot_total_time += omp_get_wtime() - timer_start;

    if (iter_count == 1) p = z;
    else {
      ld beta = cur_f / f;
      timer_start = omp_get_wtime();
      p = omp_paqb(num_threads, n, 1, z, beta, p);
      paqb_total_time += omp_get_wtime() - timer_start;
    }
    timer_start = omp_get_wtime();
    q = omp_mult_MV(num_threads, n, m, col, val, p);
    spmv_total_time += omp_get_wtime() - timer_start;

    timer_start = omp_get_wtime();
    ld tmp = omp_mult_VV(num_threads, n, p, q);
    dot_total_time += omp_get_wtime() - timer_start;
    ld alpha = cur_f / tmp;

    timer_start = omp_get_wtime();
    x = omp_paqb(num_threads, n, 1, x, alpha, p);
    paqb_total_time += omp_get_wtime() - timer_start;

    timer_start = omp_get_wtime();
    omp_paqb_res = omp_paqb(num_threads, n, 1, omp_paqb_res, -alpha, q);
    paqb_total_time += omp_get_wtime() - timer_start;

    if ((iter_count >= max_iter) | (abs(cur_f) < eps)) convergence = 0;
    else iter_count++;

  } while (convergence);

  std::cout << iter_count << std::endl;
  std::cout << dot_total_time << std::endl;
  std::cout << paqb_total_time << std::endl;
  std::cout << spmv_total_time << std::endl;

  return x;
}

int main(int argc, char **argv) {
  srand(0);
  int m = 7;
  int n = (int)atoi(argv[1]);
  int num_threads = (int)atoi(argv[2]);
  vector<ld> real_x, val, b;
  vector<int> col;

  // генерация ellpack матрицы
  tie(col, val, b, real_x) = ellpack_matrix(n, m);

  // тестирование omp функций
  omp_funcs_test(num_threads, n, m, col, val, b, real_x);

  // решение
  vector<ld> x;
  x = cg_solver(num_threads, n, m, col, val, b);

  std::cout << solution_diff(n, x, real_x) << std::endl;
  std::cout << solution_diff(n, b, line_mult_MV(n, m, col, val, x)) << std::endl;

  return 0;
}
