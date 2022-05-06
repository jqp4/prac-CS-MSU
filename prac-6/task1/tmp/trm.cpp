#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main_(int argc, char** argv) {
  int NoofRows, NoofCols, i, j, Total_threads, Noofthreads;
  float **Matrix, **Trans, **Checkoutput, flops;

  struct timeval TimeValue_Start;
  struct timezone TimeZone_Start;

  struct timeval TimeValue_Final;
  struct timezone TimeZone_Final;
  long time_start, time_end;
  double time_overhead;

  if (argc != 4) {
    exit(-1);
  }

  Noofthreads = atoi(argv[1]);
  if ((Noofthreads != 1) && (Noofthreads != 2) && (Noofthreads != 4) &&
      (Noofthreads != 8) && (Noofthreads != 16)) {
    exit(-1);
  }

  NoofRows = atoi(argv[2]);
  NoofCols = atoi(argv[3]);

  if (NoofRows <= 0 || NoofCols <= 0) {
    exit(1);
  }

  Matrix = (float**)malloc(sizeof(float*) * NoofRows);
  for (i = 0; i < NoofRows; i++) {
    Matrix[i] = (float*)malloc(sizeof(float) * NoofCols);
    for (j = 0; j < NoofCols; j++) {
      Matrix[i][j] = (i * j) * 5 + i;
    }
  }

  Trans = (float**)malloc(sizeof(float*) * NoofCols);
  Checkoutput = (float**)malloc(sizeof(float*) * NoofCols);

  for (i = 0; i < NoofCols; i++) {
    Checkoutput[i] = (float*)malloc(sizeof(float) * NoofRows);
    Trans[i] = (float*)malloc(sizeof(float) * NoofRows);
    for (j = 0; j < NoofRows; j++) {
      Checkoutput[i][j] = 0;
      Trans[i][j] = 0;
    }
  }

  gettimeofday(&TimeValue_Start, &TimeZone_Start);

  omp_set_num_threads(Noofthreads);

#pragma omp parallel for private(j)
  for (i = 0; i < NoofRows; i = i + 1) {
    Total_threads = omp_get_num_threads();
    for (j = 0; j < NoofCols; j = j + 1) {
      Trans[j][i] = Matrix[i][j];
    }
  }

  gettimeofday(&TimeValue_Final, &TimeZone_Final);

  time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
  time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
  time_overhead = (time_end - time_start) / 1000000.0;

  for (i = 0; i < NoofRows; i = i + 1)
    for (j = 0; j < NoofCols; j = j + 1)
      Checkoutput[j][i] = Matrix[i][j];

  for (i = 0; i < NoofCols; i = i + 1) {
    for (j = 0; j < NoofRows; j = j + 1) {
      if (Checkoutput[i][j] == Trans[i][j])
        continue;
      else {
        exit(-1);
      }
    }
  }

  free(Matrix);
  free(Checkoutput);
  free(Trans);
}