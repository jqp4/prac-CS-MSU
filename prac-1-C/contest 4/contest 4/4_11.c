//
//  main.c
//  contest 4
//
//  Created by  Gleb on 07.12.2019.
//  Copyright © 2019  Gleb. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STR_SIZE 2001
#define MAX_VALUE 120000
#define li long int


const char numbers_codes[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
const char operations_codes[4] = {'+', '-', '*', '/'};


li liabs(li x){
    return x >= 0 ? x : -x;
}


int str_len(char *s){
    int n = 0;
    while (s[n] != '\0'){
        n++;
    }
    return n;
}


int get_number(char a){
    for (int i = 0; i < 10; i++){
        if (a == numbers_codes[i]){
            return i;
        }
    }
    return -1;
}


int get_operation(char a){
    for (int i = 0; i < 4; i++){
        if (a == operations_codes[i]){
            return i;
        }
    }
    return -1;
}


li get_int(char *s, int i0, int n){
    int i = i0;
    li x = get_number(s[i]);
    if (x != -1){
        i++;
        li xi;
        while (i < n){
            xi = get_number(s[i]);
            if (xi != -1){
                x = x * 10 + xi;
                i++;
            } else {
                return x;
            }
        }
    }
    return x;
}


int int_len(li x){
    return x != 0 ? (int)floor(log10(x)) + 1 : 1;
}


li op_plus(li a, li b){ return a + b; }
li op_minus(li a, li b){ return a - b; }
li op_multiplication(li a, li b){ return a * b; }
li op_division(li a, li b){ return (li)(a / b); }

li (*operations[4]) (li a, li b) = {op_plus, op_minus, op_multiplication, op_division};

li calculate(li a, li b, int op){
    return operations[op](a, b);
}


int main(void){
    char *s = (char *)malloc(STR_SIZE * sizeof(char));
    scanf("%s", s);
    int i = 0, n = str_len(s), stage = 0, op = -1;
    li a = -1, b = -1, ans = MAX_VALUE + 1;
    /*
     stage :
         0 - a
         1 - operation
         2 - b
         3 - calculate: a op b
    */
    while (i < n){
        if (stage == 0){
            a = get_int(s, i, n);
            if (a != -1){
                stage = 1;
                i += int_len(a);
            } else {
                i++;
            }
        } else if (stage == 1){
            op = get_operation(s[i]);
            if (op != -1){
                stage = 2;
            } else {
                stage = 0;
            }
            i++;
        } else if (stage == 2){
            b = get_int(s, i, n);
            if (b != -1){
                if(b != 0){
                    stage = 3;
                } else {
                    stage = 0;
                }
            } else {
                stage = 0;
                i++;
            }
        } else if (stage == 3){
            ans = calculate(a, b, op);
            if (liabs(ans) <= MAX_VALUE){
                printf("%ld %c %ld = %ld\n", a, operations_codes[op], b, ans);
            }
            stage = 0;
        }
    }
    free(s);
    return 0;
}
