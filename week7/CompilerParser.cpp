#include "CompilerParser.h"
#include <iostream>
using namespace std;


/**
 * Constructor for the CompilerParser
 * @param tokens A linked list of tokens to be parsed
 */
CompilerParser::CompilerParser(std::vector<Token*> tokens) {
    token_input = tokens;
}

/**
 * Generates a parse tree for a single program
 */
ParseTree* CompilerParser::compileProgram() {

    return compileClass();
}

/**
 * Generates a parse tree for a single class
 */
ParseTree* CompilerParser::compileClass() {

    //First entry is the class keyword
    ParseTree* compClass = new ParseTree("class","");
    token_input.erase(token_input.begin());

    //Second entry is the identifier
    checker(token_input.front()->getType(),"identifier","01");
    ParseTree* temp = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    compClass->addChild(temp);
    token_input.erase(token_input.begin());

    //third is { character
    checker(token_input.front()->getValue(),"{","02");
    ParseTree* temp1 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    compClass->addChild(temp1);
    token_input.erase(token_input.begin());

    //Next call ether ClassVarDec of Subroutine
    //Checking if it contains ending bracket.
    checker(token_input.back()->getValue(),"}","03");

    while(token_input.front()->getValue() != "}"){
        if(token_input.front()->getValue() == "static" || token_input.front()->getValue() == "field"){
                compClass->addChild(compileClassVarDec());
        }
        else if(token_input.front()->getValue() == "constructor" || token_input.front()->getValue() == "function" || token_input.front()->getValue() == "method"){
                compClass->addChild(compileSubroutine());
        }
        else{
            throw std::invalid_argument("04");
        }
    }

    if(token_input.front()->getValue() != "}"){
        throw std::invalid_argument( "compileClass doesn't contain }" );
    }
    ParseTree* temp2 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    compClass->addChild(temp2);
    token_input.erase(token_input.begin());

    return compClass;
}

/**
 * Generates a parse tree for a static variable declaration or field declaration
 */
ParseTree* CompilerParser::compileClassVarDec() {

    //('static'|'field') type varName (',' varName)* ';'

    ParseTree* vardec = new ParseTree("ClassVarDec","");
    
    //adding static or field
    ParseTree* temp = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    vardec->addChild(temp);
    token_input.erase(token_input.begin());

    //next is add type
    checker(token_input.front()->getType(),"keyword","05");
    ParseTree* temp1 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    vardec->addChild(temp1);
    token_input.erase(token_input.begin());

    //next is var name
    checker(token_input.front()->getType(),"identifier","06");
    ParseTree* temp2 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    vardec->addChild(temp2);
    token_input.erase(token_input.begin());

    //while the next character is not ; we have to keep adding
    while(token_input.front()->getValue() != ";"){
        if(token_input.front()->getValue() == "," || token_input.front()->getType() == "identifier"){
            ParseTree* temp3 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
            vardec->addChild(temp3);
            token_input.erase(token_input.begin());
        }
    }

    //lastly add the ;
    checker(token_input.front()->getType(),"symbol","07");
    ParseTree* temp4 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    vardec->addChild(temp4);
    token_input.erase(token_input.begin());

    return vardec;
}

/**
 * Generates a parse tree for a method, function, or constructor
 */
ParseTree* CompilerParser::compileSubroutine() {

    //('constructor'|'function'|'method') ('void'|type) subroutineName '(' parameterList ')' subroutineBody

    ParseTree* subRoutine = new ParseTree("subroutine","");
    
    //adding constructor, function or method
    ParseTree* temp = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    subRoutine->addChild(temp);
    token_input.erase(token_input.begin());

    //add void or type

    if(token_input.front()->getValue() == "void" || token_input.front()->getValue() == "int" || token_input.front()->getValue() == "char" || token_input.front()->getValue() == "boolean" || token_input.front()->getType() == "identifier"){
        ParseTree* temp1 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
        subRoutine->addChild(temp1);
        token_input.erase(token_input.begin());
    }
    else{
        throw std::invalid_argument("08" + token_input.front()->getType() + " " + token_input.front()->getValue());
    }

    //next is subroutineName
    checker(token_input.front()->getType(),"identifier","09");
    ParseTree* temp2 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    subRoutine->addChild(temp2);
    token_input.erase(token_input.begin());

    // (
    checker(token_input.front()->getValue(),"(","10");
    ParseTree* temp3 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    subRoutine->addChild(temp3);
    token_input.erase(token_input.begin());

    // parameterList note there may not be a parameterList
    if(token_input.front()->getValue() != ")"){
        subRoutine->addChild(compileParameterList());
    }

    //else check if it contains closing brackets
    checker(token_input.front()->getValue(),")","11");
    ParseTree* temp4 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    subRoutine->addChild(temp4);
    token_input.erase(token_input.begin());

    //call subroutineBody
    subRoutine->addChild(compileSubroutineBody());

    return subRoutine;
}

