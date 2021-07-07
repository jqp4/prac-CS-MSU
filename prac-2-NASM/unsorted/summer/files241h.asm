; nasm -f elf64 -o fileopen.o files.asm
; nm fileopen.o
; ld -o fileopen fileopen.o
; nm fileopen
; ./fileopen

section .data
    msg  db "Hello world!", 10
    mgl  equ $ - msg
    filename db "myf.txt", 0
    fnl  equ $ - filename
    
section .text
global _start 
_start:
	mov  eax, 5
    mov  ebx, filename
    mov  ecx, 241h
    int  80h
    mov  esi, eax

    mov  eax, 4
    mov  ebx, esi;1
    mov  ecx, msg
    mov  edx, mgl
    int  80h

    mov  eax, 6
    mov  ebx, filename
    int  80h
    
    mov  eax, 1
    mov  ebx, 0
    int  80h