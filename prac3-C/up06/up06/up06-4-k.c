#include <time.h>
#include <stdio.h>
#include <stdlib.h>

enum
{
    START_YEAR = 1925,
    START_MON = 10,
    START_DAY = 7,
    MON_IN_YEAR = 12,
    DAY_IN_MON = 30,
    SEC_IN_DAY = 86400,
    N_PARAMS = 3
};

int
main(int argc, char *argv[])
{
    int yOld = 0, mOld = 0, dOld = 0;
    struct tm dateBegining = {0,0,0,START_DAY,START_MON,START_YEAR};
    u_int64_t begining =mktime(&dateBegining);
    while (scanf("%d %d %d", &yOld, &mOld, &dOld) == N_PARAMS) {
        u_int64_t delta = 0;
        struct tm dateOld = {0,0,0,dOld,mOld,yOld};
        u_int64_t sOld = mktime(&dateOld);
        delta = sOld - begining;
        int dYear = (delta / (MON_IN_YEAR * DAY_IN_MON * SEC_IN_DAY)) + 1;
        int dMon = (delta -  ((dYear-1) * MON_IN_YEAR * DAY_IN_MON * SEC_IN_DAY)) / (DAY_IN_MON * SEC_IN_DAY) + 1;
        int dDay = (delta - ((dYear-1) * MON_IN_YEAR * DAY_IN_MON * SEC_IN_DAY) -
                        ((dMon-1) *DAY_IN_MON * SEC_IN_DAY)) / SEC_IN_DAY + 1;
        printf("%d %d %d\n", dYear, dMon, dDay);
    }
    return 0;
}
