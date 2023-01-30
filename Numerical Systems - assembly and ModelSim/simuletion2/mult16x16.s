# Operands to multiply
.data
a: .word 0xBAD
b: .word 0xFEED

.text
main:   # Load data from memory
		la      t3, a
        lw      t3, 0(t3)
        la      t4, b
        lw      t4, 0(t4)
        
        # t6 will contain the result
        add		t6, x0, x0

        # Mask for 16x8=24 multiply
        ori		t0, x0, 0xff
        slli	t0, t0, 8
        ori		t0, t0, 0xff
        slli	t0, t0, 8
        ori		t0, t0, 0xff
        
####################
# Start of your code
	andi a4,t3,0xff#First 8 bits
	srli t3,t3,8 #move number to right
	andi a2,t3,0xff #Second 8 bits
    
    add t5,x0,x0 #helper-sum
    add t6,x0,x0 #result-sum
    
# mul second half from first number with all other number
   mul		t5, a2, t4
   and		t5, t5, t0
   
   add t6,x0,t5 #add result to sum
   add t5,x0,x0 #zero helper
   
   slli t6,t6,8 #move second half 8 bits
   
   # mul first half from first number with all other number
   mul		t5, a4, t4
   and		t5, t5, t0

   add t6,t6,t5
    
# End of your code
####################
		
finish: addi    a0, x0, 1
        addi    a1, t6, 0
        ecall # print integer ecall
        addi    a0, x0, 10
        ecall # terminate ecall