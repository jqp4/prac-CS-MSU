#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

int g = 0;

void
handler(int s)
{
    g = s == SIGINT ? 0 : (s == SIGQUIT ? 1 : s);
    signal(SIGQUIT, handler);
    signal(SIGINT, handler);
}

int
main(void)
{
    signal(SIGQUIT, handler);
    signal(SIGINT, handler);
    printf("%d\n", getpid());
    fflush(stdout);
    int x;
    int ans = 0;
    while (scanf("%d", &x) == 1) {
        if (g) __builtin_smul_overflow(ans, x, &ans);
        else __builtin_sadd_overflow(ans, x, &ans);
        printf("%d\n", ans);
        fflush(stdout);
    }
}
