#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void printPids(int pson, int y){
    printf("pid = %2d | ppid = %2d | son = %2d  :  %2d\n",
           getpid() % 100, getppid() % 100, pson % 100, y);
}
void printDyingPid(void){
    printf("kill pid %2d\n", getpid() % 100);
}


int
main(void)
{
    printPids(-1, -1);
    printf("--------------------------------------\n");
    int N;
    N = 5;//   scanf("%d", &N);
    for (int i = 1; i < N; i++) {
        pid_t pid = fork();
        if (pid) {
            //printf("%d ", i);
            printPids(pid, i);
            waitpid(pid, NULL, 0);
            printDyingPid();
            return 0;
        }
    }
    //printf("%d\n", N);
    printPids(-1, N);
    printDyingPid();
    return 0;
}

