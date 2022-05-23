#include <assert.h>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <omp.h>
#include <tuple>
#include <vector>

#define _USE_MATH_DEFINES
#define ld long double

using std::vector;

template <typename T> void visualize_vector(const vector<T> &vec) {
  for (const auto &elem : vec) {
    std::cout << elem << ' ';
  }
  std::cout << std::endl;
}

void visualize_matrix(int n, int m, const vector<int> &col, const vector<ld> &val) {
  for (int i = 0; i < n; ++i) {
    vector<ld> tmp_vec(n, 0);
    for (int j = m - 1; j >= 0; --j) {
      int index = col[i * m + j];
      tmp_vec[index] = val[i * m + j];
    }
    visualize_vector(tmp_vec);
  }
}

ld get_norm_of_solution_difference(int n, const vector<ld> &a, const vector<ld> &b) {
  ld norm = 0;
  for (int i = 0; i < n; ++i) {
    ld diff = a[i] - b[i];
    norm += diff * diff;
  }
  return sqrt(norm);
}

vector<ld> get_sum_pa_qb(int n, ld p, const vector<ld> &a, ld q, const vector<ld> &b) {
  vector<ld> c(n);
  for (int i = 0; i < n; ++i) {
    c[i] = p * a[i] + q * b[i];
  }
  return c;
}

vector<ld> omp_get_sum_pa_qb(int num_threads, int n, ld p, const vector<ld> &a, ld q, const vector<ld> &b) {
  vector<ld> c(n);
  #pragma omp parallel num_threads(num_threads)
  {
    #pragma omp for
    for (int i = 0; i < n; ++i) {
      c[i] = p * a[i] + q * b[i];
    }
  }
  return c;
}

ld multiply_vector_by_vector(int n, const vector<ld> &a, const vector<ld> &b) {
  ld c = 0.0;
  for (int i = 0; i < n; ++i) {
    c += a[i] * b[i];
  }
  return c;
}

ld omp_multiply_vector_by_vector(int num_threads, int n, const vector<ld> &a, const vector<ld> &b) {
  ld c = 0.0;
  #pragma omp parallel num_threads(num_threads)
  {
    #pragma omp for reduction(+ : c)
    for (int i = 0; i < n; ++i) {
      c += a[i] * b[i];
    }
  }
  return c;
}

vector<ld> multiply_matrix_by_vector(int n, int m, const vector<int> &col, const vector<ld> &val, const vector<ld> &b) {
  vector<ld> c(n);
  for (int i = 0; i < n; ++i) {
    ld sum = 0.0;
    for (int j = 0; j < m; ++j) {
      sum += val[i * m + j] * b[col[i * m + j]];
    }
    c[i] = sum;
  }
  return c;
}

vector<ld> omp_multiply_matrix_by_vector(int num_threads, int n, int m, const vector<int> &col, const vector<ld> &val, const vector<ld> &b) {
  vector<ld> c(n);
  #pragma omp parallel num_threads(num_threads)
  {
    #pragma omp for
    for (int i = 0; i < n; ++i) {
      ld sum = 0.0;
      for (int j = 0; j < m; ++j) {
        sum += val[i * m + j] * b[col[i * m + j]];
      }
      c[i] = sum;
    }
  }
  return c;
}

void test_omp_version_by_line_version(int num_threads, int n, int m, const vector<int> &col, const vector<ld> &val, const vector<ld> &b, const vector<ld> &x) {
  ld eps = 1.0e-7;
  auto line_version_result = multiply_matrix_by_vector(n, m, col, val, x);
  auto omp_version_result = omp_multiply_matrix_by_vector(num_threads, n, m, col, val, x);
  auto norm_of_solution_difference = get_norm_of_solution_difference(n, line_version_result, omp_version_result);
  if (norm_of_solution_difference > eps) {
    std::cerr << "Error in multiply_matrix_by_vector\n";
    exit(20);
  }
  ld dot_line_version_result = multiply_vector_by_vector(n, b, x);
  ld dot_omp_version_result = omp_multiply_vector_by_vector(num_threads, n, b, x);
  if (std::abs(dot_line_version_result - dot_omp_version_result) > eps) {
    std::cerr << "Error in multiply_vector_by_vector\n";
    exit(21);
  }
  auto pa_qb_line_version_result = get_sum_pa_qb(n, 1.0, b, 1.0, x);
  auto pa_qb_omp_version_result = omp_get_sum_pa_qb(num_threads, n, 1.0, b, 1.0, x);
  auto pa_qb_norm_of_solution_difference = get_norm_of_solution_difference(n, pa_qb_line_version_result, pa_qb_omp_version_result);
  if (pa_qb_norm_of_solution_difference > eps) {
    std::cerr << "Error in get_sum_pa_qb\n";
    exit(22);
  }
  std::cout << "!!! All test passed !!!\n";
}

