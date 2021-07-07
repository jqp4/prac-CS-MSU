#include <time.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

// https://seminar.ruro.live/ejudge1/0063

enum
{
    GROUP = 7,
    MASK = (1 << GROUP) - 1,
};

int main(void) {
    u_int32_t x, y;
    while (scanf("%u", &x) == 1 && x != EOF) {
    //while (write(STDIN_FILENO, &x, sizeof(x)) == sizeof(x) && x != EOF) {
        do {
            y = x & MASK;
            y |= (x >>= GROUP) ? 1 << GROUP : 0;
            printf("%02x ", y);
        } while (x);
        putchar('\n');
    }
    return 0;
}
