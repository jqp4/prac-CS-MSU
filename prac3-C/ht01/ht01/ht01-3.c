#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

enum
{
    F_ARG = 1,
    ROWS_ARG = 2,
    COLS_ARG = 3,
    NUM_OF_DIR = 4,
    DEC_BASE = 10,
    MODE = 0777,
};

static void outputPtr(unsigned *ptr, long cols, long rows) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            printf("%2u ", ptr[r * cols + c]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, const char * argv[])
{
    char *endln;
    long rows = strtol(argv[ROWS_ARG], &endln, DEC_BASE);
    long cols = strtol(argv[COLS_ARG], &endln, DEC_BASE);
    unsigned long long len = rows * cols;
    if (!len) {
        fprintf(stderr, "Invalid input!\n");
        return 1;
    }
    int f = open(argv[F_ARG], O_RDWR | O_CREAT | O_TRUNC, MODE);
    unsigned *ptr = mmap(NULL, len * sizeof(*ptr), PROT_WRITE, MAP_SHARED, f, 0);
    if (ptr == MAP_FAILED) {
        fprintf(stderr, "Mapping Failed!\n");
        return 1;
    }
    ftruncate(f, len * sizeof(*ptr));
    int direction = 0;
    long count = cols;
    int coords[2] = {0, 0};
    long delta = cols - rows;
    unsigned value = 0;
    while (count && value < len) {
        for (unsigned i = 0; i + 1 < count; i++) {
            ptr[coords[0] * cols + coords[1]] = ++value;
            coords[(direction + 1) & 1] += (direction >> 1) ? -1 : 1;
        }
        count += (delta = - delta - 1);
        direction = (direction + 1) % NUM_OF_DIR;
        ptr[coords[0] * cols + coords[1]] = ++value;
        coords[(direction + 1) & 1] += (direction >> 1) ? -1 : 1;
    }
    outputPtr(ptr, cols, rows);
    munmap(ptr, len * sizeof(*ptr));
    close(f);
    return 0;
}
