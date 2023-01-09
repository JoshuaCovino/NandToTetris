#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <cstdint>  // this contains uint16_t
#include <map>      // indexable dictionary
#include <string>   // process c++ string

using namespace std;

class SymbolTable {
    private:
        map<string,uint16_t> table;
        int counter;

    public:
        SymbolTable();
        ~SymbolTable();

        void addSymbol(string symbol, uint16_t value); //Adds a symbol to the symbol table
        int getSymbol(string symbol); //Gets a symbol from the symbol table
        void increaseCounter();
        int getCounter();
};

#endif /* SYMBOL_TABLE_H */