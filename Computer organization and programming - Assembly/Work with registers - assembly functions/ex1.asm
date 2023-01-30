.global _start

.section .text

_start:
movq num, %rax
xorq %rdx, %rdx
movb Bool, %bl
xorb %bl, %bl
movq $4, %rcx

divq %rcx
cmpq $0, %rdx
jz no_reminder_HW1
jnz end_HW1

no_reminder_HW1:
movb $1, Bool

end_HW1:











 