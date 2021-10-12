segment	.data
align	4
i:
	dd	3
segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	push	dword $i
	pop	eax
	push	dword [eax]
	call	printi
	add	esp, 4
	push	dword 5
	push	dword [esp]
	push	dword $i
	pop	ecx
	pop	eax
	mov	[ecx], eax
	add	esp, 4
	push	dword $i
	pop	eax
	push	dword [eax]
	call	printi
	add	esp, 4
	push	dword 0
	pop	eax
	leave
	ret
extern	printi
