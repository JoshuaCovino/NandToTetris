#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "Assembler.h"
#include "SymbolTable.h"

using namespace std;

int main(){

SymbolTable* symbolTable = new SymbolTable();
Assembler assembler;

//JLT, JGT, JEQ, JLE, JGE, JNE, JMP, NULL

//  string instr[] = {"@R1","D=M","@NEGITIVE","D;JLT","(POSITIVE)","@R1","D=M","@R0","M=D","@END","0;JMP","(NEGITIVE)","@R1","D=M","@R0","M=-D","(END)","@END","0;JMP"};

// assembler.doFirstPass(symbolTable, instr, 19);

// string temp = assembler.doSecondPass(symbolTable,instr,19);

// cout<<temp;

string temp = "0;JMP";

Assembler::InstructionJump temp1 = assembler.parseInstructionJump(temp);


cout << temp1;



}