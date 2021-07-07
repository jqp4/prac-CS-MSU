%include "io.inc"
section .data
    a    dd 5, 9, 10, 7, 3
    n    dd 5
    
section .text
global CMAIN

output_a:
    xor  ecx, ecx
    clp:
        mov  ebx, [a + ecx * 4]
        PRINT_DEC 4, ebx
        PRINT_CHAR 32
        inc  ecx
        cmp  ecx, [n]
        jl   clp
    PRINT_CHAR 10
    ret


exchange:
    push esi
    push edi
    push ebx
    push ebp
    push edx
    mov  ecx, edx
    xor  edx, edx
    shr  ecx, 1
    jecxz exchexit
    exchlp:
        mov  esi, [esp]
        sub  esi, ecx
        lea  esi, [eax + esi * 4]
        lea  edi, [eax + ecx * 4 - 4]
        mov  ebx, [esi]
        mov  ebp, [edi]
        cmp  ebx, ebp
        ja   exchskip
        mov  [esi], ebp
        mov  [edi], ebx
        inc  edx
        exchskip:
        dec  ecx
        jnz  exchlp
    exchexit:
    mov  ecx, edx
    pop  edx
    pop  ebp
    pop  ebx
    pop  edi
    pop  esi
    ret


CMAIN:
    mov  ebp, esp
    mov  eax,  a
    mov  edx, [n]
    call output_a
    call exchange
    call output_a
    xor  eax, eax
    ret