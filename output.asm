BITS 32

section .text

global _start

section .data
	tmp db 100 dup(0)
	b dd 0

_start:
	mov eax, 4
	mov ebx, 3
	mov ecx, 5
	add ebx, ecx
	imul eax, ebx
	mov [b], eax
	mov eax, [b]
	mov ebx, 48
	add eax, ebx
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
