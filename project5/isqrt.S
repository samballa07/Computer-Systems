;;; Seth Amballa
;;; 115884939
;;; samballa

;;; isqrt
   .global isqrt
isqrt:
   cpi r24, 2 ; base case (n < 2)
   brlo 1f ; returns function if lower than 2

   movw r18, r24 ;copies r24 into r18
   push r18 ;pushes onto stack for later use
   push r19
   lsr r18 ;shifts one bit to the right for division by 2
   lsr r18 ;shifts another for division by 4 in total
   movw r24, r18 ;moves the new value of r18 into r24

   call isqrt ;recursive call

   clr r19 ;clears registers 19 and 18 
   pop r19 ;restores values from stack
   pop r18
   lsl r24 ;multiples value in r24 by 2

   movw r16, r24 ;moves value of r24 into r16
   inc r16 ;increases r16 by 1
   mul r16, r16 ;squares r16, puts product into r0
   movw r20, r0 ; places product from r0 into r20
   clr r1 ;r1 must always be 0

   cp r18, r20 ;checks that the product is greater than value in r18
   brlo 1f
   movw r24, r16 ; moves r16 into r24(return value)
   
1:
   ret ;returns