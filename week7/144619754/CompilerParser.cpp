#include "CompilerParser.h"
#include <iostream>
#include <vector>

using namespace std;


/**
 * Constructor for the CompilerParser
 * @param tokens A linked list of tokens to be parsed
 */
CompilerParser::CompilerParser(std::vector<Token*> tokens) {
    tIn=tokens;
}

/**
 * Generates a parse tree for a single program
 */
ParseTree* CompilerParser::compileProgram() {
    if(tIn.front()->getType() != "keyword" && tIn.front()->getValue() != "class"){
        //throw std::invalid_argument("not a strong start");
        throw ParseException();
    }
    return compileClass();
}

/**
 * Generates a parse tree for a single class
 */
ParseTree* CompilerParser::compileClass() {

    // 'class' className '{' classVarDec* subroutineDec* '}'
    ParseTree* compClass = new ParseTree("class","");

        //adding class
    if(tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "class"){
            ParseTree* temp = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            compClass->addChild(temp);
            tIn.erase(tIn.begin());
    }
    else{
        throw ParseException();
    }

    //adding className
    if(tIn.front()->getType() == "identifier"){
            ParseTree* temp1 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            compClass->addChild(temp1);
            tIn.erase(tIn.begin());
    }
    else{
        throw ParseException();
    }

    //adding {
    if(tIn.front()->getType() == "symbol" && tIn.front()->getValue() == "{"){
            ParseTree* temp2 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            compClass->addChild(temp2);
            tIn.erase(tIn.begin());
    }
    else{
        throw ParseException();
    }

    while(tIn.front()->getValue() != "}" && (tIn.size() > 0)){

        //next will be either classVarDec or subroutine
        if( tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "static" ||
            tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "field"){
            
            compClass->addChild(compileClassVarDec());
        }
        else if( (tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "constructor") ||
                (tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "function") ||
                (tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "method")){
                    compClass->addChild(compileSubroutine());
                }
        else if(tIn.front()->getType() != "symbol" && tIn.front()->getValue() != "}"){
            //throw std::invalid_argument( "Error in parsing: 04" + tIn.front()->getType() + "  " + tIn.front()->getValue());
            throw ParseException();
        }
    }

    if(tIn.front()->getType() == "symbol" && tIn.front()->getValue() == "}"){
        ParseTree* temp3 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
        compClass->addChild(temp3);
        tIn.erase(tIn.begin());
    }
    else{
        throw ParseException();
    }

    return compClass;
}

/**
 * Generates a parse tree for a static variable declaration or field declaration
 */
ParseTree* CompilerParser::compileClassVarDec() {

    //('static'|'field') type varName (',' varName)* ';'
    ParseTree* vardec = new ParseTree("classVarDec","");

    //add static or field
    ParseTree* temp = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
    vardec->addChild(temp);
    tIn.erase(tIn.begin());

    //next add type
    if( (tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "int") ||
        (tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "char") ||
        (tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "boolean") ||
        tIn.front()->getType() == "identifier"){
            ParseTree* temp1 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            vardec->addChild(temp1);
            tIn.erase(tIn.begin());
    }
    else{
        //throw std::invalid_argument( "Error in parsing: 05" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
    }

    //next add varName
    if( tIn.front()->getType() == "identifier"){
        ParseTree* temp2 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
        vardec->addChild(temp2);
        tIn.erase(tIn.begin());
    }
    else{
        //throw std::invalid_argument( "Error in parsing: 06" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
    }

    //next add (',' varName)*
    while(tIn.front()->getValue() != ";"){

        if( (tIn.front()->getType() == "symbol" && tIn.front()->getValue() == ",") ||
            tIn.front()->getType() == "identifier" ){
                ParseTree* temp2 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
                vardec->addChild(temp2);
                tIn.erase(tIn.begin());
        }
        else{
            //throw std::invalid_argument( "Error in parsing: 07" + tIn.front()->getType() + "  " + tIn.front()->getValue());
            throw ParseException();
        }
    }
    //add ;
        ParseTree* temp1 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
        vardec->addChild(temp1);
        tIn.erase(tIn.begin());

    return vardec;
}

