;;; Seth Amballa
;;; 115884939
;;; samballa

;;; Implementation that computes the nth fibonacci number.

    .global fib                 ; exposes fib as a label for the linker
fib:                            ; the label: the next instruction starts fib() function.
    ;; Add your code here.
    cpi r24, 0 ;base case check for 0
    breq 1f ; branches to 1 if 0

    cpi r24, 1 ;base case check for 1
    breq 1f ;branches to 1, if 1

    push r24 ;saves parameter values for recursive call
    push r25

    subi r24, 1 ; subtract 1 from pointer(n - 1)
    call fib ; recursive call
   
    pop r23 ;restores values in new registers r22, r23
    pop r22
    push r24 ;saves current value of n in stack
    push r25

    subi r22, 2 ;subtracts 2 from r22(n - 2)
    mov r24, r22 ; moves r22 into r24(argument register)
    mov r25, r23
    call fib ;second recursive call

    pop r23 ;restores previous value
    pop r22
    add r24, r22 ;adds the previous value to current
    adc r25, r23 ;overflow bits are carried over
   
1:   
   ret ; returns.  necessary.
