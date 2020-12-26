%include "io.inc"

section .data
    fmtInt db '%d' , 0 
    fmtStr db '%s' , 0  
    ans    dd 1
    
section .bss
    N    resd 1
    Nm1  resd 1
    strs resd 500
    tmps resb 11  
    
section .text
extern   free
extern   scanf
extern   printf
extern   malloc
extern   strcmp
extern   strstr
extern   strlen
extern   strcpy
global   CMAIN


freestrs:
    push ebx
    xor  ebx, ebx
    lpfss:
        mov  eax, dword[strs + ebx*4] 
        push eax
        call free
        pop  eax
        inc  ebx
        cmp  ebx, [N]
        jl   lpfss
    pop  ebx
    ret
    
    
getN:
    push N
    push fmtInt
    call scanf
    add  esp, 8
    mov  eax, [N]
    dec  eax
    mov  [Nm1], eax
    ret
    
    
getstrs:
    push ebx
    xor  ebx, ebx
    lpgss:
        push tmps
        push fmtStr
        call scanf
        push tmps
        call strlen
        add  eax, 4
        push eax
        call malloc
        mov  dword[strs + ebx*4], eax
        push tmps
        push eax
        call strcpy
        add  esp, 24
        inc  ebx
        cmp  ebx, dword[N]
        jl   lpgss
    pop ebx
    ret


getanswer:
    push esi
    push edi
    xor  esi, esi
    lp1:
        mov  edi, esi
        inc  edi
        lp2:
            push dword[strs + esi*4]
            push dword[strs + edi*4]
            call strcmp
            add  esp, 8
            test eax, eax
            jz   skipga
            inc  edi
            cmp  edi, [N]
            jl   lp2
        inc  dword[ans]
        skipga:
        inc  esi
        cmp  esi, [Nm1]
        jl   lp1
    pop  edi
    pop  esi
    ret


printanswer:
    push dword[ans]
    push fmtInt
    call printf
    add  esp, 8
    ret
    


CMAIN:
    mov  ebp, esp      ; for correct debugging
    and  esp, -16      ; выравниваем стек
    sub  esp, 0x20     ; резервируем два параграфа по 16 байтов
    
    call getN
    call getstrs   
    call getanswer
    call printanswer
    call freestrs
    
    mov  esp, ebp      ; восстанавливаем стек
    xor  eax, eax
    ret
    
    
    
    