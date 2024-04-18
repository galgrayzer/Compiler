BITS 32

section .data
    tmp db 100 dup(0)

section .text
    global _start

_start:

    mov eax, 1
    mov ebx, 0
    int 0x80
