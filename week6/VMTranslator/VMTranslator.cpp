#include <string>
#include <vector>
#include <iostream>

#include "VMTranslator.h"

using namespace std;

static int symbolCounter = 0;


/**
 * VMTranslator constructor
 */
VMTranslator::VMTranslator() {
    // Your code here
    //symbolCounter = 0;
}

/**
 * VMTranslator destructor
 */
VMTranslator::~VMTranslator() {
    // Your code here
}

string VMTranslator::reg(string segment, int offset){
    if (segment == "static") return "16";
    if (segment == "local") return "LCL";
    if (segment == "argument") return "ARG";
    if (segment == "this") return "THIS";
    if (segment == "that") return "THAT";
    else{
        return "REG: NOT IN LIST";
    }
    }

/** Generate Hack Assembly code for a VM push operation */
string VMTranslator::vm_push(string segment, int offset){
    vector<string> v;
    string idxString = to_string(offset);
    string regString = reg(segment,offset);

    if(segment == "constant"){
        v.push_back("@" + idxString);
        v.push_back("D=A");
        v.push_back("@SP");
        v.push_back("A=M");
        v.push_back("M=D");
        v.push_back("@SP");
        v.push_back("M=M+1");
    }
    else if(segment == "static"){
        v.push_back("@" + regString);
        v.push_back("D=A");
        v.push_back("@" + idxString);
        v.push_back("A=D+A");
        v.push_back("D=M");
        v.push_back("@SP");
        v.push_back("A=M");
        v.push_back("M=D");
        v.push_back("@SP");
        v.push_back("M=M+1");
    }
    else if (segment == "pointer"){
        v.push_back("@3");
        v.push_back("D=A");
        v.push_back("@"+idxString);
        v.push_back("A=D+A");
        v.push_back("D=M");
        v.push_back("@SP");
        v.push_back("A=M");
        v.push_back("M=D");
        v.push_back("@SP");
        v.push_back("M=M+1");
    }
    else if (segment == "temp"){
        v.push_back("@5");
        v.push_back("D=A");
        v.push_back("@"+idxString);
        v.push_back("A=D+A");
        v.push_back("D=M");
        v.push_back("@SP");
        v.push_back("A=M");
        v.push_back("M=D");
        v.push_back("@SP");
        v.push_back("M=M+1");
    }
    else if(segment == "argument" || segment == "local" || segment == "this" || segment == "that"){
        v.push_back("@" + regString);
        v.push_back("D=M");
        v.push_back("@" + idxString);
        v.push_back("A=D+A");
        v.push_back("D=M");
        v.push_back("@SP");
        v.push_back("A=M");
        v.push_back("M=D");
        v.push_back("@SP");
        v.push_back("M=M+1");
    }
    else {
        cout << "ERROR IN VM_PUSH";
    }
    std::string s;
    for (std::vector<std::string>::const_iterator i = v.begin(); i != v.end(); ++i)
        s = s + *i + "\n";
    return s;
}

/** Generate Hack Assembly code for a VM pop operation */
string VMTranslator::vm_pop(string segment, int offset){
    vector<string> v;
    string idxString = to_string(offset);
    string regString = reg(segment,offset);

    if(segment == "static"){
        v.push_back("@" + regString);
        v.push_back("D=A");
        v.push_back("@" + idxString);
        v.push_back("D=D+A");
        v.push_back("@R13");
        v.push_back("M=D");
        v.push_back("@SP");
        v.push_back("AM=M-1");
        v.push_back("D=M");
        v.push_back("@R13");
        v.push_back("A=M");
        v.push_back("M=D");
    }
    else if (segment == "pointer"){
        v.push_back("@3");
        v.push_back("D=A");
        v.push_back("@" + idxString);
        v.push_back("D=D+A");
        v.push_back("@R13");
        v.push_back("M=D");
        v.push_back("@SP");
        v.push_back("M=M-1");
        v.push_back("A=M");
        v.push_back("D=M");
        v.push_back("@R13");
        v.push_back("A=M");
        v.push_back("M=D");
    }
    else if (segment == "temp"){
        v.push_back("@5");
        v.push_back("D=A");
        v.push_back("@" + idxString);
        v.push_back("D=D+A");
        v.push_back("@R13");
        v.push_back("M=D");
        v.push_back("@SP");
        v.push_back("M=M-1");
        v.push_back("A=M");
        v.push_back("D=M");
        v.push_back("@R13");
        v.push_back("A=M");
        v.push_back("M=D");
    }
    else if(segment == "argument" || segment == "local" || segment == "this" || segment == "that"){
        v.push_back("@" + regString);
        v.push_back("D=M");
        v.push_back("@" + idxString);
        v.push_back("D=D+A");
        v.push_back("@R13");
        v.push_back("M=D");
        v.push_back("@SP");
        v.push_back("M=M-1");
        v.push_back("A=M");
        v.push_back("D=M");
        v.push_back("@R13");
        v.push_back("A=M");
        v.push_back("M=D");
    }
    std::string s;
    for (std::vector<std::string>::const_iterator i = v.begin(); i != v.end(); ++i)
        s = s + *i + "\n";
    return s;
}