/**
 * Generates a parse tree for a subroutine's parameters
 */
ParseTree* CompilerParser::compileParameterList() {

    //   ((type varName) (',' type varName)*)?

    ParseTree* ParList = new ParseTree("parameterList","");
    
    if(token_input.front()->getValue() == "int" || token_input.front()->getValue() == "char" || token_input.front()->getValue() == "boolean" || token_input.front()->getType() == "identifier"){
        ParseTree* temp = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
        ParList->addChild(temp);
        token_input.erase(token_input.begin());
    } else {
        throw std::invalid_argument("12");
    }

    //next add varName
    if(token_input.front()->getType() == "identifier"){
        ParseTree* temp1 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
        ParList->addChild(temp1);
        token_input.erase(token_input.begin());
    } else {
        throw std::invalid_argument("13"); 
    }

    while(token_input.front()->getValue() != ")"){
        if(token_input.front()->getValue() == ","){
            ParseTree* temp2 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
            ParList->addChild(temp2);
            token_input.erase(token_input.begin());
        }
        else if(token_input.front()->getValue() == "int" || token_input.front()->getValue() == "char" || token_input.front()->getValue() == "boolean" || token_input.front()->getType() == "identifier"){
            ParseTree* temp3 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
            ParList->addChild(temp3);
            token_input.erase(token_input.begin());  
        }
        else{
            throw std::invalid_argument("14"); 
        }
    }
    return ParList;
}

/**
 * Generates a parse tree for a subroutine's body
 */
ParseTree* CompilerParser::compileSubroutineBody() {

    // '{' varDec* statements '}'

    ParseTree* subRoBody = new ParseTree("subroutineBody","");
    
    checker(token_input.front()->getValue(),"{","15");
    ParseTree* temp = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    subRoBody->addChild(temp);
    token_input.erase(token_input.begin());

    while(token_input.front()->getValue() != "}"){

        if(token_input.front()->getValue() == "var"){
            subRoBody->addChild(compileVarDec());
        }
        else if(token_input.front()->getValue() == "let" || token_input.front()->getValue() == "if" || token_input.front()->getValue() == "while" || token_input.front()->getValue() == "do" || token_input.front()->getValue() == "return"){
            subRoBody->addChild(compileStatements());
        }
        else{
            throw std::invalid_argument("16" + token_input.front()->getType() + " " +token_input.front()->getValue()); 
        }
    }
    //adding }
    ParseTree* temp1 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    subRoBody->addChild(temp1);
    token_input.erase(token_input.begin());

    return subRoBody;
}

/**
 * Generates a parse tree for a variable declaration
 */
