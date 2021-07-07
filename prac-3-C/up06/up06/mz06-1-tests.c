#include <stdio.h>
#include <stdlib.h>
#include <time.h>









#include <string.h>
const char *root = "/Users/gleb/Projects/prac/up06/up06/";
char buff[200];

char *filepath(char *filename){
    buff[0] = '\0';
    strcat(buff, root);
    strcat(buff, filename);
    return buff;
}









enum
{
    YEAR_SHIFT = 1900,
    MON_COUNT = 12,
    MAX_MDAY = 32,
    THURSDAY = 4,
};

int main1(FILE *f){
    int year;
    //if (scanf("%d", &year) == 1){
    for (year = 1910; year < 2038; year++){
        struct tm *t = calloc(1, sizeof(*t));
        t->tm_year = year - YEAR_SHIFT;
        for (int mon = 0; mon < MON_COUNT; mon++){
            t->tm_mon = mon;
            int nth = 0; // Thursday number
            for (t->tm_mday = 1; t->tm_mday < MAX_MDAY; t->tm_mday++){
                mktime(t);
                if (t->tm_mon > mon){
                    t->tm_mday = MAX_MDAY;
                } else if(t->tm_wday == THURSDAY){
                    nth++;
                    if((nth == 2 || nth == 4) && t->tm_mday % 3 != 0){
                        //printf("%d %d\n", t->tm_mon + 1, t->tm_mday);
                        fprintf(f, "%d %d %d\n", year, t->tm_mon + 1, t->tm_mday);
                    }
                }
            }
        }
    }
    return 0;
}

int main2(FILE *f){
    int year;
    //if (scanf("%d", &year) == 1){
    for (year = 1910; year < 2038; year++){
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
                    if((nth == 2 || nth == 4) && t.tm_mday % 3 != 0){
                        //printf("%d %d\n", t->tm_mon + 1, t->tm_mday);
                        fprintf(f, "%d %d %d\n", year, t.tm_mon + 1, t.tm_mday);
                    }
                }
            }
        }
    }
    return 0;
}




int main(void){
    FILE *fg = fopen(filepath("file_1_old.txt"), "w");
    FILE *ft = fopen(filepath("file_2_new.txt"), "w");
    
    main1(fg);
    main2(ft);
    
    fclose(fg);
    fclose(ft);
    return 0;
}
