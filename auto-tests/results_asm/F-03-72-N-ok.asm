segment	.bss
align	4
x:
	resb	4
segment	.bss
align	4
y:
	resb	4
segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	push	dword 1
	push	dword [esp]
	push	dword $y
	pop	ecx
	pop	eax
	mov	[ecx], eax
align	4
_L1:
	push	dword $y
	pop	eax
	push	dword [eax]
	push	dword 3
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	setle	cl
	mov	[esp], ecx
	pop	eax
	cmp	eax, byte 0
	je	near _L3
	push	dword 1
	push	dword [esp]
	push	dword $x
	pop	ecx
	pop	eax
	mov	[ecx], eax
align	4
_L4:
	push	dword $x
	pop	eax
	push	dword [eax]
	push	dword 6
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	setle	cl
	mov	[esp], ecx
	pop	eax
	cmp	eax, byte 0
	je	near _L6
	push	dword $y
	pop	eax
	push	dword [eax]
	push	dword 2
	pop	ecx
	pop	eax
	cdq
	idiv	ecx
	push	edx
	push	dword 1
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	sete	cl
	mov	[esp], ecx
	pop	eax
	cmp	eax, byte 0
	je	near _L7
	jmp	dword _L5
	jmp	dword _L8
_L7:
	push	dword $x
	pop	eax
	push	dword [eax]
	call	printi
	add	esp, 4
_L8:
align	4
_L5:
	push	dword $x
	pop	eax
	push	dword [eax]
	push	dword 1
	pop	eax
	add	dword [esp], eax
	push	dword [esp]
	push	dword $x
	pop	ecx
	pop	eax
	mov	[ecx], eax
	jmp	dword _L4
align	4
_L6:
align	4
_L2:
	push	dword $y
	pop	eax
	push	dword [eax]
	push	dword 1
	pop	eax
	add	dword [esp], eax
	push	dword [esp]
	push	dword $y
	pop	ecx
	pop	eax
	mov	[ecx], eax
	jmp	dword _L1
align	4
_L3:
	push	dword 0
	pop	eax
	leave
	ret
extern	printi
