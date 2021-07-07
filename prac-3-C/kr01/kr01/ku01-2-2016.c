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

// https://seminar.ruro.live/ejudge1/0064

enum
{
    DECBASE = 10,
    NUMLEN = 16,
    COMMA = 44,
};

int
main(int argc, char *argv[])
{
    /*if (argc < MIN_ARGC) {
        fprintf(stderr, "invalid input\n");
        return 1;
    }
    char *end;
    double rate = strtod(argv[RATE_ARGN], &end);
    if (argc - MIN_ARGC) {
        for (int i = MIN_ARGC; i < argc; i++) {
            double perc = strtod(argv[i], &end);
            rate = rate * (FPERCENT + perc) / FPERCENT;
            rate = round(rate * FACTOR) / FACTOR;
        }
    }
    fprintf(stdout, "%.4lf\n", rate);*/
    /*for (int i = 1; i < argc; i++){
        char *list = argv[i];
        char *pch;
        printf("len = %2lu : \"%s\"\n", strlen(list), list);
        printf("Looking for the '%c' character in \"%s\"\n", COMMA, list);
        pch = strchr(list, COMMA);
        while (pch != NULL) {
            printf("found at %ld\n", pch - list + 1);
            pch = strchr(pch + 1, COMMA);
        }
        printf("done\n");
    }*/
    for (int i = 1; i < argc; i++){
        long min = INT_MAX;
        long max = INT_MIN;
        unsigned long len = strlen(argv[i]) + 2;
        char list[len], strx[NUMLEN], *pcomma, *end;
        snprintf(list, len, "%s,", argv[i]);
        char *plist = list;
        list[len] = 0;
        pcomma = strchr(list, COMMA);
        while (pcomma != NULL) {
            strncpy(strx, plist, pcomma - plist);
            strx[pcomma - plist] = 0;
            long x = strtol(strx, &end, DECBASE);
            min = x < min ? x : min;
            max = x > max ? x : max;
            plist = pcomma + 1;
            pcomma = strchr(pcomma + 1, COMMA);
        }
        printf("%ld\n", max - min + 1);
    }
    return 0;
}

