%include "io.inc"

section .text
global CMAIN
CMAIN:
    mov  ebp, esp; for correct debugging    
    GET_DEC 4, eax      ;eax = A, after -> tank volume
    GET_DEC 4, ebx      ;ebx = B
    GET_DEC 4, ecx      ;ecx = C
    GET_DEC 4, esi      ;edx = V
    
    imul eax, ebx       ;eax = A*B
    imul eax, ecx       ;ebx = A*B*C = tank value
    mov  edx, 0         ;dividend is edx:eax
    div  esi            ;quotient is in eax
    
    PRINT_DEC 4, eax
    xor  eax, eax
    ret