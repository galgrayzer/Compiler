#pragma once

#include <list>

#include "../../models/headers/Token.hpp"
#include "../../models/headers/AST.hpp"
#include "../../models/headers/Stack.hpp"
#include "../../models/parser/headers/ParserDFA.hpp"
#include "ErrorHandler.hpp"

#define MAX_STACK 100

class SyntacticAnalyzer
{
private:
    list<Token> tokens;  // list of tokens
    ErrorHandler *error; // error handler

public:
    SyntacticAnalyzer(list<Token> tokens, ErrorHandler *error); // constructor
    AST *parser(string tablePath, string rulesPath);            // parse the tokens
    ~SyntacticAnalyzer();                                       // destructor
};