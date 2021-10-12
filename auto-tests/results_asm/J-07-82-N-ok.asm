segment	.text
align	4
f:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	push	dword 3
	lea	eax, [ebp+-4]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	lea	eax, [ebp+-4]
	push	eax
	pop	eax
	push	dword [eax]
	add	esp, 4
	lea	eax, [ebp+-4]
	push	eax
	pop	eax
	push	dword [eax]
	call	printi
	add	esp, 4
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
	push	dword 0
	pop	eax
	leave
	ret
extern	printi
