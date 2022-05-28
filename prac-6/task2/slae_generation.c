#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// вeктор x - случайные значения от -100 до 100
// матрица A - генерируется случайно
// вектор b - линейная комбинация соотв. строки матрицы А и вектора х

void generate_slae(FILE *output, int number_of_elements, int is_stdin) {
  int writed_object_count = 0;
  double *vector_x = (double *)malloc(number_of_elements * sizeof(double));

  for (int i = 0; i < number_of_elements; ++i) {
    if (is_stdin) scanf("%lf", &vector_x[i]);
    else vector_x[i] = (double)(rand() % 200);

    writed_object_count = fwrite(&vector_x[i], sizeof(double), 1, output);
  }

  double *vector_b = (double *)malloc(number_of_elements * sizeof(double));
  for (int i = 0; i < number_of_elements; ++i) {
    vector_b[i] = 0;
    for (int j = 0; j < number_of_elements; ++j) {
      double num;
      if (is_stdin) scanf("%lf", &num);
      else num = (double)(rand() % 200);

      vector_b[i] += num * vector_x[j];
      writed_object_count = fwrite(&num, sizeof(double), 1, output);
    }
  }

  for (int i = 0; i < number_of_elements; ++i) {
    writed_object_count = fwrite(&vector_b[i], sizeof(double), 1, output);
  }
  free(vector_x);
  free(vector_b);
}

int main(int argc, char **argv) {
  srand(42);
  if (argc > 3) exit(1);

  char slae[] = "slae_";
  char slae_filename[10];
  int number_of_elements = (int)atoi(argv[1]);
  sprintf(slae_filename, "%s%d", slae, number_of_elements);

  FILE *output_a = fopen(slae_filename, "wb");
  if (!output_a) exit(1);

  int is_stdin = 0;
  if (argc == 3) is_stdin = 1;

  generate_slae(output_a, number_of_elements, is_stdin);
  fclose(output_a);
  return 0;
}
