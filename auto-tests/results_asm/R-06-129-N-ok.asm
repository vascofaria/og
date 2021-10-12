segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 8
segment	.rodata
align	4
_L1_cdk_emitter_internal:
	dq	61.0000
segment	.text
	push	dword $_L1_cdk_emitter_internal
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
	lea	eax, [ebp+-12]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	pop	eax
	mov	[ecx+4], eax
	lea	eax, [ebp+-12]
	push	eax
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
	call	printd
	add	esp, 8
segment	.rodata
align	4
_L2_cdk_emitter_internal:
	dq	62.0000
segment	.text
	push	dword $_L2_cdk_emitter_internal
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
	push	esp
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
	lea	eax, [ebp+-12]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	pop	eax
	mov	[ecx+4], eax
	add	esp, 8
	lea	eax, [ebp+-12]
	push	eax
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
	call	printd
	add	esp, 8
	push	dword 0
	pop	eax
	leave
	ret
extern	printd
