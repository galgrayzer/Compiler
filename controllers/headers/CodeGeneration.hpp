#pragma once

#include "../../models/headers/AST.hpp"
#include "../../models/parser/headers/ParserDFA.hpp"
#include "../../models/symantic/headers/Symbol.hpp"
#include "../../models/codegen/headers/Register.hpp"

#include <unordered_set>
#include <fstream>
#include <iostream>

class CodeGeneration
{
private:
    AST *program;
    unordered_set<Symbol, Symbol::HashFunction> symbolTable;
    Register registers[6];
    vector<string> code;
    void addCode(string code);
    Register generateExpression(AST *expression);
    void codeFromAST(AST *tree);

    // utility functions
    void generateBase(ofstream &file);
    void generateIdentifiers(ofstream &file);
    void generateCode(ostream &file);
    void generateEnd(ofstream &file);
    Register getRegister();
    void setRegState(Register reg, bool state);
    void freeRegister(Register reg);
    void pushRegs(string *regs, int n);
    void popRegs(string *regs, int n);

    // function to generate code
    void outFunction(AST *outFunction);
    string ifStatement(AST *ifStatement);

public:
    CodeGeneration(AST *program, unordered_set<Symbol, Symbol::HashFunction> symbolTable);
    void generate(string filename);
    ~CodeGeneration();
};