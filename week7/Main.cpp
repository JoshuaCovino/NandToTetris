#include <iostream>
#include <vector>

#include "CompilerParser.h"
#include "Token.h"

using namespace std;

int main(int argc, char *argv[]) {
    /* Tokens for:
     *     class MyClass {
     *
     *     }
     */
    vector<Token*> tokens;

    // tokens.push_back(new Token("keyword","class"));
    // tokens.push_back(new Token("identifier","Main"));
    // tokens.push_back(new Token("symbol","{"));
    // tokens.push_back(new Token("symbol","}"));

    tokens.push_back(new Token("keyword","int"));
    tokens.push_back(new Token("identifier","a"));
    tokens.push_back(new Token("symbol",","));
    tokens.push_back(new Token("keyword","char"));
    tokens.push_back(new Token("identifier","b"));


    


    // tokens.push_back(new Token("keyword","class"));
    // tokens.push_back(new Token("identifier","Main"));
    // tokens.push_back(new Token("symbol","{"));

    //     //subroutine
    //     tokens.push_back(new Token("keyword","static"));
    //     tokens.push_back(new Token("keyword","int"));
    //     tokens.push_back(new Token("identifier","a"));
    //     tokens.push_back(new Token("symbol",";"));

    //         //compileSubroutine
    //         tokens.push_back(new Token("keyword","function"));
    //         tokens.push_back(new Token("keyword","void"));
    //         tokens.push_back(new Token("identifier","myFunc"));
    //         tokens.push_back(new Token("symbol","("));
    //         tokens.push_back(new Token("keyword","int"));
    //         tokens.push_back(new Token("identifier","a"));
    //         tokens.push_back(new Token("symbol",")"));
    //         tokens.push_back(new Token("symbol","{"));
    //         tokens.push_back(new Token("keyword","var"));
    //         tokens.push_back(new Token("keyword","int"));
    //         tokens.push_back(new Token("identifier","a"));
    //         tokens.push_back(new Token("symbol",";"));
    //             //let
    //             tokens.push_back(new Token("keyword","let"));
    //             tokens.push_back(new Token("identifier","a"));
    //             tokens.push_back(new Token("symbol","="));
    //             tokens.push_back(new Token("keyword","skip"));
    //             tokens.push_back(new Token("symbol",";"));
    //             tokens.push_back(new Token("keyword","do"));
    //             tokens.push_back(new Token("keyword","skip"));
    //             tokens.push_back(new Token("symbol",";"));
    //             tokens.push_back(new Token("keyword","return"));
    //             tokens.push_back(new Token("symbol",";"));
    //         tokens.push_back(new Token("symbol","}"));
    //     tokens.push_back(new Token("symbol","}"));
    //              tokens.push_back(new Token("symbol","}"));

    // //test 3
    // tokens.push_back(new Token("keyword","class"));
    // tokens.push_back(new Token("identifier","Main"));
    // tokens.push_back(new Token("symbol","{"));
    // tokens.push_back(new Token("keyword","function"));
    // tokens.push_back(new Token("keyword","void"));
    // tokens.push_back(new Token("identifier","myFunc"));
    // tokens.push_back(new Token("symbol","("));
    // tokens.push_back(new Token("keyword","int"));
    // tokens.push_back(new Token("identifier","a"));
    // tokens.push_back(new Token("symbol",")"));
    // tokens.push_back(new Token("symbol","{"));
    // tokens.push_back(new Token("keyword","var"));
    // tokens.push_back(new Token("keyword","int"));
    // tokens.push_back(new Token("identifier","a"));
    // tokens.push_back(new Token("symbol",";"));
    // tokens.push_back(new Token("keyword","let"));
    // tokens.push_back(new Token("identifier","a"));
    // tokens.push_back(new Token("symbol","="));
    // //note: this is part of the optional function therefore is not included
    // tokens.push_back(new Token("integerConstant","skip"));
    // tokens.push_back(new Token("symbol",";"));
    // tokens.push_back(new Token("symbol","}"));
    // tokens.push_back(new Token("symbol","}"));


    // tokens.push_back(new Token("keyword", "class"));
    // tokens.push_back(new Token("identifier", "Main"));
    // tokens.push_back(new Token("symbol", "{"));
    // tokens.push_back(new Token("keyword","static"));
    // tokens.push_back(new Token("keyword","int"));
    // tokens.push_back(new Token("identifier","a"));
    // tokens.push_back(new Token("symbol",";"));
    // tokens.push_back(new Token("symbol", "}"));

    // tokens.push_back(new Token("keyword","class"));
    // tokens.push_back(new Token("identifier","Main"));
    // tokens.push_back(new Token("symbol","{"));
    // tokens.push_back(new Token("symbol","}"));

    //tokens.push_back(new Token("",""));




    try {
        CompilerParser parser(tokens);
        //ParseTree* result = parser.compileProgram();
        ParseTree* result = parser.compileParameterList();
        if (result != NULL){
            cout << result->tostring() << endl;
        }
    } catch (ParseException e) {
        cout << "Error Parsing!" << endl;
    }
}