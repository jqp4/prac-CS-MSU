%include "io.inc"
section .data
    msgd db "%d", 0
    msgs db "%s", 0
    spa  db " "
    
section .bss
    num resd 1
    rez resd 1 
    fullString:  resb 256
    curString:   resb 256
    
section .text
CEXTERN printf
CEXTERN scanf
CEXTERN strstr
CEXTERN strcat
global CMAIN
CMAIN:
    mov  ebp, esp; for correct debugging
    push ebp
    mov  ebp, esp; for correct debugging
    and  esp, ~0xf
    
    sub  esp, 8
    push num
    push msgd
    call scanf 
    add  esp, 16
    
    sub  esp, 8
    push fullString 
    push msgs
    call scanf 
    add  esp, 16
    
    mov  dword[rez], 1
    
    sub  esp, 8
    push spa 
    push fullString
    call strcat 
    add  esp, 16
    
    mov  ecx, dword[num]
    dec  ecx 
    jz   end
    
    
abc:  
    mov  dword[num], ecx
    sub  esp, 8
    push curString 
    push msgs
    call scanf 
    add  esp, 16
    
    sub  esp,8
    push curString
    push fullString
    call strstr
    add  esp, 16
    
    test eax, eax 
    jnz  enl 
    add  dword[rez], 1
    enl:
    
    sub  esp, 8
    push spa 
    push fullString
    call strcat 
    add  esp, 16
    
    sub  esp, 8
    push curString 
    push fullString
    call strcat 
    add  esp, 16
    
    mov  ecx, dword[num]
    loop abc
    
    
    
end: 
    sub  esp, 8
    push dword[rez]
    push msgd
    call printf
    add  esp, 16
    mov  esp,ebp
    pop  ebp
    xor  eax, eax
    ret