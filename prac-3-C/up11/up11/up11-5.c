#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

enum
{
    MAXSICOUNT = 3,
    SIGAMOUNT = 2,
    MINSIMPLE = 2,
};

volatile sig_atomic_t SIMPX = 0;
volatile sig_atomic_t SICOUNT = 0;

int
isSimple(int n)
{
    int maxi = sqrt(n) + 1;
    for (int i = MINSIMPLE; i <= maxi; i++) {
        if (!(n % i)) return 0;
    }
    return 1;
}

void
handlerINT(int signal)
{
    if (SICOUNT < MAXSICOUNT) {
        SICOUNT++;
        printf("%d\n", SIMPX);
        fflush(stdout);
    } else _exit(0);
}

void
handlerTERM(int signal)
{
    _exit(0);
}

int
main(void)
{
    int signals[SIGAMOUNT] = {SIGINT, SIGTERM};
    void (*handlers[SIGAMOUNT])(int) = {handlerINT, handlerTERM};
    for (int i = 0; i < SIGAMOUNT; i++){
        struct sigaction ss;
        ss.sa_flags = SA_RESTART;
        ss.sa_handler = handlers[i];
        sigaction(signals[i], &ss, NULL);
    }
    
    int low, high;
    scanf("%d%d", &low, &high);
    printf("%d\n", getpid());
    fflush(stdout);

    for (int i = low < MINSIMPLE ? MINSIMPLE : low; i < high; i++) {
        if (isSimple(i)) SIMPX = i;
    }
    
    printf("-1\n");
    fflush(stdout);
    return 0;
}
