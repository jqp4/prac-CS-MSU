%include "io.inc"
;section .bss
;    p    resd 1
;    q    resd 1

section .data
    p_   dd 5, -6, 21
    p    dd p_
    q_   dw 34, -22
    q    dd q_ + 2
    
section .text
global CMAIN
preax:
    PRINT_DEC 4, eax
    PRINT_CHAR 10
    ret

CMAIN:
    ; int p;
    ; short *q;
    ; p && q && (p[2] += *q--);   
    ; p && q && (*(p + 8) += *(q--));
    mov  ecx, [p]        ; ecx - указатель p
    jecxz false
    mov  ecx, [q]
    jecxz false
    
    mov  eax, [p]        ; eax = p
    add  eax,  8         ; eax = p + 8
    
    mov  ecx, [q]        ; ecx = q
    movsx ecx, word[ecx] ; ecx = *q
    
    add  dword[eax], ecx ; *(p + 8) += *q
    sub  dword[q], 2
    
    mov  ecx, dword[eax]
    jecxz false
    mov  eax, 1
    jmp  done
    false:
    xor eax, eax
    done:
    call preax
    ret