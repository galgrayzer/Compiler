BITS 32

section .text

global _start

section .data
	tmp db 100 dup(0)
	ten dw 10
	c dd 0
	b dd 0
	a dd 0

_start:
	mov eax, 1
	mov [a], eax
	mov eax, 1
	mov [b], eax
	mov eax, [a]
	mov eax, eax
	call printDecimal
	mov eax, ' '
	mov [tmp], eax
	pusha
	mov eax, 4
	mov ebx, 1
	mov ecx, tmp
	mov edx, 2
	int 0x80
	popa
	mov eax, [b]
	mov eax, eax
	call printDecimal
	mov eax, ' '
	mov [tmp], eax
	pusha
	mov eax, 4
	mov ebx, 1
	mov ecx, tmp
	mov edx, 2
	int 0x80
	popa
	mov eax, 10
for_loop_0:
	push eax
	mov ebx, [a]
	mov ecx, [b]
	add ebx, ecx
	mov [c], ebx
	mov ebx, [c]
	mov eax, ebx
	call printDecimal
	mov ebx, ' '
	mov [tmp], ebx
	pusha
	mov eax, 4
	mov ebx, 1
	mov ecx, tmp
	mov edx, 2
	int 0x80
	popa
	mov ebx, [a]
	mov [b], ebx
	mov ebx, [c]
	mov [a], ebx
	pop eax
	dec eax
	jnz for_loop_0

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
