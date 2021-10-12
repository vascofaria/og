segment	.text
align	4
f:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	push	dword 5
segment	.rodata
align	4
_L1_cdk_emitter_internal:
	dq	61.0000
segment	.text
	push	dword $_L1_cdk_emitter_internal
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
segment	.rodata
align	4
_L1:
	db	"string", 0
segment	.text
	push	dword $_L1
	leave
	ret
segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 8
	call	f
	lea	eax, [ebp+-8]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
