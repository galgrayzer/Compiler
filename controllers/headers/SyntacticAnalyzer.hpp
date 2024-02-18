#pragma once

#include <list>

#include "../../models/headers/Token.hpp"
#include "../../models/headers/AST.hpp"
#include "../../models/headers/Stack.hpp"

class SyntacticAnalyzer
{
private:
    list<Token> tokens;

public:
    SyntacticAnalyzer(list<Token> tokens);
    int parser();
    ~SyntacticAnalyzer();
};