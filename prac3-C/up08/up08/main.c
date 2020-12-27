#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <string.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <fcntl.h>

const char *root = "/Users/gleb/Projects/prac/up08/up08/tests/";
int fdinp, fdout, fderr;
char buff[100];

char *filepath(char *filename){
    buff[0] = '\0';
    strcat(buff, root);
    strcat(buff, filename);
    return buff;
}


int main1(void){
    int N;
    scanf("%d", &N);
    for (int i = 1; i < N; i++) {
        pid_t pid = fork();
        dup2(fdinp, 0);
        dup2(fdout, 1);
        dup2(fderr, 2);
        if (pid) {
            printf("%d ", i);
            waitpid(pid, NULL, 0);
            return 0;
        }
    }
    printf("%d\n", N);
    return 0;
}


int main2(void){
    int x;
    printf("evwve\n");
    scanf("%d", &x);
    printf("%d\n", x);
    fprintf(stderr, "qewfffff");
    return 0;
}


int main(void){
    fdinp = open(filepath("f_inp.txt"), O_RDONLY);
    fdout = open(filepath("f_out.txt"), O_WRONLY | O_CREAT | O_TRUNC, 0660);
    fderr = open(filepath("f_err.txt"), O_WRONLY | O_CREAT | O_TRUNC, 0660);
    dup2(fdinp, 0);
    dup2(fdout, 1);
    dup2(fderr, 2);
    
    main2();
    
    close(fdinp);
    close(fdout);
    close(fderr);
    
    
    
     
    
    return 0;
}

