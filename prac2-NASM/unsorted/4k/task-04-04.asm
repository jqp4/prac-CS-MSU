%include "io.inc"
section .data
    fmtS     db `%u`  
    filename db "data.in", 0
    ;filename db "/media/sf_UbuntuShare/4k/data.in", 0
    fmtFread db `r` ,0
    fmtP     db `%u\n`
section .bss
    tmp   resd 1
    count resd 1
    file  resd 1
    
section .text
extern   fopen
extern   fclose
extern   fscanf
extern   printf
global   CMAIN
CMAIN:
    mov  ebp, esp       ; for correct debugging
    and  esp, -16       ; выравниваем стек
    sub  esp, 0x20      ; резервируем два параграфа по 16 байтов
    push fmtFread
    push filename
    call fopen
    add  esp, 8
    mov  dword[file], eax
    mov  dword[count],  0
lp:   
    push tmp            ; считываем число
    push fmtS
    push dword[file]
    call fscanf
    inc  dword[count]   ; инкриментируем счетчик (до проверки ввода, потому что так быстрее)
    add  esp, 12        ; возвращаемся к началу параграфа
    cmp  eax, -1        ; проверяем успех ввода
    jne  lp 
      
    dec  dword[count]   ; вычитаем лишнюю еденицу
    push dword[count]   ; выводим ответ и закрываем файл
    push fmtP
    call printf
    push dword[file]
    call fclose
    add  esp, 44        ; восстанавливаем стек
    mov  esp, ebp
    xor  eax, eax
    ret