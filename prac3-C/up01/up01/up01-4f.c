#include <ctype.h>

int
mystrspccmp(const char *str1, const char *str2)
{
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