std::tuple<int, int, int> split_number(int n) {
  int n_x, n_y, n_z;
  switch (n) {
  // example section
  case 8:
    std::tie(n_x, n_y, n_z) = std::tuple<int, int, int>(2, 2, 2);
    break;
  case 27:
    std::tie(n_x, n_y, n_z) = std::tuple<int, int, int>(3, 3, 3);
    break;
  // end of example section
  case 10000:
    std::tie(n_x, n_y, n_z) = std::tuple<int, int, int>(50, 20, 10);
    break;
  case 100000:
    std::tie(n_x, n_y, n_z) = std::tuple<int, int, int>(50, 50, 40);
    break;
  case 1000000:
    std::tie(n_x, n_y, n_z) = std::tuple<int, int, int>(100, 100, 100);
    break;
  default:
    std::cerr << "Invalid matrix size\n";
    exit(11);
  }
  return std::tie(n_x, n_y, n_z);
}

std::tuple<vector<int>, vector<ld>, vector<ld>, vector<ld>> generate_ellpack_matrix(int n, int m) {
  vector<int> col(n * m);
  vector<ld> val(n * m);
  int n_x, n_y, n_z;
  std::tie(n_x, n_y, n_z) = split_number(n);
  vector<vector<vector<ld>>> cartesian_grid(n_z, vector<vector<ld>>(n_y, vector<ld>(n_x)));
  for (int k = 0; k < n_z; ++k) {
    for (int j = 0; j < n_y; ++j) {
      for (int i = 0; i < n_x; ++i) {
        cartesian_grid[k][j][i] = k * n_x * n_y + j * n_x + i;
      }
    }
  }
  auto lambda = [m, &col, &val, &cartesian_grid](int i, int j, int k, int col_index, int matrix_i) {
    int matrix_j = cartesian_grid[k][j][i];
    ld number = std::cos(matrix_i * matrix_j + M_PI);
    // ld number = 1;
    col[matrix_i * m + col_index] = matrix_j;
    val[matrix_i * m + col_index] = number;
    return number;
  };
  for (int k = 0; k < n_z; ++k) {
    for (int j = 0; j < n_y; ++j) {
      for (int i = 0; i < n_x; ++i) {
        int matrix_i = cartesian_grid[k][j][i];
        ld sum = 0.0;
        int col_index = 0;
        if (k > 0) {
          sum += std::abs(lambda(i, j, k - 1, col_index, matrix_i));
          ++col_index;
        }
        if (k < n_z - 1) {
          sum += std::abs(lambda(i, j, k + 1, col_index, matrix_i));
          ++col_index;
        }
        if (j > 0) {
          sum += std::abs(lambda(i, j - 1, k, col_index, matrix_i));
          ++col_index;
        }
        if (j < n_y - 1) {
          sum += std::abs(lambda(i, j + 1, k, col_index, matrix_i));
          ++col_index;
        }
        if (i > 0) {
          sum += std::abs(lambda(i - 1, j, k, col_index, matrix_i));
          ++col_index;
        }
        if (i < n_x - 1) {
          sum += std::abs(lambda(i + 1, j, k, col_index, matrix_i));
          ++col_index;
        }
        col[matrix_i * m + col_index] = matrix_i;
        val[matrix_i * m + col_index] = 1.5 * sum;
      }
    }
  }
  vector<ld> x(n);
  for (int i = 0; i < n; ++i) {
    x[i] = (ld)(rand() % 10 - 5);
  }
  vector<ld> b = multiply_matrix_by_vector(n, m, col, val, x);
  return std::tie(col, val, b, x);
}

