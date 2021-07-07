#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int
parse_rwx_permissions(const char *str)
{
    const char template[]  = "rwxrwxrwx";
    if (!str) {
        return -1;
    }
    size_t len = strlen(template);
    unsigned short rez = 0;
    printf("%c", str[len]);
    if (str[len] != 0) {
        return -1;
    }
    
    
    size_t len = strlen(str);
    if (str[len] != 0) {
        return -1;
    }
    
    
    
    
    for (int i = 0; i < len; ++i) {
        if ((unsigned char)str[i] == (unsigned char)template[i]){
            rez = rez | (1 << (len - i - 1));
        } else if(str[i] != '-'){
            return -1;
        }
    }
    return rez;
}

int 
main()
{
    printf("%d\n",parse_rwx_permissions(NULL));
}
