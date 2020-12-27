#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

enum
{
    YEAR_SHIFT = 1900,
};

void
getNprintTime(int fd, char mode)
{
    time_t t0;
    struct tm t;
    read(fd, &t0, sizeof(t0));
    memset(&t, 0, sizeof(t));
    t = *localtime(&t0);
    switch (mode) {
        case 'd':
            printf("D:%02d\n", t.tm_mday);
            break;
        case 'm':
            printf("M:%02d\n", t.tm_mon + 1);
            break;
        case 'y':
            printf("Y:%04d\n", t.tm_year + YEAR_SHIFT);
            break;
    }
}

int
main(int argc, char *argv[])
{
    int fd[2];
    pipe(fd);
    pid_t pid1 = fork();
    if (!pid1) {
        // сын
        pid_t pid2 = fork();
        if (!pid2) {
            //внук
            pid_t pid3 = fork();
            if (!pid3) {
                //правнук
                time_t t;
                time(&t);
                write(fd[1], &t, sizeof(t));
                write(fd[1], &t, sizeof(t));
                write(fd[1], &t, sizeof(t));
                return 0;
            }
            waitpid(pid3, NULL, 0);
            getNprintTime(fd[0], 'd');
            return 0;
        }
        waitpid(pid2, NULL, 0);
        getNprintTime(fd[0], 'm');
        return 0;
    }
    waitpid(pid1, NULL, 0);
    getNprintTime(fd[0], 'y');
    close(fd[0]);
    close(fd[1]);
    return 0;
}
