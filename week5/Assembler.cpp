#include "Assembler.h"
#include "SymbolTable.h"
#include <string>
#include <iostream>
#include <vector>

using namespace std;

Assembler::Assembler() {
}

Assembler::~Assembler() {
}

/**
 * Assembler first pass; populates symbol table with label locations.
 * @param instructions An array of the assembly language instructions.
 * @param symbolTable The symbol table to populate.
 */
void Assembler::doFirstPass(SymbolTable* symbolTable, string instructions[], int numOfInst) {

    int counter = 0;

    for(int i = 0; i < numOfInst; i++){

        if(parseInstructionType(instructions[i]) == L_INSTRUCTION){
            //add it to the symbolTable
            string str = parseSymbol(instructions[i]);
            symbolTable->addSymbol(str,i-counter);
            counter++;
        }
    }
}

/**
 * Translates a set of instructions to machine code.
 * @param instructions An array of the assembly language instructions to be converted to machine code.
 * @param symbolTable The symbol table to reference/update.
 * @return A string containing the generated machine code as lines of 16-bit binary instructions.
 */
string Assembler::doSecondPass(SymbolTable* symbolTable, string instructions[], int numOfInst) {
    
    string output = ""; 

    for(int i = 0; i < numOfInst; i++){

        if(parseInstructionType(instructions[i]) == A_INSTRUCTION){
            string temp = translateSymbol(instructions[i],symbolTable);
            output = output + temp;
        }
        else if(parseInstructionType(instructions[i]) == C_INSTRUCTION){
            
            string c = translateComp(parseInstructionComp(instructions[i]));
            string d = translateDest(parseInstructionDest(instructions[i]));
            string j = translateJump(parseInstructionJump(instructions[i]));
            output = output + "111" + c + d + j;
        }
    }
    return output;
}

/**
 * Parses the type of the provided instruction
 * @param instruction The assembly language representation of an instruction.
 * @return The type of the instruction (A_INSTRUCTION, C_INSTRUCTION, L_INSTRUCTION, NULL)
 */
Assembler::InstructionType Assembler::parseInstructionType(string instruction) {

    //TESTED WORKING

    //(A,D,M,-,!,0,1)
    //regular expressions

    char temp = instruction.at(0);

    if(temp == '@'){
        return A_INSTRUCTION;
    }
    else if(temp == '(' && instruction.at(instruction.size()-1) == ')'){
        return L_INSTRUCTION;
    }
    else if(temp =='D'||temp=='A'||temp=='M'|| temp=='-' || temp=='!' || temp=='0' || temp=='1'){
        return C_INSTRUCTION;
    }
    else{
        return NULL_INSTRUCTION;
    }
}

/**
 * Parses the destination of the provided C-instruction
 * @param instruction The assembly language representation of a C-instruction.
 * @return The destination of the instruction (A, D, M, AM, AD, MD, AMD, NULL)
 */
Assembler::InstructionDest Assembler::parseInstructionDest(string instruction) {

    string::size_type index = instruction.find('=');

    if(index == string::npos){
        return NULL_DEST;
    } else {
        string temp = instruction.substr(0,index);
        if(temp == "A"){
            return A;
        }
        else if(temp == "D"){
            return D;
        }
        else if(temp == "M"){
            return M;
        }
        else if(temp == "AM"){
            return AM;
        }
        else if(temp == "AD"){
            return AD;
        }
        else if(temp == "MD"){
            return MD;
        }
        else if(temp == "AMD"){
            return AMD;
        }
    }
} 
//AMD=0;jmp

/**
 * Parses the jump condition of the provided C-instruction
 * @param instruction The assembly language representation of a C-instruction.
 * @return The jump condition for the instruction (JLT, JGT, JEQ, JLE, JGE, JNE, JMP, NULL)
 */
Assembler::InstructionJump Assembler::parseInstructionJump(string instruction) {


    string::size_type index = instruction.find(';');

    if(index != string::npos){
       string temp = instruction.substr(index+1,instruction.size()-1);

        if(temp == "JLT"){
            return JLT;
        }
        else if(temp == "JGT"){
            return JGT;
        }
        else if(temp == "JEQ"){
            return JEQ;
        }
        else if(temp == "JLE"){
            return JLE;
        }
        else if(temp == "JGE"){
            return JGE;
        }
        else if(temp == "JNE"){
            return JNE;
        }
        else if(temp == "JMP"){
            return JMP;
        }
    } else {
        return NULL_JUMP;
    }
}

