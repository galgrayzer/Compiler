BITS 32

section .text

global _start

section .data
    msg db 0, 0

_start:
    mov esi, 26
    mov byte msg[0], 'A'
print_loop:
    mov eax, 4;system call number 4 is sys_write
    mov ebx, 1;file descriptor 1 is stdout
    mov ecx, msg;pointer to the message
    mov edx, 2
    int 0x80

    inc byte msg[0]

    push dword [msg]
    mov byte msg[0], ' '
    mov eax, 4;system call number 4 is sys_write
    mov ebx, 1;file descriptor 1 is stdout
    mov ecx, msg;pointer to the message
    mov edx, 2
    int 0x80
    pop dword [msg]

    dec esi
    jnz print_loop

    ; syscall exit(0)
    mov eax, 1;system call number 1 is exit()
    mov ebx, 0;exit(0)
    int 0x80;call it