segment	.data
align	4
r:
	dq	61.0000
segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	push	dword 0
	pop	eax
	leave
	ret
