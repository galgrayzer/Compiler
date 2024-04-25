#pragma once

using namespace std;

#include <string>

enum SymbolType
{
    INT,
    CHAR,
    BOOL,
};

class Symbol
{
public:
    string name; // name of the symbol
    int type;    // type of the symbol
    int scope;   // scope of the symbol
    int line;    // line number

    Symbol(string name, int type, int scope, int line); // constructor

    bool operator==(const Symbol &other) const; // operator overloading

    struct HashFunction // hash function for symbol
    {
        size_t operator()(const Symbol &symbol) const
        {
            return hash<string>()(symbol.name);
        }
    };
};
