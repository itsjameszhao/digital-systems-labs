#include "testasm.h"

/* the attribute "used" tells the compiler to not inline this function */
__attribute__((used))
unsigned int fib(unsigned char n) {
  unsigned int out;

  asm volatile (R"(

   rjmp start_of_assembly_fib

   .data
   ; any data values/static storage can go here

   .text

start_of_assembly_fib:
   ; Your assembly language program goes here

   ; Tell the compiler to move the argument n into registers 
   ; r24
   mov r24,%1

   ; we call recurse, and are the only part that has access to jmp end_of_assembly_fib
   ; all other calls bottom out to a ret
   call recurse
   jmp end_of_assembly_fib
   ;
   ;  --- YOUR CODE GOES HERE ---
   ;       r24 = n

   ; if the input is less than 2, we can just return its value,
   ; but we have to be sure to clear out the top bit as the function
   ; definition is for a 16bit int
recurse:
   cpi r24, 2
   clr r25
   brcc continue_func
   ret

   ; if procedure made it here, then the value is >= 2, so some actual computation is needed
continue_func:

   ; first thing we do is save n for future calls
   push r24 

   ; decrement r24 for fib(n-1), then call recurse
   dec r24
   call recurse 

   ; pop n back into r25 and decrement it twice for fib(n-2)
   pop r25
   dec r25
   dec r25

   ; r24 at this point has the return value of our first fib call (8bit value), but we need to push
   ; it onto the stack since r24 is needed by recurse as its argument
   push r24

   ; set r24 to the correct argument for recurse, and call
   mov r24, r25
   call recurse


   ; pop the result from our first fib call into r25, r24 has the result of our 
   ; second fib call, so we add them and return
   pop r25
   add r24, r25
   clr r25
   ret
   ; rjmp end_of_assembly_fib
   ;
   ;  --- YOUR CODE ENDS ---
   ;      The result must be in the register pair r25:r24
   ;      You can also rjmp to the label end_of_assembly if you need to.
   ;
end_of_assembly_fib: 
   ; -- move r24 to the 16-bit word in variable out
   clr r25
   movw %0,r24

)" : "=w" (out)  /* out is %0, and a 16-bit variable that is modified
        by the assembly language */
   : "r" (n)  /* n is %1
        variables */
   : "r25", "r24");   /* r24, r25 are modified by this assembly
       language fragment */

  return out;
}
