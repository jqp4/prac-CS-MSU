#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdint.h>
#include <sys/mman.h>

enum
{
    DECBASE = 10,
    BASE4 = 4,
};

int main(int argc, char *argv[]) {
    char *end;
    pid_t pids[argc];
    if (!(pids[0] = fork())) _exit(0); // фиктивный нулевой сын
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') argv[i][0] = '+';
        unsigned long long x = strtoll(argv[i], &end, DECBASE);
        if (!(pids[i] = fork())) {
            int sum = 0;
            while (x) {
                sum += x % BASE4;
                x /= BASE4;
            }
            waitpid(pids[i - 1], NULL, 0);
            printf("%d\n", sum);
            fflush(stdout);
            _exit(0);
        }
    }
    return 0;
}
