//
//  main.c
//  exam
//
//  Created by  Gleb on 12.01.2020.
//  Copyright © 2020  Gleb. All rights reserved.
//


#include <stdio.h>


void b_print(unsigned n, unsigned len) {
    for (int i = 0; i < len; i++) {
        printf("%d", n & 1);
        n >>= 1;
    }
}


void b_print_n(unsigned n, unsigned len) {
    b_print(n, len);
    printf("\n");
}



unsigned count0(unsigned x){
    unsigned k = 0;
    while (x) {
        if (!(x & 1)) k++;
        x = x >> 1;
    }
    
    return k;
}


unsigned get_length(unsigned n){
    unsigned l = 0;
    
    while (n) {
        l++;
        n >>= 1;
    }
    
    return l;
}


unsigned make_mask (unsigned num){
    return  num ? 1 << (get_length(num) - 1) : 0;
}


void right_b_shift(unsigned *x, unsigned *y, unsigned x_mask, unsigned y_mask) {
    unsigned x_r_bit = *x & 1;
    unsigned y_r_bit = *y & 1;
    
    *x = *x >> 1;
    *y = *y >> 1;
    
    if (y_r_bit == 1) *x = *x | x_mask;
    if (x_r_bit == 1) *y = *y | y_mask;
}


void rrr (unsigned *x, unsigned *y){
    unsigned k = count0(*x);
    unsigned x_mask = make_mask(*x);
    unsigned y_mask = make_mask(*y);
    
    for (int i = 0; i < k; i++) {
        right_b_shift(x, y, x_mask, y_mask);
    }
}



int main_(int argc, const char * argv[]) {
    unsigned x = 105, y = 5;
    
    // для печати
    unsigned x_len = get_length(x);
    unsigned y_len = get_length(y);
    unsigned k = count0(x);
    b_print_n (x, x_len);
    b_print_n (y, y_len);
    printf("k = %d\n", k);
    b_print (x, x_len);
    b_print_n (y, y_len);
    // конец печати
    
    rrr(&x, &y);
    
    b_print (x, x_len);
    b_print_n (y, y_len);
        
    return 0;
}


void f(char k){
    static char c = k;
    printf("%c", c);
}




int main(void){
    f('t');
    f('a');
    
    
    return 0;
}
