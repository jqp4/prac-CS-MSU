#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>

enum
{
    BYTE = 8,
    BUFFSIZE = 4,
    MODE = 0600,
};

int main(int argc, char *argv[]) {
    unsigned int x;
    int8_t buf[BUFFSIZE];
    char *filename = argv[1];
    int fout = open(filename, O_RDWR | O_CREAT | O_TRUNC, MODE);
    while (scanf("%u", &x) != EOF) {
        
        write(fout, buf, BUFFSIZE);
    }
    close(fout);
    return EXIT_SUCCESS;
}
