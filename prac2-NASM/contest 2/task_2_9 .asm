%include "io.inc"

section .text
global CMAIN
CMAIN:
    mov  ebp, esp        ;for correct debugging
    mov  ax, -1001       ;upper  border (y)
    mov  di, -1001       ;right  border (x)
    mov  bx,  1001       ;bottom border (y)
    mov  si,  1001       ;left   border (x)
    xor  ecx, ecx        ;int i = 0;
    
    lp1:
        GET_DEC 2, dx    ;scanf("%hd", &x);
        cmp  dx, si
        jnl  skip1
        mov  si, dx
        skip1:
        cmp  dx, di
        jng  skip2
        mov  di, dx
        skip2:
        GET_DEC 2, dx    ;scanf("%hd", &y);
        cmp  dx, bx
        jnl  skip3
        mov  bx, dx
        skip3:
        cmp  dx, ax
        jng  skip4
        mov  ax, dx
        skip4:
        inc  ecx         ;i++;
        cmp  ecx, 4      ;again if i < 4
        jl   lp1            
    
    GET_DEC 2, cx        ;scanf("%hd", &px); //cx = px
    GET_DEC 2, dx        ;scanf("%hd", &py); //dx = py
    cmp  cx, si
    jle  skip5
    cmp  cx, di
    jge  skip5
    cmp  dx, bx
    jle  skip5
    cmp  dx, ax
    jge  skip5
    PRINT_CHAR 89
    PRINT_CHAR 69
    PRINT_CHAR 83
    jmp  skip6
    skip5:
    PRINT_CHAR 78
    PRINT_CHAR 79
    skip6:
    xor  eax, eax
    ret