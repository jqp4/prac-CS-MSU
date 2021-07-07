#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdlib.h>
#include <fcntl.h>

enum
{
    NAMESIZE = 16,
    PENNYC = 100,
    MINUS = '-',
};

struct Purchase
{
    char name[NAMESIZE];
    int cost;
};

int
sortPs(int *f, int *s, int pc, unsigned ns)
{
    if (pc > *f || ns == 0){
        *s = *f;
        *f = pc;
        return 1;
    } else if (pc == *f){
        return 0;
    } else if (pc > *s || ns == 1){
        *s = pc;
        return 1;
    }
    return 0;
}

int
main(int argc, char *argv[])
{
    int first, second;
    struct Purchase p;
    unsigned nsorts = 0;
    first = second = INT_MIN;
    for (int i = 1; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY, S_IRWXU);
        while (read(fd, &p, sizeof(p)) == sizeof(p)) {
            nsorts += sortPs(&first, &second, p.cost, nsorts);
        }
        close(fd);
    }
    if (nsorts >> 1) {
        long long ans = (long long)second;
        ans *= second >= 0 ? 1 : - putchar(MINUS) / MINUS;
        printf("%lld.%02lld\n", ans / PENNYC, ans % PENNYC);
    }
    return EXIT_SUCCESS;
}







