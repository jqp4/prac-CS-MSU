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
    MAXLEN4 = 200,
    BASE4 = 4,
};


int decto4(long long dec){
    long long tmp;
    int base4num[MAXLEN4];
    int sign = dec > 0 ? 1 : -1;
    tmp = dec * sign;
    int sum = 0;
    int i = 1;
    while (tmp) {
        base4num[i++] = tmp % BASE4;
        tmp /= BASE4;
    }
    for (int j = i - 1; j > 0; j--) {
        sum += base4num[j];
    }
    return sum;
}

int main(int argc, char *argv[]) {
    char *end;
    pid_t pids[argc];
    if (!(pids[0] = fork())) _exit(0); // фиктивный нулевой сын
    for (int i = 1; i < argc; i++) {
        long long x = strtoll(argv[i], &end, DECBASE);
        if (!(pids[i] = fork())) {
            int sum = decto4(x);
            waitpid(pids[i - 1], NULL, 0);
            printf("%d\n", sum);
            fflush(stdout);
            _exit(0);
        }
    }
    return 0;
}
