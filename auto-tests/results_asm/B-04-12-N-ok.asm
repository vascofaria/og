segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	push	dword 1
	push	dword 3
	pop	eax
	sub	dword [esp], eax
	push	dword 5
	pop	eax
	sub	dword [esp], eax
	call	printi
	add	esp, 4
	push	dword 0
	pop	eax
	leave
	ret
extern	printi
