segment	.bss
align	4
x:
	resb	4
segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	push	dword 4
	push	dword [esp]
	push	dword $x
	pop	ecx
	pop	eax
	mov	[ecx], eax
	add	esp, 4
	push	dword $x
	pop	eax
	push	dword [eax]
	call	printi
	add	esp, 4
	push	dword 0
	pop	eax
	leave
	ret
extern	printi
