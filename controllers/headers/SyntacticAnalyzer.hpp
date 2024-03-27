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
    list<Token> tokens;
    ErrorHandler *error;

public:
    SyntacticAnalyzer(list<Token> tokens, ErrorHandler *error);
    AST *parser(string tablePath, string rulesPath);
    ~SyntacticAnalyzer();
};