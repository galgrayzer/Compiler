BITS 32

section .text

global _start

section .data
	tmp db 100 dup(0)
	ten dw 10

_start:
	mov eax, 0
	mov ebx, 0
	cmp eax, 0
	je falseLabel_0
	cmp ebx, 0
	je falseLabel_0
	mov eax, 1
	jmp endLabel_0
falseLabel_0:
	mov eax, 0
endLabel_0:
	mov eax, eax
	call printDecimal

	mov eax, 1
	mov ebx, 0
	int 0x80

printDecimal:
	xor ecx, ecx
mone:
	inc ecx
	xor edx, edx
	div word [ten]
	push edx
	cmp eax, 0
	jg mone

putChar:
	pop edx
	add edx, '0'
	mov [tmp], edx
	pusha
	mov eax, 4
	mov ebx, 1
	mov ecx, tmp
	mov edx, 2
	int 0x80
	popa
	loop putChar
	ret
