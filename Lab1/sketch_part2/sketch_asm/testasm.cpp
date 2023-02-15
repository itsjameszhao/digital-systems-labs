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
   ;  --- YOUR CODE GOES HERE ---
   ;       r24 = a, r25 = b
   ;
   call sumval
   mov r22, r24 ; move r24 -> r22
   mov r23, r25 ; move r25 -> r23
   mov r24,%1 ; re-copy a and b
   mov r25,%2
   
   call diffval
   add r24, r22
   adc r25, r23
   rjmp end_of_assembly_testasm
   
sumval:
   add r24, r25
   clr r25
   adc r25, r25
   ret
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