/**
 * Generates a parse tree for a method, function, or constructor
 */
ParseTree* CompilerParser::compileSubroutine() {

    //('constructor'|'function'|'method') ('void'|type) subroutineName '(' parameterList ')' subroutineBody

    ParseTree* subRoutine = new ParseTree("subroutine","");

    //check to see if we have a constructor, function or method
    if( (tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "constructor") ||
        (tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "function") ||
        (tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "method")) {
            ParseTree* temp = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            subRoutine->addChild(temp);
            tIn.erase(tIn.begin());
        }
    else{
        //throw std::invalid_argument( "Error in parsing: 10" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
    }

    //adding void or type
    if( (tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "void") ||
        (tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "int") ||
        (tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "char") ||
        (tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "boolean") ||
        tIn.front()->getType() == "identifier"){
            ParseTree* temp1 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            subRoutine->addChild(temp1);
            tIn.erase(tIn.begin());
        }
    //adding subroutineName
    if(tIn.front()->getType() == "identifier"){
            ParseTree* temp2 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            subRoutine->addChild(temp2);
            tIn.erase(tIn.begin());
    }
    else{
        //throw std::invalid_argument( "Error in parsing: 11" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
    }

    //adding (
    if(tIn.front()->getType() == "symbol" && tIn.front()->getValue() == "("){
            ParseTree* temp3 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            subRoutine->addChild(temp3);
            tIn.erase(tIn.begin());
    }
    else{
        //throw std::invalid_argument( "Error in parsing: 12" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
    }

    //calling parameterList
    //if(tIn.front()->getValue() != ")"){
        subRoutine->addChild(compileParameterList());
    //}
    //adding )
    if(tIn.front()->getType() == "symbol" && tIn.front()->getValue() == ")"){
            ParseTree* temp4 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            subRoutine->addChild(temp4);
            tIn.erase(tIn.begin());
    }
    else{
        //throw std::invalid_argument( "Error in parsing: 13" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
    }
    
    //calling subroutineBody
    subRoutine->addChild(compileSubroutineBody());

    return subRoutine;
}

/**
 * Generates a parse tree for a subroutine's parameters
 */
ParseTree* CompilerParser::compileParameterList() {

    //((type varName) (',' type varName)*)?
    ParseTree* ParList = new ParseTree("parameterList","");


    if((tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "int") ||
        (tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "char") ||
        (tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "boolean") ||
        tIn.front()->getType() == "identifier"){
            //There is a type 
            ParseTree* temp = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            ParList->addChild(temp);
            tIn.erase(tIn.begin());
        }
    else{
        return ParList;
    }


    //next add varName
    if( tIn.front()->getType() == "identifier"){
        ParseTree* temp1 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
        ParList->addChild(temp1);
        tIn.erase(tIn.begin());
    }
    else{
        //throw std::invalid_argument( "Error in parsing: 09" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
    }

    //while to capture (',' type varName)
    while(  !tIn.empty() && ((tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "int") ||
            (tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "char") ||
            (tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "boolean") ||
            (tIn.front()->getType() == "identifier") ||
            (tIn.front()->getType() == "symbol" && tIn.front()->getValue() == ","))){

                ParseTree* temp2 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
                ParList->addChild(temp2);
                tIn.erase(tIn.begin());
            }
    return ParList;
}

/**
 * Generates a parse tree for a subroutine's body
 */
ParseTree* CompilerParser::compileSubroutineBody() {

        // '{' varDec* statements '}'

    ParseTree* subRoBody = new ParseTree("subroutineBody","");

    //adding {
    if(tIn.front()->getType() == "symbol" && tIn.front()->getValue() == "{"){
            ParseTree* temp = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            subRoBody->addChild(temp);
            tIn.erase(tIn.begin());
    }
    else{
        //throw std::invalid_argument( "Error in parsing: 14" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
    }
    //add varDec or statements
    while((tIn.front()->getType() != "symbol" && tIn.front()->getValue() != "}")){

        //add varDec
        if(tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "var"){
            subRoBody->addChild(compileVarDec());
        }
        else if( (tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "let") ||
                 (tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "if") ||
                 (tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "while") ||
                 (tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "do") ||
                 (tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "return")){
                    subRoBody->addChild(compileStatements());
                 }
        else{
        //throw std::invalid_argument( "Error in parsing: 15" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
        }
    }
    //adding }
    if(tIn.front()->getType() == "symbol" && tIn.front()->getValue() == "}"){
            ParseTree* temp1 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            subRoBody->addChild(temp1);
            tIn.erase(tIn.begin());
    }
    return subRoBody;
}