ParseTree* CompilerParser::compileVarDec() {

    //  'var' type varName (',' varName)* ';'

    ParseTree* vardec = new ParseTree("varDec","");
    //adding in var
    ParseTree* temp = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    vardec->addChild(temp);
    token_input.erase(token_input.begin());
    
    //add type
    if(token_input.front()->getValue() == "int" || token_input.front()->getValue() == "char" || token_input.front()->getValue() == "boolean" || token_input.front()->getType() == "identifier"){
        ParseTree* temp = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
        vardec->addChild(temp);
        token_input.erase(token_input.begin());
    } else {
        throw std::invalid_argument("17" + token_input.front()->getType() + " " + token_input.front()->getValue());
    }

    //add varName
    if(token_input.front()->getType() == "identifier"){
        ParseTree* temp1 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
        vardec->addChild(temp1);
        token_input.erase(token_input.begin());
    } else {
        throw std::invalid_argument("18"); 
    }

    while(token_input.front()->getValue() != ";"){
        if(token_input.front()->getValue() == ","){
            ParseTree* temp2 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
            vardec->addChild(temp2);
            token_input.erase(token_input.begin());
        }
        else if(token_input.front()->getType() == "identifier"){
            ParseTree* temp3 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
            vardec->addChild(temp3);
            token_input.erase(token_input.begin()); 
        }
        else{
        throw std::invalid_argument("19"+ token_input.front()->getType() + " " + token_input.front()->getValue()); 
        }
    }
    //adding in ;
    ParseTree* temp4 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    vardec->addChild(temp4);
    token_input.erase(token_input.begin());

    return vardec;
}

/**
 * Generates a parse tree for a series of statements
 */
ParseTree* CompilerParser::compileStatements() {

    ParseTree* statments = new ParseTree("statements","");
    if(token_input.front()->getValue() == "let"){
        statments->addChild(compileLet());
    }
    else if(token_input.front()->getValue() == "if"){
        statments->addChild(compileIf());
    }
    else if(token_input.front()->getValue() == "while"){
        statments->addChild(compileWhile());
    }
    else if(token_input.front()->getValue() == "do"){
        statments->addChild(compileDo());
    }
    else if(token_input.front()->getValue() == "return"){
        statments->addChild(compileReturn());
    }
    else{
        throw std::invalid_argument("20"+ token_input.front()->getType() + " " + token_input.front()->getValue()); 
    }
    return statments;
}

/**
 * Generates a parse tree for a let statement
 */
ParseTree* CompilerParser::compileLet() {

    // 'let' varName ('[' expression ']')? '=' expression ';'
    ParseTree* let = new ParseTree("letStatement","");

    //cout << token_input.front()->getType() << "   " << token_input.front()->getValue() << "\n";

    //Adding Let
    ParseTree* temp = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    let->addChild(temp);
    token_input.erase(token_input.begin());

    //cout << token_input.front()->getType() << "   " << token_input.front()->getValue() << "\n";

    //Adding varName
    checker(token_input.front()->getType(),"identifier","21"+ token_input.front()->getType() + " " +token_input.front()->getValue());
    ParseTree* temp1 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    let->addChild(temp1);
    token_input.erase(token_input.begin());

    if(token_input.front()->getValue() == "["){
        ParseTree* temp2 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
        let->addChild(temp2);
        token_input.erase(token_input.begin());

        let->addChild(compileExpression());


        //adding ]
        ParseTree* temp3 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
        let->addChild(temp3);
        token_input.erase(token_input.begin());
    }

    checker(token_input.front()->getValue(),"=","22");
    ParseTree* temp4 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    let->addChild(temp4);
    token_input.erase(token_input.begin());

    //add 2nd expression
    let->addChild(compileExpression());

    checker(token_input.front()->getValue(),";","23" + token_input.front()->getType() + " " +token_input.front()->getValue());
    ParseTree* temp5 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    let->addChild(temp5);
    token_input.erase(token_input.begin());

    return let;
}

/**
 * Generates a parse tree for an if statement
 */
ParseTree* CompilerParser::compileIf() {

    // 'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}' )?
    ParseTree* compif = new ParseTree("ifStatement","");

    //Adding If
    ParseTree* temp = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    compif->addChild(temp);
    token_input.erase(token_input.begin());

    checker(token_input.front()->getValue(),"(","24");
    ParseTree* temp1 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    compif->addChild(temp1);
    token_input.erase(token_input.begin());

    compif->addChild(compileExpression());

    checker(token_input.front()->getValue(),")","25");
    ParseTree* temp2 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    compif->addChild(temp2);
    token_input.erase(token_input.begin());

    checker(token_input.front()->getValue(),"{","26");
    ParseTree* temp3 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    compif->addChild(temp3);
    token_input.erase(token_input.begin());

    compif->addChild(compileStatements());

    checker(token_input.front()->getValue(),"}","27");
    ParseTree* temp4 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    compif->addChild(temp4);
    token_input.erase(token_input.begin());

    if(token_input.front()->getValue() == "else"){

        //adding else
        ParseTree* temp5 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
        compif->addChild(temp5);
        token_input.erase(token_input.begin());
        
        //adding {
        checker(token_input.front()->getValue(),"{","28");
        ParseTree* temp3 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
        compif->addChild(temp3);
        token_input.erase(token_input.begin());

        compif->addChild(compileStatements());

        //adding }
        checker(token_input.front()->getValue(),"}","29");
        ParseTree* temp4 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
        compif->addChild(temp4);
        token_input.erase(token_input.begin());
    }
    return compif;
}

