segment	.data
align	4
x:
	dd	10
segment	.data
align	4
y:
	dd	3
segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	push	dword $y
	pop	eax
	push	dword [eax]
	push	dword $x
	pop	eax
	push	dword [eax]
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	setg	cl
	mov	[esp], ecx
	call	printi
	add	esp, 4
	push	dword 0
	pop	eax
	leave
	ret
extern	printi
