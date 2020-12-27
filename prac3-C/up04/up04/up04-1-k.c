#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h> 
#include <limits.h>
enum { CSIZE = sizeof(unsigned short) };

int
main(int argc, char *argv[])
{
    unsigned short cur;
    unsigned char cons[CSIZE];
    int file_discriptor = creat(argv[1], 0600);
    while (scanf("%hu", &cur) != EOF){
        cons[0] = cur >> CHAR_BIT;
        cons[1] = cur;
        if (write(file_discriptor, cons, CSIZE) != CSIZE) {
            close(file_discriptor);
            return 1;
        }
    }
    close(file_discriptor);
    return 0;
}