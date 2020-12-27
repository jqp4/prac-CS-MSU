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

enum
{
    POINT = 46,
    SLASH = 47,
    LOSIZE = 6,
    DECBASE = 10,
    NUMLEN = 16,
    COMMA = 44,
};


int check(unsigned long len, char *s){
    for (int i = 0; i < len; i++) {
        if (s[i] < 'a' || s[i] > 'z') {
            return 0;
        }
    }
    unsigned long half = len / 2 + len % 2;
    for (int i = 0; i < half; i++) {
        if (s[i] != s[len - 1 -i]) {
            return 0;
        }
    }
    return 1;
}


int
main(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++){
        unsigned long len = strlen(argv[i]);
        if (len && check(len, argv[i])){
            printf("%s\n", argv[i]);
        }
    }
    
    
    return 0;
}

