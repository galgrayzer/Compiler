#pragma once

#include <unordered_set>
#include <unordered_map>

#include "../../controllers/headers/ErrorHandler.hpp"
#include "../../models/headers/AST.hpp"
#include "../../models/symantic/headers/Symbol.hpp"
#include "../../models/parser/headers/ParserDFA.hpp"

class SymanticAnalyzer
{
private:
    ErrorHandler *error;
    AST *parseTree;
    unordered_set<Symbol, Symbol::HashFunction> symbolTable;
    unordered_map<string, int> typeMap = {
        {"int", INT},
        {"bool", BOOL},
        {"char", CHAR},
    };
    int IdentifyType(Token *token);
    void CreateSymbolTable(AST *tree, int scope);
    AST *symanticHelper(AST *tree, int scope);

public:
    SymanticAnalyzer(AST *parseTree, ErrorHandler *error);
    void printSymbolTable();
    unordered_set<Symbol, Symbol::HashFunction> getSymbolTable();
    AST *symantic();
    ~SymanticAnalyzer();
};