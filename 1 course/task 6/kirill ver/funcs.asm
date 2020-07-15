section .rodata

    f1_const dq 3.0
    f2_const dq 2.0

section .text
global f1
f1: ;f1(x) = e^(-x) + 3
     push ebp
     mov ebp, esp
     finit
     fld qword [ebp + 8] ; x
     fchs ; -x
     fldl2e ; log_2(e)
     fmulp ; (-x) * log_2(e)
     fld ST0
     fld1
     fxch ST0, ST2
     frndint
     fsub ST1, ST0
     fxch ST1, ST0
     f2xm1
     fadd ST0, ST2
     fscale
     fld qword[f1_const]
     faddp
     mov esp, ebp
     pop ebp
     ret

global df1
df1: ;f'1(x) = -e^(-x)
    push ebp
    mov ebp, esp
    finit
    fld qword [ebp + 8] ; x
    fchs ; -x
    fldl2e ; log_2(e)
    fmulp ; (-x) * log_2(e)
    fld ST0
    fld1
    fxch ST0, ST2
    frndint
    fsub ST1, ST0
    fxch ST1, ST0
    f2xm1
    fadd ST0, ST2
    fscale
    fchs
    mov esp, ebp
    pop ebp
    ret

global f2
f2: ;f2(x) = 2x-2
    push ebp
    mov ebp, esp
    finit
    fld qword[ebp + 8];x
    fld qword[f2_const];2
    fmulp ;2x
    fld qword[f2_const];+2
    fsubp
    mov esp, ebp
    pop ebp
    ret

global df2
df2: ;f2'(x) = 2
    push ebp
    mov ebp, esp
    finit
    fld qword [f2_const]
    mov esp, ebp
    pop ebp
    ret

global f3
f3: ;f3(x) = 1/x
    push ebp
    mov ebp, esp
    finit
    fld1
    fld qword[ebp + 8];x
    fdivp
    mov esp, ebp
    pop ebp
    ret

global df3
df3: ;f3'(x) = -1/(x^2)
    push ebp
    mov ebp, esp
    finit
    fld1
    fld qword[ebp + 8];x
    fld ST0
    fmulp
    fdivp
    fchs
    mov esp, ebp
    pop ebp
    ret
