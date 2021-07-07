%include "io.inc"
 
CEXTERN scanf
CEXTERN printf
 
section .bss
    val resd 1
   
section .data
    fmtin db `%u`, 0
    fmtout db `0x%08X\n`, 0
   
section .text
global CMAIN
CMAIN:
    mov ebp, esp; for correct debugging
    and esp, -16
.loop:
    sub esp, 8
    push val
    push fmtin
    call scanf
    cmp eax, -1
    je .end
    add esp, 8
    push dword[val]
    push fmtout
    call printf
    add esp, 8
    jmp .loop
.end:
    mov esp, ebp
    xor eax, eax
    ret