/**
 * Generates a parse tree for a variable declaration
 */
ParseTree* CompilerParser::compileVarDec() {
    //  'var' type varName (',' varName)* ';'
    ParseTree* vardec = new ParseTree("varDec","");

    if(tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "var"){
            ParseTree* temp = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            vardec->addChild(temp);
            tIn.erase(tIn.begin());
    }
    else{
        //throw std::invalid_argument( "Error in parsing: 16" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
    }

    //next add type
    if( (tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "int") ||
        (tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "char") ||
        (tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "boolean") ||
        tIn.front()->getType() == "identifier"){
            ParseTree* temp1 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            vardec->addChild(temp1);
            tIn.erase(tIn.begin());
    }
    else{
        //throw std::invalid_argument( "Error in parsing: 17" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
    }

    //next add varName
    if( tIn.front()->getType() == "identifier"){
        ParseTree* temp2 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
        vardec->addChild(temp2);
        tIn.erase(tIn.begin());
    }
    else{
        //throw std::invalid_argument( "Error in parsing: 18" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
    }

    while((tIn.front()->getType() != "symbol" && tIn.front()->getValue() != ";")){

        if(tIn.size() <= 1){
            break;
        }

        if( (tIn.front()->getType() == "symbol" && tIn.front()->getValue() == ",") ||
            (tIn.front()->getType() == "identifier")){
                ParseTree* temp3 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
                vardec->addChild(temp3);
                tIn.erase(tIn.begin());
            }
        else{
            //throw std::invalid_argument( "Error in parsing: 19" + tIn.front()->getType() + "  " + tIn.front()->getValue());
            throw ParseException();
        }
    }

    //adding ;
    if(tIn.front()->getType() == "symbol" && tIn.front()->getValue() == ";"){
            ParseTree* temp4 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            vardec->addChild(temp4);
            tIn.erase(tIn.begin());
    }
    return vardec;
}

/**
 * Generates a parse tree for a series of statements
 */
ParseTree* CompilerParser::compileStatements() {
    
    ParseTree* statments = new ParseTree("statements","");

    if(tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "let"){
        statments->addChild(compileLet());
    }
    if(tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "if"){
        statments->addChild(compileIf());
    }
    if(tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "while"){
        statments->addChild(compileWhile());
    }
    if(tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "do"){
        statments->addChild(compileDo());
    }
    if(tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "return"){
        statments->addChild(compileReturn());
    }

    // else{
    //     throw std::invalid_argument("20"+ tIn.front()->getType() + " " + tIn.front()->getValue()); 
    // }
    return statments;
}

/**
 * Generates a parse tree for a let statement
 */
ParseTree* CompilerParser::compileLet() {

    // 'let' varName ('[' expression ']')? '=' expression ';'
    ParseTree* let = new ParseTree("letStatement","");

    //adding let
    if(tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "let"){
            ParseTree* temp = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            let->addChild(temp);
            tIn.erase(tIn.begin());
    }
    else{
        //throw std::invalid_argument( "Error in parsing: 21" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
    }
    //next add varName
    if( tIn.front()->getType() == "identifier"){
        ParseTree* temp1 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
        let->addChild(temp1);
        tIn.erase(tIn.begin());
    }
    else{
        //throw std::invalid_argument( "Error in parsing: 22" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
    }

    //checking for [
    if(tIn.front()->getValue() == "["){
        ParseTree* temp2 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
        let->addChild(temp2);
        tIn.erase(tIn.begin());

        let->addChild(compileExpression());

        if(tIn.front()->getValue() == "]"){
                
            ParseTree* temp3 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            let->addChild(temp3);
            tIn.erase(tIn.begin());
        }else{
            //throw std::invalid_argument( "Error in parsing: 22" + tIn.front()->getType() + "  " + tIn.front()->getValue());
            throw ParseException();
        }
    }

    //add =
    if(tIn.front()->getType() == "symbol" && tIn.front()->getValue() == "="){
            ParseTree* temp4 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            let->addChild(temp4);
            tIn.erase(tIn.begin());
    }
    else{
            //throw std::invalid_argument( "Error in parsing: 23" + tIn.front()->getType() + "  " + tIn.front()->getValue());
            throw ParseException();
    }

    //add expression
    let->addChild(compileExpression());

    if(tIn.front()->getType() == "symbol" && tIn.front()->getValue() == ";"){
            ParseTree* temp5 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            let->addChild(temp5);
            tIn.erase(tIn.begin());
    }
    else{
            //throw std::invalid_argument( "Error in parsing: 23" + tIn.front()->getType() + "  " + tIn.front()->getValue());
            throw ParseException();
    }

    return let;
}

