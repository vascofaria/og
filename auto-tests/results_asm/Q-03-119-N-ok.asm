segment	.data
align	4
r:
	dq	61.0000
segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 8
	lea	eax, [ebp+-8]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	push	dword $r
	push	dword [esp]
	lea	eax, [ebp+-8]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	add	esp, 4
	lea	eax, [ebp+-8]
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
	call	printd
	add	esp, 8
	call	println
	push	dword 0
	pop	eax
	leave
	ret
extern	printd
extern	println
