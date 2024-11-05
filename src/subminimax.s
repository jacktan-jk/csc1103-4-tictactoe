//@ subminimax.s

//@ data sector is for initialized variables and constants
.section .data
    // No data defined here

//@ text sector is for the actual code
.section .text
.global min                          /* start the assembly code */
.global max
.global isMovesLeft

/*
The max function
R0 - a (first integer), R1 - b (second integer)
results - Maximum of a and b
*/
max:
    PUSH {lr}              /* Save the link register */

    /* Load arguments from the stack */
    LDR R0, [sp, #4]       /* Load a (first argument) */
    LDR R1, [sp, #8]       /* Load b (second argument) */
    
    CMP R0, R1             /* Compare a (R0) and b (R1) */
    BGT return_a           /* If a > b, branch to return_a */
    MOV R0, R1             /* Otherwise, move b into R0 (the return value) */
    
    POP {lr}               /* Restore the link register */
    BX lr                  /* Return to the caller */

return_a:
    MOV R0, R0             /* Move a into R0 (the return value) */

    POP {lr}               /* Restore the link register */
    BX lr                  /* Return to the caller */


/*
The min function
R0 - a (first integer), R1 - b (second integer)
results - Minimum of a and b
*/
min:
    PUSH {lr}              /* Save the link register */

    /* Load arguments from the stack */
    LDR R0, [sp, #4]       /* Load a (first argument) */
    LDR R1, [sp, #8]       /* Load b (second argument) */

    CMP R0, R1             //@ Compare a (R0) and b (R1)
    BLT return_a_min       //@ If a < b, branch to return_a_min
    MOV R0, R1             //@ Otherwise, move b into R0 (the return value)

    POP {lr}               /* Restore the link register */
    BX lr                  //@ end; return address

return_a_min:
    MOV R0, R0             //@ Move a into R0 (the return value)

    POP {lr}               /* Restore the link register */
    BX lr                  //@ end; return address;


/* 
The isMovesLeft Function
R0 - pointer to the board (3R3 array of integers)
results - 1 if Moves left (true), 0 otherwise (false)
*/

isMovesLeft:
    PUSH {lr}              //@ Save lr to the stack
    MOV R2, #0             //@ Outer loop counter (i = 0)

outer_loop:
    CMP R2, #3            //@ Compare i with 3
    BGE return_false      //@ if i >= 3, branch to return_false

    MOV R3, #0            //@ Inner loop counter (j = 0)

inner_loop:
    CMP R3, #3             //@ Compare j with 3
    BGE increment_i        //@ If j >= 3, increment i

    //@ Calculate the address of board[i][j]
    LDR R4, [R0, R2, LSL #2]    //@ Load board[i] address
    LDR R5, [R4, R3, LSL #2]    //@ Load board[i][j] 

    CMP R5, #0             //@ Compare board[i][j] with EMPTY (assuming EMPTY = 0)
    BEQ return_true        //@ If it is empty, return true

    ADD R3, R3, #1         //@ Increment j
    B inner_loop           //@ Repeat inner loop

increment_i:
    ADD R2, R2, #1         //@ Increment i
    B outer_loop           //@ Repeat outer loop

return_false:
    MOV R0, #0             //@ Return 0 (false)

    POP {lr}               //@ Restore lr from the stack
    BX lr                  //@ Return from function

return_true:
    MOV R0, #1             //@ Return 1 (true)
    
    POP {lr}               //@ Restore lr from the stack
    BX lr                  //@ Return from function