/** Generate Hack Assembly code for a VM add operation */
string VMTranslator::vm_add(){
    vector<string> add;
    add.push_back("@SP");
    add.push_back("AM=M-1");
    add.push_back("D=M");
    add.push_back("A=A-1");
    add.push_back("M=D+M");
    std::string s;
    for (std::vector<std::string>::const_iterator i = add.begin(); i != add.end(); ++i)
        s = s + *i + "\n";
    return s;
}

/** Generate Hack Assembly code for a VM sub operation */
string VMTranslator::vm_sub(){
    vector<string> sub;
    sub.push_back("@SP");
    sub.push_back("AM=M-1");
    sub.push_back("D=M");
    sub.push_back("A=A-1");
    sub.push_back("M=M-D");
    std::string s;
    for (std::vector<std::string>::const_iterator i = sub.begin(); i != sub.end(); ++i)
        s = s + *i + "\n";
    return s;
}

/** Generate Hack Assembly code for a VM neg operation */
string VMTranslator::vm_neg(){
    vector<string> neg;
    neg.push_back("@SP");
    neg.push_back("A=M");
    neg.push_back("A=A-1");
    neg.push_back("M=-M");
    std::string s;
    for (std::vector<std::string>::const_iterator i = neg.begin(); i != neg.end(); ++i)
        s = s + *i + "\n";
    return s;
}

/** Generate Hack Assembly code for a VM eq operation */
string VMTranslator::vm_eq(){

    vector<string> v;
    string LabTrue = "JEQtrue" + to_string(symbolCounter);
    string LabFalse = "JEQfalse" + to_string(symbolCounter);
    v.push_back("@SP");
    v.push_back("AM=M-1");
    v.push_back("D=M");
    v.push_back("@SP");
    v.push_back("AM=M-1");
    v.push_back("D=M-D");
    v.push_back("@"+LabTrue);
    v.push_back("D;JEQ");
    v.push_back("D=0");
    v.push_back("@"+LabFalse);
    v.push_back("0;JMP");
    v.push_back("("+LabTrue+")");
    v.push_back("D=-1");
    v.push_back("("+LabFalse+")");
    v.push_back("@SP");
    v.push_back("A=M");
    v.push_back("M=D");
    v.push_back("@SP");
    v.push_back("M=M+1");
    symbolCounter++;
    std::string s;
    for (std::vector<std::string>::const_iterator i = v.begin(); i != v.end(); ++i)
        s = s + *i + "\n";
    return s;
}

/** Generate Hack Assembly code for a VM gt operation */
string VMTranslator::vm_gt(){
    vector<string> v;
    string LabTrue = "JGTtrue" + to_string(symbolCounter);
    string LabFalse = "JGTfalse" + to_string(symbolCounter);
    v.push_back("@SP");
    v.push_back("AM=M-1");
    v.push_back("D=M");
    v.push_back("@SP");
    v.push_back("AM=M-1");
    v.push_back("D=M-D");
    v.push_back("@" + LabTrue);
    v.push_back("D;JGT");
    v.push_back("D=0");
    v.push_back("@" + LabFalse);
    v.push_back("0;JMP");
    v.push_back("("+LabTrue+")");
    v.push_back("D=-1");
    v.push_back("("+LabFalse+")");
    v.push_back("@SP");
    v.push_back("A=M");
    v.push_back("M=D");
    v.push_back("@SP");
    v.push_back("M=M+1");
    symbolCounter++;
    std::string s;
    for (std::vector<std::string>::const_iterator i = v.begin(); i != v.end(); ++i)
        s = s + *i + "\n";
    return s;
}

