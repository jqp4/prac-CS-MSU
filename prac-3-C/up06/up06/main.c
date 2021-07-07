#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>


const char expansion[] = ".txt";

enum
{
    EXPANSION_LEN = 4,
};


int generator(int argc, char *argv[]){
    srand((unsigned)time(NULL));
    DIR *dir = opendir(argv[1]);
    for (int i = 0; i < 53; i++) {
        int lennamefnr = 4;//rand() % 100 + 1;
        char fnr[lennamefnr + EXPANSION_LEN + 1];
        for (int j = 0; j < lennamefnr; j++){
            fnr[j] = rand() % 95 + 32;
        }
        for (int j = 0; j < EXPANSION_LEN; j++){
            fnr[lennamefnr + j] = expansion[j];
        }
        //fnr[lennamefnr + EXPANSION_LEN + 1] = 0;
        char filename[PATH_MAX + 1];
        unsigned long len = strlen(argv[1]) + strlen(fnr) + 2;
        snprintf(filename, len, "%s/%s", argv[1], fnr);
        int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
        int lensttext = rand() % 100 + 70;
        char st[lensttext + 1];
        for (int j = 0; j < lensttext; j++){
            st[j] = rand() % 128;
        }
        write(fd, st, sizeof(st));
        close(fd);
    }
    closedir(dir);
    return 0;
}


int main(int argc, char *argv[]){
    DIR *dir = opendir(argv[1]);
    if (!dir) {
        fprintf(stderr, "invalid input: %s\n", strerror(errno));
        return 1;
    }
    
    
    generator(argc, argv);
    
    
    return 0;
}
