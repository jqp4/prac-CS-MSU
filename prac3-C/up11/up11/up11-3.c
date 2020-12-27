#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

enum
{
    DECBASE = 10,
    ARGSHIFT = 2,
};
 
void
par(int n, char *argv[], int *count)
{
    int status;
    for (int i = 0; i < n; i++) {
        if (!fork()) {
            char cmd[PATH_MAX];
            FILE *fin = fopen(argv[i + ARGSHIFT], "r");
            fgets(cmd, sizeof(cmd), fin);
            cmd[strlen(cmd) - 1] = 0;
            fclose(fin);
            execlp(cmd, cmd, NULL);
            _exit(1);
        }
    }
    while (wait(&status) > 0) {
        *count += WIFEXITED(status) && !WEXITSTATUS(status) ? 1 : 0;
    }
}
 
void
seq(int n, int argc, char *argv[], int *count)
{
    int pid, status;
    for (int i = n + ARGSHIFT; i < argc; i++) {
        if (!(pid = fork())) {
            char cmd[PATH_MAX];
            FILE *fin = fopen(argv[i], "r");
            fgets(cmd, sizeof(cmd), fin);
            cmd[strlen(cmd) - 1] = 0;
            fclose(fin);
            execlp(cmd, cmd, NULL);
            _exit(1);
        }
        waitpid(pid, &status, 0);
        *count += WIFEXITED(status) && !WEXITSTATUS(status) ? 1 : 0;
    }
}
 
int
main(int argc, char *argv[])
{
    char *end;
    int count = 0;
    int n = (int)strtol(argv[1], &end, DECBASE);
    par(n, argv, &count);
    if (n <= argc + ARGSHIFT) {
        seq(n, argc, argv, &count);
    }
    printf("%d\n", count);
    return 0;
}
