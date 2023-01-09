   @i
    M=1 //Initilise i=1
    @R2
    D=M
    @size//size of the array
    M=D

    @R1
    A=M
    D=M
    @max //max = arr[0]
    M=D

(LOOP)
    @i
    D=M
    @size
    D=D-M
    @FINAL
    D;JGE // jump if i - size > 0

    //Find next value
    @R1  //arr[0]
    D=M
    @i
    D=D+M
    A=D //arr[0+i]
    D=M // D= arr[i]
    @currentVal
    M=D

    @max      
    D=M
    @OverflowEdgecaseMaxNegitive //overflow edgecase. If max < 0 then just check if a > 0
    D;JLT
    @OverflowEdgecaseMaxPositive //overflow edgecase. If max > 0 then just check if a < 0
    D;JGT

(CONTINUE)
    @max
    D=M
    @currentVal
    D=D-M // max - arr[i] < 0 then swap
    @SWAP
    D;JLT
    @SKIP //Otherwise skip
    0;JMP

(OverflowEdgecaseMaxNegitive)
    @currentVal
    D=M
    @SWAP
    D;JGT //if arr[i] > 0 then swap
    @CONTINUE
    0;JMP

(OverflowEdgecaseMaxPositive)
    @currentVal
    D=M
    @SKIP //if arr < 0 skip
    D;JLT
    @CONTINUE
    0;JMP

(SKIP)
    @i
    M=M+1
    @LOOP
    0;JMP

(SWAP)
    @currentVal
    D=M
    @max
    M=D
    @SKIP
    0;JMP

(FINAL)
    @max
    D=M
    @R0
    M=D
    @END
    0;JMP
(END)
    @END
    0;JMP