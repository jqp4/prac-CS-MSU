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
    HALFBYTE = 4,
    BUFFSIZE = 4,
    LASTBYTE = 0xff,
    SHIFT = 0xfff,
    MODE = 0600,
};

int
main(int argc, char *argv[])
{
    /*if (argc < MIN_ARGC){
        char *message = "invalid input\n";
        write(STDOUT_FILENO, message, strlen(message));
        return 1;
    }*/
    unsigned int x, y;
    int8_t buf[BUFFSIZE];
    char *filename = argv[1];
    int fout = open(filename, O_RDWR | O_CREAT | O_TRUNC, MODE);
    while (scanf("%u", &x) != EOF) {
        y = x & SHIFT;
        x = ((x - y) << HALFBYTE) + y;
        for (int i = 0; i < BUFFSIZE; i++) {
            buf[BUFFSIZE - i - 1] = x & LASTBYTE;
            x >>= BYTE;
        }
        write(fout, buf, sizeof(buf));
    }
    close(fout);
    return EXIT_SUCCESS;
}
