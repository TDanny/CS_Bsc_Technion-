.global _start

.section .text
_start:
    movq root, %rax
    cmpq $0, %rax
    je empty_tree_HW1
    movq new_node(%rip), %rbx
traverse_HW1:
    movq (%rax), %rbx #data in current node
    movq %rax, %rcx
    cmpq new_node, %rbx
    je end_HW1
    js right_HW1
    jmp left_HW1
left_HW1:
    movq 8(%rax), %rax
	movq $8, %rsi
    cmpq $0, %rax #reached a leaf
    jne traverse_HW1
	movq $new_node, %r9
    movq %r9, (%rcx, %rsi)
    jmp end_HW1
right_HW1:
    movq 16(%rax), %rax
	movq $16, %rsi
    cmpq $0, %rax #reached a leaf
    jne traverse_HW1
    movq $new_node, %r9
    movq %r9, (%rcx, %rsi)
    jmp end_HW1
empty_tree_HW1:
    movq $new_node, root
    jmp end_HW1
    
end_HW1:
