//
//  main.c
//  contest 3
//
//  Created by  Gleb on 25.10.2019.
//  Copyright © 2019  Gleb. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int min(int a, int b){
    return a < b ? a : b;
}


int LCM(int a, int b){ // НОК
    int m = a * b;
    while (a != 0 && b != 0){
        if (a > b)
            a %= b;
        else
            b %= a;
        }
    return (int) (m / (a+b));
}



struct Fraction{
    int undiv;
    int numerator;
    int denominator;
};


struct Fraction_math{
    void (*output)(struct Fraction a);
    struct Fraction (*reduce)(struct Fraction a);
    struct Fraction (*add)(struct Fraction a, struct Fraction b);
    struct Fraction (*new_fraction/*constructor*/)(struct Fraction_math frm, int num, int den);
    struct Fraction (*input_new_fraction/*constructor*/)(struct Fraction_math frm);
};



struct Fraction frm_reduce(struct Fraction a){
    int m = min(a.numerator, a.denominator);
    for (int i = 2; i <= m; i++){
        if (a.numerator % i == 0){
            if (a.denominator % i == 0){
                a.numerator = (int)(a.numerator / i);
                a.denominator = (int)(a.denominator / i);
                i = 1;
            }
        }
    }
    a.undiv += (int)(a.numerator / a.denominator);
    a.numerator = a.numerator % a.denominator;
    return a;
}


struct Fraction frm_add(struct Fraction a, struct Fraction b){
    int new_den = LCM(a.denominator, b.denominator);
    a.numerator = a.numerator * (int)(new_den / a.denominator) + b.numerator * (int)(new_den / b.denominator);
    a.denominator = new_den;
    return a;
}


void frm_output(struct Fraction a){
    printf("%d %d %d", a.undiv, a.numerator, a.denominator);
}


struct Fraction frm_new_fraction/*constructor*/(struct Fraction_math frm, int num, int den){
    struct Fraction a;
    
    a.undiv = 0;
    a.numerator = num;
    a.denominator = den;
    
    return frm.reduce(a);
}


struct Fraction frm_input_new_fraction/*constructor*/(struct Fraction_math frm){
    struct Fraction a;
    int num, den;
    scanf("%d %d", &num, &den);
    
    a.undiv = 0;
    a.numerator = num;
    a.denominator = den;
    
    return frm.reduce(a);
}


struct Fraction_math fraction_math/*constructor*/(void){
    struct Fraction_math frm;
    
    frm.reduce = frm_reduce;
    frm.output = frm_output;
    frm.add = frm_add;
    frm.new_fraction = frm_new_fraction;
    frm.input_new_fraction = frm_input_new_fraction;
    
    return frm;
}


int main(void){
    struct Fraction_math frm = fraction_math();
    struct Fraction ans = frm.new_fraction(frm, 0, 1);
    
    int n;
    scanf("%d", &n);
    
    for (int i = 0; i < n; i++){
        ans = frm.reduce(frm.add(ans, frm.input_new_fraction(frm)));
    }
    
    frm.output(ans);
    return 0;
}
