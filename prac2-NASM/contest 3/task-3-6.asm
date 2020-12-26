%include "io.inc"

section .text
global getnullcount
getnullcount:
    push ebp              ; save ebp
    mov  ebp, esp
    mov  ecx, [ebp + 8]   ; get arg (ecx = A)
    xor  eax, eax         ; nullcount = 0
    gnclp:
        jecxz gnclpe
        shr  ecx, 1
        jc   gncskip
        inc  eax
        gncskip:
        jmp  gnclp
    gnclpe:
    pop  ebp              ; recovery ebp
    ret                   ; answer in eax

global CMAIN
CMAIN:
    mov  ebp, esp
    GET_UDEC   4, edi
    xor  esi, esi
    test edi, edi
    jz   exit
    mov  ecx, edi
    lp1:
        GET_UDEC   4, eax
        push eax
        loop lp1
    GET_UDEC 4, ebx
    lp2:
        call getnullcount
        pop  ecx          ; clear args in stack
        cmp  eax, ebx
        jne  skip
        inc  esi
        skip:
        dec  edi
        jnz  lp2
    exit:
    PRINT_UDEC 4, esi
    xor eax, eax
    ret