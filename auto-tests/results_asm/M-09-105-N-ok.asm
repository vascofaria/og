segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 12
	push	dword 1
segment	.rodata
align	4
_L1_cdk_emitter_internal:
	dq	3.00000
segment	.text
	push	dword $_L1_cdk_emitter_internal
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
	lea	eax, [ebp+-8]
	push	eax
	pop	eax
	push	dword [eax]
	push	dword 16
	call	printi
	add	esp, 4
	push	dword 0
	pop	eax
	leave
	ret
extern	printi
