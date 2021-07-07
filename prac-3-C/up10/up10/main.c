#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>


int
main(void)
{
    int fd[2];
    pipe(fd);
    pid_t pid1 = fork();
    if (pid1) {
        //отец
        int32_t c;
        /*int done = 0;
        while (!done) {
            scanf("%d", &c);
            write(fd[1], &c, sizeof(c));
            if (c == EOF) {
                done = 1;
            }
        }*/
        while(scanf("%d", &c) != EOF) {
            write(fd[1], &c, sizeof(c));
        }
        waitpid(pid1, NULL, 0);
        
        close(fd[1]);
        return 0;
    } else {
        // сын
        pid_t pid2 = fork();
        if (!pid2) {
            //внук
            int32_t c;
            //int done = 0;
            long long sum = 0;
            /*while (!done) {
                read(fd[0], &c, sizeof(c));
                if (c == EOF) {
                    done = 1;
                } else {
                    sum += c;
                }
            }*/
            
            
            //while(read(fd[0], &c, sizeof(c)) == sizeof(c)) {
            //    sum += c;
            //}
            
            
            
            close(fd[0]);
            printf("<%lld>\n", sum);
            return 0;
        }
        waitpid(pid2, NULL, 0);
        return 0;
    }
}
