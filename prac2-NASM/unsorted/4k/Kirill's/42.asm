%include "io.inc"
section .data
    
    ;Запрос у пользователя
    msg1 db "Enter your string: ", 0
    ;Вспомогательные параметры
    msg2 db "%s", 0
    ;Хранение символов
    fLetter db 0
    sLetter db 0
    

    
section .bss
    fsl    resd 1
    ssl    resd 1
    fidx   resd 1
    sidx   resd 1
    contf resd 1
    ;Объявление строки
    
    fString:   resb 2560
    sString:   resb 2560
section .text
    CEXTERN printf
    CEXTERN scanf
    CEXTERN strlen
    CEXTERN strstr
global CMAIN
CMAIN:
    mov ebp, esp; for correct debugging
    push ebp
    mov ebp, esp; for correct debugging
    and esp, ~0xf
    sub esp, 8
    push fString 
    push msg2
    call scanf 
    mov dword[fidx], 0
    mov dword[sidx], 0
    add esp, 16
    sub esp, 8 
    push fString 
    call strlen 
    add esp, 16
    mov dword[fsl], eax
    sub esp, 8
    
    push sString 
    push msg2
    call scanf 
    add esp, 16
    sub esp, 8 
    push sString 
    call strlen 
    add esp, 16
    mov dword[ssl], eax
    
    mov ebx, [fsl]
    mov al, [fString + ebx -1]
    mov [fLetter], al
    mov eax, dword[fsl]
    mov edx, dword[ssl]
    mov dword[contf], 0
    cmp eax, edx 
    jg first_bigger
    jl second_bigger 
    je fend 
first_bigger:
    ;mov ebx, [fidx]
    ;mov al, [fString + ebx]
    ;mov [fLetter], al
    ;mov ebx, [sidx]
    ;mov al, [sString + ebx]
    ;mov [sLetter], al
    sub esp, 8
    push sString
    push fString
     
    call strstr
    add  esp, 16
    test eax, eax 
    jz  fend 
    PRINT_STRING "2 1"
    jmp end
second_bigger:
    sub esp, 8
    push fString
    push sString
    call strstr
    add esp, 16
    test eax, eax 
    jz fend 
    PRINT_STRING "1 2"
    jmp end
fend:
    PRINT_STRING "0"
end:
    mov esp,ebp
    pop ebp
    xor eax, eax
    ret
 