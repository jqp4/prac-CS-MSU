%include "io.inc"
section .data
    fmtS      db '%d' , 0  
    fmtP      db '%d ', 0
    fmtPlast  db '%d' , 0
    filenameI db "input.txt", 0
    filenameO db "output.txt", 0
    ;filenameI db "/media/sf_UbuntuShare/4k/input.txt", 0
    ;filenameO db "/media/sf_UbuntuShare/4k/output.txt", 0
    fmtFread  db 'r', 0    
    fmtFwrite db 'w', 0
section .bss
    tmp  resd 1
    n    resd 1
    file resd 1
    seq  resd 1000
    i resd 1    
    j resd 1
    
section .text
extern   fopen
extern   fclose
extern   fscanf
extern   printf
extern   fprintf
extern   qsort
global   CMAIN

print_seq:
    mov  eax, [n]
    test eax, eax
    jz   exitps
    dec  eax
    xor  ecx, ecx
    test eax, eax
    jz   skipps
    lpps:
        PRINT_DEC 4, [seq + ecx*4]  ; this is for debug
        PRINT_CHAR 32
        inc  ecx
        cmp  ecx, eax
        jl   lpps
    skipps:
    PRINT_DEC 4, [seq + ecx*4]      ; last print
    exitps:
    ret
    
    
fprint_seq:
    push fmtFwrite
    push filenameO
    call fopen
    mov  dword[file], eax
    push ebx
    push esi
    mov  esi, [n]
    test esi, esi
    jz   exitfps
    dec  esi
    xor  ebx, ebx
    test esi, esi
    jz   skipfps
    lpfps:
        lea  eax, [seq + ebx * 4]
        push dword[eax]
        push fmtP
        push dword[file]
        call fprintf
        add  esp, 12
        inc  ebx
        cmp  ebx, esi
        jl   lpfps
    skipfps:
    lea  eax, [seq + ebx * 4]      ; last print
    push dword[eax]
    push fmtPlast
    push dword[file]
    call fprintf
    add  esp, 12
    exitfps:
    pop  esi
    pop  ebx
    push dword[file]
    call fclose
    add  esp, 12
    ret


get_seq:
    push ebp
    mov  ebp, esp
    push fmtFread
    push filenameI
    call fopen
    add  esp, 8
    mov  dword[file], eax
    xor  ebx, ebx
    lpgs:   
        push tmp                     ; считываем число
        push fmtS
        push dword[file]
        call fscanf
        add  esp, 12                 ; возвращаемся к началу параграфа
        cmp  eax, -1                 ; проверяем успех ввода
        je   skipgs 
        mov  eax, dword[tmp]         ; заносим число в последовательность
        mov  dword[seq + ebx*4], eax 
        inc  ebx
        jmp  lpgs
    skipgs:  
    mov  dword[n], ebx
    push dword[file]
    call fclose
    mov  esp, ebp
    pop  ebp
    ret


sort_seq_old:
    ; bubbleSort    ; не нужная функция
    push ebp
    mov  ebp, esp
    push esi
    push edi
    mov  eax, [n]
    dec  eax
    xor  edx, edx       ; i
    lpss1:
        xor  ecx, ecx   ; j
        lpss2:
            mov  esi, [seq + ecx*4]     ; esi = seq[j]
            mov  edi, [seq + ecx*4 + 4] ; edi = seq[j + 1]
            cmp  esi, edi               ; if (a[j] > a[j + 1]) {
            jng  skipss
            mov  [seq + ecx*4], edi     ; swap(a[j], a[j + 1]) }
            mov  [seq + ecx*4 + 4], esi
            skipss:
            inc  ecx
            cmp  ecx, eax
            jl   lpss2
            inc  edx
            cmp  edx, eax
            jl   lpss1
    pop  edi
    pop  esi
    mov  esp, ebp
    pop  ebp
    ret


comp:
    mov  eax, dword[esp + 4]   ; eax = i
    mov  eax, dword[eax]
    mov  ecx, dword[esp + 8]   ; ecx = j
    mov  ecx, dword[ecx]
    cmp  eax, ecx
    je   ret0
    jg   ret1
    jl   retm1
    ret0:
    mov  eax, 0
    ret
    ret1:
    mov  eax, 1
    ret
    retm1:
    mov  eax, -1
    ret


CMAIN:
    mov  ebp, esp      ; for correct debugging
    and  esp, -16      ; выравниваем стек
    sub  esp, 0x20     ; резервируем два параграфа по 16 байтов
    call get_seq       ; считываем последовательность
    
    push comp          ; сортировка последовательности
    push 4
    push dword[n]
    push seq
    call qsort
    
    ;call print_seq    ; выводим последовательность на экран (for debug)
    call fprint_seq    ; выводим последовательность в файл
    mov  esp, ebp      ; восстанавливаем стек
    xor  eax, eax
    ret
    
    
    
    