%include "io.inc"

section .text
global CMAIN

    
CMAIN:
    mov  ebp, esp
    mov  ecx, 32
    xor  ebx, ebx
    GET_UDEC  4, eax
lp:
    shr  eax, 1
    jnc  l1
    add  ebx, 1  
     
l1:
    loop lp
    PRINT_DEC 4, ebx
    xor  eax, eax
    ret