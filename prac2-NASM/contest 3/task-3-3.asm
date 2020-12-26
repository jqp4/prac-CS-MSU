%include "io.inc"

section .text
global recpr
recpr: 
    GET_DEC   4, eax
    test eax, eax
    jz   recpre
    push eax
    call recpr
    pop  eax
    PRINT_DEC 4, eax
    PRINT_CHAR 32
    recpre:
    ret
    
global CMAIN
CMAIN:
    mov  ebp, esp
    call recpr
    xor  eax, eax
    ret