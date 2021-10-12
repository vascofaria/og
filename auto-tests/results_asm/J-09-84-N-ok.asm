segment	.text
align	4
f:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	push	dword 2
	pop	eax
	leave
	ret
segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	call	f
	push	eax
	call	printi
	add	esp, 4
	push	dword 0
	pop	eax
	leave
	ret
extern	printi