string Assembler::parseCompHelper(string instruction){

    //TESTED WORKING
    
    string::size_type index1 = instruction.find('=');
    string::size_type index2 = instruction.find(';');

    if(index1 != string::npos){
        //contains '='
        if(index2 != string::npos){
            //contains ';' ie D = M+1;JMP
            return instruction.substr((index1)+1,(index2)-2);
        } else {
            //doesn't contain jump ie A=A+1
            return instruction.substr(index1+1,instruction.size()-1);
        }
    } else {
        //doesn't contain '='
        if(index2 != string::npos){
            // D;JMP
            return instruction.substr(0,index2);
        }
        else{
            //A
            return instruction;
        }
    }
}

//constants not and negitive


/**
 * Parses the computation/op-code of the provided C-instruction
 * @param instruction The assembly language representation of a C-instruction.
 * @return The computation/op-code of the instruction (CONST_0, ... ,D_ADD_A , ... , NULL)
 */
Assembler::InstructionComp Assembler::parseInstructionComp(string instruction) {

    string inst = parseCompHelper(instruction);

    if(inst == "0"){
        return CONST_0;
    }
    else if(inst == "1"){
        return CONST_1;
    }
    else if(inst == "-1"){
        return CONST_NEG_1;
    }
    else if(inst == "D"){
        return VAL_D;
    }
    else if(inst == "A"){
        return VAL_A;
    }
    else if(inst == "!D"){
        return NOT_D;
    }
    else if(inst == "!A"){
        return NOT_A;
    }
    else if(inst == "-D"){
        return NEG_D;
    }
    else if(inst == "-A"){
        return NEG_A;
    }
    else if(inst == "D+1"){
        return D_ADD_1;
    }
    else if(inst == "1+D"){
        return D_ADD_1;
    }
    else if(inst == "A+1"){
        return A_ADD_1;
    }
    else if(inst == "1+A"){
        return A_ADD_1;
    }
    else if(inst == "D-1"){
        return D_SUB_1;
    }
    else if(inst == "A-1"){
        return A_SUB_1;
    }
    else if(inst == "D+A"){
        return D_ADD_A;
    }
    else if(inst == "A+D"){
        return D_ADD_A;
    }
    else if(inst == "D-A"){
        return D_SUB_A;
    }
    else if(inst == "A-D"){
        return A_SUB_D;
    }
    else if(inst == "D&A"){
        return D_AND_A;
    }
    else if(inst == "A&D"){
        return D_AND_A;
    }
    else if(inst == "D|A"){
        return D_OR_A;
    }
    else if(inst == "A|D"){
        return D_OR_A;
    }
    else if(inst == "M"){
        return VAL_M;
    }
    else if(inst == "!M"){
        return NOT_M;
    }
    else if(inst == "-M"){
        return NEG_M;
    }
    else if(inst == "M+1"){
        return M_ADD_1;
    }
    else if(inst == "1+M"){
        return M_ADD_1;
    }
    else if(inst == "M-1"){
        return M_SUB_1;
    }
    else if(inst == "D+M"){
        return D_ADD_M;
    }
    else if(inst == "M+D"){
        return D_ADD_M;
    }
    else if(inst == "D-M"){
        return D_SUB_M;
    }
    else if(inst == "M-D"){
        return M_SUB_D;
    }
    else if(inst == "D&M"){
        return D_AND_M;
    }
    else if(inst == "M&D"){
        return D_AND_M;
    }
    else if(inst == "D|M"){
        return D_OR_M;
    }
    else if(inst == "M|D"){
        return D_OR_M;
    }
    else{
        return NULL_COMP;
    }
}


/**
 * Generates the binary bits of the dest part of a C-instruction
 * @param dest The destination of the instruction
 * @return A string containing the 3 binary dest bits that correspond to the given dest value.
 */
string Assembler::translateDest(InstructionDest dest) {
    if(dest == A){
        return "100";
    }
    else if(dest == D){
        return "010";
    }
    else if(dest == M){
        return "001";
    }
    else if(dest == AM){
        return "101";
    }
    else if(dest == AD){
        return "110";
    }
    else if(dest == MD){
        return "011";
    }
    else if(dest == AMD){
        return "111";
    } else {
        return "000";
    }
}

/**
 * Generates the binary bits of the jump part of a C-instruction
 * @param jump The jump condition for the instruction
 * @return A string containing the 3 binary jump bits that correspond to the given jump value.
 */
