#pragma once

using namespace std;

#define DFA_PATH "./DFAs/lexer/lexerDFA.txt"

#include <fstream>
#include <list>
#include <iostream>

#include "../../models/headers/Token.hpp"
#include "../../models/lexer/headres/LexerDFA.hpp"
#include "ErrorHandler.hpp"

class LexicalAnalyzer
{
private:
    // Attributes
    char *filePath;
    DFA *dfa;
    ErrorHandler *error;

    // Methods
    Token *tokenizer(string token, int state, int line);

public:
    // Constructor and Destructor
    LexicalAnalyzer(char *path, ErrorHandler *error);
    ~LexicalAnalyzer();

    // Methods
    list<Token> lexer();
};