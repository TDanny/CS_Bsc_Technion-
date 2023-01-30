.global _start


.section .text
_start:
	xor %r8,%r8
	xor %r9,%r9  #counter=0
	xor %r10, %r10   #WE WILL USE REGISTER R10 TO BE TEMP IF NEEDED


	movslq num, %r8
	leaq source, %rax
	leaq destination, %rbx
	cmpq $0, %r8    #if num==0 , no need to copy anything.
	je end_HW1
	test %r8, %r8
	js negative_HW1 #if the number is negative.
	

positive_HW1:
	cmpq %rax, %rbx  #if ADDRESSES source==destination and NUM is POSITIVE finish.
	je end_HW1
	js loop_HW1  #destination before source - act regular
	jg sourceBeforeDest_HW1
	

loop_HW1: # REGULAR CASE FOR POSITIVE NUM
	cmpq %r9,%r8
	je end_HW1
	movb (%rax,%r9), %cl
	movb %cl, (%rbx,%r9)
	inc %r9
	jmp loop_HW1


sourceBeforeDest_HW1: # we need to copy destination starting from the end of it.
	cmpq $0, %r8	
	je end_HW1	
	dec %r8	
	movb (%rax,%r8), %cl
	movb %cl, (%rbx,%r8)
	jmp sourceBeforeDest_HW1
	
	

negative_HW1:
	imull $-1, num, %r8d
	dec %r8
	cmpq %rax, %rbx
	je swap_HW1
	jg srcPreiorNG_HW1
	js destPreiorNG_HW1
		
	

srcPreiorNG_HW1:   # REGULAR CASE FOR NEGATIVE NUM AND CHECKS ALWAYS IF OVERLAP IS NEEDED
	movb (%rax), %cl
	movb %cl, (%rbx,%r8)
	incq %rax
	decq %r8
	cmpq $-1, %r8   #IF WE LOOPED "NUM" TIMES- FINISHED
	je end_HW1
	cmpq %rax, %rbx   #IF SRC ADDRESSES ISN'T SAME AS DEST - CONNTINUE SAME LOOP
	jne srcPreiorNG_HW1  
	
	#IF WE HERE WE NEED TO DO SWAP TO THE OVERLAPS SECTIONS IN THE ARRAYS USING SWAP_HW1
	jmp swap_HW1
	

destPreiorNG_HW1: # DESTINATION ADDRESS IS BEFORE SOURCE ADDRESS IN THE MEMORY 
	movb (%rax,%r8), %cl
	movb %cl, (%rbx) #MOVE FROM THE END OF SRC TO START OF DEST
	decq %r8
	incq %rbx
	cmpq $-1, %r8    #IF WE LOOPED "NUM" TIMES- FINISHED
	je end_HW1
	cmpq %rax, %rbx   #IF SRC ADDRESSES ISN'T SAME AS DEST - CONNTINUE SAME LOOP
	jnz destPreiorNG_HW1

	#IF WE HERE WE NEED TO DO SWAP TO THE OVERLAPS SECTIONS IN THE ARRAYS USING SWAP_HW1
	jmp swap_HW1



swap_HW1:  #IN CASE SOURCE==DESTINATION
	cmpq %r9, %r8
	je end_HW1
	js end_HW1 
	movb (%rax,%r8), %cl
	movb (%rax,%r9), %sil
	movb %cl, (%rax,%r9)
	movb %sil, (%rax,%r8)
	dec %r8
	inc %r9
	jmp swap_HW1




end_HW1:
