segment	.text
align	4
f:
	push	ebp
	mov	ebp, esp
	sub	esp, 8
	lea	eax, [ebp+8]
	push	eax
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
	sub	esp, 4
	push	dword 10
	fild	dword [esp]
	sub	esp, byte 4
	fstp	qword [esp]
	call	f
	add	esp, 4
	sub	esp, byte 8
	fstp	qword [esp]
	call	printd
	add	esp, 8
	push	dword 2
	fild	dword [esp]
	sub	esp, byte 4
	fstp	qword [esp]
	call	f
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