/**
 * Generates a parse tree for an if statement
 */
ParseTree* CompilerParser::compileIf() {
    
    // 'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}' )?
    ParseTree* compif = new ParseTree("ifStatement","");

    //adding if
    if(tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "if"){
            ParseTree* temp = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            compif->addChild(temp);
            tIn.erase(tIn.begin());
    }
    else{
        //throw std::invalid_argument( "Error in parsing: 24" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
    }
    //adding (
    if(tIn.front()->getType() == "symbol" && tIn.front()->getValue() == "("){
            ParseTree* temp1 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            compif->addChild(temp1);
            tIn.erase(tIn.begin());
    }
    else{
        //throw std::invalid_argument( "Error in parsing: 25" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
    }

    //call expression
    compif->addChild(compileExpression());

        //adding )
    if(tIn.front()->getType() == "symbol" && tIn.front()->getValue() == ")"){
            ParseTree* temp2 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            compif->addChild(temp2);
            tIn.erase(tIn.begin());
    }
    else{
        //throw std::invalid_argument( "Error in parsing: 26" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
    }

        //adding {
    if(tIn.front()->getType() == "symbol" && tIn.front()->getValue() == "{"){
            ParseTree* temp3 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            compif->addChild(temp3);
            tIn.erase(tIn.begin());
    }
    else{
        //throw std::invalid_argument( "Error in parsing: 27" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
    }

    //adding statments
    compif->addChild(compileStatements());

        //adding }
    if(tIn.front()->getType() == "symbol" && tIn.front()->getValue() == "}"){
            ParseTree* temp4 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            compif->addChild(temp4);
            tIn.erase(tIn.begin());
    }
    else{
        //throw std::invalid_argument( "Error in parsing: 28" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
    }

    if(tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "else"){

        //adding else
        ParseTree* temp5 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
        compif->addChild(temp5);
        tIn.erase(tIn.begin());

        //adding {
        if(tIn.front()->getType() == "symbol" && tIn.front()->getValue() == "{"){
            ParseTree* temp6 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            compif->addChild(temp6);
            tIn.erase(tIn.begin());
        }
        else{
            //throw std::invalid_argument( "Error in parsing: 29" + tIn.front()->getType() + "  " + tIn.front()->getValue());
            throw ParseException();
        }
        compif->addChild(compileStatements());

        //adding }
        if(tIn.front()->getType() == "symbol" && tIn.front()->getValue() == "}"){
            ParseTree* temp7 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            compif->addChild(temp7);
            tIn.erase(tIn.begin());
        }
        else{
            //throw std::invalid_argument( "Error in parsing: 30" + tIn.front()->getType() + "  " + tIn.front()->getValue());
            throw ParseException();
        }
    }
    return compif;
}

/**
 * Generates a parse tree for a while statement
 */
