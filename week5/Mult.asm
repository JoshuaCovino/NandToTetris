// This file is based on part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: Mult.asm

// Multiplies R1 and R2 and stores the result in R0.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

    @R0 //Make sure that R0 is equal to zero
    M=0

    @R1
    D=M
    @R2
    D=D-M // R1 - R2       R1 < R2 swap ------> R1-R2 < 0
    @SWAP
    D;JLT

(CONTINUE)

    //store the value of R1 into value
    @R1
    D=M
    @value
    M=D

    //store the value of R2 into counter
    @R2
    D=M
    @counter
    M=D

    //If counter is negitive then flip the signs
    @counter
    D=M
    @LOOP
    D;JGE //jump to LOOP if counter >= 0

    //change the sign if negitive
    @counter
    M=-M
    @value
    M=-M


(LOOP)
    @counter
    D=M
    @END
    D;JLE //jump to END when counter <= 0

    //RAM[0] = RAM[0] + value
    @value
    D=M
    @R0
    M=M+D

    //decrement the counter
    @counter
    M=M-1
    @LOOP
    0;JMP

(END)
    @END
    0;JMP

(SWAP)
    @R1
    D=M
    @temp
    M=D // Temp = R1
    @R2
    D=M
    @R1
    M=D
    @temp
    D=M
    @R2
    M=D
    @CONTINUE
    0;JMP