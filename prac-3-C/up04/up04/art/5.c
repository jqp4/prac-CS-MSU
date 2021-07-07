#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

extern unsigned long long
hash_function(unsigned long long);

enum
{
    DEC = 10,
    HEX = 16,
    ARG_NUM = 3
};

int
main(int argc, char *argv[])
{
    if (argc < ARG_NUM + 1) {
        fprintf(stderr, "invalid arguments\n");
        return 1;
    }
    int des = open(argv[1], O_RDWR | O_CREAT, 0660);
    if (des == -1) {
        fprintf(stderr, "open error\n");
        return 1;
    }
    unsigned long long h = strtoull(argv[2], NULL, HEX);
    if (errno == ERANGE) {
        fprintf(stderr, "overflow with hash number\n");
        return 1;
    }
    off_t count = strtoull(argv[3], NULL, DEC);
    if (errno == ERANGE) {
        fprintf(stderr, "overflow with count number\n");
        return 1;
    }
    off_t cur = lseek(des, 0, SEEK_END);
    if (cur == -1) {
        fprintf(stderr, "lseek error1\n");
        return 1;
    }
    while (count--) {
        if(lseek(des, cur + sizeof(h) * count, SEEK_SET) == -1) {
            fprintf(stderr, "lseek error\n");
            return 1;
        }
        int s = 0;
        while (s != sizeof(h)) {
            int k;
            k = write(des, &h + s, sizeof(h) - s);
            if (k == -1) {
                fprintf(stderr, "write error\n");
                return 1;
            }
            s += k;
        }
        h = hash_function(h);
    }
    close(des);
    return 0;
}
