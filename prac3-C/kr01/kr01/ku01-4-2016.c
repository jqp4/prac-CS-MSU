#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdio.h>
#include <math.h>

// https://seminar.ruro.live/ejudge1/0066

enum
{
    POINT = 46,
    SLASH = 47,
    LOSIZE = 6,
};


struct FileEntry
{
    unsigned short len;
    int offset;
    char str[];
};

void printFileEntry(struct FileEntry *fe) {
    printf("len    = %d\n", fe->len);
    printf("offset = %d\n", fe->offset);
    printf("str[]  = \"%s\"\n", fe->str);
    printf("---------------\n");
}

void readFileEntry(int fd, struct FileEntry *fe){
    read(fd, &fe->len, sizeof(fe->len));
    read(fd, &fe->offset, sizeof(fe->offset));
    read(fd, &fe->str, fe->len);
    fe->str[fe->len] = 0;
}


int getMaxStr(int fd, struct FileEntry *fe){
    readFileEntry(fd, fe);
    printFileEntry(fe);
    if (fe->offset){
        //printf("delta = %d\n", fe->offset - fe->len - LOSIZE);
        lseek(fd, fe->offset - fe->len - LOSIZE, SEEK_CUR);
        struct FileEntry newfe;
        getMaxStr(fd, &newfe);
    }
    return 2;
}



int
main(int argc, char *argv[])
{
    int fd = open(argv[1], O_RDONLY, 0660);
    struct FileEntry fe;
    getMaxStr(fd, &fe);
    
    
    
    
    return 0;
}

