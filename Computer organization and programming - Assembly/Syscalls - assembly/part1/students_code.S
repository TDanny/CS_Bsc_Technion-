.global count_above , atam_atol , get_path

.section .data
.section .text
count_above:
	
	xorq %rcx, %rcx
	xorq %r9, %r9	
	xorq %r12, %r12
	xorq %r8,%r8  
	xorq %rbx, %rbx 
	movq %rdi, %r9 # save seperator symbol
	movq %rsi , %r12 #save limit
	xorq %r15, %r15 # counter for the number which greater then limit
	xorq %r13 , %r13  
	xorq %rax, %rax
	pushq %rdi
	pushq %rsi
	pushq %r8
	pushq %r9
	call get_path  #now we got the address which hold the path to the file we need to read.
	popq %r9	
	popq %r8
	popq %rsi
	popq %rdi
	movq %rax , %r13 #backup the adress
	
	#open the file with read premissions only	
	movq $2, %rax
	movq %r13, %rdi
	movq $0, %rsi
	movq $0444, %rdx #read only premission
	syscall

	#we need to check that we opened the file currectly:
	cmpq $-1 , %rax
	je fail_HW2
	movq %rax, %r8 #SAVE DESCRIPTOR
	xorq %r14, %r14  #counter to the for loop
	xorq %r10, %r10 #WILL HOLD THE ADDRESS TO READ FROM 
	#I WANT TO PUSH THE ARRAY TO THE STACK
	sub $21, %rsp
	movq %rsp, %r10
	movq %rsp ,%rcx #SAVE THE PLACE WE STARTED TO READ FROM
	

loop_HW2:
	#lets start to read
	
	movq $0, %rax
	movq %r8, %rdi
	movq %rsp , %rsi
	movq $1, %rdx
	syscall
	
	#after each read we will check if we read the seperator sign
	#movb inputbuf(, %r14 , 1), %bl	
	cmpb (%rsp), %r9b
	je next_HW2
	cmpb $0, (%rsp)
	je next_HW2
	cmpq $0, %rax 	#AT THE END OF THE FILE ,SYSREAD WOULDNT READ AT ALL AND THATS WHY %RAX=0
	je next_HW2
	inc %r14
	add $1, %rsp
	jmp loop_HW2

next_HW2:
	cmpq $0, %r14 #if we read nothing meaning EOF
	je finish_HW2 
	movb $0, (%rsp) #im adding null terminator in ascii which is 0	
	#movb $0, inputbuf(, %r14 , 1) #im adding null terminator in ascii which is 0
	#lets call atam_atol function 	
	 #pointer to the input buffer is copyed to %rdi becuase its first
	movq %r10, %rsp     
	pushq %rdi
	pushq %rsi
	pushq %r8
	pushq %r9
	
call_atam_HW2:
	movq %r10 , %rdi #parameter to get inside the atam_alog function
	call atam_atol
	popq %r9
	popq %r8
	popq %rsi
	popq %rdi
debug_HW2:
	cmpq %rax, %r12  #COMPARE THE NUMBER TO LIMIT
	jg new_word_HW2
	jz new_word_HW2
	inc %r15         #RAISE NUMBER OF NUMBERS GREATER THEN LIMIT
	jmp new_word_HW2 #REPEAT THE PROCESS WITH NEW WORD
			
new_word_HW2:
	movq $0, %r14  #reset counter
	movq %rsp, %r10
	jmp loop_HW2

finish_HW2:
	#CLOSE THE FILE WE ARE READING FROM:
	movq $3, %rax
	movq %r8, %rdi
	syscall
	
	movq %r15, %rax 
	add $21, %rsp
	ret
	

fail_HW2:
	movq $-1 ,%rdi
	movq $60, %rax
	syscall

  ret
	 
