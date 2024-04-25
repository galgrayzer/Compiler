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
    ErrorHandler *error;                                     // error handler
    AST *parseTree;                                          // root of the AST
    unordered_set<Symbol, Symbol::HashFunction> symbolTable; // symbol table
    unordered_map<string, int> typeMap = {
        // map of types
        {"int", INT},
        {"bool", BOOL},
        {"char", CHAR},
    };
    bool isCmpOp(string token);                   // check if token is a comparison operator
    int IdentifyType(Token *token);               // identify type of token
    void CreateSymbolTable(AST *tree, int scope); // create symbol table
    AST *symanticHelper(AST *tree, int scope);    // helper function for symantic analysis

public:
    SymanticAnalyzer(AST *parseTree, ErrorHandler *error);        // constructor
    void printSymbolTable();                                      // print symbol table
    unordered_set<Symbol, Symbol::HashFunction> getSymbolTable(); // get symbol table
    AST *symantic();                                              // perform symantic analysis
    ~SymanticAnalyzer();                                          // destructor
};