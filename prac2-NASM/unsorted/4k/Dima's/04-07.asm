%include "io.inc"
CEXTERN scanf
CEXTERN printf
CEXTERN malloc
CEXTERN strcmp 

section .rodata
fmt_str db "%s",0
fmt_in db "%d",0
fmt_out db "%d",0
section .bss
vec resb 6000
n resd 1
i resd 1
s1 resd 1
same resd 1 
ans resd 1
section .text
global CMAIN
CMAIN:
    mov ebp, esp; for correct debugging
    and esp, 0xfffffff0
    sub esp, 16
    mov dword[esp], fmt_in
    mov dword[esp+4], n
    call scanf
    ;mov dword[esp], fmt_str
;    mov dword[esp+4], s1
;    call scanf
    ;----------------
    mov dword[i], 0
sc_str:
    mov ecx, dword[i]
    cmp ecx, dword[n]
    jge sc_str_end
    mov ecx, dword[i]
    mov eax, ecx
    imul eax, 12
    add eax, vec
    mov dword[esp], fmt_str
    mov dword[esp+4], eax
    call scanf
    inc dword[i]
    jmp sc_str    
sc_str_end:
    ;----------------
    mov dword[i], 0
    mov eax, dword[n]
    mov dword[ans], eax
chek_same:
    mov ecx, dword[i]
    cmp ecx, dword[n]
    jge chek_same_end
    mov eax, dword[i]
    mov dword[esp+12], eax
    imul eax, 12
    add eax, vec
    mov dword[s1], eax
    ;mov dword[esp], fmt_str
;    mov eax, dword[s1]
;    mov dword[esp+4], eax
;    call printf
    mov dword[i], 0
    mov dword[same], 0
    ;--------------------
    chek_same1:
        mov ecx, dword[i]
        cmp ecx, dword[n]
        jge chek_same_end1
        mov eax, dword[i]
        imul eax, 12
        add eax, vec
        mov dword[esp+4], eax
        mov eax, dword[s1]
        mov dword[esp], eax
        call strcmp
        cmp eax, 0
        je find_same
    find_same_ret:
        inc dword[i]   
        jmp chek_same1
    del_same:
        dec dword[ans]
        jmp   del_same_ret
    find_same: 
        inc dword[same]
        jmp find_same_ret
    chek_same_end1:
        cmp dword[same], 1
        jne del_same
    del_same_ret:
    ;--------------------
    mov eax, dword[esp+12]
    mov dword[i], eax
    imul eax,12
    add eax, vec
    call strset
    inc dword[i] 
    jmp  chek_same  
chek_same_end:
    ;----------------
    mov dword[esp], fmt_out
    mov eax, dword[ans]
    mov dword[esp+4], eax
    call printf
    mov esp, ebp
    xor eax, eax
    ret
    
    
strset:
    mov dword[eax], 0
    add eax, 1
    mov dword[eax], 0
    add eax, 1
    mov dword[eax], 0
    add eax, 1
    mov dword[eax], 0
    add eax, 1    
    mov dword[eax], 0
    add eax, 1
    mov dword[eax], 0
    add eax, 1
    mov dword[eax], 0
    add eax, 1
    mov dword[eax], 0
    add eax, 1
    mov dword[eax], 0
    ret
    