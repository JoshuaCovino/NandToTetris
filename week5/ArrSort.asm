// Sorts the array of length R2 whose first element is at RAM[R1] in descending order in place. Sets R0 to True (-1) when complete.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

//implement bubble sort
//for (int i = 0; i< (n-1); i++){
//    for(int j = 0; j < n - i - 1; j++){
//    
//      if(arr[j] > arr[j+1]) then swap
//    }
//}

    @R1
    D=M
    D=D-1
    @Finished  //Jump if array is 1 element or smaller
    D;JLE

    @i
    M=0
    @j
    M=0
    @R2
    D=M
    @size
    M=D
(ILoop)
    //check condition jump if need be
    @i
    D=M
    @size
    D=D-M
    D=D+1
    @Finished
    D;JGT //if i > n-1  or  i + 1 - n > 0 jump

(JLoop)
    @j
    D=M
    @size
    D=D-M
    @i
    D=D+M
    D=D+1
    @InnerLoopFinished  //jump if j > n-i-1
    D;JGE

    @R1
    D=M
    @j
    D=D+M
    D=D+1
    A=D
    D=M 
    @temp1 //arr[j+1]
    M=D

    @R1
    D=M
    @j
    D=D+M
    A=D
    D=M //arr[j]
    @temp3
    M=D

    @temp3
    D=M

    @overflow1 //arr[j] > 0 and arr[j+1] < 0 skip
    D;JGT
    @overflow2 //arr[j] < 0 and arr[j+1] > 0 swap
    D;JLT

(CONTINUE)
    @temp3
    D=M
    @temp1
    D=D-M

    @Swap
    D;JLT //arr[j] < arr[j+1]  or     arr[j] - arr[j+1] < 0 jump

(SKIP)   
    @j
    M=M+1
    @JLoop
    0;JMP

(InnerLoopFinished)
    @j
    M=0 //reset J
    @i
    M=M+1
    @ILoop
    0;JMP

(Swap)
    //swap arr[j] <-> arr[j+1]
    //store the location of arr[j] and arr[j+1]
    @R1
    D=M
    @j
    D=D+M
    @location //arr[j]
    M=D
    D=D+1
    @location2//arr[j+1]
    M=D

    @location
    A=M
    D=M
    @temp2 //temp2 = arr[j]
    M=D  

    @location2
    A=M
    D=M //D = arr[j+1]
    @location
    A=M
    M=D //arr[j] = arr[j+1]

    @temp2
    D=M
    @location2
    A=M
    M=D //arr[j+1] = temp = arr[j]    

    @j
    M=M+1
    @JLoop
    0;JMP

(overflow1) //arr[j] > 0 and arr[j+1] < 0 skip
    @temp1
    D=M
    @SKIP
    D;JLT
    @CONTINUE
    0;JMP

(overflow2) //arr[j] < 0 and arr[j+1] > 0 swap
    @temp1
    D=M
    @Swap
    D;JGT
    @CONTINUE
    0;JMP

(Finished)
    @R0
    M=-1
    @END
    0;JMP

(END)
    @END
    0;JMP