	.file	"test12.c"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq $0xeeeeeeeeeeeeeeee, %rbx
	movq $0xeeeeeeeeeeeeeeee, %r12
	movq $0xeeeeeeeeeeeeeeee, %r13
	movq $0xeeeeeeeeeeeeeeee, %r14
	movq $0xeeeeeeeeeeeeeeee, %r15
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movl	$1000, %esi
	movl	$44, %edi
	call	count_above@PLT
	movzwl	%ax, %eax
	mov	$255, %edi
	cmpq	(%rsp), %r15
	cmovne	%edi,%eax
	cmpq	0x8(%rsp), %r14
	cmovne	%edi,%eax
	cmpq	0x10(%rsp), %r13
	cmovne	%edi,%eax
	cmpq	0x18(%rsp), %r12
	cmovne	%edi,%eax
	cmpq	0x20(%rsp), %rbx
	cmovne	%edi,%eax
	add	$0x28, %rsp
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
