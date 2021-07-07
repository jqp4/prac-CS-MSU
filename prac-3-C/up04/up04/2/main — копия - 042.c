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

#include <time.h>
//up04-2
//write(STDOUT_FILENO, p.name, NAMESIZE);
//printf(" %d\n", p.cost);
void generator(int argc, char *argv[]){
    srand((unsigned)time(NULL));
    struct Purchase p;
    p.name[0] = 'j';
    int fd = open(argv[1], O_WRONLY | O_TRUNC, S_IRWXU);
    int m[] = {INT_MIN, INT_MAX};
    int len = sizeof(m) / sizeof(int);
    for (int i = 0 ; i < len; i++) {
        p.cost = m[i];
        write(fd, &p, sizeof(p));
    }
    close(fd);
}

int
sortPs(int *f, int *s, int pc)
{
    if (pc > *f || *f == INT_MIN){
        *s = *f;
        *f = pc;
        return 1;
    } else if (pc == *f){
        return 0;
    } else if (pc > *s || *s == INT_MIN){
        *s = pc;
        return 1;
    }
    return 0;
}

int
main(int argc, char *argv[])
{
    generator(argc, argv);
    int first, second;
    struct Purchase p;
    unsigned nsorts = 0;
    first = second = INT_MIN;
    for (int i = 1; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY, S_IRWXU);
        while (read(fd, &p, sizeof(p)) == sizeof(p)) {
            nsorts += sortPs(&first, &second, p.cost);
        }
        close(fd);
    }
    if (nsorts >> 1) {
        long ans = (long)second;
        ans *= second >= 0 ? 1 : - putchar(MINUS) / MINUS;
        printf("%ld.%02ld\n", ans / PENNYC, ans % PENNYC);
    }
    return EXIT_SUCCESS;
}










