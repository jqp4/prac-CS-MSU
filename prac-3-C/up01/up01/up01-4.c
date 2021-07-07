#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

int mystrspccmp1_(const char *str1, const char *str2){
    unsigned char c1, c2;
    while ((c1 = (unsigned char)(*str1)) + ((c2 = (unsigned char)(*str2)) != 0)) {
        if (isspace(c1)) {
            str1++;
            continue;
        }
        if (isspace(c2)) {
            str2++;
            continue;
        }
        if (c1 != c2) {
            return c1 - c2;
        }
        str1++;
        str2++;
    }
    return 0;
}


int mystrspccmp1(const char *str1, const char *str2){
    unsigned char cr1 = (unsigned char)(*str1);
    unsigned char cr2 = (unsigned char)(*str2);
    while (cr1 || cr2) {
        if (isspace(cr1)) {
            str1++;
        } else if (isspace(cr2)) {
            str2++;
        } else if (cr1 != cr2) {
            return cr1 - cr2;
        } else {
            str1++;
            str2++;
        }
        cr1 = (unsigned char)(*str1);
        cr2 = (unsigned char)(*str2);
    }
    return 0;
}


int mystrspccmp2(const char *str1, const char *str2){
    unsigned char *s1 = (unsigned char*) str1;
    unsigned char *s2 = (unsigned char*) str2;
    unsigned char c1 = (unsigned char) *s1++;
    unsigned char c2 = (unsigned char) *s2++;
    while (1) {
        while (isspace(c1)) {
            c1 = (unsigned char) *s1++;
        }
        while (isspace(c2)) {
            c2 = (unsigned char) *s2++;
        }
        if (c1 == '\0' || c1 != c2) {
            return c1 - c2;
        }
        c1 = (unsigned char) *s1++;
        c2 = (unsigned char) *s2++;
    }
}

int main(void){
    srand((unsigned)time(NULL));
    for (int j = 0; j < 100000; j++){
        int n1 = rand() % 50;
        int n2 = rand() % 50;
        unsigned char s1[n1 + 1];
        unsigned char s2[n2 + 1];
        for (int i = 0; i < n1; i++) s1[i] = rand() % 256;
        for (int i = 0; i < n2; i++) s2[i] = rand() % 256;
        s1[n1] =  '\0';
        s2[n2] =  '\0';
        int ans1 = mystrspccmp1(s1, s2);  // gleb
        int ans2 = mystrspccmp2(s1, s2);  // igor
        if (ans1 != ans2) {
            printf("%d, ans = %2d, %2d\n", j, ans1, ans2);
            printf("s1 = %s\ns2 = %s\n", s1, s2);
        }
    }
    printf("Done\n");
    return 0;
}

int main_(void){
    char *s1 = " aa";
    char *s2 = " aa";
    int ans1 = mystrspccmp1(s1, s2);
    //int ans2 = mystrspccmp2(s1, s2);
    //if (ans1 != ans2)
    printf("s1 = %s\ns2 = %s\nans = %d\n", s1, s2, ans1);
    return 0;
}
