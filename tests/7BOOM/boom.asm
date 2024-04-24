BITS 32

section .text

global _start

section .data
	tmp db 100 dup(0)
	ten dw 10
	a dd 0

_start:
	mov eax, 1
	mov [a], eax
	mov eax, 100
for_loop_0:
	push eax
	mov ebx, [a]
	mov ecx, 7
	push edx
	push eax
	mov eax, ebx
	mov esi, ecx
	xor edx, edx
	idiv esi
	mov esi, edx
	pop eax
	pop edx
	mov ebx, esi
	mov ecx, 0
	cmp ebx, ecx
	setz bl
	cmp ebx, 0
	jz else_1
	mov ecx, 'B'
	mov [tmp], ecx
	pusha
	mov eax, 4
	mov ebx, 1
	mov ecx, tmp
	mov edx, 2
	int 0x80
	popa
	jmp end_1
else_1:
	mov ecx, [a]
	mov eax, ecx
	call printDecimal
end_1:
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
	mov ecx, 1
	add ebx, ecx
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
