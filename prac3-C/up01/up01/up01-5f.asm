.intel_syntax noprefix
.text
.global exchange
exchange:
    pushad
    mov  ecx, edx
    xor  edx, edx
    shr  ecx, 1
    jecxz exchexit
    exchlp:
    mov  esi, [esp + 20]
    sub  esi, ecx
    lea  esi, [eax + esi * 4]
    lea  edi, [eax + ecx * 4 - 4]
    mov  ebx, [esi]
    mov  ebp, [edi]
    cmp  ebx, ebp
    ja   exchskip
    mov  [esi], ebp
    mov  [edi], ebx
    inc  edx
    exchskip:
    dec  ecx
    jnz  exchlp
    exchexit:
    mov  [esp + 24], edx
    popad
    ret