BITS 32

section .text

global _start

section .data
	tmp db 100 dup(0)
	ten dw 10
	temp dd 0
	b dd 0
	a dd 0

_start:
	mov eax, 30
	mov [a], eax
	mov eax, 15
	mov [b], eax
	mov eax, 0
	mov [temp], eax
	mov eax, [a]
	mov ebx, [b]
	cmp eax, ebx
	setl al
	cmp eax, 0
	jz end_0
	mov ebx, [a]
	mov [temp], ebx
	mov ebx, [b]
	mov [a], ebx
	mov ebx, [temp]
	mov [b], ebx
end_0:
while_1:
	mov eax, [b]
	mov ebx, 0
	cmp eax, ebx
	setg al
	cmp eax, 0
	jz end_1
	mov ebx, [a]
	mov ecx, [b]
	cmp ebx, ecx
	setg bl
	cmp ebx, 0
	jz else_2
	mov ecx, [a]
	mov edx, [b]
	sub ecx, edx
	mov [a], ecx
	jmp end_2
else_2:
	mov ecx, [b]
	mov edx, [a]
	sub ecx, edx
	mov [b], ecx
end_2:
	jmp while_1
end_1:
	mov eax, [a]
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