vector<ld> solver_cg(int num_threads, int n, int m, const vector<int> &col, const vector<ld> &val, const vector<ld> &b) {
  vector<ld> x(n, 0);
  ld spmv_total_time = 0.0;
  ld spmv_timer_start;
  ld spmv_timer_stop;
  spmv_timer_start = omp_get_wtime();
  vector<ld> spmv_vec = omp_multiply_matrix_by_vector(num_threads, n, m, col, val, x);
  spmv_timer_stop = omp_get_wtime();
  spmv_total_time += spmv_timer_stop - spmv_timer_start;

  ld pa_qb_total_time = 0.0;
  ld pa_qb_timer_start;
  ld pa_qb_timer_stop;
  vector<ld> r(n);
  pa_qb_timer_start = omp_get_wtime();
  r = omp_get_sum_pa_qb(num_threads, n, 1, b, -1, spmv_vec);
  pa_qb_timer_stop = omp_get_wtime();
  pa_qb_total_time += pa_qb_timer_stop - pa_qb_timer_start;

  ld dot_total_time = 0.0;
  ld dot_timer_start;
  ld dot_timer_stop;
  int convergence = 1;
  int k = 1;
  ld eps = 1.0e-7;
  int max_iter = 10000;
  ld cur_f = 0;
  vector<ld> p(n, 0);
  vector<ld> q(n, 0);
  vector<ld> z(n, 0);
  do {
    z = r;

    dot_timer_start = omp_get_wtime();
    ld f = cur_f;
    cur_f = omp_multiply_vector_by_vector(num_threads, n, r, z);
    dot_timer_stop = omp_get_wtime();
    dot_total_time += dot_timer_stop - dot_timer_start;

    if (k == 1) {
      p = z;
    } else {
      ld beta = cur_f / f;
      pa_qb_timer_start = omp_get_wtime();
      p = omp_get_sum_pa_qb(num_threads, n, 1, z, beta, p);
      pa_qb_timer_stop = omp_get_wtime();
      pa_qb_total_time += pa_qb_timer_stop - pa_qb_timer_start;
    }
    spmv_timer_start = omp_get_wtime();
    q = omp_multiply_matrix_by_vector(num_threads, n, m, col, val, p);
    spmv_timer_stop = omp_get_wtime();
    spmv_total_time += spmv_timer_stop - spmv_timer_start;

    dot_timer_start = omp_get_wtime();
    ld tmp = omp_multiply_vector_by_vector(num_threads, n, p, q);
    dot_timer_stop = omp_get_wtime();
    dot_total_time += dot_timer_stop - dot_timer_start;
    ld alpha = cur_f / tmp;

    pa_qb_timer_start = omp_get_wtime();
    x = omp_get_sum_pa_qb(num_threads, n, 1, x, alpha, p);
    pa_qb_timer_stop = omp_get_wtime();
    pa_qb_total_time += pa_qb_timer_stop - pa_qb_timer_start;

    pa_qb_timer_start = omp_get_wtime();
    r = omp_get_sum_pa_qb(num_threads, n, 1, r, -alpha, q);
    pa_qb_timer_stop = omp_get_wtime();
    pa_qb_total_time += pa_qb_timer_stop - pa_qb_timer_start;
    // std::cout << "F_" << k << " = " << cur_f << std::endl;
    if ((k >= max_iter) | (std::abs(cur_f) < eps)) {
      convergence = 0;
    } else {
      ++k;
    }
  } while (convergence);
  std::cout << "Iteration count = " << k << std::endl;
  std::cout << "dot total time = " << dot_total_time << std::endl;
  std::cout << "paqb total time = " << pa_qb_total_time << std::endl;
  std::cout << "spmv total time = " << spmv_total_time << std::endl;

  return x;
}

int main(int argc, char **argv) {
  srand(42);
  assert(argc == 3 && "Invalid count of ommand line arguments");
  int n = (int)atoi(argv[1]);
  int m = 7;
  assert(n > 0 && "Invalid matrix size");
  int num_threads = (int)atoi(argv[2]);
  assert(num_threads > 0 && "Invalid number of threads");

  vector<int> col;
  vector<ld> val;
  vector<ld> b;
  vector<ld> real_x;
  // Generate ellpack matrix
  std::tie(col, val, b, real_x) = generate_ellpack_matrix(n, m);

  // std::cout << "Vector real_x = {\n";
  // visualize_vector(real_x);
  // std::cout << "}\n";
  // std::cout << "Vector b = {\n";
  // visualize_vector(b);
  // std::cout << "}\n";
  // std::cout << "Matrix a = {\n";
  // visualize_matrix(n, m, col, val);
  // std::cout << "}\n";

  // Test omp functions
  test_omp_version_by_line_version(num_threads, n, m, col, val, b, real_x);

  // Get solution
  vector<ld> x;
  x = solver_cg(num_threads, n, m, col, val, b);
  // std::cout << "Vector x = {\n";
  // visualize_vector(x);
  // std::cout << "}\n";
  std::cout << "Solution difference = " << get_norm_of_solution_difference(n, x, real_x) << std::endl;
  std::cout << "Slae residual norm = " << get_norm_of_solution_difference(n, b, multiply_matrix_by_vector(n, m, col, val, x)) << std::endl;
}
