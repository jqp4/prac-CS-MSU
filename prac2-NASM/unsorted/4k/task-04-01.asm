%include "io.inc"
section .bss
    tmp  resd 1
section .data
    fmtS db       `%u`, 0
    fmtP db `0x%08X\n`, 0
   
section .text
extern   scanf
extern   printf
global   CMAIN
CMAIN:
    mov  ebp, esp       ; for correct debugging
    and  esp, -16       ; выравниваем стек
    sub  esp, 0x20      ; резервируем два параграфа по 16 байтов
lp:
    push tmp            ; считываем число
    push fmtS
    call scanf
    cmp  eax, -1        ; проверяем успех ввода
    je   exit
    push dword[tmp]     ; выводим число
    push fmtP
    call printf
    add  esp, 16        ; возвращаемся к началу параграфа
    jmp  lp
exit:
    mov  esp, ebp       ; восстанавливаем стек
    xor  eax, eax
    ret