/** Generate Hack Assembly code for a VM lt operation */
string VMTranslator::vm_lt(){
    vector<string> v;
    string LabTrue = "JLTtrue" + to_string(symbolCounter);
    string LabFalse = "JLTfalse" + to_string(symbolCounter);
    v.push_back("@SP");
    v.push_back("AM=M-1"); 
    v.push_back("D=M");
    v.push_back("@SP");
    v.push_back("AM=M-1");
    v.push_back("D=M-D");
    v.push_back("@"+LabTrue);
    v.push_back("D;JLT");
    v.push_back("D=0");
    v.push_back("@"+LabFalse);
    v.push_back("0;JMP");
    v.push_back("("+LabTrue+")");
    v.push_back("D=-1");
    v.push_back("("+LabFalse+")");
    v.push_back("@SP");
    v.push_back("A=M");
    v.push_back("M=D");
    v.push_back("@SP");
    v.push_back("M=M+1");
    symbolCounter++;
    std::string s;
    for (std::vector<std::string>::const_iterator i = v.begin(); i != v.end(); ++i)
        s = s + *i + "\n";
    return s;
}

/** Generate Hack Assembly code for a VM and operation */
string VMTranslator::vm_and(){
    vector<string> AND;
    AND.push_back("@SP");
    AND.push_back("AM=M-1");
    AND.push_back("D=M");
    AND.push_back("A=A-1");
    AND.push_back("M=D&M");
    std::string s;
    for (std::vector<std::string>::const_iterator i = AND.begin(); i != AND.end(); ++i)
        s = s + *i + "\n";
    return s;
}

/** Generate Hack Assembly code for a VM or operation */
string VMTranslator::vm_or(){
    vector<string> OR;
    OR.push_back("@SP");
    OR.push_back("AM=M-1");
    OR.push_back("D=M");
    OR.push_back("A=A-1");
    OR.push_back("M=D|M");
    std::string s;
    for (std::vector<std::string>::const_iterator i = OR.begin(); i != OR.end(); ++i)
        s = s + *i + "\n";
    return s;
}

/** Generate Hack Assembly code for a VM not operation */
string VMTranslator::vm_not(){
    vector<string> NOT;
    NOT.push_back("@SP");
    NOT.push_back("A=M");
    NOT.push_back("A=A-1");
    NOT.push_back("M=!M");
    std::string s;
    for (std::vector<std::string>::const_iterator i = NOT.begin(); i != NOT.end(); ++i)
        s = s + *i + "\n";
    return s;
}

/** Generate Hack Assembly code for a VM label operation */
string VMTranslator::vm_label(string label){

    vector<string> v;
    v.push_back("("+label+")");
    std::string s;
    for (std::vector<std::string>::const_iterator i = v.begin(); i != v.end(); ++i)
        s = s + *i + "\n";
    return s;
}

/** Generate Hack Assembly code for a VM goto operation */
string VMTranslator::vm_goto(string label){

    vector<string> v;
    v.push_back("@"+label);
    v.push_back("0;JMP");
    std::string s;
    for (std::vector<std::string>::const_iterator i = v.begin(); i != v.end(); ++i)
        s = s + *i + "\n";
    return s;
}

/** Generate Hack Assembly code for a VM if-goto operation */
string VMTranslator::vm_if(string label){

    vector<string> v;
    v.push_back("@SP");
    v.push_back("M=M-1");
    v.push_back("A=M");
    v.push_back("D=M");
    v.push_back("@"+label);
    v.push_back("D;JNE"); //jump if D is not equal to 0
    std::string s;
    for (std::vector<std::string>::const_iterator i = v.begin(); i != v.end(); ++i)
        s = s + *i + "\n";
    return s;
}

