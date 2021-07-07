#include <stdio.h>
#include <limits.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

enum
{
    BYTE = 8,
    MASK = 0x80,
};

int fd, pd[2];
pid_t pid1 = 0;
pid_t pid2 = 0;

void handler(int signal){
    if (!pid1) {
        static u_int8_t x = 0;
        static int iter = 0;
        // 1 son
        if (!pid2) {
            read(pd[0], &pid2, sizeof(pid2));
            close(pd[0]);
        }
        if (signal == SIGUSR1) {
            x <<= 1;
            if (++iter == BYTE) {
                printf("%c", x);
                fflush(stdout);
                iter = 0;
            }
            kill(pid2, SIGALRM);
        } else if (signal == SIGUSR2) {
            x = (x << 1) + 1;
            if (++iter == BYTE) {
                printf("%c", x);
                fflush(stdout);
                iter = 0;
            }
            kill(pid2, SIGALRM);
        } else if (signal == SIGIO) {
            _exit(0);
        }
    //} else if (!pid2) {
    } else {
        // 2 son
        if (signal == SIGALRM) {
            static u_int8_t x = 0;
            static int iter = 0;
            if (!iter) {
                if (read(fd, &x, sizeof(x)) == sizeof(x)) {
                    iter = BYTE;
                } else {
                    kill(pid2, SIGIO);
                    _exit(0);
                }
            }
            u_int8_t x0 = x & MASK;
            x <<= 1;
            iter--;
            kill(pid1, x0 ? SIGUSR2 : SIGUSR1);
        }
    }
}

int main(int argc, char *argv[]){
    struct sigaction ss;
    ss.sa_handler = handler;
    ss.sa_flags = SA_RESTART;
    sigaction(SIGIO, &ss, NULL);
    sigaction(SIGUSR1, &ss, NULL);
    sigaction(SIGUSR2, &ss, NULL);
    sigaction(SIGALRM, &ss, NULL);
    fd = open(argv[1], O_RDONLY);
    pipe(pd);
    if (!(pid1 = fork())) while (1);
    if (!(pid2 = fork())) while (1);
    write(pd[1], &pid2, sizeof(pid2));
    close(pd[1]);
    kill(pid2, SIGALRM);
    wait(NULL);
    wait(NULL);
    close(fd);
    return 0;
}