/**
 * Generates a parse tree for a while statement
 */
ParseTree* CompilerParser::compileWhile() {

    // 'while' '(' expression ')' '{' statements '}'
    ParseTree* compWhile = new ParseTree("whileStatement","");

    ParseTree* temp = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    compWhile->addChild(temp);
    token_input.erase(token_input.begin());

    checker(token_input.front()->getValue(),"(","30");
    ParseTree* temp1 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    compWhile->addChild(temp1);
    token_input.erase(token_input.begin());

    compWhile->addChild(compileExpression());

    checker(token_input.front()->getValue(),")","31");
    ParseTree* temp2 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    compWhile->addChild(temp2);
    token_input.erase(token_input.begin());

    checker(token_input.front()->getValue(),"{","32");
    ParseTree* temp3 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    compWhile->addChild(temp3);
    token_input.erase(token_input.begin());

    compWhile->addChild(compileStatements());

    checker(token_input.front()->getValue(),"}","33");
    ParseTree* temp4 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    compWhile->addChild(temp4);
    token_input.erase(token_input.begin());

    return compWhile;
}

/**
 * Generates a parse tree for a do statement
 */
ParseTree* CompilerParser::compileDo() {

    // 'do' expression ';'
    ParseTree* compDo = new ParseTree("doStatement","");

    //adding do
    ParseTree* temp = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    compDo->addChild(temp);
    token_input.erase(token_input.begin());

    compDo->addChild(compileExpression());

    checker(token_input.front()->getValue(),";","34");
    ParseTree* temp1 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    compDo->addChild(temp1);
    token_input.erase(token_input.begin());

    return compDo;
}

/**
 * Generates a parse tree for a return statement
 */
ParseTree* CompilerParser::compileReturn() {

    // 'return' (expression)?
    ParseTree* compReturn = new ParseTree("returnStatement","");
    //Adding return 
    ParseTree* temp = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    compReturn->addChild(temp);
    token_input.erase(token_input.begin());

    if(token_input.front()->getValue() == "skip" || token_input.front()->getType() == "keyword" || token_input.front()->getType() == "integerConstant" || token_input.front()->getType() == "stringConstant" || token_input.front()->getType() == "identifier"){
        compReturn->addChild(compileExpression());
    }

    ParseTree* temp1 = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
    compReturn->addChild(temp1);
    token_input.erase(token_input.begin());


    return compReturn;
}

/**
 * Generates a parse tree for an expression 
 */
ParseTree* CompilerParser::compileExpression() {

    //optional
    ParseTree* expression = new ParseTree("expression","");

    //'SKIP' | (term (op term))*)

    if(token_input.front()->getValue() == "skip"){
        ParseTree* temp = new ParseTree(token_input.front()->getType(),token_input.front()->getValue());
        expression->addChild(temp);
        token_input.erase(token_input.begin());
    }

    return expression;
}

/**
 * Generates a parse tree for an expression term
 */
ParseTree* CompilerParser::compileTerm() {

    //optional
    //ParseTree* term = new ParseTree("Term","");


    return NULL;
}

/**
 * Generates a parse tree for an expression list
 */
ParseTree* CompilerParser::compileExpressionList() {

    //optional
    //ParseTree* list = new ParseTree("List","");


    return NULL;
}


const char* ParseException::what() {
    return "An Exception occurred while parsing!";
}

void CompilerParser::checker(std::string a,std::string b, std::string c){
    if(a != b){
        throw std::invalid_argument( "Error in parsing: " + c );
    }
}