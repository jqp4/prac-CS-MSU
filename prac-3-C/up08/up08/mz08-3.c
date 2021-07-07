#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <stdlib.h>

enum
{
    DEC_BASE = 10,
    LEN = 8,
    N = 3,
};

int main(void)
{
    pid_t pid[N];
    char *end, s[N][LEN];
    for (int i = 0; i < N; i++) {
        scanf("%s", s[i]);
    }
    
    for (int i = 0; i < N; i++) {
        if (!(pid[i] = fork())) {
            long x = strtol(s[i], &end, DEC_BASE);
            long x2 = x * x;
            printf("%d %ld\n", i + 1, x2);
            return 0;
        } else {
            int status = 0;
            wait(&status);
        }
    }
    return 0;
}
