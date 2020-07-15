//
//  main.c
//  eth
//
//  Created by  Gleb on 11.06.2020.
//  Copyright © 2020  Gleb. All rights reserved.
//

#include <stdio.h>

int main(void) {
    int *p;
    short *q;
    int arr[3] = {1, 2, 3};
    short tmp = 4;
    p = arr;
    q = &tmp;
    
    
    
    
    int eax;
    eax = p && q && (p[2] += *q--);
    printf("%d\n", eax);
    return 0;
}

















