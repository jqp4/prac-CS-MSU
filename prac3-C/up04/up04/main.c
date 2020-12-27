#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

enum
{
    MODE = 0660,
    DECBASE = 10,
    HEXBASE = 16,
};

unsigned long long hash_function(unsigned long long);

int
main(int argc, char *argv[])
{
    int fd = open(argv[1], O_WRONLY | O_CREAT, MODE);
    char *end;
    unsigned long long h = strtoll(argv[2], &end, HEXBASE);
    unsigned long long count = strtoll(argv[3], &end, DECBASE);
    
    
    unsigned long long sizefile = lseek(fd, sizeof(h) * count, SEEK_END);
    for (int i = 0; i < count; i++){
        lseek(fd, sizefile - sizeof(h) * (i + 1), SEEK_SET);
        write(fd, &h, sizeof(h));
        h = hash_function(h);
    }
    return 0;
}
