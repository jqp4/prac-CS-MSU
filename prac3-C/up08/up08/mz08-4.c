#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int
main(void)
{
    int N;
    scanf("%d", &N);
    for (int i = 1; i < N; i++) {
        pid_t pid = fork();
        if (pid) {
            printf("%d\n", i);
            waitpid(pid, NULL, 0);
            return 0;
        }
    }
    printf("%d", N);
    return 0;
}
