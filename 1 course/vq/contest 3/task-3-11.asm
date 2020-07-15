%include "io.inc"

section .text
global ecxdiv3
ecxdiv3:
    xor  eax, eax
    ed3lp1:
        jecxz cd3skip1
        shr  ecx, 1
        jnc  ed3lp2
        inc  eax
    ed3lp2:
        jecxz cd3skip1
        shr  ecx, 1
        jnc  ed3lp1
        dec  eax
        jmp  ed3lp1
    cd3skip1:
    cmp  eax,   0
    jnl  cd3skip2
    mov  ecx,  -1
    mul  ecx
    cd3skip2:
    cmp  eax, 3
    jl   cd3exit
    cd3lp3:
        sub  eax, 3
        cmp  eax, 3
        jge  cd3lp3
    cd3exit:
    ret
    
global printanswer
printanswer:
    test eax, eax
    jnz  paskipno
    PRINT_CHAR 89 
    PRINT_CHAR 69
    PRINT_CHAR 83
    PRINT_CHAR 10
    ret
    paskipno:
    PRINT_CHAR 78
    PRINT_CHAR 79
    PRINT_CHAR 10
    ret
    
global CMAIN
CMAIN:
    mov  ebp, esp
    GET_UDEC   4, ebx
    lp: 
        GET_UDEC   4, ecx
        call ecxdiv3
        call printanswer
        dec  ebx
        jnz  lp
    xor  eax, eax
    ret