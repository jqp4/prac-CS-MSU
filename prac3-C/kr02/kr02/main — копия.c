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
    FILEARG = 2,
    PIPESIZE = 2,
    N = 10,
};

int fd;
pid_t pid;

void
handler(int s)
{
    int ans = 0;
    if (s == SIGUSR1) {
        for (int i = 0; i < N; i++) {
            int x;
            if (read(fd, &x, sizeof(x)) == sizeof(x)) {
                 __builtin_sadd_overflow(ans, x, &ans);
            } else {
                if (i == 0) {
                    kill(pid, SIGUSR1);
                }
                printf("%d\n", ans);
                fflush(stdout);
                _exit(0);
            }
        }
        printf("%d\n", ans);
        fflush(stdout);
    } else if (s == SIGUSR2) {
        for (int i = 0; i < N; i++) {
            int x;
            if (read(fd, &x, sizeof(x)) == sizeof(x)) {
                 __builtin_smul_overflow(ans, x, &ans);
            } else {
                if (i == 0) {
                    kill(pid, SIGUSR1);
                }
                printf("%d\n", ans);
                fflush(stdout);
                _exit(0);
            }
        }
        printf("%d\n", ans);
        fflush(stdout);
        
        
        
        
        
        
        
    }
}

int
main(int argc, char *argv[])
{
    struct sigaction ss;
    ss.sa_handler = handler;
    ss.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &ss, NULL);
    sigaction(SIGUSR2, &ss, NULL);
    fd = open(argv[FILEARG], O_RDONLY);
    int pp[PIPESIZE];
    pipe(pp);
    close(pp[0]);
    close(pp[1]);
    pid = fork();
    if (!pid) {
        dup2(pp[0], 0);
        dup2(pp[1], 1);
        close(pp[0]);
        close(pp[1]);
        execlp(argv[1], argv[1], NULL);
    }
    wait(NULL);
    close(fd);
    return 0;
}