string Assembler::translateJump(InstructionJump jump) {

    if(jump == JLT){
        return "100";
    }
    else if(jump == JGT){
        return "001";
    }
    else if(jump == JEQ){
        return "010";
    }
    else if(jump == JLE){
        return "110";
    }
    else if(jump == JGE){
        return "011";
    }
    else if(jump == JNE){
        return "101";
    }
    else if(jump == JMP){
        return "111";
    } else {
        return "000";
    }
}

/**
 * Generates the binary bits of the computation/op-code part of a C-instruction
 * @param comp The computation/op-code for the instruction
 * @return A string containing the 7 binary computation/op-code bits that correspond to the given comp value.
 */
string Assembler::translateComp(InstructionComp comp) {
    if(comp == CONST_0){
        return "0101010";
    }
    else if(comp == CONST_1){
        return "0111111";
    }
    else if(comp == CONST_NEG_1){
        return "0111010";
    }
    else if(comp == VAL_A){
        return "0110000";
    }
    else if(comp == VAL_M){
        return "1110000";
    }
    else if(comp == VAL_D){
        return "0001100";
    }
    else if(comp == NOT_A){
        return "0110001";
    }
    else if(comp == NOT_M){
        return "1110001";
    }
    else if(comp == NOT_D){
        return "0001101";
    }
    else if(comp == NEG_A){
        return "0110011";
    }
    else if(comp == NEG_M){
        return "1110011";
    }
    else if(comp == NEG_D){
        return "0001111";
    }
    else if(comp == A_ADD_1){
        return "0110111";
    }
    else if(comp == M_ADD_1){
        return "1110111";
    }
    else if(comp == D_ADD_1){
        return "0011111";
    }
    else if(comp == A_SUB_1){
        return "0110010";
    }
    else if(comp == M_SUB_1){
        return "1110010";
    }
    else if(comp == D_SUB_1){
        return "0001110";
    }
    else if(comp == D_ADD_A){
        return "0000010";
    }
    else if(comp == D_ADD_M){
        return "1000010";
    }
    else if(comp == D_SUB_A){
        return "0010011";
    }
    else if(comp == D_SUB_M){
        return "1010011";
    }
    else if(comp == A_SUB_D){
        return "0000111";
    }
    else if(comp == M_SUB_D){
        return "1000111";
    }
    else if(comp == D_AND_A){
        return "0000000";
    }
    else if(comp == D_AND_M){
        return "1000000";
    }
    else if(comp == D_OR_A){
        return "0010101";
    }
    else if(comp == D_OR_M){
        return "1010101";
    } else {
        return "ERROR: NULL COMP TRANSLATE COMP";
    }
}

string Assembler::decimalToBinary(int num){
    string str;
      while(num){
      if(num & 1) // 1
        str+='1';
      else // 0
        str+='0';
      num>>=1; // Right Shift by 1 
    } 

    //reverse 
    int n = str.length();
    for (int i = 0; i < n / 2; i++){
    swap(str[i], str[n - i - 1]);
    }

    while(str.size() < 16){
        str = str.insert(0,"0");
    }
      return str;
}

/**
 * Parses the symbol of the provided A/L-instruction
 * @param instruction The assembly language representation of a A/L-instruction.
 * @return A string containing either a label name (L-instruction),
 *         a variable name (A-instruction), or a constant integer value (A-instruction)
 */
string Assembler::parseSymbol(string instruction) {

//TESTED

    if(instruction.at(0) == '@'){
        return instruction.substr(1,instruction.size()-1);
    }
    else if(instruction.at(0) == '(' && instruction.at(instruction.size()-1) == ')'){
        return instruction.substr(1,instruction.size()-2);
    }
    else {
        cout << "";
    }
}

/**
 * Generates the binary bits for an A-instruction, parsing the value, or looking up the symbol name.
 * @param symbol A string containing either a label name, a variable name, or a constant integer value
 * @param symbolTable The symbol table for looking up label/variable names
 * @return A string containing the 15 binary bits that correspond to the given sybmol.
 */
string Assembler::translateSymbol(string symbol, SymbolTable* symbolTable) {
    
    string str = parseSymbol(symbol); //5 or END for example temp

    if(symbolTable->getSymbol(str) == -1){

        //It is not currently in the symbol table

        if(str.find_first_not_of("0123456789") == std::string::npos){
        return decimalToBinary(stoi(str));
        }

        else{
            symbolTable->addSymbol(str,symbolTable->getCounter());
            symbolTable->increaseCounter();
            int temp = symbolTable->getSymbol(str); //eg temp
            return decimalToBinary(temp);    
        }
    }
    else{
        //It is in the symbol table
        int temp = symbolTable->getSymbol(str);
        return decimalToBinary(temp);
    }
}