%include "io.inc"

section .text
global CMAIN
CMAIN:
    mov  ebp, esp; for correct debugging    
    GET_DEC 4, ebx      ;ebx = Vx, after -> tmp
    GET_DEC 4, ebp      ;ebp = Vy, after -> tmp 
    GET_DEC 4, edx      ;edx = ax/2, after -> Sx
    GET_DEC 4, edi      ;edi = ay/2, after -> Sx
    GET_DEC 4, eax      ;eax = t
    
    imul ebx, eax       ;ebx = tmp = Vx*t
    imul edx, eax       ;edx = Sx = ax/2*t
    imul edx, eax       ;edx = Sx = ax/2*t^2
    add  edx, ebx       ;edx = Sx = Vx*t + ax/2*t^2 
    
    imul ebp, eax       ;ebx = tmp = Vx*t
    imul edi, eax       ;edx = Sx = ax/2*t
    imul edi, eax       ;edx = Sx = ax/2*t^2
    add  edi, ebp       ;edx = Sx = Vx*t + ax/2*t^2 
            
    PRINT_DEC 4, edx
    PRINT_CHAR 32
    PRINT_DEC 4, edi
    xor  eax, eax
    ret