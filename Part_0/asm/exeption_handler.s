.section .reset, "ax"
	movia	r2, main
	jmp		r2 			/* branch to main program */


.section .exceptions, "ax"
.set		noat	
.set		nobreak
.global EXCEPTION_HANDLER
EXCEPTION_HANDLER:
	subi	sp,  sp, 128
	stw	et,  96(sp)
	rdctl	et,  ctl4
	beq	et,  r0, SKIP_EA_DEC	# Interrupt is not external         
		subi	ea,  ea, 4			/* Must decrement ea by one instruction  for external interupts, so that the interrupted instruction will be run */
	SKIP_EA_DEC:
		stw	r1,  4(sp)				# Save all registers 
		stw	r2,  8(sp)
		stw	r3,  12(sp)
		stw	r4,  16(sp)
		stw	r5,  20(sp)
		stw	r6,  24(sp)
		stw	r7,  28(sp)
		stw	r8,  32(sp)
		stw	r9,  36(sp)
		stw	r10, 40(sp)
		stw	r11, 44(sp)
		stw	r12, 48(sp)
		stw	r13, 52(sp)
		stw	r14, 56(sp)
		stw	r15, 60(sp)
		stw	r16, 64(sp)
		stw	r17, 68(sp)
		stw	r18, 72(sp)
		stw	r19, 76(sp)
		stw	r20, 80(sp)
		stw	r21, 84(sp)
		stw	r22, 88(sp)
		stw	r23, 92(sp)
		stw	r25, 100(sp)			# r25 = bt (skip r24 = et, because it is saved above)
		stw	r26, 104(sp)			# r26 = gp
		# skip r27 because it is sp, and there is no point in saving this
		stw	r28, 112(sp)			# r28 = fp
		stw	r29, 116(sp)			# r29 = ea
		stw	r30, 120(sp)			# r30 = ba
		stw	r31, 124(sp)			# r31 = ra
		addi	fp,  sp, 128

		call	interrupt_handler		# Call the C language interrupt handler

		ldw	r1,  4(sp)				# Restore all registers
		ldw	r2,  8(sp)
		ldw	r3,  12(sp)
		ldw	r4,  16(sp)
		ldw	r5,  20(sp)
		ldw	r6,  24(sp)
		ldw	r7,  28(sp)
		ldw	r8,  32(sp)
		ldw	r9,  36(sp)
		ldw	r10, 40(sp)
		ldw	r11, 44(sp)
		ldw	r12, 48(sp)
		ldw	r13, 52(sp)
		ldw	r14, 56(sp)
		ldw	r15, 60(sp)
		ldw	r16, 64(sp)
		ldw	r17, 68(sp)
		ldw	r18, 72(sp)
		ldw	r19, 76(sp)
		ldw	r20, 80(sp)
		ldw	r21, 84(sp)
		ldw	r22, 88(sp)
		ldw	r23, 92(sp)
		ldw	r24, 96(sp)
		ldw	r25, 100(sp)			# r25 = bt
		ldw	r26, 104(sp)			# r26 = gp
		# skip r27 because it is sp, and we did not save this on the stack
		ldw	r28, 112(sp)			# r28 = fp
		ldw	r29, 116(sp)			# r29 = ea
		ldw	r30, 120(sp)			# r30 = ba
		ldw	r31, 124(sp)			# r31 = ra

		addi	sp,  sp, 128
	eret
	