segment	.data
align	4
x:
segment	.rodata
align	4
_L1:
	db	"ole", 0
segment	.data
	dd	_L1
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
	call	prints
	add	esp, 4
	push	dword 0
	pop	eax
	leave
	ret
extern	prints
