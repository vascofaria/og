segment	.text
align	4
f:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	lea	eax, [ebp+8]
	push	eax
	pop	eax
	push	dword [eax]
	call	prints
	add	esp, 4
	push	dword 10
	pop	eax
	leave
	ret
segment	.text
align	4
g:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	lea	eax, [ebp+8]
	push	eax
	pop	eax
	push	dword [eax]
	call	printi
	add	esp, 4
	push	dword 10
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
segment	.rodata
align	4
_L1:
	db	"ola", 0
segment	.text
	push	dword $_L1
	call	f
	add	esp, 4
	push	eax
	add	esp, 4
	push	dword 0
	pop	eax
	leave
	ret
extern	printi
extern	prints
