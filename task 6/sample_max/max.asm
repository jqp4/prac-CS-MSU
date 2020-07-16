SECTION .rodata
    fmtS    DB "%d%d", 0
    fmtP    DB "%d", 10, 0
    
SECTION .text
EXTERN scanf
EXTERN printf
GLOBAL main
main:
    mov     ebp, esp                      ; for correct debugging
    LEA     ECX, [ESP + 4]                ; выравниваем стек ;mov ecx, esp ;add ecx, 4
    AND     ESP, -16
    PUSH    DWORD [ECX - 8]
    PUSH    EBP                           ; создаём фрейм
    MOV     EBP, ESP
    PUSH    ECX                           ; сохраняем ECX
    SUB     ESP, 20                       ; резервируем два параграфа по 16 байтов
    
    MOV     DWORD [ESP], fmtS             ; готовим параметры для scanf()
    LEA     EAX, [ESP + 12]               ; локальная переменная
    MOV     DWORD [ESP + 4], EAX
    LEA     EAX, [ESP + 16]               ; локальная переменная
    MOV     DWORD [ESP + 8], EAX
    CALL    scanf
    
    MOV     EAX, DWORD [ESP + 16]         ; вычисляем максимум
    CMP     EAX, DWORD [ESP + 12]
    CMOVL   EAX, DWORD [ESP + 12]
    MOV     DWORD [ESP], fmtP             ; готовим параметры для printf()
    MOV     DWORD [ESP + 4], EAX
    CALL    printf    
    
    ADD     ESP, 20                       ; восстанавливаем стек
    POP     ECX
    LEA     ESP, [ECX - 4]    
    XOR     EAX, EAX                      ; возвращаем 0
    RET
