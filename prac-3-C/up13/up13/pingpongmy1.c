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
#include <sys/eventfd.h>

enum
{
    HEXBASE = 16,
    DECBASE = 10,
};

void work(uint64_t *ptr, int rfd, int wfd, uint64_t maxval){
    int pid = getpid();
    while (*ptr < maxval) {
        uint64_t res;
        if (-1 == read(rfd, &res , sizeof(res))) {}
        
        printf("%d %llu\n", pid, (unsigned long long)*ptr);
        fflush(stdout);
        ++*ptr;
        
        
        res = 1;
        if (-1 == write(wfd, &res , sizeof(res))) {}
    }
    printf("end of son %d\n", pid);
}

int main(int argc, char *argv[]){
    //uint64_t nproc = strtol(argv[1], NULL, DECBASE);
    //key_t key = strtol(argv[2], NULL, HEXBASE);
    uint64_t maxval = strtol(argv[3], NULL, DECBASE);
    uint64_t *mem = mmap(NULL, sizeof(mem), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    int fd1 = eventfd(1, 0);
    int fd2 = eventfd(0, 0);
    pid_t p1, p2;
    
    
    if (!(p1 = fork())) {
        work(mem, fd1, fd2, maxval);
        _exit(0);
    }
    if (!(p2 = fork())) {
        work(mem, fd2, fd1, maxval);
        _exit(0);
    }
    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);
    printf("end of father\n");
}
