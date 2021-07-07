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
    MODE = 0600,
};

void work(int semid, int64_t *ptr, int64_t *sender, int name, uint64_t maxval, int nproc)
{
    while (1) {
        struct sembuf b1 = { .sem_num = name, -1, 0 };
        if (0 < semop(semid, &b1, 1)) break;
        
        printf("%d %lld %lld\n", name, (long long) *ptr, (long long) *sender);
        fflush(stdout);
        ++(*ptr);
        if ((*ptr) > maxval) {
            semctl(semid, 0, IPC_RMID);
            break;
        }
        
        
        
        
        int64_t v = (*ptr) % nproc;

        struct sembuf b2 = { .sem_num = (v * v * v * v) % nproc + 1, 1, 0 };
        if (0 < semop(semid, &b2, 1)) break;
        *sender = name;
    }
    /*while (*ptr == maxval) {
        //if (semctl(semid, name, GETVAL) == 1) {
            struct sembuf b = { .sem_num = name, -1, 0 };
            int r = semop(semid, &b, 1);
            if (r < 0) { printf("%d, semop1\n", name); exit(1); }
            printf("%d %lld %lld\n", name, (long long) *ptr, (long long) *sender);
            fflush(stdout);
            ++(*ptr);
        //}
    }*/
}


int main(int argc, char *argv[])
{
    int nproc = (int) strtol(argv[1], NULL, DECBASE);
    key_t key = (int) strtol(argv[2], NULL, HEXBASE);
    uint64_t maxval = strtol(argv[3], NULL, DECBASE);
    int semid = semget(key, nproc + 1, MODE | IPC_CREAT | IPC_EXCL);
    semctl(semid, 1, SETVAL, 1);
    int64_t *ptr = mmap(NULL, sizeof(ptr), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    int64_t *sender = mmap(NULL, sizeof(ptr), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    
    for (int i = 0; i < nproc; i++){
        if (!fork()) {
            work(semid, ptr, sender, i + 1, maxval, nproc);
            _exit(0);
        }
    }
    for (int i = 0; i < nproc; i++){
        wait(NULL);
    }
    
    munmap(ptr, sizeof(ptr));
    munmap(sender, sizeof(sender));
    semctl(semid, 0, IPC_RMID);
    return 0;
}


struct Shmseg
{
    long long int parent;
    long long int idxer;
};



int
main_(int argc, char *argv[])
{
    int semid;
    int n;
    sscanf(argv[1], "%d", &n);
    int key;
    sscanf(argv[2], "%d", &key);
    long long int maxval;
    sscanf(argv[3], "%lld", &maxval);

    if ((semid = semget(key, n, IPC_CREAT | IPC_EXCL | 0600))<0) {
        // fprintf(stderr, "Cant get semget\n");
        _exit(1);
    }
    // long long int *info; // info = {pval, parrant}
    struct Shmseg *info =  mmap(NULL, sizeof(*info), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    int64_t *ptr = mmap(NULL, sizeof(ptr), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    int64_t *sender = mmap(NULL, sizeof(ptr), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    info->idxer = 0;
    info->parent = 0;
    for (long long i = 0; i < n; ++i) {
        int pid;
        if (!(pid = fork())) {
            while (1){
                struct sembuf buf = { .sem_num = i, -1, 0};
                if (semop(semid, &buf, 1)<0) {
                    break;
                }
                printf("%lld %ld %ld\n", i+1, *ptr, *sender);
                fflush(stdout);
                (*ptr)++;
                if ((*ptr) > maxval) {
                    semctl(semid, 0, IPC_RMID);
                    break;
                }
                *sender = i + 1;
                int64_t v = (*ptr) % n;
                struct sembuf next_buf = {.sem_num = (v * v * v * v) % n, 1, 0};
                if ((semop(semid, &next_buf, 1))<0) {
                    break;
                }
                // free(&buf);
                // free(&next_buf);
            }
            exit(0);
        }
    }
    semctl(semid, 0, SETVAL, 1);
    while (wait(NULL) != -1) {}
    // free(info);
    return 0;


}





