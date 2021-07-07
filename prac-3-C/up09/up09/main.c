#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>

enum
{
    FNSHIFT = 2,
    OPENERR = -1,
    RETVERR = 42,
    MODE = 0660,
    N = 3,
};

int main(int argc, char *argv[]){
    int pid, status;
    if (!(pid = fork())) {
        int fd1 = open(argv[2], O_RDONLY);
        int fd2 = open(argv[3], O_WRONLY | O_CREAT | O_APPEND, MODE);
        int fd3 = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, MODE);
        if (fd1 == OPENERR || fd2 == OPENERR || fd3 == OPENERR) {
            fprintf(stderr, "file open error\n");
            return RETVERR;
        }
        if (dup2(fd1, 0) == -1 || dup2(fd2, 1) == -1 || dup2(fd3, 2) == -1) {
            fprintf(stderr, "dup2 error\n");
            return RETVERR;
        }
        close(fd1);
        close(fd2);
        close(fd3);
        if (execlp(argv[1], argv[1], NULL) == -1){
            fprintf(stderr, "execlp error\n");
            return RETVERR;
        }
        for (int i = 1; i <= N; i++) {
            close(i);
        }
        exit(0);
    }
    waitpid(pid, &status, 0);
    fprintf(stdout, "%d\n", status);
    return 0;
}
