segment	.data
align	4
x:
	dd	10
segment	.data
align	4
y:
	dd	3
segment	.data
align	4
z:
	dd	30
segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	push	dword $x
	pop	eax
	push	dword [eax]
	push	dword $y
	pop	eax
	push	dword [eax]
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	setne	cl
	mov	[esp], ecx
	push	dword [esp]
	pop	eax
	cmp	eax, byte 0
	jne	near _L1
	push	dword 7
	push	dword [esp]
	push	dword $z
	pop	ecx
	pop	eax
	mov	[ecx], eax
	pop	eax
	or	dword [esp], eax
align	4
_L1:
	add	esp, 4
	push	dword $z
	pop	eax
	push	dword [eax]
	call	printi
	add	esp, 4
	push	dword 0
	pop	eax
	leave
	ret
extern	printi
