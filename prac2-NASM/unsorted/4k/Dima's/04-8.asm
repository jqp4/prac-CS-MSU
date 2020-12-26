%include "io.inc"
section .rodata
fmt_in db "%d",0
fmt_out db "%d ",0
file1 db "input.txt" ,0
file2 db "output.txt", 0
fmt_file1 db "r",0
fmt_file2 db "w",0

section .bss
f1 resd 1
f2 resd 1
val resd 1
i resd 1
n resd 1
vec resd 1000
section .text
CEXTERN qsort
CEXTERN fprintf
CEXTERN fscanf
CEXTERN fopen
CEXTERN fclose
global CMAIN
CMAIN:

    mov ebp, esp; for correct debugging
    and esp, 0xfffffff0
    sub esp,8
    push fmt_file1
    push file1
    call fopen   
    mov dword[f1], eax
    add esp, 8
    push fmt_file2
    push file2
    call fopen   
    mov dword[f2], eax
    add esp, 12
;-------------------------     
    mov dword[i] , 0
.loop1:
    push val
    push fmt_in
    push dword[f1]
    call fscanf
    cmp eax, -1
    je endl1
    mov ecx, dword[i]
    mov ebx, dword[val]
    mov dword[vec + ecx*4], ebx
    add esp, 12
    inc dword[i]
    jmp .loop1
endl1:
    mov eax, dword[i]
    mov dword[n], eax
;--------------------------
    add esp, 16
    push comp   
    mov eax, 4
    push eax
    push dword[n]
    push vec
    call qsort
    add esp, 12
    pop ecx
    sub esp,4
;--------------------------

    mov dword[i], 0
.loop2:
    mov ecx, dword[i]
    push dword[vec + ecx*4]
    push fmt_out
    push dword[f2]
    call fprintf
    inc dword[i]
    mov ecx, dword[i]
    cmp ecx, dword[n]
    jge endl2
    add esp, 12
    jmp .loop2
endl2:  

    mov esp, ebp
    xor eax, eax
    ret
    
comp:
        mov eax, dword  [esp+4]
        mov edx, dword  [esp+8]
        mov eax, dword  [eax]
        mov edx, dword  [edx]
        cmp eax, edx
        jg exit1
        jl exit2
        je exit3
exit1:
        mov eax, 1
        ret
exit2:
        mov eax, -1
        ret
exit3:
        mov eax, 0
        ret