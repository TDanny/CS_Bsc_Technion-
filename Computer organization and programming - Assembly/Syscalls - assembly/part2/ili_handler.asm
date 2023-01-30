.globl my_ili_handler
.extern what_to_do, old_ili_handler

.text
.align 4, 0x90

my_ili_handler:

  #saves all registers on the stack
  pushq %rax
  pushq %rbx
  pushq %rcx
  pushq %rdx	
  pushq %r8
  pushq %r9
  pushq %r10
  pushq %r11
  pushq %r12
  pushq %r13
  pushq %r14
  pushq %r15
  pushq %rsi
  pushq %rbp
  pushq %rsp

  #we will use rdi, rax and rbx
  xorq %rdi, %rdi
  xorq %rax, %rax
  xorq %rbx,%rbx
  movq 120(%rsp),%rbx
  movq (%rbx), %rbx
  cmpb $0x0f, %bl
  jne one_byte_opcode

  #the opcode is 2 bytes, we will check the second
  movb %bh, %al
  movq %rax, %rdi
  call what_to_do
  cmpq $0, %rax
  je default_handler
  jmp my_handler

one_byte_opcode:
  movb %bl, %al
  movq %rax, %rdi
  call what_to_do
  cmpq $0, %rax
  je default_handler
  jmp my_handler

default_handler:
  popq %rsp
  popq %rbp
  popq %rsi
  popq %r15
  popq %r14
  popq %r13
  popq %r12
  popq %r11
  popq %r10
  popq %r9
  popq %r8
  popq %rdx
  popq %rcx
  popq %rbx
  popq %rax

  jmp * old_ili_handler
  jmp finish

my_handler:
  movq %rax, %rdi
  
  popq %rsp
  popq %rbp
  popq %rsi
  popq %r15
  popq %r14
  popq %r13
  popq %r12
  popq %r11
  popq %r10
  popq %r9
  popq %r8
  popq %rdx
  popq %rcx
  popq %rbx
  popq %rax
	
  addq $2, (%rsp) 
	
finish:
  iretq
