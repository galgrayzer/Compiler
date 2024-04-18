BITS 32

section .text

global _start

section .data
	tmp db 100 dup(0)

_start:
	mov eax, 'H'
	mov [tmp], eax
	pusha
	mov eax, 4
	mov ebx, 1
	mov ecx, tmp
	mov edx, 2
	int 0x80
	popa
	mov eax, 'e'
	mov [tmp], eax
	pusha
	mov eax, 4
	mov ebx, 1
	mov ecx, tmp
	mov edx, 2
	int 0x80
	popa
	mov eax, 'l'
	mov [tmp], eax
	pusha
	mov eax, 4
	mov ebx, 1
	mov ecx, tmp
	mov edx, 2
	int 0x80
	popa
	mov eax, 'l'
	mov [tmp], eax
	pusha
	mov eax, 4
	mov ebx, 1
	mov ecx, tmp
	mov edx, 2
	int 0x80
	popa
	mov eax, 'o'
	mov [tmp], eax
	pusha
	mov eax, 4
	mov ebx, 1
	mov ecx, tmp
	mov edx, 2
	int 0x80
	popa
	mov eax, ' '
	mov [tmp], eax
	pusha
	mov eax, 4
	mov ebx, 1
	mov ecx, tmp
	mov edx, 2
	int 0x80
	popa
	mov eax, 'W'
	mov [tmp], eax
	pusha
	mov eax, 4
	mov ebx, 1
	mov ecx, tmp
	mov edx, 2
	int 0x80
	popa
	mov eax, 'o'
	mov [tmp], eax
	pusha
	mov eax, 4
	mov ebx, 1
	mov ecx, tmp
	mov edx, 2
	int 0x80
	popa
	mov eax, 'r'
	mov [tmp], eax
	pusha
	mov eax, 4
	mov ebx, 1
	mov ecx, tmp
	mov edx, 2
	int 0x80
	popa
	mov eax, 'l'
	mov [tmp], eax
	pusha
	mov eax, 4
	mov ebx, 1
	mov ecx, tmp
	mov edx, 2
	int 0x80
	popa
	mov eax, 'd'
	mov [tmp], eax
	pusha
	mov eax, 4
	mov ebx, 1
	mov ecx, tmp
	mov edx, 2
	int 0x80
	popa
	mov eax, '!'
	mov [tmp], eax
	pusha
	mov eax, 4
	mov ebx, 1
	mov ecx, tmp
	mov edx, 2
	int 0x80
	popa
	mov eax, 1
	mov ebx, 0
	int 0x80
