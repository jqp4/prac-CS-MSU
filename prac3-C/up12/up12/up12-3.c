#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

enum
{
    SIGAMOUNT = 2,
};

volatile sig_atomic_t COUNT = 0;

void
handlerSURS1(int signal)
{
    printf("%d %d\n", signal, (COUNT += 5));
    fflush(stdout);
    if (COUNT < 0) _exit(0);
}

void
handlerSURS2(int signal)
{
    printf("%d %d\n", signal, (COUNT -= 4));
    fflush(stdout);
    if (COUNT < 0) _exit(0);
}

int
main(void)
{
    int signals[SIGAMOUNT] = {SIGUSR1, SIGUSR2};
    void (*handlers[SIGAMOUNT])(int) = {handlerSURS1, handlerSURS2};
    for (int i = 0; i < SIGAMOUNT; i++){
        struct sigaction ss;
        ss.sa_flags = SA_RESTART;
        ss.sa_handler = handlers[i];
        sigaction(signals[i], &ss, NULL);
    }
    printf("%d\n", getpid());
    fflush(stdout);
    while (1) pause();
    return 0;
}
