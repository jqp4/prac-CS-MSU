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
    
    
    BSIZE = 16,
    N_COP = 100,
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
    //int m[] = {INT_MIN, INT_MAX};
    int m[] = {INT_MIN, INT_MAX};
    int len = sizeof(m) / sizeof(int);
    for (int i = 0 ; i < len; i++) {
        p.cost = m[i];
        write(fd, &p, sizeof(p));
    }
    close(fd);
}

struct recepi
{
    char name[BSIZE];
    int cop;
};

int
maink(int argc, char *argv[])
{
    int file_discriptor;
    struct recepi cur;
    int max = INT_MIN, max2 = INT_MIN;
    unsigned int brd;
    int first = 1, second  = 0;
    for (int i = 1; i < argc; ++i) {
        if ((file_discriptor = open(argv[i], O_RDONLY)) == -1) {
            fprintf(stderr, "Error opening file\n");
            return 1;
        }
        while ((brd = read(file_discriptor, &cur, sizeof(cur))) == sizeof(cur)) {
            if (first || cur.cop > max) {
                second = !first;
                max2 = max;
                max = cur.cop;
                first = 0;
            } else if (cur.cop == max) {
                //pass
            } else if (cur.cop > max2 || !second) {
                max2 = cur.cop;
                second = 1;
            }
        }
        if (brd == -1) {
            fprintf(stderr, "Error reading file\n");
            return 1;
        }
        close(file_discriptor);
    }
    if (second) {
        int extra = max2 == INT_MIN;
        if (max2 < 0) {
            putchar('-');
            max2 = -(max2 + extra);
        }
        printf("k %d.%02d\n", max2 / N_COP, max2 % N_COP + extra);
    }
    return 0;
}








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
    generator(argc, argv);
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
        long ans = (long)second;
        ans *= second >= 0 ? 1 : - putchar(MINUS) / MINUS;
        printf("g %ld.%02ld\n", ans / PENNYC, ans % PENNYC);
    }
    maink(argc, argv);
    return EXIT_SUCCESS;
}







