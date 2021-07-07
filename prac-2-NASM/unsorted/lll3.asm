%include "io.inc"
section .text
global f
f:
    ; if (!q) return 42;
    ; return (f(q->next) % 2) ? q->s : q->c;
    push ebp
    mov  ebp, esp
    push ecx
    sub  esp, 4
    mov  eax, 42
    mov  ecx, [ebp + 8]   ; ecx = q
    jecxz exit
    mov  edx, [ecx + 4]   ; edx = q->next = *(q + 4)
    ;push edx
    mov  [esp], edx
    call f                ; eax = f(q->next)
    add  esp, 4
    and  eax, 1           ; eax = f(q->next) % 2
    test eax, eax
    jz  retqc
    mov  eax, [ecx + 2]   ; eax = q->s
    jmp  exit
    retqc:
    mov  eax, [ecx]       ; eax = q->c
    exit:
    add  esp, 4
    pop  ecx
    pop  ebp
    ret