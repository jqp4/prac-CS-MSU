#include <stdio.h>

/* SBC - Six Bit Code */

enum
{
    BIT_2 = 1 << 2,
    BIT_3 = 1 << 3,
    ALPHLEN = 26,
    DIGITAM = 10,
    CODE_0 = 1,
    CODE_9 = DIGITAM,
    CODE_LA = CODE_9 + 1,
    CODE_LZ = CODE_9 + ALPHLEN,
    CODE_A = CODE_LZ + 1,
    CODE_Z = CODE_LZ + ALPHLEN,
    DELTA_09 = '0' - CODE_0,
    DELTA_LAZ = 'a' - CODE_LA,
    DELTA_AZ = 'A' - CODE_A
};

int
ASCIItoSBC(int a)
{
    if ('0' <= a && a <= '9'){
        return a - DELTA_09;
    }
    if ('A' <= a && a <= 'Z'){
        return a - DELTA_AZ;
    }
    if ('a' <= a && a <= 'z'){
        return a - DELTA_LAZ;
    }
    return -1;
}

int
SBCtoASCII(int a)
{
    if (CODE_0 <= a && a <= CODE_9){
        return a + DELTA_09;
    }
    if (CODE_LA <= a && a <= CODE_LZ){
        return a + DELTA_LAZ;
    }
    if (CODE_A <= a && a <= CODE_Z){
        return a + DELTA_AZ;
    }
    if (a == 0){
        return '@';
    }
    return '#';
}

int
breakSBC(int a)
{
    a -= a & BIT_2;
    a ^= BIT_3;
    return a;
}

int
main(void)
{
    int a;
    while ((a = getchar()) != EOF) {
        if ((a = ASCIItoSBC(a)) != -1) {
            putchar(SBCtoASCII(breakSBC(a)));
        }
    }
    return 0;
}
