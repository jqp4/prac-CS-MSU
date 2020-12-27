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

// https://seminar.ruro.live/ejudge1/0065

enum
{
    POINT = 46,
    SLASH = 47,
};


void
normalize_path(char *buf)
{
    char *s = buf;
    while (*(s++)) {}
    for (int skips = *(s -= 2) == SLASH ? 1 : 0; s > buf; s--) {
        if (*s == POINT){
            skips++;
        } else if (*s == SLASH && skips){
            *s = POINT;
            skips--;
        } else {
            *s = skips ? POINT : *s;
        }
    }
    char *r = buf;
    while (*s) {
        if (*s != POINT){
            *r = *s;
            r++;
        }
        s++;
    }
    *r = 0;
}

int
main(int argc, char *argv[])
{
    /*for (int i = 1; i < argc; i++){
        char *list = argv[i];
        char *pch;
        printf("len = %2lu : \"%s\"\n", strlen(list), list);
        printf("Looking for the '%c' character in \"%s\"\n", SLASH, list);
        pch = strchr(list, SLASH);
        while (pch != NULL) {
            printf("found at %ld\n", pch - list + 1);
            pch = strchr(pch + 1, SLASH);
        }
        printf("done\n");
    }*/
    /*for (int i = 1; i < argc; i++){
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
    }*/
    for (int i = 1; i < argc; i++){
        printf("%s\n", argv[i]);
        normalize_path(argv[i]);
        printf("%s\n", argv[i]);
        printf("-----------\n");
    }
    return 0;
}

//   /a/b/c/ /a/b/../ /a/b/../../.. /a/b/c/./../d/e/.././
