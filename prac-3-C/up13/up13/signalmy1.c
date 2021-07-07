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
#include <sys/signalfd.h>

int main(void){
    sigset_t ss;
    sigemptyset(&ss);
    sigaddset(&ss, SIGINT);
    sigprocmask(SIG_BLOCK, &ss, NULL);
    int sfd = signalfd(-1, &ss, 0);
    
    
    
    for (int i = 0; i < 5; i++){
        struct signalfd_siginfo sss;
        if (-1 == read(sfd, &sss, sizeof(sss))) {}
        printf("   received SIGINT\n");
    }
    printf("end of father\n");
}





/*while (1) {
    int c;
    printf(" > ");
    scanf("%d", &c);
    if (c == 1) raise(SIGUSR1);
    else if (c == 2) raise(SIGUSR2);
}*/
