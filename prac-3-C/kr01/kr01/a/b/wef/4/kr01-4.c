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
    YEAR_SHIFT = 1900,
    SATURDAY = 6,
    SUNDAY = 0,
    DELTA = 6,
    WEEK = 7,
};

int
main(int argc, char *argv[])
{
    int years, y, m, d;
    struct tm t;
    memset(&t, 0, sizeof(t));
    scanf("%d-%d-%d %d", &y, &m, &d, &years);
    t.tm_year = y - YEAR_SHIFT;
    t.tm_mon  = m - 1;
    t.tm_mday = d;
    mktime(&t);
    for (int i = 0; i < years * 2; i++) {
        t.tm_mon += DELTA;
        mktime(&t);
        int wd = t.tm_wday;
        if (wd == SATURDAY || wd == SUNDAY) {
            int dwd = (WEEK - wd + 1) % WEEK;
            t.tm_mday += dwd;
            mktime(&t);
            printf("%d-%02d-%02d\n", t.tm_year + YEAR_SHIFT, t.tm_mon + 1, t.tm_mday);
            t.tm_mday -= dwd;
            mktime(&t);
        } else {
            printf("%d-%02d-%02d\n", t.tm_year + YEAR_SHIFT, t.tm_mon + 1, t.tm_mday);
        }
    }
    return 0;
}

