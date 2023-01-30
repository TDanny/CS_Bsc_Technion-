.text
main:   # Start of your code
	lw 		x1,8(x0)
	lw		x2,12(x0)
	sub		x1,x0,x1
	sub		x2,x0,x2
	sw		x1,16(x0)
	sw		x2,20(x0)
        # End of your code
        add		t6, x0, x0
        beq		t6, x0, finish

deadend: beq	t6, x0, deadend        

finish:
        lw		t4, 0(x0)
        lw		t5, 4(x0)
        sw		t5, 0xFF(t4)
        beq		t6, x0, deadend


