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

void work(int name, uint64_t *ptr, int rfd, int wfd, uint64_t maxval){
    while (*ptr < maxval) {
        uint64_t res;
        if (-1 == read(rfd, &res , sizeof(res))) {}
        
        printf("%d: %llu\n", name, (unsigned long long)*ptr);
        fflush(stdout);
        ++*ptr;
        
        res = 1;
        if (-1 == write(wfd, &res , sizeof(res))) {}
    }
    printf("end of son %d, %d\n", name, getpid());
}

int main(int argc, char *argv[]){
    uint64_t nproc = 2;//strtol(argv[1], NULL, DECBASE);
    //key_t key = strtol(argv[2], NULL, HEXBASE);
    uint64_t maxval = strtol(argv[3], NULL, DECBASE);
    uint64_t *mem = mmap(NULL, sizeof(mem), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    int fd1 = eventfd(1, 0);
    int fd2 = eventfd(0, 0);
    pid_t sonpid[nproc];
    
    for (int i = 0; i < nproc; i++){
        if (!(sonpid[i] = fork())) {
            work(i, mem, fd1, fd2, maxval);
            _exit(0);
        }
    }
    for (int i = 0; i < nproc; i++){
        //waitpid(sonpid[i], NULL, 0);
        wait(NULL);
    }
    printf("end of father\n");
}
