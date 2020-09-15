	.file	"palindrome_reference.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.global	is_palindrome
	.type	is_palindrome, @function
is_palindrome:
	push r28
	push r29
	rcall .
	rcall .
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 4 */
/* stack size = 6 */
.L__stack_usage = 6
	std Y+4,r25
	std Y+3,r24
	ldd r24,Y+3
	ldd r25,Y+4
	call strlen
	std Y+2,r24
	std Y+1,__zero_reg__
	rjmp .L2
.L5:
	ldd r24,Y+1
	mov r24,r24
	ldi r25,0
	ldd r18,Y+3
	ldd r19,Y+4
	add r24,r18
	adc r25,r19
	movw r30,r24
	ld r20,Z
	ldd r24,Y+2
	mov r18,r24
	ldi r19,0
	ldd r24,Y+1
	mov r24,r24
	ldi r25,0
	movw r22,r18
	sub r22,r24
	sbc r23,r25
	movw r24,r22
	sbiw r24,1
	ldd r18,Y+3
	ldd r19,Y+4
	add r24,r18
	adc r25,r19
	movw r30,r24
	ld r24,Z
	cp r20,r24
	breq .L3
	ldi r24,0
	rjmp .L4
.L3:
	ldd r24,Y+1
	subi r24,lo8(-(1))
	std Y+1,r24
.L2:
	ldd r24,Y+2
	mov r25,r24
	lsr r25
	ldd r24,Y+1
	cp r24,r25
	brlo .L5
	ldi r24,lo8(1)
.L4:
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r29
	pop r28
	ret
	.size	is_palindrome, .-is_palindrome
	.ident	"GCC: (GNU) 6.4.0"
