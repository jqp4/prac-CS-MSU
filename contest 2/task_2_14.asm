%include "io.inc"
%define ce 10

section .data
    count1 dd 0
    count2 dd 0
    
section .bss
    n0   resd 1
    N    resd 1
    K    resd 1

section .text
global CMAIN
CMAIN:
    mov  ebp, esp   
    GET_DEC   4, esi         ;N
    GET_DEC   1, edi         ;K
    mov  [N], esi
    mov  [K], edi
    mov  ecx, edi
    mov  ebx, 1
    shl  ebx, cl             ;ebx = 2^K         
    cmp  ebx, esi
    jg   exit
    lp1:
        inc  ecx
        shl  ebx, 1  
        cmp  ebx, esi
        jle  lp1
    dec  ecx                 ;ecx = n0
    mov  [n0], ecx
    cmp  edi, 0
    jne  skip4
    mov  edi, ecx            ;if k = 0: edi = count1 = n0
    jmp  skip5
    skip4:
    mov  ecx, edi
    mov  eax, 1
    lp2:
        mul  ecx
        loop lp2    
    mov  esi, eax            ;esi = K!
    mov  ecx, edi            ;ecx = K
    xor  edi, edi            ;edi = count1
    lp3:
        mov  eax, 1          ;eax = delta count1 
        mov  ebx, ecx
        sub  ebx, [K]        ;ebx = n - K
        lp6:
            inc  ebx     
            mul  ebx
            cmp  ebx, ecx
            jl   lp6
        xor  edx, edx
        div  esi             ;eax = n(n - 1)(n - 2)/K!
        add  edi, eax        ;count1 += eax
        inc  ecx
        cmp  ecx, [n0]       ;n < n0
        jl   lp3    
    skip5: 
    mov [count1], edi
    ;------------------------------------------------------------------------
    mov  esi, [N]
    mov  eax, [n0]
    mov  dh,  byte[K]
    mov  ebx, 1              ;loop from 2^n0 to N
    mov  cl,  byte[n0]
    shl  ebx, cl             ;ebx - первое проверяемое число (2^n0)
    inc  ebx
    lp4:
        xor  dl,  dl         ;dl  - колличество нулей в числе ebx 
        mov  edi, ebx        ;edi - tmp
        mov  ecx, eax        ;loop from n0 to 1
        lp5:
            shr  edi, 1
            jc   skip6
            inc  dl
            skip6:
            loop lp5
            
            
        cmp  dl,  dh         ;анализируем количество нулей dl
        jne  skip2
        inc  dword[count2]
        jmp  skip3
        skip2:
        dec  dl
        cmp  dl,  dh
        jle  skip3
        inc  dl
        sub  dl,  dh
        mov  edi, 1
        mov  cl,  dl
        shl  edi, cl
        dec  edi
        add  ebx, edi
        cmp  ebx, esi
        jle  lp4
        skip3:
        inc  ebx
        cmp  ebx, esi
        jle  lp4
    ;mov [count2], edi
    ;------------------------------------------------------------------------
    exit:
    mov  esi, [count1]
    add  esi, [count2]
    PRINT_DEC 4, esi
    ;PRINT_CHAR '='
    ;PRINT_DEC 4, count1
    ;PRINT_CHAR '+'
    ;PRINT_DEC 4, count2
    xor  eax, eax
    ret