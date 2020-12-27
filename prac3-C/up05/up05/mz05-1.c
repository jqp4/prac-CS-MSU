#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>

enum
{
    KBYTE = 1024
};

int
main(int argc, char *argv[])
{
    off_t size = 0;
    for (int i = 1; i < argc; i++) {
        struct stat b;
        if (!lstat(argv[i], &b) && S_ISREG(b.st_mode) && !(b.st_size % KBYTE) && b.st_nlink == 1){
            size += b.st_size;
        }
    }
    printf("%lld\n", size);
    return 0;
}
