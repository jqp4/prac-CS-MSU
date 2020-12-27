#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>

enum
{
    HEXBASE = 16,
    DECBASE = 10,
    MODE = 0600,
};

void work(int semid, u_int64_t *ptr, int *sender, int name, int64_t maxval, int nproc){
    while (1) {
        struct sembuf b1[] = { { .sem_num = name, -1, 0 }, };
        int r = semop(semid, b1, sizeof(b1) / sizeof(b1[0]));
        if (r < 0) _exit(0);
        printf("%d %llu %d\n", name, (unsigned long long) *ptr, *sender);
        fflush(stdout);
        
        if (++(*ptr) > maxval) {
            semctl(semid, 0, IPC_RMID);
            _exit(0);
        }
        
        *sender = name;
        u_int64_t v = (*ptr) % nproc;
        int next = (v * ((v * v * v) % nproc)) % nproc + 1;
        struct sembuf b2[] = { { .sem_num = next, 1, 0}, };
        r = semop(semid, b2, sizeof(b2) / sizeof(b2[0]));
        if (r < 0) _exit(0);
    }
}


int main(int argc, char *argv[])
{
    int nproc = (int) strtol(argv[1], NULL, DECBASE);
    key_t key = (int) strtol(argv[2], NULL, HEXBASE);
    u_int64_t maxval = strtol(argv[3], NULL, DECBASE);
    int semid = semget(key, nproc + 1, MODE | IPC_CREAT | IPC_EXCL);
    semctl(semid, 1, SETVAL, 1);
    u_int64_t *ptr = mmap(NULL, sizeof(ptr), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    int *sender = mmap(NULL, sizeof(sender), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    
    for (int i = 0; i < nproc; i++) {
        if (!fork()) {
            work(semid, ptr, sender, i + 1, maxval, nproc);
        }
    }
    
    for (int i = 0; i < nproc; i++) {
        wait(NULL);
    }
    
    munmap(ptr, sizeof(ptr));
    munmap(sender, sizeof(sender));
    return 0;
}

