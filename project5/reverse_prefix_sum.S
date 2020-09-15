;;; Seth Amballa
;;; 115884939
;;; samballa

   .data
pctd:
      .asciz " %d" ; format string
pctd2:
      .asciz "%d" ;format string for first element

   .text

;;;Reverse prefix sum
   .global reverse_prefix_sum
reverse_prefix_sum:

   movw r26, r24 ;copies array into X pointer
   ldi r21, 0 ;sets registers to zero
   ldi r24, 0
   ldi r25, 0
   ld r20, X+ ;loads r20 with X post incremented

   cpi r20, -1 ;base case(n == -1) returns
   breq 1f ;branches to returns
   
   push r26 ;saves values of X
   push r27
   push r20 ; saves the current element for later use

   movw r24, r26 ;sets r24 with next element to send in recursive call
   call reverse_prefix_sum ; recursive call

   pop r20 ;restores the previous element
   pop r27 ;restores X
   pop r26

   add r24, r20 ; adds the previous element to the current one for prefix sum
   adc r25, r21 ;incase its a 16 bit integers

   sbiw r26, 1 ;moves array back one
   st X, r24 ; adds the new sum which exists in r24 to array

1:
   ret ;returns function


;;;Print array
   .global print_array
print_array:

   movw r26, r24 ;copies r24 to X pointer register
   clr r25 ;clears contents of r24, 25
   ld r24, X+ ; loads the first element in array to r25 with post increment

   cpi r24, -1 ; checks that array hasn't reached end
   breq 1f ; branches to return if -1

   ldi r22, lo8(pctd2) ;loads low byte of format string into r22
   ldi r23, hi8(pctd2) ;loads high byte of format string into r23
   call pint ;call to print first element

   ld r24, X+

2:
   cpi r24, -1 ; branches to 1(return) if element is -1
   breq 1f
   
   ldi r22, lo8(pctd)  ;loads a new format string for the rest of elements
   ldi r23, hi8(pctd) 
   call pint ;call to print element
   clr r25 ;clears contents of r25, r24
   ld r24, X+  ; loads the next value of array and increments r26
   jmp 2b ;goes to top of the loop

1:
   clr r25 ; clears contents of r24, r25
   ldi r24, 10 ;loads newline char into r24
   call putchar ;prints newline
   ret ; returns


pint:
;; prints an integer value, r22/r23 have the format string
      push r27 ;saves these registers
      push r26
      push r25
      push r24
      push r23
      push r22
      call printf
      pop r22 ;restores registers
      pop r23
      pop r24
      pop r25
      pop r26
      pop r27

      ret