/** Generate Hack Assembly code for a VM function operation */
string VMTranslator::vm_function(string function_name, int n_vars){
    vector<string> v;
    v.push_back("("+function_name+")");
    string temp = "";
    for(int i = 0; i < n_vars; i++){
        temp += vm_push("constant",0);
    }
    v.push_back(temp);
    std::string s;
    for (std::vector<std::string>::const_iterator i = v.begin(); i != v.end(); ++i)
        s = s + *i + "\n";
    return s;
}

/** Generate Hack Assembly code for a VM call operation */
string VMTranslator::vm_call(string function_name, int n_args){
    vector<string> v;
    v.push_back("@FUNC"+to_string(symbolCounter));
    v.push_back("D=A");
    v.push_back("@SP");
    v.push_back("A=M");
    v.push_back("M=D");
    v.push_back("@SP");
    v.push_back("M=M+1");
    v.push_back("@LCL");///
    v.push_back("D=M");
    v.push_back("@SP");
    v.push_back("A=M");
    v.push_back("M=D");
    v.push_back("@SP");
    v.push_back("M=M+1");
    v.push_back("@ARG");////
    v.push_back("D=M");
    v.push_back("@SP");
    v.push_back("A=M");
    v.push_back("M=D");
    v.push_back("@SP");
    v.push_back("M=M+1");
    v.push_back("@THIS");/////
    v.push_back("D=M");
    v.push_back("@SP");
    v.push_back("A=M");
    v.push_back("M=D");
    v.push_back("@SP");
    v.push_back("M=M+1");
    v.push_back("@THAT");////
    v.push_back("D=M");
    v.push_back("@SP");
    v.push_back("A=M");
    v.push_back("M=D");
    v.push_back("@SP");
    v.push_back("M=M+1");
    v.push_back("@"+to_string(n_args));/////
    v.push_back("D=A");
    v.push_back("@SP");
    v.push_back("D=M-D");
    v.push_back("@5");
    v.push_back("D=D-A");
    v.push_back("@ARG");//////
    v.push_back("M=D");
    v.push_back("@SP");
    v.push_back("D=M");
    v.push_back("@LCL");
    v.push_back("M=D");
    v.push_back("@"+function_name);
    v.push_back("0;JMP");
    v.push_back("(FUNC"+to_string(symbolCounter)+")");
    symbolCounter++;
    std::string s;
    for (std::vector<std::string>::const_iterator i = v.begin(); i != v.end(); ++i)
        s = s + *i + "\n";
    return s;
}


/** Generate Hack Assembly code for a VM return operation */
string VMTranslator::vm_return(){
    vector<string> v;
    v.push_back("@LCL");
    v.push_back("D=M");
    v.push_back("@STATE");
    v.push_back("M=D");
    v.push_back("@5");
    v.push_back("D=A");
    v.push_back("@STATE");
    v.push_back("A=M-D");
    v.push_back("D=M");
    v.push_back("@TEMP1");
    v.push_back("M=D");
    v.push_back("@SP");
    v.push_back("M=M-1");
    v.push_back("A=M");
    v.push_back("D=M");
    v.push_back("@ARG");
    v.push_back("A=M");
    v.push_back("M=D");
    v.push_back("@ARG");
    v.push_back("D=M+1");
    v.push_back("@SP");
    v.push_back("M=D");
    v.push_back("@STATE");
    v.push_back("A=M-1");
    v.push_back("D=M");
    v.push_back("@THAT");
    v.push_back("M=D");
    v.push_back("@2");
    v.push_back("D=A");
    v.push_back("@STATE");
    v.push_back("A=M-D");
    v.push_back("D=M");
    v.push_back("@THIS");
    v.push_back("M=D");
    v.push_back("@3");
    v.push_back("D=A");
    v.push_back("@STATE");
    v.push_back("A=M-D");
    v.push_back("D=M");
    v.push_back("@ARG");
    v.push_back("M=D");
    v.push_back("@4");
    v.push_back("D=A");
    v.push_back("@STATE");
    v.push_back("A=M-D");
    v.push_back("D=M");
    v.push_back("@LCL");
    v.push_back("M=D");
    v.push_back("@TEMP1");
    v.push_back("A=M");
    v.push_back("0;JMP");
    std::string s;
    for (std::vector<std::string>::const_iterator i = v.begin(); i != v.end(); ++i)
        s = s + *i + "\n";
    return s;
}