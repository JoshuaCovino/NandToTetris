load ArrMax.asm,
output-file ArrMax02.out,
compare-to ArrMax02.cmp,
output-list RAM[0]%D2.6.2 RAM[1]%D2.6.2 RAM[2]%D2.6.2 RAM[2048]%D2.6.2 RAM[2049]%D2.6.2 RAM[2050]%D2.6.2 RAM[2051]%D2.6.2;

set PC 0,
set RAM[0]    0,  // Set R0
set RAM[1] 2048,  // Set R1
set RAM[2]    4,  // Set R2
set RAM[2048] -30000,  // Set Arr[0]
set RAM[2049] 30500,  // Set Arr[1]
set RAM[2050] -32000,  // Set Arr[2]
set RAM[2051] 30200;  // Set Arr[3]
repeat 300 {
  ticktock;       // Run for 300 clock cycles
}
set RAM[1] 2048,  // Restore arguments in case program used them
set RAM[2] 4,
output;           // Output to file

