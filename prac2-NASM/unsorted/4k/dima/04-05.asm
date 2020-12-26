%include "io.inc"

CEXTERN printf  
CEXTERN scanf
CEXTERN malloc
CEXTERN realloc
CEXTERN free
section .rodata
fmt_in db "%u",0
fmt_out db "%u",0

section .bss
vec resd 1
n resd 1
i resd 1
val resd 1
res resd 1
section .text
global CMAIN
CMAIN:
    mov ebp, esp; for correct debugging
    and esp, 0xfffffff0
    sub esp, 16
    mov dword[n], 1
    mov dword[i], 1
    mov dword[res], 0
    mov dword[esp], fmt_in
    mov dword[esp+4], val
    call scanf
    mov eax, dword[val]
    cmp eax, 0
    je exit
    ;-----------------
    mov eax, dword[n]
    imul eax, 4
    mov dword[esp], eax
    call malloc
    mov dword[vec], eax 
    mov ecx, dword[val]
    mov dword[eax], ecx
    ;-----------------
realc:
    mov eax, dword[n]
    imul eax, 2
    mov dword[n], eax
    imul eax, 4
    mov dword[esp+4], eax
    mov eax, dword[vec]  
    mov dword[esp], eax
    call realloc
    mov dword[vec], eax
sc:    
    mov dword[esp], fmt_in
    mov dword[esp+4], val
    call scanf
    mov eax, dword[val]
    cmp eax, 0
    je .res
    mov ecx, dword[i]
    mov ebx, dword[vec]
    mov dword[ebx+ecx*4], eax
    inc dword[i]
    mov ecx, dword[i]
    cmp ecx, dword[n]
    je realc
    jmp sc
.res: 
    dec dword[i]
    mov ecx, dword[i]
    mov ebx, dword[vec ]
    mov eax, dword[ebx + ecx*4]
    mov dword[val], eax 
.loop1:
    dec dword[i]
    mov ebx, dword[i]
    cmp ebx, 0
    jl exit
    mov ecx, dword[vec]
    mov eax, dword[ecx + ebx*4]
    cmp eax, dword[val]
    jl .inc
    jge .loop1
.inc: 
    inc dword[res]
    jmp .loop1  

exit: 
    mov ecx, dword[vec]
    mov dword[esp], ecx
    call free
    mov dword[esp], fmt_out
    mov ecx, dword[res]
    mov dword[esp+4], ecx
    call printf    
    mov esp, ebp
    xor eax, eax
    ret
    