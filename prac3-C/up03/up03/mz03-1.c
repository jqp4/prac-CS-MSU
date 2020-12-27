#include <stdlib.h>
#include <stdio.h>

typedef int STYPE;
typedef unsigned int UTYPE;

int bitcount0(STYPE value){
    UTYPE uns = (UTYPE) value;
    int count = 0;
    int bit;
    while (uns) {
        bit = uns & 1;
        uns >>= 1;
        if (bit) {
            count++;
        }
    }
    return count;
}


int bitcount(STYPE value){
    int v_len = 0;
    UTYPE u_val = (UTYPE)value;
    /*while (u_val) {
        if (u_val & 1) {
            v_len += 1;
        }
        u_val >>= 1;
    }*/
    for (;;) {
        if (u_val & 1) {
            v_len += 1;
        }
        u_val >>= 1;
        if (u_val == 0){
            return v_len;
        }
    }
}


int main(void){
    int l = 9999999;
    for (int x = -l; x < l; x++){
        //printf("%d %d\n", bitcount(x), bitcount0(x));
        if (bitcount(x) != bitcount0(x)){
            printf("%d\n", x);
        }
    }
    printf("done\n");
    
    return 0;
}

