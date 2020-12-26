%include "io.inc"

section .text
global CMAIN

l1:           
    mov  edx, esi        ;d = tmp
    cmp  edx, ebx        ;if(d >= b)
    jl   l1e             ;exit if false
    mov  esi, ebx        ;swap(&b, &d)
    mov  ebx, edx
    mov  edx, esi
    cmp  ebx, eax        ;if(b >= a)
    jl   l1e             ;exit if false
    mov  esi, eax        ;swap(&a, &b)
    mov  eax, ebx
    mov  ebx, esi
    jmp  l1e             ;exit



CMAIN:
    mov  ebp, esp        ;for correct debugging
    xor  esi, esi        ;int tmp;
    mov  eax, 0x80000000 ;int a
    mov  ebx, 0x80000000 ;int b
    xor  ecx, ecx        ;int N
    mov  edx, 0x80000000 ;int d
    GET_DEC 4, ecx       ;scanf("%d", &N);
lp:    
    GET_DEC 4, esi       ;scanf("%d", &tmp);
    cmp  esi, edx        ;if(tmp >= d)
    jge  l1              ;jmp if true
l1e:
    loop lp
    PRINT_DEC 4, eax
    PRINT_CHAR 32
    PRINT_DEC 4, ebx
    PRINT_CHAR 32
    PRINT_DEC 4, edx
    PRINT_CHAR 32
    xor  eax, eax
    ret