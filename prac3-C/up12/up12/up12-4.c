#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

enum
{
    OPENMODE = 0751,
    MARGN = 2,
};

int
main(int argc, char *argv[])
{
    int fdout = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, OPENMODE);
    dprintf(fdout, "#! /usr/bin/python3 %s\n", argv[1]);
    dprintf(fdout, "print((lambda x: ((x + 1) * x // 2) %% %s)(int(input())))", argv[MARGN]);
    close(fdout);
    return 0;
}


