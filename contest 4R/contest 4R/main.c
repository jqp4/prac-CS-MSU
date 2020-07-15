//
//  main.c
//  contest 4R
//
//  Created by  Gleb on 30.05.2020.
//  Copyright © 2020  Gleb. All rights reserved.
//

#include <stdio.h>

int eax, ebx;
char cl;


void F(void){
    if (cl == 42) eax *= ebx;
    else if (cl == 43) eax += ebx;
    else if (cl == 45) eax -= ebx;
    else if (cl == 47) eax /= ebx;
}


int main(void){
    scanf("%d%c%d", &eax, &cl, &ebx);
    F();
    printf("%d", eax);
    return 0;
}
