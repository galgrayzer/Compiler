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
    string name;
    int type;
    int scope;
    int line;

    Symbol(string name, int type, int scope, int line);

    bool operator==(const Symbol &other) const;

    struct HashFunction
    {
        size_t operator()(const Symbol &symbol) const
        {
            return hash<string>()(symbol.name);
        }
    };
};
