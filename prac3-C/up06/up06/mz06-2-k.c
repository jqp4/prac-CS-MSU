#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>

enum { WDELTA = 2 };
const char* suf = ".exe";
int
checkfname(const char *name) {
    size_t l = strlen(name);
    if (l < 4) {
        return 0;
    }
    for (size_t i = l-4, j = 0; i < l; ++i, ++j) {
        if (name[i] != suf[j]) {
            return 0;
        }
    }
    return 1;
}

int
main(int argc, char *argv[])
{
    unsigned int cnt = 0;
    DIR *d = opendir(argv[1]);
    if (!d) {
        fprintf(stderr, "opendir: %s\n", strerror(errno));
        return 1;
    }
    struct dirent *dd;
    char input_filename[PATH_MAX+1];
    while ((dd = readdir(d))) {
        struct stat file;
        snprintf(input_filename, strlen(argv[1]) + strlen(dd->d_name) + WDELTA ,"%s/%s", argv[1], dd->d_name);
        if (lstat(input_filename, &file) == 0){
            if (S_ISREG(file.st_mode) && !access(input_filename, X_OK) && checkfname(dd->d_name)) {
                cnt ++;
            }
        }
    }
    closedir(d);
    printf("%u\n", cnt);
    return 0;
}

