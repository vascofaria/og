segment	.text
align	4
main:
	push	ebp
	mov	ebp, esp
	sub	esp, 8
	lea	eax, [ebp+8]
	push	eax
	pop	eax
	push	dword [eax]
	push	dword 0
	push	dword 3
	pop	ecx
	sal	dword [esp], cl
	pop	eax
	add	dword [esp], eax
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
	fld	qword [esp]
	add	esp, byte 8
	leave
	ret
segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 12
	push	dword 61
	lea	eax, [ebp+-12]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	pop	eax
	mov	[ecx+4], eax
	lea	eax, [ebp+-12]
	push	eax
	call	main
	add	esp, 4
	sub	esp, byte 8
	fstp	qword [esp]
	call	printd
	add	esp, 8
	push	dword 0
	pop	eax
	leave
	ret
extern	printd
