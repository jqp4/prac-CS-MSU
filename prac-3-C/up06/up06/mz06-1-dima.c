#include <time.h>
#include <stdio.h>
#include <stdlib.h>

enum
{
    YEAR = 1900,
    MON = 12,
    DAY = 31
};


int 
main(void)
{
    int year;
    scanf("%d", &year);
    struct tm *calend = calloc(1, sizeof(*calend));
    calend->tm_year = year - YEAR;
    calend->tm_mon = 0;
    calend->tm_mday = 1;
    calend->tm_isdst = -1;
    for(int i = 0; i < MON; ++i) {
        calend->tm_mon = i;
        mktime(calend);
        int tmp;
        if (calend->tm_wday <= 4) {
            tmp = 4 - calend->tm_wday;
        } else {
            tmp = 4 - calend->tm_wday + 7;
        }
        int day = calend->tm_mday + tmp + 7;
        if (day % 3 != 0 && day <= DAY) {
            printf("%d %d\n", calend->tm_mon + 1, day);          
        }
        day += 14;
        if (day % 3 != 0 && day <= DAY) {
            printf("%d %d\n", calend->tm_mon + 1, day);
        }
    }
    free(calend);
}
