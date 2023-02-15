#include "testasm.h"

__attribute__((used))
unsigned int sumval(unsigned char a, unsigned char b) {
  unsigned int out;

  asm volatile (R"(

   rjmp start_of_assembly_sumval

   .data
   ; any data values/static storage can go here

   .text

start_of_assembly_sumval:
   ; Your assembly language program goes here
   ;
   ; For this example, your program must not modify any registers other than 
   ; r24 and r25. In other words, save and restore all the registers that
   ; are modified by your code.

   ; Tell the compiler to move the arguments a, b into registers 
   ; r24, r25 respectively
   ;
   mov r24,%1
   mov r25,%2
   ;
   ;  --- YOUR CODE GOES HERE ---
   ;       r24 = a, r25 = b
   ;
   add r24, r25
   clr r25
   adc r25, r25
   
   ;rjmp end_of_assembly_sumval
   ;
   ;  --- YOUR CODE ENDS ---
   ;      The result must be in the register pair r25:r24
   ;      You can also rjmp to the label end_of_assembly if you need to.
   ;
end_of_assembly_sumval: 
   ; -- move r25:r24 to the 16-bit word in variable out
   movw %0,r24

)" : "=w" (out)  /* out is %0, and a 16-bit variable that is modified
        by the assembly language */
   : "r" (a), "r" (b)  /* a is %1, b is %2, both are register-sized
        variables */
   : "r25", "r24");   /* r24, r25 are modified by this assembly
       language fragment */

  return out;
}


/* the attribute "used" tells the compiler to not inline this function */
__attribute__((used))
unsigned int diffval(unsigned char a, unsigned char b) {
  unsigned int out;

  asm volatile (R"(

   rjmp start_of_assembly_diffval

   .data
   ; any data values/static storage can go here

   .text

start_of_assembly_diffval:
   ; Your assembly language program goes here

   ; Tell the compiler to move the arguments a, b into registers 
   ; r24, r25 respectively
   ;
   mov r24,%1
   mov r25,%2
   ;
   ;  --- YOUR CODE GOES HERE ---
   ;       r24 = a, r25 = b
   ;
   cp r25, r24
   brcc nocarry
   ; r24 >= r25
   sub r24, r25
   clr r25
   rjmp end_of_assembly_diffval

nocarry:
   ; r25 > r24
   sub r25, r24
   mov r24, r25
   clr r25

   
   rjmp end_of_assembly_diffval
   ;
   ;  --- YOUR CODE ENDS ---
   ;      The result must be in the register pair r25:r24
   ;      You can also rjmp to the label end_of_assembly if you need to.
   ;
end_of_assembly_diffval: 
   ; -- move r25:r24 to the 16-bit word in variable out
   movw %0,r24

)" : "=w" (out)  /* out is %0, and a 16-bit variable that is modified
        by the assembly language */
   : "r" (a), "r" (b)  /* a is %1, b is %2, both are register-sized
        variables */
   : "r25", "r24");   /* r24, r25 are modified by this assembly
       language fragment */

  return out;
}



/* the attribute "used" tells the compiler to not inline this function */
__attribute__((used))
unsigned int testasm(unsigned char a, unsigned char b) {
  unsigned int out;

  asm volatile (R"(

   rjmp start_of_assembly_testasm

   .data
   ; any data values/static storage can go here

   .text

start_of_assembly_testasm:
   ; Your assembly language program goes here

   ; Tell the compiler to move the arguments a, b into registers 
   ; r24, r25 respectively
   ;
   mov r24,%1
   mov r25,%2
   ;
   ;
   ;  ------ CODE FOR testasm BELOW ------
   
   ; save our args for the subsequent calls
   push 25
   push 24
   call sumval

   ;save the output into safe registers
   mov r22, r24
   mov r23, r25

   ;get our args back for diffval
   pop 24
   pop 25
   call diffval

   ; add our results into r24:r25 and return from testasm
   add r24, r22
   adc r25, r23
   rjmp end_of_assembly_testasm

   ;  ------ CODE FOR testasm ABOVE ------
   ;
   ;


   ;
   ;  ------ CODE FOR SUMVAL BELOW ------
   ;       r24 = a, r25 = b
   ;
sumval:
   ; we assume r24 and r25 include our args
   ; we add them, and since the function may return an int
   ; we have to use up both registers to store result (overflow)
   add r24, r25
   clr r25
   adc r25, r25
   ; after we have the result, it is at desired location, so we return
   ret

   ;  ------ CODE FOR SUMVAL ABOVE ------




   ;  ------ CODE FOR DIFFVAL BELOW ------

   ;
   ; we have two possible executions: a > b or b >= a
   ; for this we use a compare and a jump to seperate the two
   ; the result is always in r24 (subtraction is safe here, no overflows and such)
   ; and we clear r25 since there should be nothing there for an int type
   ; only one ret call is executed
   ;
diffval:
   cp r25, r24
   brcc no_carry
   ; r24 >= r25
   sub r24, r25
   clr r25
   ret
no_carry:
   ; r25 > r24
   sub r25, r24
   mov r24, r25
   clr r25
   ret

   ;  ------ CODE FOR DIFFVAL ABOVE ------



   ;
   ;  --- YOUR CODE ENDS ---
   ;      The result must be in the register pair r25:r24
   ;      You can also rjmp to the label end_of_assembly if you need to.
   ;




end_of_assembly_testasm: 
   ; -- move r25:r24 to the 16-bit word in variable out
   movw %0,r24

)" : "=w" (out)  /* out is %0, and a 16-bit variable that is modified
		    by the assembly language */
   : "r" (a), "r" (b)  /* a is %1, b is %2, both are register-sized
			  variables */
   : "r25", "r24");   /* r24, r25 are modified by this assembly
			 language fragment */

  return out;
}
