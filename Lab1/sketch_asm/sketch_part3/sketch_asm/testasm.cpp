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
   call recurse
   jmp end_of_assembly_fib
   ;
   ;  --- YOUR CODE GOES HERE ---
   ;       r24 = n

recurse:
   cpi r24, 2
   clr r25
   brcc continue_func
   ret
continue_func:
   ; in this case, compute recursive and add to r24
   push r24 
   ; decrement r24
   dec r24
   ; call fib
   call recurse 
   ; pop into r25
   pop r25
   ; decrement n by 2
   dec r25
   dec r25
   ; push r24
   push r24
   ; set r24 to r25
   mov r24, r25
   ; call fib 
   call recurse
   ; pop from stack and into r25
   pop r25
   ; add the two and set to r24
   add r24, r25
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
