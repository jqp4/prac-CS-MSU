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
};

int fd;

void
handler(int s)
{
    if (s == SIGUSR1) {
        
    } else if (s == SIGUSR2) {
        
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
    dup2(pp[0], 0);
    dup2(pp[1], 1);
    close(pp[0]);
    close(pp[1]);
    execlp(argv[1], argv[PARGN], NULL);
    
    
    
    
    
    int x;
    int ans = 0;
    while (scanf("%d", &x) == 1) {
        //if (g) __builtin_smul_overflow(ans, x, &ans);
        //else __builtin_sadd_overflow(ans, x, &ans);
        printf("%d\n", ans);
        fflush(stdout);
    }
}
