#include <unistd.h> // For write() and STDOUT_FILENO
#include <stdlib.h>   // For EXIT_SUCCESS and EXIT_FAILURE

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <string.h>

enum {
    MIN_ARGC = 2,
    BUFFSIZE = 2,
    FNAMEARG = 1,
    BINSHLEN = 16
};

/*const char *root = "/Users/gleb/Projects/prac/up03/up03/";
char buff[300];

char *filepath(char *filename){
    buff[0] = '\0';
    strcat(buff, root);
    strcat(buff, filename);
    return buff;
}
 
 while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0){
     //if (write(fout, buf, BUFFSIZE) != n){}
     x = atoi(buf);//(short)buf;
     printf("<%d>\n", x);
 }*/

int main(int argc, char *argv[]){
    if (argc < MIN_ARGC){
        char *message = "invalid input\n";
        write(STDOUT_FILENO, message, strlen(message));
        return 1;
    }
    unsigned short x;
    char buf[BUFFSIZE];
    char *filename = argv[FNAMEARG];
    int fout = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
    while (fscanf(stdin, "%hu", &x)){
        for (int i = BINSHLEN - 1; i >= 0; i--){
            *buf = (x & (1 << i--)) ? '1' : '0';
            *(buf + 1) = (x & (1 << i)) ? '1' : '0';
            write(fout, buf, BUFFSIZE);
        }
        *buf = ' ';
        write(fout, buf, 1);
    }
    close(fout);
    return EXIT_SUCCESS;
}


int main_(void){
    char buf[BUFFSIZE];
    int x1 = 1, x2 = 0;
    
    *buf = x1 ? '1' : '0';
    *(buf + 1) = x2 ? '1' : '0';
    
    write(STDOUT_FILENO, buf, BUFFSIZE);
    
    //unlink(filepath("wef.txt"));
    //remove(filepath("wef.txt"));
    //ssize_t n = write(fout, buf, BUFFSIZE);
    return 0;
}
