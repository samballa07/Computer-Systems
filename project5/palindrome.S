;;; Seth Amballa
;;; 115884939
;;; samballa

;;; Palindrome
         .global is_palindrome
is_palindrome:
         
   movw r30, r24
         
   call Strlen ;retrieves length of value in r24
   adiw r24, 1 ;adds one to accomodate for integer division
   movw r22, r24 ;moves (strlen+1) into r22
   lsr r22 ;divides the value by 2 to use as the upper limit

   ;;r26 is pointing after the null character of string after call to Strlen
   sbiw r26, 2 ;moves r26 back twice so it is now pointing to the last character

   ldi r18, 1 ;initialize lower index
4:       
         
   cp r22, r18 ;if r22 < r18, branch to 1
   brlo 1f

   ld r16, Z ;loads the front end of str into r16
   ld r17, X ;loads the back end of the str into r17

   cp r16, r17 ;branches to 3 if both are equal
   breq 3f

   jmp 2f ;branches to 2 if not equal and returns false

3:
   adiw r30, 1 ;adds 1 unit to pointer register(next character in strinf)
   sbiw r26, 1 ;subtracts 1 unit from pointer register(previous char in string)

   inc r18 ;increases index
   jmp 4b ; jumps to the top of the loop

2:
   ldi r24, 0 ;sets return value to false
   ret ;returns

1:       
   ldi r24, 1; sets return value to true
   ret ; returns



;;; Strlen
Strlen:
    ;;; uint16_t Strlen(char *arg)
    movw r26, r24       ;; copy argument to X (r27:26) pointer
    ldi r24, 0          ;; initialize return value to zero
    clr r25
2:  
    ld r22, X+          ;; load X with post-increment
    cpi r22, 0          ;; if loaded value was zero, branch 1f (label 1, forward)
    breq 1f
    adiw r24, 1         ;; increment return value
    jmp 2b              ;; jump 2b (label 2, backward)
1:
    ret                 ;; return
