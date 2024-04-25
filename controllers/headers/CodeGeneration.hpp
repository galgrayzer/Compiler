#pragma once

using namespace std;

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
    AST *program;   // root of the AST
    int labelCount; // count of labels
    unordered_set<Symbol, Symbol::HashFunction>
        symbolTable;                              // symbol table
    Register registers[6];                        // registers
    vector<string> code;                          // code to be generated
    void addCode(string code);                    // add code to the code vector
    Register generateExpression(AST *expression); // generate code for expression
    void codeFromAST(AST *tree);                  // generate code from AST

    // utility functions
    void generateBase(ofstream &file);           // generate base code
    void generateIdentifiers(ofstream &file);    // generate identifiers
    void generateCode(ostream &file);            // generate code
    void generateEnd(ofstream &file);            // generate end code
    Register getRegister();                      // get a free register
    void setRegState(Register reg, bool state);  // set register state
    void freeRegister(Register reg);             // free register
    void pushRegs(string *regs, int n);          // push registers
    void popRegs(string *regs, int n);           // pop registers
    string convert32to8(Register reg, int byte); // convert 32 bit register to 8 bit register

    // function to generate code
    void outFunction(AST *outFunction);         // out function
    void forStatement(AST *forStatement);       // for statement
    void whileStatement(AST *whileStatement);   // while statement
    void assignStatement(AST *assignStatement); // assign statement
    void ifStatement(AST *ifStatement);         // if statement

public:
    CodeGeneration(AST *program, unordered_set<Symbol, Symbol::HashFunction> symbolTable); // constructor
    void generate(string filename);                                                        // generate code
    ~CodeGeneration();                                                                     // destructor
};