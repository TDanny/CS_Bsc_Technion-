.global _start



.section .text
_start:
leaq array1,%rax #use the address
leaq array2,%rbx
leaq mergedArray, %rcx
xor %r8, %r8 #index for array1
xor %r9, %r9 #index for array2
xor %r10, %r10 #index for merged
xor %r11, %r11 #last number entered to marged

loop_HW1:  
cmpl $0, (%rax, %r8,4)
je complete2_HW1
cmpl $0, (%rbx, %r9,4)
je complete1_HW1
movl (%rax, %r8,4), %edx #%edx will be a temp1
movl (%rbx, %r9,4), %esi #%esi will be temp2
cmpl %edx, %esi
jns enter_array2
cmpl %edx , %r11d
je repeatA_HW1
movl %edx,(%rcx,%r10,4) #enter array1
movl %edx,%r11d
inc %r8
inc %r10
jmp loop_HW1



repeatA_HW1:
inc %r8
jmp loop_HW1



repeatB_HW1:
inc %r9
jmp loop_HW1



enter_array2: #if (%rbx) is greater then (%rax)
cmpl %esi , %r11d
je repeatB_HW1
movl %esi,(%rcx, %r10,4)
movl %esi,%r11d
inc %r9
inc %r10
jmp loop_HW1


complete2_HW1:  #array1 ended and now we need to insert all the array2 leftovers to mergedarray
cmpl $0, (%rbx, %r9,4)
je end_HW1
movl (%rbx, %r9,4),%esi
movl %esi,(%rcx, %r10,4)
cmpl %esi , %r11d #check if exist already in marged
je repeatB_HW1
movl %esi,%r11d
inc %r9
inc %r10
jmp complete2_HW1

complete1_HW1:  #array2 ended and now we need to insert all the array1 leftovers to mergedarray
cmpl $0, (%rax, %r8,4)
je end_HW1
movl (%rax, %r8,4),%edx
movl %edx,(%rcx, %r10,4) 
cmpl %edx , %r11d #check if exist already in marged
je repeatA_HW1
movl %edx,%r11d
inc %r8
inc %r10
jmp complete1_HW1


end_HW1:
    movl $0, (%rcx, %r10,4)


										