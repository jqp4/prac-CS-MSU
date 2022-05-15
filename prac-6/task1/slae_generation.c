#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void generate_slae(FILE *output, int number_of_elements, int is_stdin) {
  int writed_object_count = 0;
  double *vector_x = (double *)malloc(number_of_elements * sizeof(double));

  for (int i = 0; i < number_of_elements; ++i) {
    if (is_stdin) scanf("%lf", &vector_x[i]);
    else vector_x[i] = (double)(rand() % 200 - 100);

    writed_object_count = fwrite(&vector_x[i], sizeof(double), 1, output);
  }

  double *vector_b = (double *)malloc(number_of_elements * sizeof(double));

  for (int i = 0; i < number_of_elements; ++i) {
    vector_b[i] = 0;
    for (int j = 0; j < number_of_elements; ++j) {
      double num;
      if (is_stdin) {
        scanf("%lf", &num);
      } else {
        num = (double)(rand() % 200 - 100);
      }
      vector_b[i] += num * vector_x[j];
      writed_object_count = fwrite(&num, sizeof(double), 1, output);
    }
  }

  for (int i = 0; i < number_of_elements; ++i)
    writed_object_count = fwrite(&vector_b[i], sizeof(double), 1, output);

  free(vector_x);
  free(vector_b);
}

int main(int argc, char **argv) {
  srand(42);
  if (argc > 3) exit(1);

  char slae[] = "slae_";
  char slaeFilename[10];
  int numberOfElements = (int)atoi(argv[1]);
  sprintf(slaeFilename, "%s%d", slae, numberOfElements);

  FILE *output_a = fopen(slaeFilename, "wb");
  if (!output_a) {
    printf("Error opening file \"%s\"\n", "a");
    exit(1);
  }
  int is_stdin = 0;
  if (argc == 3) {
    is_stdin = 1;
  }
  generate_slae(output_a, numberOfElements, is_stdin);
  fclose(output_a);

  return 0;
}
