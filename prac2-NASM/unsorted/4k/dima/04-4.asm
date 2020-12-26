%include "io.inc"

section .data

fmt_in db `%u`
file db "data.in", 0x00
fmt_file db `r` ,0x00
fmt_out db `%u\n`
section .bss
a resd 1
f1 resd 1
res resd 1
section .text
CEXTERN fscanf
CEXTERN printf
CEXTERN fopen
CEXTERN fclose

global CMAIN
CMAIN:
    mov ebp, esp; for correct debugging
    and esp , 0xfffffff0
    sub esp , 8
    push fmt_file
    push file
    call fopen
    mov dword[f1] , eax
    
    mov dword[res] , 0
.loop:   
    sub esp , 12
    push a
    push fmt_in
    push dword[f1]
    call fscanf
    cmp eax , -1
    je .end
    inc dword[res]
    jmp .loop    
        
.end:    
    push dword[res]
    push fmt_out
    call printf
    
    sub esp, 4
    push dword[f1]
    call fclose
    mov esp , ebp
    xor eax, eax
    ret