ParseTree* CompilerParser::compileWhile() {

    // 'while' '(' expression ')' '{' statements '}'
    ParseTree* compWhile = new ParseTree("whileStatement","");

    //adding while
    if(tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "while"){
            ParseTree* temp = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            compWhile->addChild(temp);
            tIn.erase(tIn.begin());
    }
    else{
        //throw std::invalid_argument( "Error in parsing: 31" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
    }
    //adding (
    if(tIn.front()->getType() == "symbol" && tIn.front()->getValue() == "("){
            ParseTree* temp1 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            compWhile->addChild(temp1);
            tIn.erase(tIn.begin());
    }
    else{
        //throw std::invalid_argument( "Error in parsing: 32" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
    }

    //add expression 
    compWhile->addChild(compileExpression());

    //adding )
    if(tIn.front()->getType() == "symbol" && tIn.front()->getValue() == ")"){
            ParseTree* temp9 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            compWhile->addChild(temp9);
            tIn.erase(tIn.begin());
    }
    else{
        //throw std::invalid_argument( "Error in parsing: 33" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
    }

    //adding {
    if(tIn.front()->getType() == "symbol" && tIn.front()->getValue() == "{"){
            ParseTree* temp2 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            compWhile->addChild(temp2);
            tIn.erase(tIn.begin());
    }
    else{
        //throw std::invalid_argument( "Error in parsing: 33" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
    }

    //add statments
    compWhile->addChild(compileStatements());

    //adding }
    if(tIn.front()->getType() == "symbol" && tIn.front()->getValue() == "}"){
            ParseTree* temp3 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            compWhile->addChild(temp3);
            tIn.erase(tIn.begin());
    }
    else{
        //throw std::invalid_argument( "Error in parsing: 34" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
    }
    return compWhile;
}

/**
 * Generates a parse tree for a do statement
 */
ParseTree* CompilerParser::compileDo() {

    // 'do' expression ';'
    ParseTree* compDo = new ParseTree("doStatement","");
    

    //adding do
    if(tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "do"){
            ParseTree* temp = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            compDo->addChild(temp);
            tIn.erase(tIn.begin());
    }
    else{
        //throw std::invalid_argument( "Error in parsing: 35" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
    }

    compDo->addChild(compileExpression());

    //adding ;
    if(tIn.front()->getType() == "symbol" && tIn.front()->getValue() == ";"){
            ParseTree* temp3 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            compDo->addChild(temp3);
            tIn.erase(tIn.begin());
    }
    else{
        //throw std::invalid_argument( "Error in parsing: 34" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
    }
    return compDo;
}

/**
 * Generates a parse tree for a return statement
 */
ParseTree* CompilerParser::compileReturn() {

    // 'return' (expression)? ;
    ParseTree* compReturn = new ParseTree("returnStatement","");

    //adding return
    if(tIn.front()->getType() == "keyword" && tIn.front()->getValue() == "return"){
            ParseTree* temp = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            compReturn->addChild(temp);
            tIn.erase(tIn.begin());
    }
    else{
        //throw std::invalid_argument( "Error in parsing: 35" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
    }


    if(tIn.front()->getValue() == "skip" || tIn.front()->getType() == "keyword" || tIn.front()->getType() == "integerConstant" || 
        tIn.front()->getType() == "stringConstant" || tIn.front()->getType() == "identifier"){
            
            compReturn->addChild(compileExpression());

    }

    //adding ;
    if(tIn.front()->getType() == "symbol" && tIn.front()->getValue() == ";"){
            ParseTree* temp1 = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
            compReturn->addChild(temp1);
            tIn.erase(tIn.begin());
    }
    else{
        //throw std::invalid_argument( "Error in parsing: 36" + tIn.front()->getType() + "  " + tIn.front()->getValue());
        throw ParseException();
    }
    return compReturn;
}

/**
 * Generates a parse tree for an expression
 */
ParseTree* CompilerParser::compileExpression() {

    ParseTree* expression = new ParseTree("expression","");
    //'SKIP' | (term (op term))*)

    if(tIn.front()->getValue() == "skip"){
        ParseTree* temp = new ParseTree(tIn.front()->getType(),tIn.front()->getValue());
        expression->addChild(temp);
        tIn.erase(tIn.begin());
    }

    return expression;
}

/**
 * Generates a parse tree for an expression term
 */
ParseTree* CompilerParser::compileTerm() {
    return NULL;
}

/**
 * Generates a parse tree for an expression list
 */
ParseTree* CompilerParser::compileExpressionList() {
    return NULL;
}


const char* ParseException::what() {
    return "An Exception occurred while parsing!";
}
