//
//  main.c
//  contest 2 reverse
//
//  Created by  Gleb on 07.04.2020.
//  Copyright © 2020  Gleb. All rights reserved.
//

#include <stdio.h>

int main(void) {
    int ecx, ebx, eax;
    scanf("%d", &ecx);
    ebx = 1;
    eax ^= eax;
    for(;ecx > 0; ecx--){
        eax ^= ebx;
        ebx ^= eax;
        eax ^= ebx;
        ebx += eax;
    }
    printf("%u", eax);
    printf("\n");
    eax ^= eax;
    return 0;
}


/*
 %include "io.inc"

 SECTION .text

 GLOBAL CMAIN
 CMAIN:
     GET_DEC         4, ECX

     MOV             EBX, 1
     XOR             EAX, EAX

 .L:
     XOR             EAX, EBX
     XOR             EBX, EAX
     XOR             EAX, EBX

     ADD             EBX, EAX
     LOOP            .L

     PRINT_UDEC      4, EAX
     NEWLINE

     XOR             EAX, EAX
     RET
 */
