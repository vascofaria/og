segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	push	dword 3
	push	dword 10
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	setge	cl
	mov	[esp], ecx
	call	printi
	add	esp, 4
	push	dword 0
	pop	eax
	leave
	ret
extern	printi
