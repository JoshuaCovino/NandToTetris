#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <string>
#include <map>
#include "SymbolTable.h"

using namespace std;

class Assembler {

   public:
    /** Instruction types */
    enum InstructionType {
        A_INSTRUCTION,
        C_INSTRUCTION,
        L_INSTRUCTION,
        NULL_INSTRUCTION
    };

    /** C-instruction destinations */
    enum InstructionDest {
        A,
        D,
        M,
        AM,
        AD,
        MD,
        AMD,
        NULL_DEST
    };

    /** C-instruction jump conditions */
    enum InstructionJump {
        JLT,
        JGT,
        JEQ,
        JLE,
        JGE,
        JNE,
        JMP,
        NULL_JUMP
    };

    /** C-instruction computations/op-codes */
    enum InstructionComp {
        CONST_0,
        CONST_1,
        CONST_NEG_1,
        VAL_A,
        VAL_M,
        VAL_D,
        NOT_A,
        NOT_M,
        NOT_D,
        NEG_A,
        NEG_M,
        NEG_D,
        A_ADD_1,
        M_ADD_1,
        D_ADD_1,
        A_SUB_1,
        M_SUB_1,
        D_SUB_1,
        D_ADD_A,
        D_ADD_M,
        D_SUB_A,
        D_SUB_M,
        A_SUB_D,
        M_SUB_D,
        D_AND_A,
        D_AND_M,
        D_OR_A,
        D_OR_M,
        NULL_COMP
    };

    /** Practical Assignment 5 methods */
    Assembler();
    ~Assembler();
    
    void doFirstPass(SymbolTable* symbolTable, string instructions[], int numOfInst); //Resolve L instructions. Put them into symbol table
    string doSecondPass(SymbolTable* symbolTable, string instructions[], int numOfInst); //generate machine code

    //array that contains the instructions. //NumOfInst = size of the array

    InstructionType parseInstructionType(string instruction); //eg D=M give type back
    InstructionDest parseInstructionDest(string instruction); //return enum val  D;JMP
    InstructionJump parseInstructionJump(string instruction); //return enum jump ect
    InstructionComp parseInstructionComp(string instruction);
    string parseSymbol(string instruction); //give whole instruction and take out the bracket or @ symbol 

    string translateDest(InstructionDest dest); //take in enum and spit out binary  -> three bits
    string translateJump(InstructionJump jump); // -> three bits
    string translateComp(InstructionComp comp); // -> 7 bits
    string translateSymbol(string symbol, SymbolTable* symbolTable); // A instructions Like @5 turn into binary 5 else if lookup L 

    string decimalToBinary(int val);
    string parseCompHelper(string instruction);
};

#endif /* ASSEMBLER_H */