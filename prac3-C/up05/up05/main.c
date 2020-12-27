#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>


int
main(int argc, char *argv[])
{
    int x = 1;
    char *y = (char*)&x;
    int LE = y[0];
    
    
    unsigned short x16;
    unsigned min = INT_MAX;
    int fd = open(argv[1], O_RDONLY);
    
    if (LE){
        unsigned char ss[2];
        while (read(fd, &ss, sizeof(ss)) == sizeof(ss)){
            x16 = ((unsigned short)ss[1] << 8) + (unsigned short)ss[0] ;
            if (x16 % 2 == 0 && x16 < min){
                min = x16;
            }
        }
    } else {
        while (read(fd, &x16, sizeof(x16)) == sizeof(x16)){
            if (x16 % 2 == 0 && x16 < min){
                min = x16;
            }
        }
    }
    
    
    
    if (min != INT_MAX){
        printf("%u\n", min);
    }
    close(fd);
    return 0;
}
