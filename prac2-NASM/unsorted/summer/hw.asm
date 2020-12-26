; nasm -f elf64 -o syscall.o hw.asm
; nm syscall.o
; ld -o syscall syscall.o
; nm syscall
; ./syscall

section .data
    msg  db "Hello world!", 10
    mgl  equ $ - msg
    
section .text
global _start 
_start:
    mov  eax, 4
    mov  ebx, 1
    mov  ecx, msg
    mov  edx, mgl
    int  80h
    
    mov  eax, 1
    mov  ebx, 0
    int  80h