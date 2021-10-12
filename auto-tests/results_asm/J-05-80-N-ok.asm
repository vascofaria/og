segment	.text
align	4
pi:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	push	dword 314
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
	call	pi
	push	eax
	call	printi
	add	esp, 4
	push	dword 0
	pop	eax
	leave
	ret
extern	printi
