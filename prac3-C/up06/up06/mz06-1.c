#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

enum
{
    YEAR_SHIFT = 1900,
    MON_COUNT = 12,
    MAX_MDAY = 32,
    THURSDAY = 4,
    SECOND_T = 2,
    FOURTH_T = 4,
    MOD_THREE = 3,
};

int main(void){
    int year;
    if (scanf("%d", &year) == 1){
        struct tm t;
        memset(&t, 0, sizeof(t));
        t.tm_year = year - YEAR_SHIFT;
        for (int mon = 0; mon < MON_COUNT; mon++){
            t.tm_mon = mon;
            int nth = 0; // Thursday number
            for (t.tm_mday = 1; t.tm_mday < MAX_MDAY; t.tm_mday++){
                mktime(&t);
                if (t.tm_mon > mon){
                    t.tm_mday = MAX_MDAY;
                } else if(t.tm_wday == THURSDAY){
                    nth++;
                    if((nth == SECOND_T || nth == FOURTH_T) && t.tm_mday % MOD_THREE != 0){
                        printf("%d %d\n", t.tm_mon + 1, t.tm_mday);
                    }
                }
            }
        }
    }
}
