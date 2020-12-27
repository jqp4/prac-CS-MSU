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
    HEXBASE = 16,
    DECBASE = 10,
};

void work(int semid, int64_t *ptr, int s1, int s2)
{
    int nnn = 5;
    while (nnn--) {
        struct sembuf b1[] =
        {
            { .sem_num = s1, -1, 0 },
        };
        int r = semop(semid, b1, sizeof(b1) / sizeof(b1[0]));
        if (r < 0) { printf("semop1\n"); exit(1); }
        
        printf("%d %lld\n", getpid(), (long long) *ptr);
        fflush(stdout);
        ++(*ptr);

        struct sembuf b2[] =
        {
            { .sem_num = s2, 1, 0 },
        };
        r = semop(semid, b2, sizeof(b2) / sizeof(b2[0]));
        if (r < 0) { printf("semop2\n"); exit(1); }
    }
    printf("end of son %d\n", getpid());
}

int main_(int argc, char *argv[])
{
    //uint64_t nproc = strtol(argv[1], NULL, DECBASE);
    key_t key = strtol(argv[2], NULL, HEXBASE);
    //uint64_t maxval = strtol(argv[3], NULL, DECBASE);

    int semid = semget(key, 3, 0600 | IPC_CREAT | IPC_EXCL);

    semctl(semid, 1, SETVAL, 1);

    int64_t *ptr = mmap(NULL, sizeof(ptr), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);

    if (!fork()) {
        work(semid, ptr, 1, 2);
        _exit(0);
    }

    if (!fork()) {
        work(semid, ptr, 2, 1);
        _exit(0);
    }

    wait(NULL);
    wait(NULL);
    semctl(semid, 0, IPC_RMID);
    printf("end of father\n");
    return 0;
}




void show(int semid, int len){
    unsigned short a[len];
    semctl(semid, 0, GETALL, a);
    for (int i = 0; i < len; i++){
        printf("%d ", a[i]);
    }
    printf("\n");
}



int main(int argc, char *argv[])
{
    uint64_t nproc = strtol(argv[1], NULL, DECBASE);
    key_t key = strtol(argv[2], NULL, HEXBASE);
    int semid = semget(key, nproc, 0600 | IPC_CREAT | IPC_EXCL);
    semctl(semid, 1, SETVAL, 1);
    show(semid, nproc);
    
    
    struct sembuf b1[] =
    {
        { .sem_num = 1, -1, 0 },
    };
    int r = semop(semid, b1, sizeof(b1) / sizeof(b1[0]));
    if (r < 0) { printf("semop1\n"); exit(1); }
    show(semid, nproc);
    
    
    //munmap(ptr, sizeof(ptr));
    semctl(semid, 0, IPC_RMID);
    printf("end of father\n");
    return 0;
}
