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



int main(int argc, char *argv[]) {
    int x;
    while (scanf("%d", &x) > 0) {
        printf(x > 5 ? "summon\n" : "disqualify\n");
    }
    return 0;
}
