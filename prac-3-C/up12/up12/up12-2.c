#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

enum
{
    PIPESIZE = 2,
    DECBASE = 10,
    CARGN = 1,
    MARGN = 2,
    PARGN = 3,
};

void
closeAllPipes(int *pipe12, int *pipe24)
{
    close(pipe12[0]);
    close(pipe12[1]);
    close(pipe24[0]);
    close(pipe24[1]);
}

/*void
progTest(void)
{
    int buf;
    while (scanf("%d", &buf) > 0) {
        printf("%d\n", buf);
        fflush(stdout);
    }
    close(0);
    close(1);
}*/

int
main(int argc, char *argv[])
{
    char *end;
    int count = (int)strtol(argv[CARGN], &end, DECBASE);
    int modulo = (int)strtol(argv[MARGN], &end, DECBASE);
    int pipe12[PIPESIZE], pipe24[PIPESIZE];
    pid_t p1, p2, p3, p4;
    pipe(pipe12);
    pipe(pipe24);
    
    if (!(p1 = fork())) {
        if (!(p2 = fork())) {
            dup2(pipe12[0], 0);
            dup2(pipe24[1], 1);
            closeAllPipes(pipe12, pipe24);
            execlp(argv[PARGN], argv[PARGN], NULL);
            close(0);
            close(1);
            _exit(1);
        }
        dup2(pipe12[1], 1);
        closeAllPipes(pipe12, pipe24);
        for (int i = 1; i <= count; i++) {
            printf("%d ", (int)(((unsigned long long)i * i) % modulo));
            fflush(stdout);
        }
        close(1);
        _exit(0);
    }
    
    if (!(p3 = fork())) {
        if (!(p4 = fork())) {
            dup2(pipe24[0], 0);
            closeAllPipes(pipe12, pipe24);
            int x;
            while (scanf("%d", &x) > 0) {
                printf("%d\n", x);
                fflush(stdout);
            }
            close(0);
            _exit(0);
        }
        closeAllPipes(pipe12, pipe24);
        wait(NULL);
        _exit(0);
    }
    closeAllPipes(pipe12, pipe24);
    wait(NULL);
    wait(NULL);
    printf("0\n");
    fflush(stdout);
    return 0;
}
