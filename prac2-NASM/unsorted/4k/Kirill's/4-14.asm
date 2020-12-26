%include "io.inc"

section .bss
    head resd 3
    tail resd 3
    min resd 1
    ptmin resd 1
    finp resd 1
    foutp resd 1
    
section .data
    n dd 0
    a dd 0
    ;fin db '/home/parallels/Desktop/input.txt', 0
    ;fout db '/home/parallels/Desktop/output.txt', 0
    fin db '/media/sf_UbuntuShare/4k/input.txt', 0
    fout db '/media/sf_UbuntuShare/4k/output.txt', 0
    r db 'r', 0
    w db 'w', 0
    fmt db '%d ',0
    tmp db 0
section .text
    CEXTERN printf
    CEXTERN malloc
    CEXTERN free
    CEXTERN scanf
    CEXTERN fscanf
    CEXTERN fprintf
    CEXTERN fopen
    CEXTERN fclose

global removeMin
global findMin
global printMin
global sorting
global CMAIN
CMAIN:
    mov ebp, esp; for correct debugging
    ;mov ebp, esp; for correct debugging
    ;write your code here
    push ebp
    mov ebp,esp
    and esp,~0xf
    
    sub esp,16
    mov dword[esp],fin
    mov dword[esp+4],r 
    call fopen
    mov [finp],eax
    add esp, 16
    
    sub esp, 16
    mov dword[esp],fout
    mov dword[esp+4],w  
    call fopen
    mov [foutp],eax
    add esp, 16
    
    sub esp, 16
    mov dword[esp],12
    call malloc
    mov [head],eax
    call malloc
    mov [tail],eax
    add esp, 16
    mov ebx,[head]
    mov [eax],ebx
    mov [ebx+4],eax
    
for:
    mov eax,[finp]
    sub esp, 16
    mov dword[esp],eax
    mov dword[esp+4],fmt
    mov dword[esp+8],a
    call fscanf
    add esp, 16
    
    cmp eax,1
    jnz rof
    sub esp, 16
    mov dword[esp],12
    call malloc
    add esp, 16
    
    mov ecx,[tail]
    mov ebx,[ecx]
    mov [ebx+4],eax
    mov [eax],ebx
    mov edx,[a]
    mov [eax+8],edx
    mov [ecx],eax
    mov dword[eax+4],ecx
    jmp for
rof:        
    ;PRINT_DEC 4,eax
    call sorting

    sub esp, 16
    mov eax,[finp]
    mov [esp],eax
    call fclose
    add esp, 16
    
    sub esp, 16
    mov eax,[foutp]
    mov [esp],eax
    call fclose
    add esp, 16
    
    mov esp,ebp
    pop ebp
    xor eax, eax
    ret
    
sorting:
    push ebp
    mov ebp,esp
    and esp,~0xf

forsorting:
    mov eax,[head]
    mov eax,[eax+4]
    cmp eax,[tail]
    jz rofsorting
    call findMin
    call printMin
    call removeMin
    jmp forsorting
rofsorting:
    mov esp,ebp
    pop ebp        
    ret
    
    
    
findMin:
    push eax
    push ebx
    
    mov ebx,[head]
    mov ebx,[ebx+4]
    mov [ptmin],ebx
    mov eax,[ebx+8]
    mov [min],eax
    mov ebx,[ebx+4]
    
forfM:
    cmp ebx,[tail]
    jz roffM
    mov eax,[min]
    cmp eax,[ebx+8] 
    jle extrafM ; if min <= cur
    mov [ptmin],ebx
    mov eax,[ebx+8]
    mov [min],eax
extrafM: ; else
    mov ebx,[ebx+4]
    jmp forfM
roffM:
    pop ebx
    pop eax
    ret
    
printMin:
    mov eax,[min]
    sub esp, 16
    mov [esp+8],eax
    mov eax,[foutp]
    mov [esp],eax
    mov dword[esp+4],fmt
    call fprintf
    add esp, 16
    ret
    
removeMin:
    mov eax,[ptmin]
    mov ebx,[eax]
    mov ecx,[eax+4]
    mov [ebx+4],ecx
    mov [ecx],ebx
    sub esp,16
    mov [esp],eax
    call free
    add esp, 16
    ret   