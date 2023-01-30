.global _start

.section .text
_start:
	movq head(%rip), %rdi
	movl val(%rip), %eax
	movsx %eax, %rax
	xor %rbx, %rbx
	xor %r10, %r10
	testq %rdi, %rdi
	je end_HW1
.loop_HW1:
	movq (%rdi), %rcx
	cmpq %rax, %rcx #val==*node
	jne .no_cond_HW1
	testq %rbx, %rbx
	jne .not_first_HW1
	movq %rdi, %r8
	movq %r10, %r11
	jmp .not_first_and_second_HW1
.not_first_HW1:
	cmpq $2, %rbx
	je end_HW1
	movq %r10, %r12
	movq %rdi, %r9
.not_first_and_second_HW1:
	inc %rbx
.no_cond_HW1:
	movq %rdi, %r10
	movq 8(%rdi), %rdi
	testq %rdi, %rdi
	jne .loop_HW1

	cmpq $2, %rbx
	jne end_HW1
	cmpq $0, %r11
	jz .no_pre_node_HW1
	movq %r9, 8(%r11)
.no_pre_node_HW1:
	movq %r8, 8(%r12)
	movq 8(%r9), %rax
	movq 8(%r8), %rbx
	movq %rax, 8(%r8)
	movq %rbx, 8(%r9)
	cmpq %r8, head
	jne end_HW1
	movq %r9, head
end_HW1:
