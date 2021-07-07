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
    MIN_ARGC = 3,
    DECBASE = 10,
};

void were(unsigned long lenzero, char *dirname, long long maxsize, int depth) {
    DIR *D = opendir(dirname);
    struct dirent *rd;
    char name[PATH_MAX + 1];
    while ((rd = readdir(D)) != NULL) {
        unsigned long len = 2 + strlen(dirname) + strlen(rd->d_name);
        snprintf(name, len ,"%s/%s", dirname, rd->d_name);
        struct stat fb;
        if (lstat(name, &fb) == 0 && !S_ISLNK(fb.st_mode)){
            if (S_ISREG(fb.st_mode) && fb.st_size <= maxsize && access(name, 3)) {
                for (unsigned long i = lenzero; i < len - 1; i++){
                    putchar(name[i]);
                }
                putchar('\n');
            } else if (S_ISDIR(fb.st_mode) && rd->d_name[0] != '.' && depth < 4) {
                were(lenzero, name, maxsize, depth + 1);
            }
        }
    }
    closedir(D);
    R_OK
}

int
main(int argc, char *argv[])
{
    if (argc < MIN_ARGC) {
        fprintf(stderr, "invalid input\n");
        return 1;
    }
    char *end;
    char *dirname = argv[1];
    unsigned long lenzero = strlen(dirname);
    while (dirname[--lenzero]!= '/') {}
    long long maxsize = strtoll(argv[2], &end, DECBASE);
    were(lenzero, dirname, maxsize, 1);
    return 0;
}

