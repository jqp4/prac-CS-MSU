//
//  main.c
//  contest 2 reverse
//
//  Created by  Gleb on 07.04.2020.
//  Copyright © 2020  Gleb. All rights reserved.
//

#include <stdio.h>

int main(void) {
    unsigned int eax, cf;
    int ebx;
    scanf("%u", &eax);
    ebx = eax;
    ebx--;
    eax ^= ebx;
    cf = eax == -1;
    eax += 1;
    //RCR EAX, 1
    eax >>= 1;
    cf <<= 31;
    eax += cf;
    //----------
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
                        GET_UDEC        4, EAX

                        MOV             EBX, EAX
                        DEC             EBX
                        XOR             EAX, EBX
                        ADD             EAX, 1
                        RCR             EAX, 1

                        PRINT_UDEC      4, EAX
                        NEWLINE

                        XOR             EAX, EAX
                        RET
 */
