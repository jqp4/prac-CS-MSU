%include "io.inc"
%define  node_size 8
%define  NULL 0

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
    n    resd 1
    tmp  resd 1
    file resd 1
    root resd 1
    node resd 1
    
section .text
extern   fopen
extern   fclose
extern   fscanf
extern   fprintf
extern   malloc
global   CMAIN

print_list:
    mov  eax, [root]
    cmp  eax, NULL
    je   exitps
    lppl:
        PRINT_DEC 4, [eax]  ; this is for debug
        PRINT_CHAR 32
        mov  eax, [eax + 4]
        cmp  eax, NULL
        jne  lppl
    exitps:
    ret
    
    
fprint_list:
    push fmtFwrite
    push filenameO
    call fopen
    mov  dword[file], eax
    push ebx
    mov  ebx, [root]
    cmp  ebx, NULL
    je   exitfpl
    lpfpl:
        push dword[ebx]
        push fmtP
        push dword[file]
        call fprintf
        add  esp, 12
        mov  ebx, [ebx + 4]
        cmp  ebx, NULL
        jne  lpfpl
    exitfpl:
    pop  ebx
    push dword[file]
    call fclose
    add  esp, 12
    ret


get_list:
    push ebp
    mov  ebp, esp
    push fmtFread
    push filenameI
    call fopen
    add  esp, 8
    mov  dword[file], eax
    xor  ebx, ebx
    ; считываем первое число
    push tmp                     
    push fmtS
    push dword[file]
    call fscanf
    add  esp, 12                 ; возвращаемся к началу параграфа
    cmp  eax, -1                 ; проверяем успех ввода
    je   skipgs 
    ; заносим первое число в список
    push node_size
    call malloc
    mov  [root], eax             ; root = node->addr
    mov  [node], eax             ; node->addr = malloc(node_size)
    mov  ecx, dword[tmp]        
    mov  [eax], ecx              ; node->data = tmp
    mov  dword[eax + 4], NULL    ; node->next = NULL
    inc  ebx
    lpgs:   
        push tmp                     ; считываем число
        push fmtS
        push dword[file]
        call fscanf
        add  esp, 12                 ; возвращаемся к началу параграфа
        cmp  eax, -1                 ; проверяем успех ввода
        je   skipgs 
        ; заносим число в список
        push node_size
        call malloc
        mov  edx, [node]
        mov  [node], eax           
        mov  [edx + 4], eax
        mov  ecx, dword[tmp]        
        mov  [eax], ecx            
        mov  dword[eax + 4], NULL   
        inc  ebx
        jmp  lpgs
    skipgs:  
    mov  dword[n], ebx
    push dword[file]
    call fclose
    mov  esp, ebp
    pop  ebp
    ret


sort_list:
    mov  edx, [n]
    cmp  edx, 1
    jle  exitsl
    dec  edx
    push esi
    push edi
    lpsl1:
        mov  ecx, [n]
        dec  ecx
        mov  esi, [root]
        lpsl2: 
            mov  eax, [esi]             
            mov  edi, [esi + 4]
            cmp  eax, [edi]
            jl   skipsl1                   
            xchg eax, [edi]
            mov  [esi], eax                
            skipsl1:
                mov  esi, edi
                loop lpsl2
        dec  edx
        jnz  lpsl1
    pop  edi
    pop  esi
    exitsl:
    ret
    

CMAIN:
    mov  ebp, esp      ; for correct debugging
    and  esp, -16      ; выравниваем стек
    sub  esp, 0x20     ; резервируем два параграфа по 16 байтов
    call get_list      ; считываем список
    call sort_list     ; сортируем список
    call print_list    ; выводим список на экран (for debug)
    call fprint_list   ; выводим список в файл
    mov  esp, ebp      ; восстанавливаем стек
    xor  eax, eax
    ret
    
    
    
    