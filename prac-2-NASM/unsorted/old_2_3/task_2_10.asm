%include "io.inc"

section .text
global CMAIN
CMAIN:
    mov  ebp, esp
    GET_DEC   4, ebx
    mov  ecx, 1
    lp1:
        inc  ecx
        cmp  ecx, 0x5000
        je   skip
        xor  edx, edx
        mov  eax, ebx
        div  ecx
        cmp  edx, 0
        jne  lp1
        mov  ecx, eax
        jmp  exit 
    skip:  
    mov  ecx, eax
    lp:
        xor  edx, edx
        mov  eax, ebx
        div  ecx
        cmp  edx, 0
        je   exit
        loop lp
    exit:
    PRINT_DEC 4, ecx    
    xor  eax, eax
    ret