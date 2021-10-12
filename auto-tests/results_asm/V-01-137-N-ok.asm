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
	pop	eax
	cmp	eax, byte 0
	je	near _L1
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
_L2:
	db	"string1", 0
segment	.text
	push	dword $_L2
	leave
	ret
	jmp	dword _L3
_L1:
	push	dword 6
segment	.rodata
align	4
_L2_cdk_emitter_internal:
	dq	62.0000
segment	.text
	push	dword $_L2_cdk_emitter_internal
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
segment	.rodata
align	4
_L4:
	db	"string2", 0
segment	.text
	push	dword $_L4
	leave
	ret
_L3:
segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 8
