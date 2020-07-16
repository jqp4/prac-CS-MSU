%include "io.inc"

section .text 
global getsdig
getsdig:
    push ebp
    mov  ebp, esp
    xor  ecx, ecx
    gsdlp:
        xor  edx, edx
        div  ebx
        add  ecx, edx
        test eax, eax
        jnz  gsdlp
    gsde:
    mov  eax, ecx
    pop  ebp
    ret
    
global CMAIN
CMAIN:
    mov  ebp, esp
    GET_UDEC   4, eax
    GET_UDEC   4, ebx
    mov  esi, eax
    lp:
        call getsdig
        add  esi, eax
        cmp  eax, ebx
        jge  lp
    add  esi, eax
    PRINT_UDEC 4, esi
    xor eax, eax
    ret