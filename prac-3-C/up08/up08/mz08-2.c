#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <stdlib.h>

int main(void){
    int status = 0;
    pid_t pid1 = fork();
    if (!pid1) {
        pid_t pid2 = fork();
        if (!pid2) {
            printf("3 ");
        } else {
            waitpid(pid2, &status, WUNTRACED);
            printf("2 ");
        }
    } else {
        waitpid(pid1, &status, WUNTRACED);
        printf("1\n");
    }
    return 0;
}
